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

// app/conductor/conductor.h
// 2013-6-7 17:55:04
// js200300953

#ifndef __FILE_APP_CONDUCTOR_CONDUCTOR_H__
#define __FILE_APP_CONDUCTOR_CONDUCTOR_H__

#include <stdint.h>

typedef  struct
{
    float w;
    float x;
    float y;
    float z;
}quaternion;

int32_t conductor_init(void);
int32_t conductor_checkEvent(void);
int32_t conductor_readBatteryVoltage(void);
float   conductor_getBatteryVoltage(void);
int32_t conductor_getReadyToFly(void);
int32_t conductor_halt(void);
int32_t conductor_sendControl(float force,const quaternion *target);
int32_t conductor_onReceivedPacket(const uint8_t packet[],int32_t length);
int32_t actFly_onCheckEvent(void);
int32_t actFly_onKeyEvent(int32_t act_,int32_t key,int32_t event,int32_t param);

#endif // __FILE_APP_CONDUCTOR_CONDUCTOR_H__
