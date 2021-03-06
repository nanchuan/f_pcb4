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

// app/control/control.h
// 2013-1-8 16:30:06
// js200300953

#ifndef __FILE_APP_CONTROL_CONTROL_H__
#define __FILE_APP_CONTROL_CONTROL_H__

#include "app/math/interface.h"
#include "pid.h"

void    ctrl_init(void);
void    ctrl_checkEvent(void);
void    ctrl_enterHaltMode(void);
void    ctrl_enterLockAttitudeMode(void);
void    ctrl_enterLockThrottleMode(void);
int32_t ctrl_lockAttitude_getReady(void);
int32_t ctrl_lockAttitude_control(const quaternion * delta);
int32_t ctrl_lockAttitude_setBaseThrottle(float baseThrottle);
void ctrl_lockThrottle_setThrottle(const float th[4]);
const float * ctrl_getCurrentThrottle(void);

#endif // __FILE_APP_CONTROL_CONTROL_H__
