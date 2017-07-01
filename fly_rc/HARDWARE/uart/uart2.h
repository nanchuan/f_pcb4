//     Copyright (c) 2013 js200300953@qq.com All rights reserved.
//         ==================================================
//         ========Բ�㲩ʿ΢�����������������������========
//         ==================================================
//     Բ�㲩ʿ΢�������������������������λ��������λ��Bootloader
// ����λ��App��ң�س��򣬼����ǵ�Դ���룬�����ܳơ���������
//     �������ṩ�ο���js200300953�����������κε�����������ʹ�ø�����
// �����ֵ���ʧ����
//     ����������ѧϰΪĿ���޸ĺ�ʹ�ã�������������ҵ��Ŀ��ʹ�ø�������
//     �޸ĸ�����ʱ�����뱣��ԭ��Ȩ������
// 
//     �������ϼ���
// http://blog.sina.com.cn/js200300953
// http://www.etootle.com/
// http://www.eeboard.com/bbs/forum-98-1.html#separatorline
// Բ�㲩ʿ΢�����������QQȺ��276721324

// bsp/uart1.h
// 2013-6-1 13:46:18
// js200300953

#ifndef __FILE_BSP_UART1_H__
#define __FILE_BSP_UART1_H__

#include <stdint.h>

enum
{
    UART2_SUCCEED = 0,
    UART2_FAILED  = 1
};

void    uart2_init(void);
uint8_t uart2_transmit(const void * data,uint32_t len);
int32_t uart2_receiveValidBufferSize(void);
int32_t uart2_readReceiveBuffer(void * outputBuffer,int32_t outputBufferLength);

uint8_t  uart2_receivePushToBuffer(uint8_t data);

#endif // __FILE_BSP_UART1_H__