//     Copyright (c) 2013 js200300953@qq.com All rights reserved.
//         ==================================================
//         ========Բ�㲩ʿ΢����������������������========
//         ==================================================
//     Բ�㲩ʿ΢������������������������λ��������λ��Bootloader
// ����λ��App��ң�س��򣬼����ǵ�Դ���룬�����ܳơ��������
//     ������ṩ�ο���js200300953����������κε�����������ʹ�ø����
// �����ֵ���ʧ����
//     ���������ѧϰΪĿ���޸ĺ�ʹ�ã�������������ҵ��Ŀ��ʹ�ø������
//     �޸ĸ����ʱ�����뱣��ԭ��Ȩ������
// 
//     �������ϼ���
// http://blog.sina.com.cn/js200300953
// http://www.etootle.com/
// http://www.eeboard.com/bbs/forum-98-1.html#separatorline
// Բ�㲩ʿ΢�����������QQȺ��276721324

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
