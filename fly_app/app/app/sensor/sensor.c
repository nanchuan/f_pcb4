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
// 2012-11-7 13:43:43
// js200300953

#include "bsp/mpu6050.h"
#include "bsp/hmc5883.h"
#include "app/attitude/interface.h"
#include "sensor.h"

void sensor_init(void);
void sensor_checkEvent(void);
void sensor_getRawAcc(int16_t acc[3]);
void sensor_getRawGyr(int16_t gyr[3]);
void sensor_getRawMag(int16_t mag[3]);
void sensor_getCalibratedAcc(float acc[3]);
void sensor_getCalibratedGyr(float gyr[3]);
void sensor_getCalibratedMag(float mag[3]);

void sensor_init(void)
{
}

void sensor_checkEvent(void)
{
    if(mpu6050_dataIsReady())
    {
        float acc[3],gyr[3];
        if(mpu6050_read() != 0)
            return;
        mpu6050_getCalibratedAcc(acc);
        mpu6050_getCalibratedGyr(gyr);
        //
        // �򵥡������ĸ�ͨ��
        const static float TH = 0.01; // ��ֵ��rad/s��
        for(int i=0;i<3;i++)
        {
            if(gyr[i] > -TH && gyr[i] < TH)
                gyr[i] = 0;
        }
        //
        attitude_inputAcc(acc);
        attitude_inputGyr(gyr);
        //
        attitude_mixGyrAccMag();
    }
    //
    /*
    if(hmc5883_isDataReady())
    {
        float mag[3];
        if(hmc5883_read() != 0)
            return;
        hmc5883_getCalibratedMag(mag);
        //
        attitude_inputMag(mag);
    }
    */
}

void sensor_getRawAcc(int16_t acc[3])
{
    mpu6050_getRawAcc(acc);
}

void sensor_getRawGyr(int16_t gyr[3])
{
    mpu6050_getRawGyr(gyr);
}

void sensor_getRawMag(int16_t mag[3])
{
    hmc5883_getRawMag(mag);
}

void sensor_getCalibratedAcc(float acc[3])
{
    mpu6050_getCalibratedAcc(acc);
}

void sensor_getCalibratedGyr(float gyr[3])
{
    mpu6050_getCalibratedGyr(gyr);
}

void sensor_getCalibratedMag(float mag[3])
{
    hmc5883_getCalibratedMag(mag);
}
