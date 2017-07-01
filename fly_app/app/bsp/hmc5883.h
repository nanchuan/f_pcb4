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

// bsp/hmc5883.h
// 2012-12-9 14:46:39
// js200300953

#ifndef __FILE_BSP_HMC5883_H__
#define __FILE_BSP_HMC5883_H__

#include <stdint.h>

enum
{
    HMC5883_SUCCEED = 0,
    HMC5883_FAILED = 1,
};

int32_t hmc5883_init(void);
int32_t hmc5883_isDataReady(void);
int32_t hmc5883_read(void);
int32_t hmc5883_getRawMag(int16_t mag[3]);
int32_t hmc5883_getCalibratedMag(float mag[3]);

#endif // __FILE_BSP_HMC5883_H__
