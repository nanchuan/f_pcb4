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

// app/communication/engine.h
// 2013-6-1 16:09:59
// js200300953

#ifndef __FILE_APP_COMMUNICATION_ENGINE_H__
#define __FILE_APP_COMMUNICATION_ENGINE_H__

#include <stdint.h>

int32_t engine_init(void);
void    engine_checkEvent(void);
int32_t engine_handlePacket_serial(const uint8_t * packet,int32_t length);
int32_t engine_handlePacket_nrf(const uint8_t * packet,int32_t length);
int32_t engine_transmitBegin(uint8_t packetType);
int32_t engine_transmitContent(const void * part,int32_t length);
int32_t engine_transmitEnd(void);
int32_t engine_maxTransmitContentSize(void);

#endif // __FILE_APP_COMMUNICATION_ENGINE_H__
