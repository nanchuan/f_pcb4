//     Copyright (c) 2013 js200300953@qq.com All rights reserved.
//         ==================================================
//         ========Բ�㲩ʿ΢����������������������========
//         ==================================================
//     Բ�㲩ʿ΢������������������������λ��������λ��Bootloader
// ����λ��App��ң�س��򣬼����ǵ�Դ���룬�����ܳơ��������
//     ������ṩ�ο���js200300953����������κε�����������ʹ�ø����
// �����ֵ���ʧ����
//     ���������ѧϰΪĿ���޸ĺ�ʹ�ã�������������ҵ��Ŀ��ʹ�ø������
//     �޸ĸ����ʱ�����뱣��ԭ��Ȩ������
// 
//     �������ϼ���
// http://blog.sina.com.cn/js200300953
// http://www.etootle.com/
// http://www.eeboard.com/bbs/forum-98-1.html#separatorline
// Բ�㲩ʿ΢�����������QQȺ��276721324

// control/control.c
// 2013-1-8 16:30:59
// js200300953

#include <math.h>
#include <bsp/time.h>
#include <bsp/motor.h>
#include <app/communication/interface.h>
#include <app/math/interface.h>
#include <app/attitude/interface.h>
#include <app/param/interface.h>
#include <app/exception/exception.h>
#include "control.h"
#include "pid.h"

enum
{
    CTRL_MODE_HALT = 1,      // ͣ����
    CTRL_MODE_LOCK_THROTTLE, // ��������ģʽ��
    CTRL_MODE_LOCK_ATTITUDE, // ����Ŀ����̬ģʽ��
};

const uint32_t CTRL_PERIOD_MS = 10;

static struct
{
    struct
    {
        quaternion  baseAttitude;
        quaternion  target;
        float       baseThrottle;
        int32_t     isReady;
    }lockAttitude;
    float throttle[4];
    uint8_t mode;
}control;

void    ctrl_init(void);
void    ctrl_checkEvent(void);
void    ctrl_enterHaltMode(void);
void    ctrl_enterLockAttitudeMode(void);
void    ctrl_enterLockThrottleMode(void);
int32_t ctrl_lockAttitude_getReady(void);
int32_t ctrl_lockAttitude_control(const quaternion * c);
int32_t ctrl_lockAttitude_setBaseThrottle(float baseThrottle);
void    ctrl_lockThrottle_setThrottle(const float th[4]);
const float * ctrl_getCurrentThrottle(void);
//
static void ctrl_output(void);
static void ctrl_outputSame(float all_esc_throttle);
static void ctrl_outputByTargetAttitude(const quaternion * target,float baseThrottle);
static int32_t ctrl_lockAttitude_output(void);

void ctrl_init(void)
{
    control.mode = CTRL_MODE_HALT;
    pid_init();
    for(int32_t i=0;i<4;i++)
        control.throttle[i] = 0;
}

void ctrl_checkEvent(void)
{
    TIME_EXECUTE_PERIODICALLY_BEGIN(CTRL_PERIOD_MS)
    {
        // �����б�쳣��
        if(control.mode != CTRL_MODE_HALT)
        {
            exception_checkInclination(attitude_getAttitude());
        }
        //
        switch(control.mode)
        {
            case CTRL_MODE_HALT :
                ctrl_outputSame(0);
                break;
            case CTRL_MODE_LOCK_ATTITUDE :
                ctrl_lockAttitude_output();
                break;
            case CTRL_MODE_LOCK_THROTTLE :
                break;
            //
        }
    } TIME_EXECUTE_PERIODICALLY_END();
}

void ctrl_output(void)
{
    motor_setForce(control.throttle);
}

void ctrl_outputSame(float esc_throttle)
{
    for(int i=0;i<4;i++)
        control.throttle[i] = esc_throttle;
    ctrl_output();
}

void ctrl_outputByTargetAttitude(const quaternion * target,float baseThrottle)
{
    //
    // ������ת�
    const quaternion * cur = attitude_getAttitude();
    quaternion cur_rev;
    quaternion delta;
    cur_rev.w =   cur->w;
    cur_rev.x = - cur->x;
    cur_rev.y = - cur->y;
    cur_rev.z = - cur->z;
    quaternion_mult(&delta,&cur_rev,target);
    quaternion_normalize(&delta);
    //
    // ����PID��
    float output[3] = {0,0,0};
    pid_iterate(&(delta.x),output);
    //
    // �������š�
    control.throttle[0] = baseThrottle - output[0] + output[1] + output[2];
    control.throttle[1] = baseThrottle - output[0] - output[1] - output[2];
    control.throttle[2] = baseThrottle + output[0] + output[1] - output[2];
    control.throttle[3] = baseThrottle + output[0] - output[1] + output[2];
    //
    // ������š�
    ctrl_output();
}

void ctrl_enterHaltMode(void)
{
    control.mode = CTRL_MODE_HALT;
    ctrl_outputSame(0);
    //
    cmd_returnMessage(
        "\xE8\xBF\x9B\xE5\x85\xA5\xE5\x81"
        "\x9C\xE6\x9C\xBA\xE6\xA8\xA1\xE5\xBC\x8F"
        /*"����ͣ��ģʽ"*/);
}

void ctrl_enterLockAttitudeMode(void)
{
    control.mode = CTRL_MODE_LOCK_ATTITUDE;
    control.lockAttitude.isReady = 0;
    //
    cmd_returnMessage(
        "\xE8\xBF\x9B\xE5\x85\xA5\xE9\x94"
        "\x81\xE5\xAE\x9A\xE5\xA7\xBF\xE6"
        "\x80\x81\xE6\xA8\xA1\xE5\xBC\x8F"
        /*"����������̬ģʽ"*/);
}

void ctrl_enterLockThrottleMode(void)
{
    control.mode = CTRL_MODE_LOCK_THROTTLE;
    ctrl_outputSame(0);
    //
    cmd_returnMessage(
        "\xE8\xBF\x9B\xE5\x85\xA5\xE9\x94"
        "\x81\xE5\xAE\x9A\xE6\xB2\xB9\xE9"
        "\x97\xA8\xE6\xA8\xA1\xE5\xBC\x8F"
        /*"������������ģʽ"*/);
}

void ctrl_lockThrottle_setThrottle(const float th[4])
{
    if(control.mode != CTRL_MODE_LOCK_THROTTLE)
        return;
    //
    for(int i=0;i<4;i++)
        control.throttle[i] = th[i];
    ctrl_output();
}

int32_t ctrl_lockAttitude_output(void)
{
    if(! control.lockAttitude.isReady
        || control.lockAttitude.baseThrottle < 0.001)
    {
        ctrl_outputSame(0);
        return 0;
    }
    ctrl_outputByTargetAttitude(&(control.lockAttitude.target),
        control.lockAttitude.baseThrottle);
    //
    return 0;
}

int32_t ctrl_lockAttitude_getReady(void)
{
    if(control.mode != CTRL_MODE_LOCK_ATTITUDE)
        return -1;
    //
    control.lockAttitude.baseThrottle = 0;
    //
    // ����Ŀ����̬��
    quaternion yawCali = *attitude_getAttitude();
    yawCali.x = 0;
    yawCali.y = 0;
    //
    quaternion_mult(&(control.lockAttitude.baseAttitude),
                    &(yawCali),
                    &(param_getRamParam()->horizon));
    quaternion_normalize(&(control.lockAttitude.baseAttitude));
    control.lockAttitude.target = control.lockAttitude.baseAttitude;
    //
    (void)pid_init();
		//
    control.lockAttitude.isReady = 1;
    //
    return 0;
}

int32_t ctrl_lockAttitude_control(const quaternion * c)
{
    if(control.mode != CTRL_MODE_LOCK_ATTITUDE
        || (! control.lockAttitude.isReady))
        return -1;
    //
    quaternion_mult(&(control.lockAttitude.target),
                    &(control.lockAttitude.baseAttitude),c);
    //
    ctrl_lockAttitude_output();
    return 0;
}

int32_t ctrl_lockAttitude_setBaseThrottle(float baseThrottle)
{
    if(control.mode != CTRL_MODE_LOCK_ATTITUDE
        || (! control.lockAttitude.isReady))
        return -1;
    //
    control.lockAttitude.baseThrottle = baseThrottle;
    ctrl_lockAttitude_output();
    return 0;
}

const float * ctrl_getCurrentThrottle(void)
{
    return control.throttle;
}
