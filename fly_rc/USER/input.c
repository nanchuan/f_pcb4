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

// app/input/input.c
// 2013-6-9 16:13:40
// js200300953

#include "joystick.h"
#include "time.h"
#include "input.h"
#include "conductor.h"

#define INPUT_KEY_AMOUNT 10
#define INPUT_DOUBLE_CLICK_INTERVAL_MS 300

static int32_t input_currentObject = 0; // 当前对象。
static input_keyEventCallback_t g_keyEventCallback = 0;
static int32_t g_periodFromFirstDown[INPUT_KEY_AMOUNT]; // 用于消抖和判断长按。
static uint32_t g_timeFromLastDown[INPUT_KEY_AMOUNT]; // 用于判断双击。
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
    // joystick的值可以看成是长度为4的float数组。
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
        joystickFilter_refresh(); // 每10ms调用一次。
        //
        uint32_t now = time_nowMs();
        int32_t keyStatus = input_getKeyStatus();
        //
        for(int i=0;i<INPUT_KEY_AMOUNT;i++)
        {
            if(keyStatus & g_keyList[i])
            {
                // 按下的状态。
                //
                g_periodFromFirstDown[i] ++;
                //
                if(g_periodFromFirstDown[i] == 0)
                {
                    // 第一次检测到按下，忽略。
                }
                else if(g_periodFromFirstDown[i] == 1)
                {  
                    // 判断双击。
                    if((g_timeFromLastDown[i] + INPUT_DOUBLE_CLICK_INTERVAL_MS) > now)
                        g_keyEventCallback(input_currentObject,g_keyList[i],INPUT_EVENT_DOUBLE_CLICK,
                                            g_periodFromFirstDown[i]);
										else// 第2次检测到按下状态，触发Down事件。
                    		g_keyEventCallback(input_currentObject,g_keyList[i],INPUT_EVENT_DOWN,
                                        g_periodFromFirstDown[i]);
                    g_timeFromLastDown[i] = now;
                }
                else if(g_periodFromFirstDown[i] >= 30 && g_periodFromFirstDown[i]%10 == 0)
                {
                    // 超过30次检测到按下状态，触发Press事件。
                    g_keyEventCallback(input_currentObject,g_keyList[i],INPUT_EVENT_PRESS,
                                        g_periodFromFirstDown[i]);
                }
            }
            else if(g_periodFromFirstDown[i] >= 0)
            {
                // 弹起状态，而且上次是按下状态。
                //
                if(g_periodFromFirstDown[i] >= 1)
                {
                    // 超过1次检测到按下状态而释放，表明是释放按键，否则为噪声误触发。
                    g_keyEventCallback(input_currentObject,g_keyList[i],INPUT_EVENT_UP,
                                        g_periodFromFirstDown[i]);
                }
                g_periodFromFirstDown[i] = -1;
            }
            //
            // 其他情况就是按键保持弹起的状态。
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
    // if(上次不是按下状态的话，joy绝对值必须大于UP才能判为按下
    //    上次已经是按下状态了，joy绝对值只需大于DOWN就可以判为按下)
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



