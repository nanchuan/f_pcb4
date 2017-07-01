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

// app/exception/exception.h
// 2013-1-9 13:48:35
// js200300953

#include <bsp/time.h>
#include <app/communication/interface.h>
#include <app/control/interface.h>
#include <app/status/interface.h>
#include <app/param/interface.h>
#include <app/math/interface.h>
#include "exception.h"

void exception_init(void);
void exception_checkEvent(void);
//static void exception_halt(void);
void exception_LowBateryVoltage(void);
void exception_checkInclination(const quaternion * attitude);

void exception_init(void)
{
}

void exception_checkEvent(void)
{
}

/*
static void exception_halt(void)
{
    while(1)
    {
    }
}
*/

void exception_checkBateryVoltage(float voltage)
{
    if(voltage < param_getRamParam()->haltVoltage)
    {
        ctrl_enterHaltMode();
        cmd_returnMessage(
            "\xE7\x94\xB5\xE5\x8E\x8B\xE8\xBF" \
            "\x87\xE4\xBD\x8E\xEF\xBC\x8C\xE8" \
            "\xAF\xB7\xE5\x85\x85\xE7\x94\xB5" 
            "\xE3\x80\x82");//电压过低，请充电。
    }
}

void exception_checkInclination(const quaternion * attitude)
{
    quaternion q = *attitude;
    quaternion_normalize(&q);
    //
    float cos_incli = 1 - 2*(q.x*q.x + q.y*q.y); // 倾角余弦值。
    //
    if(cos_incli < param_getRamParam()->haltInclination_cos)
    {
        ctrl_enterHaltMode();
        cmd_returnMessage("\xE5\x80\xBE\xE6\x96\x9C\xE5\x81\x9C\xE6\x9C\xBA");//"倾斜停机"
    }
}
