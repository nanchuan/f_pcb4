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
        // 简单、暴力的高通。
        const static float TH = 0.01; // 阈值，rad/s。
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
