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

// app/param/param.h
// 2013-4-8 9:07:11
// js200300953

#ifndef __FILE_APP_PARAM_PARAM_H__
#define __FILE_APP_PARAM_PARAM_H__

#include <stdint.h>
#include <app/math/interface.h>

// ��������Ľṹ�壬��Աλ�ò������ġ�
typedef struct
{
    // ������Ϣ��pos=0��
    uint8_t version; // �����ṹ�İ汾��
    uint8_t length;  // �����ܳ��ȣ���λ���֣����ֽ���/4��
    uint16_t reserve_zero_0;
    //
    // ������У��������pos=[1,18]��
    float acc_offset[3];
    float acc_gain[3];
    float gyr_offset[3];
    float gyr_gain[3];
    float mag_offset[3];
    float mag_gain[3];
    //
    // ��Ԫ����ʾ��ˮƽ�棬pos=[19,22]��
    quaternion horizon;
    //
    // ���Ʋ�����[xyz][pid]��pos=[23,31]��
    struct
    {
        float p,i,d; // ˳���ϸ�P��I��D��
    }xyz_pid[3];
    //
    // ͣ����ص�ѹ����λ��V��pos=32��
    float haltVoltage;
    //
    // ��б������ǵ����ң�pos=33��
    float haltInclination_cos;
    //
    uint32_t crc;
} param_t;

int32_t         param_init(void);
const param_t * param_getRamParam(void);
param_t *       param_setRamParam(void);
const param_t * param_getFlashParam(void);
int32_t         param_writeToFlash(void);
int32_t         param_resetFlash(void);

#endif // __FILE_APP_PARAM_PARAM_H__
