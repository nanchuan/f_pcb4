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

// bsp/i2c.h
// 2013-1-7 18:07:22
// js200300953

#ifndef __FILE_BSP_I2C_H__
#define __FILE_BSP_I2C_H__

#include "stm32f10x.h"

void i2cInit(void);
int32_t i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
int32_t i2cWriteBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data);

#define i2c_init() i2cInit()
#define i2c_write(dev,reg,data,len) (!i2cWriteBuffer((dev)>>1,reg,len,(uint8_t *)data))
#define i2c_read(dev,reg,data,len) (!i2cRead((dev)>>1,reg,len,(uint8_t *)data))

/*
// bw for busy wait
void i2c_bw_init(void);
uint32_t i2c_bw_write(uint8_t dev,uint8_t reg,const void * data,uint32_t len);
uint32_t i2c_bw_read(uint8_t dev,uint8_t reg,void * data,uint32_t len);
*/

#endif // __FILE_BSP_I2C_H__
