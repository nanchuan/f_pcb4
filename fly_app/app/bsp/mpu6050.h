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

// bsp/mpu6050.h
// 2013-1-7 18:47:38
// js200300953

#ifndef __FILE_BSP_MPU6050_H__
#define __FILE_BSP_MPU6050_H__

#include <stdint.h>

enum
{
    MPU6050_SUCCEED = 0,
    MPU6050_FAILED = 1,
};

int32_t mpu6050_init(void);
int32_t mpu6050_dataIsReady(void);
int32_t mpu6050_read(void);
int32_t mpu6050_getRawAcc(int16_t acc[3]);
int32_t mpu6050_getRawGyr(int16_t gyr[3]);
int32_t mpu6050_getCalibratedAcc(float acc[3]);
int32_t mpu6050_getCalibratedGyr(float gyr[3]);

#endif // __FILE_BSP_MPU6050_H__
