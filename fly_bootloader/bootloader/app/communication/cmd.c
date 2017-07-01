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

// app/communication/cmd.c
// 2013-1-8 14:23:01
// js200300953

#include <stdint.h>
#include <app/engine/interface.h>
#include "protocol.h"
#include "engine.h"
#include "cmd.h"

int32_t cmd_returnMessage(const char * msg);
void    cmd_bootloaderCmd(const uint8_t param[],int32_t param_length);
void    cmd_returnBootloaderInfo(void);

int32_t cmd_returnMessage(const char * msg)
{
    // ȷ���������ݵĳ��ȣ�ȡ�ַ������Ⱥ�����������С��һ����
    int32_t len = 0;
    while(len < engine_maxTransmitContentSize() && msg[len] != '\0')
        len ++;
    //
    if(engine_transmitBegin(PROTOCOL_RETURN_MESSAGE) != 0)
        return 1;
    engine_transmitContent(msg,len);
    engine_transmitEnd();
    //
    return 0;
}

void cmd_bootloaderCmd(const uint8_t param[],int32_t param_length)
{
    engine1_onCmd(param,param_length);
}

void cmd_returnBootloaderInfo(void)
{
    cmd_returnMessage("Bootloader v2.1");
    //cmd_returnMessage(__DATE__);
    //cmd_returnMessage(__TIME__);
}

