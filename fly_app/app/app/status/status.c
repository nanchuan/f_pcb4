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

// app/status/status.c
// 2013-1-9 14:10:04
// js200300953

#include "bsp/time.h"
#include "bsp/battery.h"
#include "app/exception/interface.h"
#include "status.h"

static status_t status;

void status_init(void);
void status_checkEvent(void);

void status_init(void)
{
    for(int i=0;i<sizeof(status);i++)
        ((uint8_t *)&status)[i] = 0;
    status.bateryVoltage = 4.2;
}

void status_checkEvent(void)
{
    TIME_EXECUTE_PERIODICALLY_BEGIN(100)
    {
        const static float RATE = 0.05;
        status.bateryVoltage = (1-RATE) * status.bateryVoltage + RATE * battery_getVoltage();
        //
        exception_checkBateryVoltage(status.bateryVoltage);
    }TIME_EXECUTE_PERIODICALLY_END()
}

float status_getBateryVoltage(void)
{
    return status.bateryVoltage;
}
