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

// app/status/status.c
// 2013-1-9 14:10:04
// js200300953

#include "bsp/time.h"
#include "bsp/battery.h"
#include "app/exception/interface.h"
#include "status.h"

static status_t status;

void status_init(void);
void status_checkEvent(void);

void status_init(void)
{
    for(int i=0;i<sizeof(status);i++)
        ((uint8_t *)&status)[i] = 0;
    status.bateryVoltage = 4.2;
}

void status_checkEvent(void)
{
    TIME_EXECUTE_PERIODICALLY_BEGIN(100)
    {
        const static float RATE = 0.05;
        status.bateryVoltage = (1-RATE) * status.bateryVoltage + RATE * battery_getVoltage();
        //
        exception_checkBateryVoltage(status.bateryVoltage);
    }TIME_EXECUTE_PERIODICALLY_END()
}

float status_getBateryVoltage(void)
{
    return status.bateryVoltage;
}
