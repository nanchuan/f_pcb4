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

// app/sensor/sensor.h
// 2012-11-7 13:42:39
// js200300953

#ifndef __FILE_APP_SENSOR_SENSOR_H__
#define __FILE_APP_SENSOR_SENSOR_H__

#include <stdint.h>

void sensor_init(void);
void sensor_checkEvent(void);
void sensor_getRawAcc(int16_t acc[3]);
void sensor_getRawGyr(int16_t gyr[3]);
void sensor_getRawMag(int16_t mag[3]);
void sensor_getCalibratedAcc(float acc[3]);
void sensor_getCalibratedGyr(float gyr[3]);
void sensor_getCalibratedMag(float mag[3]);

#endif // __FILE_APP_SENSOR_SENSOR_H__
