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

// bsp/mpu6050.c
// 2013-1-7 18:47:54
// js200300953

#include "mpu6050.h"
#include "i2c.h"
#include "time.h"
#include <app/param/interface.h>

#define MPU6050_ADDRESS 0xD0
#define MPU6050_REG_RAW_DATA 0x3B
#define MPU6050_REG_ACC 0x3B
#define MPU6050_REG_TEM 0x41
#define MPU6050_REG_GYR 0x43

static struct
{
    int16_t acc[3];
    int16_t tem;
    int16_t gyr[3];
}mpu6050_rawData;

int32_t mpu6050_init(void);
int32_t mpu6050_isDataReady(void);
int32_t mpu6050_read(void);
int32_t mpu6050_getRawAcc(int16_t acc[3]);
int32_t mpu6050_getRawGyr(int16_t gyr[3]);
int32_t mpu6050_getCalibratedAcc(float acc[3]);
int32_t mpu6050_getCalibratedGyr(float gyr[3]);

int32_t mpu6050_init(void)
{
    for(int i=0;i<sizeof(mpu6050_rawData);i++)
        ((uint8_t *)&mpu6050_rawData)[i] = 0;
    //
    // ���ID��
    uint8_t id = 0;
    if(i2c_read(MPU6050_ADDRESS,0x75,&id,1) != 0)
        return MPU6050_FAILED;
    if(id != 0x68)
        return MPU6050_FAILED;
    //
    // ����MPU6050�Ĳ�����
    const uint8_t param[][2] = 
    {
        // {�Ĵ�����ַ,�Ĵ���ֵ},
        {0x6B,1     }, // �˳�˯��ģʽ����ȡ��ʱ��Ϊ����X�ᡣ
        {0x19,4     }, // ȡ��ʱ��4��Ƶ��1k/4��ȡ����Ϊ25Hz��
        {0x1A,2     }, // ��ͨ�˲�����ֹƵ��100Hz���ҡ�
        {0x1B,3<<3  }, // �������̣�2000dps��
        {0x1C,2<<3  }, // ���ٶȼ����̣�8g��
        {0x37,0x32  }, // �ж��ź�Ϊ�ߵ�ƽ�����������ֱ���ж�ȡ��������ʧ��ֱͨ����I2C��
        {0x38,1     }, // ʹ�á�����׼���á��жϡ�
        {0x6A,0x00  }, // ��ʹ�ø���I2C��
    };
    for(int i=0;i<sizeof(param)/2;i++)
    {
        if(i2c_write(MPU6050_ADDRESS,param[i][0],&(param[i][1]),1) != 0)
            return MPU6050_FAILED;
        //
        uint8_t check = 0;
        if(i2c_read(MPU6050_ADDRESS,param[i][0],&check,1) != 0)
            return MPU6050_FAILED;
        if(check != param[i][1])
            return MPU6050_FAILED;
    }
    //
    return MPU6050_SUCCEED;
}

int32_t mpu6050_dataIsReady(void)
{
    static uint32_t target = 0;
    uint32_t now = time_nowMs();
    if(target <= now)
    {
        target = now + 3;//333Hz
        //
        return 1;
    }
    return 0;
}

int32_t mpu6050_read(void)
{
    if(i2c_read(MPU6050_ADDRESS,MPU6050_REG_RAW_DATA,&mpu6050_rawData
        ,sizeof(mpu6050_rawData)) != 0)
        return MPU6050_FAILED;
    //
    // ���תС�ˡ�
    uint8_t * p = (uint8_t *)&mpu6050_rawData;
    for(int i=0;i<sizeof(mpu6050_rawData)/2;i++)
    {
        uint8_t tmp = p[2*i];
        p[2*i] = p[2*i + 1];
        p[2*i + 1] = tmp;
    }
    //
    return MPU6050_SUCCEED;
}

// acc : LSB
int32_t mpu6050_getRawAcc(int16_t acc[3])
{
    for(int i=0;i<3;i++)
        acc[i] = mpu6050_rawData.acc[i];
    //
    return MPU6050_SUCCEED;
}

//gyr : LSB
int32_t mpu6050_getRawGyr(int16_t gyr[3])
{
    for(int i=0;i<3;i++)
        gyr[i] = mpu6050_rawData.gyr[i];
    //
    return MPU6050_SUCCEED;
}

// acc : m/s2
int32_t mpu6050_getCalibratedAcc(float acc[3])
{
    const param_t *p = param_getRamParam();
    //
    for(int i=0;i<3;i++)
        acc[i] = (p->acc_offset[i] + mpu6050_rawData.acc[i]) * p->acc_gain[i];
    //
    return MPU6050_SUCCEED;
}

// gyr : rad/s
int32_t mpu6050_getCalibratedGyr(float gyr[3])
{
    const param_t *p = param_getRamParam();
    //
    for(int i=0;i<3;i++)
        gyr[i] = (p->gyr_offset[i] + mpu6050_rawData.gyr[i]) * p->gyr_gain[i];
    //
    return MPU6050_SUCCEED;
}
