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

// app/communication/serial.h
// 2013-6-1 18:11:55
// js200300953

#ifndef __FILE_APP_COMMUNICATION_SERIAL_H__
#define __FILE_APP_COMMUNICATION_SERIAL_H__

#include <stdint.h>

#define COM_SERIAL_CONTENT_SIZE_MAX (32-1-1-2)

int32_t com_serial_init(void);
void    com_serial_checkEvent(void);
int32_t com_serial_transmitBegin(uint8_t packetType);
int32_t com_serial_transmitContent(const void * part,int32_t length);
int32_t com_serial_transmitEnd(void);

#endif // __FILE_APP_COMMUNICATION_SERIAL_H__
