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

// app/input/input.c
// 2013-6-9 16:13:40
// js200300953

#include "joystick.h"
#include "time.h"
#include "input.h"
#include "conductor.h"

#define INPUT_KEY_AMOUNT 10
#define INPUT_DOUBLE_CLICK_INTERVAL_MS 300

static int32_t input_currentObject = 0; // ��ǰ����
static input_keyEventCallback_t g_keyEventCallback = 0;
static int32_t g_periodFromFirstDown[INPUT_KEY_AMOUNT]; // �����������жϳ�����
static uint32_t g_timeFromLastDown[INPUT_KEY_AMOUNT]; // �����ж�˫����
static const int32_t g_keyList[INPUT_KEY_AMOUNT] = 
{
    INPUT_KEY_1                 ,
    INPUT_KEY_2                 ,
    /*INPUT_KEY_3                 ,
    INPUT_KEY_4                 ,
    INPUT_KEY_JOYSTICK_A_BUTTON ,
    INPUT_KEY_JOYSTICK_B_BUTTON ,*/
    //
    INPUT_KEY_JOYSTICK_A_RIGHT  ,
    INPUT_KEY_JOYSTICK_A_UP     ,
    INPUT_KEY_JOYSTICK_A_LEFT   ,
    INPUT_KEY_JOYSTICK_A_DOWN   ,
    //
    INPUT_KEY_JOYSTICK_B_RIGHT  ,
    INPUT_KEY_JOYSTICK_B_UP     ,
    INPUT_KEY_JOYSTICK_B_LEFT   ,
    INPUT_KEY_JOYSTICK_B_DOWN   
};

static int32_t input_getKeyStatus(void);

int32_t input_init(void)
{
    //joystick_init();
    //
    for(int i=0;i<INPUT_KEY_AMOUNT;i++)
    {
        g_periodFromFirstDown[i] = -1;
        g_timeFromLastDown[i] = 0;
    }
    //
		input_setKeyEventCallback(0, actFly_onKeyEvent);
		
    return 0;
}

static const float JOYSTICK_FILTER_FACTOR = 0.5;
static joystick_t g_value = {0,0,0,0};

int32_t joystickFilter_init(void)
{
    return 0;
}

int32_t joystickFilter_refresh(void)
{
    joystick_t cur = joystick_getValue();
    //
    // joystick��ֵ���Կ����ǳ���Ϊ4��float���顣
    for(int i=0;i<4;i++)
    {
        ((float *)&g_value)[i] = ((float *)&g_value)[i] * (1-JOYSTICK_FILTER_FACTOR)
                               + ((float *)&cur)[i] * JOYSTICK_FILTER_FACTOR;
    }
    //
    return 0;
}

joystick_t joystickFilter_getValue(void)
{
    return g_value;
}

int32_t input_checkEvent(void)
{
    if(g_keyEventCallback == 0)
        return 0;
    //
    TIME_EXECUTE_PERIODICALLY_BEGIN(10)
    {
        joystickFilter_refresh(); // ÿ10ms����һ�Ρ�
        //
        uint32_t now = time_nowMs();
        int32_t keyStatus = input_getKeyStatus();
        //
        for(int i=0;i<INPUT_KEY_AMOUNT;i++)
        {
            if(keyStatus & g_keyList[i])
            {
                // ���µ�״̬��
                //
                g_periodFromFirstDown[i] ++;
                //
                if(g_periodFromFirstDown[i] == 0)
                {
                    // ��һ�μ�⵽���£����ԡ�
                }
                else if(g_periodFromFirstDown[i] == 1)
                {  
                    // �ж�˫����
                    if((g_timeFromLastDown[i] + INPUT_DOUBLE_CLICK_INTERVAL_MS) > now)
                        g_keyEventCallback(input_currentObject,g_keyList[i],INPUT_EVENT_DOUBLE_CLICK,
                                            g_periodFromFirstDown[i]);
										else// ��2�μ�⵽����״̬������Down�¼���
                    		g_keyEventCallback(input_currentObject,g_keyList[i],INPUT_EVENT_DOWN,
                                        g_periodFromFirstDown[i]);
                    g_timeFromLastDown[i] = now;
                }
                else if(g_periodFromFirstDown[i] >= 30 && g_periodFromFirstDown[i]%10 == 0)
                {
                    // ����30�μ�⵽����״̬������Press�¼���
                    g_keyEventCallback(input_currentObject,g_keyList[i],INPUT_EVENT_PRESS,
                                        g_periodFromFirstDown[i]);
                }
            }
            else if(g_periodFromFirstDown[i] >= 0)
            {
                // ����״̬�������ϴ��ǰ���״̬��
                //
                if(g_periodFromFirstDown[i] >= 1)
                {
                    // ����1�μ�⵽����״̬���ͷţ��������ͷŰ���������Ϊ�����󴥷���
                    g_keyEventCallback(input_currentObject,g_keyList[i],INPUT_EVENT_UP,
                                        g_periodFromFirstDown[i]);
                }
                g_periodFromFirstDown[i] = -1;
            }
            //
            // ����������ǰ������ֵ����״̬��
        }
    }TIME_EXECUTE_PERIODICALLY_END()
    //
    return 0;
}

int32_t input_setKeyEventCallback(int32_t this_,input_keyEventCallback_t callback)
{
    input_currentObject = this_;
    g_keyEventCallback = callback;
    return 0;
}

int32_t input_getKeyStatus(void)
{
    int32_t keyStatus = key_getKeyValue();
    static int32_t preKeyStatus = 0;
    joystick_t joy = joystickFilter_getValue();
    //
    const static float DOWN_THROTTLE = 0.4;
    const static float UP_THROTTLE = 0.6;
    //
    // if(�ϴβ��ǰ���״̬�Ļ���joy����ֵ�������UP������Ϊ����
    //    �ϴ��Ѿ��ǰ���״̬�ˣ�joy����ֵֻ�����DOWN�Ϳ�����Ϊ����)
    if(!(preKeyStatus & INPUT_KEY_JOYSTICK_A_RIGHT)  && joy.a_horizontal > UP_THROTTLE || 
        (preKeyStatus & INPUT_KEY_JOYSTICK_A_RIGHT)  && joy.a_horizontal > DOWN_THROTTLE)
           keyStatus |= INPUT_KEY_JOYSTICK_A_RIGHT;
    if(!(preKeyStatus & INPUT_KEY_JOYSTICK_A_LEFT)   && joy.a_horizontal < -UP_THROTTLE || 
        (preKeyStatus & INPUT_KEY_JOYSTICK_A_LEFT)   && joy.a_horizontal < -DOWN_THROTTLE)
           keyStatus |= INPUT_KEY_JOYSTICK_A_LEFT;
    if(!(preKeyStatus & INPUT_KEY_JOYSTICK_A_UP)     && joy.a_vertical   > UP_THROTTLE || 
        (preKeyStatus & INPUT_KEY_JOYSTICK_A_UP)     && joy.a_vertical   > DOWN_THROTTLE)
           keyStatus |= INPUT_KEY_JOYSTICK_A_UP;
    if(!(preKeyStatus & INPUT_KEY_JOYSTICK_A_DOWN)   && joy.a_vertical   < -UP_THROTTLE || 
        (preKeyStatus & INPUT_KEY_JOYSTICK_A_DOWN)   && joy.a_vertical   < -DOWN_THROTTLE)
           keyStatus |= INPUT_KEY_JOYSTICK_A_DOWN;
    //
    if(!(preKeyStatus & INPUT_KEY_JOYSTICK_B_RIGHT)  && joy.b_horizontal > UP_THROTTLE || 
        (preKeyStatus & INPUT_KEY_JOYSTICK_B_RIGHT)  && joy.b_horizontal > DOWN_THROTTLE)
           keyStatus |= INPUT_KEY_JOYSTICK_B_RIGHT;
    if(!(preKeyStatus & INPUT_KEY_JOYSTICK_B_LEFT)   && joy.b_horizontal < -UP_THROTTLE || 
        (preKeyStatus & INPUT_KEY_JOYSTICK_B_LEFT)   && joy.b_horizontal < -DOWN_THROTTLE)
           keyStatus |= INPUT_KEY_JOYSTICK_B_LEFT;
    if(!(preKeyStatus & INPUT_KEY_JOYSTICK_B_UP)     && joy.b_vertical   > UP_THROTTLE || 
        (preKeyStatus & INPUT_KEY_JOYSTICK_B_UP)     && joy.b_vertical   > DOWN_THROTTLE)
           keyStatus |= INPUT_KEY_JOYSTICK_B_UP;
    if(!(preKeyStatus & INPUT_KEY_JOYSTICK_B_DOWN)   && joy.b_vertical   < -UP_THROTTLE || 
        (preKeyStatus & INPUT_KEY_JOYSTICK_B_DOWN)   && joy.b_vertical   < -DOWN_THROTTLE)
           keyStatus |= INPUT_KEY_JOYSTICK_B_DOWN;
    //
    preKeyStatus = keyStatus;
    return keyStatus;
}



