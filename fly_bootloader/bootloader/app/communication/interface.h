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

// app/communication/interface.h
// 2013-1-8 12:55:11
// js200300953

#ifndef __FILE_APP_COMMUNICATION_INTERFACE_H__
#define __FILE_APP_COMMUNICATION_INTERFACE_H__

#include <stdint.h>
#include "protocol.h"

void communication_init(void);
void communication_checkEvent(void);
int32_t cmd_returnMessage(const char * msg);
int32_t engine_transmitBegin(uint8_t packetType);
int32_t engine_transmitContent(const void * part,int32_t length);
int32_t engine_transmitEnd(void);
int32_t engine_maxTransmitContentSize(void);

#define com_transmitBegin(packetType)       engine_transmitBegin(packetType)
#define com_transmitContent(part,length)  engine_transmitContent(part,length)
#define com_transmitEnd()                   engine_transmitEnd()
#define com_maxTransmitContentSize()        engine_maxTransmitContentSize()

#endif // __FILE_APP_COMMUNICATION_INTERFACE_H__
