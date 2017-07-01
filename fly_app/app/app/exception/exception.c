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

// app/exception/exception.h
// 2013-1-9 13:48:35
// js200300953

#include <bsp/time.h>
#include <app/communication/interface.h>
#include <app/control/interface.h>
#include <app/status/interface.h>
#include <app/param/interface.h>
#include <app/math/interface.h>
#include "exception.h"

void exception_init(void);
void exception_checkEvent(void);
//static void exception_halt(void);
void exception_LowBateryVoltage(void);
void exception_checkInclination(const quaternion * attitude);

void exception_init(void)
{
}

void exception_checkEvent(void)
{
}

/*
static void exception_halt(void)
{
    while(1)
    {
    }
}
*/

void exception_checkBateryVoltage(float voltage)
{
    if(voltage < param_getRamParam()->haltVoltage)
    {
        ctrl_enterHaltMode();
        cmd_returnMessage(
            "\xE7\x94\xB5\xE5\x8E\x8B\xE8\xBF" \
            "\x87\xE4\xBD\x8E\xEF\xBC\x8C\xE8" \
            "\xAF\xB7\xE5\x85\x85\xE7\x94\xB5" 
            "\xE3\x80\x82");//��ѹ���ͣ����硣
    }
}

void exception_checkInclination(const quaternion * attitude)
{
    quaternion q = *attitude;
    quaternion_normalize(&q);
    //
    float cos_incli = 1 - 2*(q.x*q.x + q.y*q.y); // �������ֵ��
    //
    if(cos_incli < param_getRamParam()->haltInclination_cos)
    {
        ctrl_enterHaltMode();
        cmd_returnMessage("\xE5\x80\xBE\xE6\x96\x9C\xE5\x81\x9C\xE6\x9C\xBA");//"��бͣ��"
    }
}
