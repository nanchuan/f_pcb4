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

// app/conductor/conductor.h
// 2013-6-7 17:55:04
// js200300953

#ifndef __FILE_APP_CONDUCTOR_CONDUCTOR_H__
#define __FILE_APP_CONDUCTOR_CONDUCTOR_H__

#include <stdint.h>

typedef  struct
{
    float w;
    float x;
    float y;
    float z;
}quaternion;

int32_t conductor_init(void);
int32_t conductor_checkEvent(void);
int32_t conductor_readBatteryVoltage(void);
float   conductor_getBatteryVoltage(void);
int32_t conductor_getReadyToFly(void);
int32_t conductor_halt(void);
int32_t conductor_sendControl(float force,const quaternion *target);
int32_t conductor_onReceivedPacket(const uint8_t packet[],int32_t length);
int32_t actFly_onCheckEvent(void);
int32_t actFly_onKeyEvent(int32_t act_,int32_t key,int32_t event,int32_t param);

#endif // __FILE_APP_CONDUCTOR_CONDUCTOR_H__
