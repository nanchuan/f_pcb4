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

// main.c
// 2013-1-7 10:51:49
// js200300953

#include "bsp/bsp.h"
#include "app/communication/interface.h"
#include "app/sensor/interface.h"
#include "app/exception/interface.h"
#include "app/attitude/interface.h"
#include "app/control/interface.h"
#include "app/status/interface.h"
#include "app/param/interface.h"

int main(void)
{
    bsp_init();
    param_init();
    //
    communication_init();
    sensor_init();
    attitude_init();
    exception_init();
    status_init();
    ctrl_init();
    //
    while(1)
    {
        communication_checkEvent();
        sensor_checkEvent();
        ctrl_checkEvent();
        exception_checkEvent();
        status_checkEvent();
    }
}
