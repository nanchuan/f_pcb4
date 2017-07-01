//     Copyright (c) 2013 js200300953@qq.com All rights reserved.
//         ==================================================
//         ========圆点博士微型四轴飞行器配套软件声明========
//         ==================================================
//     圆点博士微型四轴飞行器配套软件包括上位机程序、下位机Bootloader
// 、下位机App和遥控程序，及它们的源代码，以下总称“软件”。
//     软件仅提供参考，js200300953不对软件作任何担保，不对因使用该软件
// 而出现的损失负责。
//     软件可以以学习为目的修改和使用，但不允许以商业的目的使用该软件。
//     修改该软件时，必须保留原版权声明。
// 
//     更多资料见：
// http://blog.sina.com.cn/js200300953
// http://www.etootle.com/
// http://www.eeboard.com/bbs/forum-98-1.html#separatorline
// 圆点博士微型四轴飞行器QQ群：276721324

// app/conductor/conductor.c
// 2013-6-7 17:55:16
// js200300953

#include <math.h>
#include "time.h"
#include "protocol.h"
#include "nrf24l01p.h"
#include "joystick.h"
#include "input.h"
#include "conductor.h"
#include "led.h"

#define com_transmitBegin(packetType)    com_nrf24l01p_transmitBegin(packetType)
#define com_transmitContent(part,length) com_nrf24l01p_transmitContent(part,length)
#define com_transmitEnd()                com_nrf24l01p_transmitEnd()


static float g_batteryVoltage = 3.7;

int32_t conductor_init(void)
{
    return 0;
}

int32_t conductor_checkEvent(void)
{
    return 0;
}

int32_t conductor_readBatteryVoltage(void)
{
    if(com_transmitBegin(PROTOCOL_GET_STATUS) != 0)
        return -1;
    //
    uint8_t content[] = {PROTOCOL_STATUS_BATERY_VOLTAGE};
    com_transmitContent(content,sizeof(content));
    com_transmitEnd();
    //
    return 0;
}

float conductor_getBatteryVoltage(void)
{
    return g_batteryVoltage;
}

int32_t conductor_getReadyToFly(void)
{
    if(com_transmitBegin(PROTOCOL_SET_CONTROL_MODE) != 0)
        return -1;
    uint8_t cmd = PROTOCOL_SET_CONTROL_MODE_LOCK_ATTITUDE;
    com_transmitContent(&cmd,sizeof(cmd));
    com_transmitEnd();
    //
    if(com_transmitBegin(PROTOCOL_LOCK_ATTITUDE) != 0)
        return -1;
    cmd = PROTOCOL_LOCK_ATTITUDE_GET_READY;
    com_transmitContent(&cmd,sizeof(cmd));
    com_transmitEnd();
    //
    return 0;
}

int32_t conductor_halt(void)
{
    if(com_transmitBegin(PROTOCOL_SET_CONTROL_MODE) != 0)
        return -1;
    uint8_t cmd = PROTOCOL_SET_CONTROL_MODE_HALT;
    com_transmitContent(&cmd,sizeof(cmd));
    com_transmitEnd();
    //
    return 0;
}

int32_t conductor_sendControl(float force,const quaternion *target)
{
    if(com_transmitBegin(PROTOCOL_LOCK_ATTITUDE) != 0)
        return -1;
    //
    uint8_t cmd = PROTOCOL_LOCK_ATTITUDE_CONTROL;
    com_transmitContent(&cmd,sizeof(cmd));
    com_transmitContent(&force,sizeof(force));
    com_transmitContent(target,sizeof(*target));
    com_transmitEnd();
    //								
    return 0;
}

// packet:|type|content|。
int32_t conductor_onReceivedPacket(const uint8_t packet[],int32_t length)
{
    switch(packet[0])
    {
    case PROTOCOL_RETURN_STATUS :
        if(length == 6 && packet[1] == (uint8_t)PROTOCOL_STATUS_BATERY_VOLTAGE)
        {
            float v = 0;
            for(int32_t i=0;i<4;i++)
                ((uint8_t *)&v)[i] = packet[2+i];
            //
            if(v > 0 && v < 6)
                g_batteryVoltage = v;
        }
        break;
    }
    //
    return 0;
}

static const float ACTFLY_JOYSTICK_CHANGE_THROTTLE = 0.005;
static const float ACTFLY_ANGLE_HALF_RANGE = 10 * (3.1415926f / 180.0f);

void quaternion_normalize(quaternion * q)
{
    float norm_r = 1.0f / sqrt(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z);
    q->w *= norm_r;
    q->x *= norm_r;
    q->y *= norm_r;
    q->z *= norm_r;
}

#include "serial.h"
// 计算油门和目标姿态。
// 返回值：
//   0 => 摇杆值改变了，油门和目标姿态已计算好。
//   1 => 摇杆值没改变，油门和目标姿态没计算。
int32_t actFly_makeForceAndTarget(float *force,quaternion *target)
{
    static joystick_t pre = {0,0,0,0};
    joystick_t cur = joystickFilter_getValue();
    //
    
		//if(com_serial_transmitBegin(PROTOCOL_RETURN_STATUS) != 0)
    //    return -1;
    
    //uint8_t cmdtmp = PROTOCOL_STATUS_CURRENT_THROTTLE;
    //com_serial_transmitContent(&cmdtmp,1);
    //com_serial_transmitContent(&cur,sizeof(cur));
    //com_serial_transmitEnd();
		
    if(    fabs(cur.a_vertical   - pre.a_vertical)   < ACTFLY_JOYSTICK_CHANGE_THROTTLE
        && fabs(cur.b_vertical   - pre.b_vertical)   < ACTFLY_JOYSTICK_CHANGE_THROTTLE
        && fabs(cur.b_horizontal - pre.b_horizontal) < ACTFLY_JOYSTICK_CHANGE_THROTTLE)
        return 1;
    pre = cur;
    //
    *force = cur.a_vertical;
    if(*force < 0)
        *force = 0;
    if(*force > 1)
        *force = 1;
    //
    target->x =   cur.b_vertical * ACTFLY_ANGLE_HALF_RANGE;
    target->y = - cur.b_horizontal * ACTFLY_ANGLE_HALF_RANGE;
    float tmp = 1 - target->x*target->x - target->y*target->y;
    if(tmp > 0)
        target->w = sqrtf(tmp);
    else
    {
        target->w = 0;
        quaternion_normalize(target);
    }
    //
    return 0;
}

int32_t actFly_onCheckEvent()
{
    TIME_EXECUTE_PERIODICALLY_BEGIN(33)
    {
        static float force = 0;
        static quaternion targetxx = {1,0,0,0};
        if(actFly_makeForceAndTarget(&force,&targetxx) == 0)
            conductor_sendControl(force,&targetxx);
    }TIME_EXECUTE_PERIODICALLY_END()
    //
    TIME_EXECUTE_PERIODICALLY_BEGIN(1117)
    {
        conductor_readBatteryVoltage();
    }TIME_EXECUTE_PERIODICALLY_END()
    //
    return 0;
}

int32_t actFly_onKeyEvent(int32_t act_,int32_t key,int32_t event,int32_t param)
{
    switch(key)
    {
    case INPUT_KEY_1 :
				if(event == INPUT_EVENT_DOWN)
				{
        		if(com_transmitBegin(PROTOCOL_SET_CONTROL_MODE) != 0)
				        return -1;
				    uint8_t cmd = PROTOCOL_SET_CONTROL_MODE_LOCK_ATTITUDE;
				    com_transmitContent(&cmd,sizeof(cmd));
				    com_transmitEnd();
						LED1_ON;
				}
				else if(event == INPUT_EVENT_UP)
				{
						if(com_transmitBegin(PROTOCOL_LOCK_ATTITUDE) != 0)
				        return -1;
				    uint8_t cmd = PROTOCOL_LOCK_ATTITUDE_GET_READY;
				    com_transmitContent(&cmd,sizeof(cmd));
				    com_transmitEnd();
				}
        break;
    case INPUT_KEY_2 :
        if(event == INPUT_EVENT_DOWN)
        {
						conductor_halt();
						LED1_OFF;
        }
        break;
    }
    return 0;
}

