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

// app/status/status.h
// 2013-1-9 14:09:49
// js200300953

#ifndef __FILE_APP_STATUS_STATUS_H__
#define __FILE_APP_STATUS_STATUS_H__

#include "app/control/interface.h"
#include "interface.h"

void status_init(void);
void status_checkEvent(void);
float status_getBateryVoltage(void);
//#define status_getCurrentThrottle() ctrl_getCurrentThrottle()

#endif // __FILE_APP_STATUS_STATUS_H__
