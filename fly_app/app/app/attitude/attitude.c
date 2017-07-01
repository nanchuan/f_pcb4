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

// app/attitude/attitude.c
// 2012-11-7 18:12:59
// js200300953

#include "bsp/time.h"
#include "app/math/interface.h"
#include "attitude.h"
#include "accfilter.h"
#include "gyrfilter.h"
#include "magfilter.h"
#include "mix.h"

static quaternion attitude;

void attitude_init(void);
const quaternion * attitude_getAttitude(void);
void attitude_inputAcc(const float acc[3]);
void attitude_inputGyr(const float gyr[3]);
void attitude_inputMag(const float mag[3]);
void attitude_mixAccGyrMag(void);

void attitude_init(void)
{
    accfilter_init();
    gyrfilter_init();
    magfilter_init();
    //
    quaternion_loadIdentity(&attitude);
}

// ��ȡ��̬��ֻ����
const quaternion * attitude_getAttitude(void)
{
    return &attitude;
}

// acc : ���ٶȲ���ֵ��m/s2��
void attitude_inputAcc(const float acc[3])
{
    accfilter_input(acc);
}

// gyr : ���ٶȲ���ֵ��rad/s��
void attitude_inputGyr(const float gyr[3])
{
    gyrfilter_input(gyr);
}

// mag : �ų��������ֵ���궨������Ϊ1��
void attitude_inputMag(const float mag[3])
{
    magfilter_input(mag);
}

void attitude_mixGyrAccMag(void)
{
    //
    // ������ּ�����ж��Ƿ����
    static uint64_t time_pre_us = 0;
    uint64_t time_now_us = time_nowUs();
    int32_t time_interval_us = time_now_us - time_pre_us;
    time_pre_us = time_now_us;
    if(time_interval_us > 1000*1000) // ����1�����Ϊ�쳣��������
        return;
    float time_interval_s = time_interval_us * (1.0f/1e6f);
    //
    //mix_gyrAccMag_crossMethod(&attitude,gyrfilter_current(),accfilter_getCurrent(),magfilter_getCurrent(),time_interval_s);
    mix_gyrAcc_crossMethod(&attitude,gyrfilter_current(),accfilter_getCurrent(),time_interval_s);
}
