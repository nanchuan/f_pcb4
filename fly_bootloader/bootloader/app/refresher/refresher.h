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

// app/refresher/refresher.h
// 2013-2-21 21:04:21
// js200300953

#ifndef __FILE_APP_REFRESHER_REFRESHER_H__
#define __FILE_APP_REFRESHER_REFRESHER_H__

#include <stdint.h>

int32_t refresher_init(void);
int32_t refresher_checkApp(void);
int32_t refresher_setAppSizeCrc(uint32_t size_block,uint32_t crc);
int32_t refresher_eraseApp(void);
int32_t refresher_write(const uint32_t *data,int32_t length,int32_t blockIndexOfFirst);
int32_t refresher_isWriteComplete(void);
int32_t refresher_getRemainBlock(uint8_t * buffer,int32_t bufferLength);
int32_t refresher_getWrittenAmount_block(void);
int32_t refresher_jumpToApp(void);

#endif // __FILE_APP_REFRESHER_REFRESHER_H__
