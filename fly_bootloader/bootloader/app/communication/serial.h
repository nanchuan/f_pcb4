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

// app/communication/serial.h
// 2013-6-1 18:11:55
// js200300953

#ifndef __FILE_APP_COMMUNICATION_SERIAL_H__
#define __FILE_APP_COMMUNICATION_SERIAL_H__

#include <stdint.h>

#define COM_SERIAL_CONTENT_SIZE_MAX (32-1-1-2)

int32_t com_serial_init(void);
void    com_serial_checkEvent(void);
int32_t com_serial_transmitBegin(uint8_t packetType);
int32_t com_serial_transmitContent(const void * part,int32_t length);
int32_t com_serial_transmitEnd(void);

#endif // __FILE_APP_COMMUNICATION_SERIAL_H__
