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

// bsp/rf.h
// 2013-1-7 14:01:42

#ifndef __FILE_BSP_RF_H__
#define __FILE_BSP_RF_H__

#include <stdint.h>

// �ⲿʵ�ֵĺ������ص���
void rf_handleReceivedData(const uint8_t * data,int32_t len);

int32_t     rf_init(void);
int32_t     rf_transmit(const uint8_t * data,uint32_t len);
void        rf_startReceive(void);
void        rf_stopReceive(void);
void        rf_checkEvent(void);

#endif // __FILE_BSP_RF_H__
