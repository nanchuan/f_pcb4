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

// bsp/joystick.h
// 2013-6-7 15:05:35
// js200300953

#ifndef __FILE_BSP_JOYSTICK_H__
#define __FILE_BSP_JOYSTICK_H__

#include <stdint.h>

// 摇杆的值，大约标准化到[-1,1]。
typedef struct
{
    float a_horizontal;
    float a_vertical;
    float b_horizontal;
    float b_vertical;
}joystick_t;

#ifndef _BV
    #define _BV(n) (1<<(n))
#endif

typedef enum
{
    KEY_1                   = _BV(0),
    KEY_2                   = _BV(1),
}key_t;

typedef enum
{
    KEY_UP   = 0,
    KEY_DOWN = 1,
}keyStatus_t;


int32_t key_getKeyValue(void);
void joystick_init(void);
void joystick_zero(void);
joystick_t joystick_getValue(void);
float battery_getRcVoltage(void);

#endif // __FILE_BSP_JOYSTICK_H__
