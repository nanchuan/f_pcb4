//     Copyright (c) 2013 js200300953@qq.com All rights reserved.
//         ==================================================
//         ========圆点博士微型四轴飞行器配套软件声明========
//         ==================================================
//     圆点博士微型四轴飞行器配套软件包括上位机程序、下位机Bootloader
// 和下位机App，及它们的源代码，以下总称“软件”。
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

// app/communication/communication.c
// 2013-1-8 12:55:24
// js200300953

#include <bsp/time.h>
#include <bsp/led.h>
#include "communication.h"
#include "engine.h"
#include "cmd.h"

void communication_init(void);
void communication_checkEvent(void);

void communication_init(void)
{
    engine_init();
    //
    cmd_returnBootloaderInfo();
}

void communication_checkEvent(void)
{
    engine_checkEvent();
    //
    TIME_EXECUTE_PERIODICALLY_BEGIN(200)
    {
        led_reverse();
    }TIME_EXECUTE_PERIODICALLY_END()
}

