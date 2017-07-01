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

// bsp/rf.h
// 2013-1-7 14:01:42

#ifndef __FILE_BSP_RF_H__
#define __FILE_BSP_RF_H__

#include <stdint.h>

// 外部实现的函数。回调。
void rf_handleReceivedData(const uint8_t * data,int32_t len);

int32_t     rf_init(void);
int32_t     rf_transmit(const uint8_t * data,uint32_t len);
void        rf_startReceive(void);
void        rf_stopReceive(void);
void        rf_checkEvent(void);

#endif // __FILE_BSP_RF_H__
