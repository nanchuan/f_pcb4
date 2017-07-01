//     Copyright (c) 2013 js200300953@qq.com All rights reserved.
//         ==================================================
//         ========Բ�㲩ʿ΢����������������������========
//         ==================================================
//     Բ�㲩ʿ΢������������������������λ��������λ��Bootloader
// ����λ��App�������ǵ�Դ���룬�����ܳơ��������
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

// app/communication/communication.c
// 2013-1-8 12:55:24
// js200300953

#include <bsp/time.h>
#include <bsp/led.h>
#include "communication.h"
#include "engine.h"
#include "cmd.h"

void communication_init(void);
void communication_checkEvent(void);

void communication_init(void)
{
    engine_init();
    //
    cmd_returnBootloaderInfo();
}

void communication_checkEvent(void)
{
    engine_checkEvent();
    //
    TIME_EXECUTE_PERIODICALLY_BEGIN(200)
    {
        led_reverse();
    }TIME_EXECUTE_PERIODICALLY_END()
}

