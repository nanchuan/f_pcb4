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

// app/status/status.h
// 2013-1-9 14:09:49
// js200300953

#ifndef __FILE_APP_STATUS_STATUS_H__
#define __FILE_APP_STATUS_STATUS_H__

#include "app/control/interface.h"
#include "interface.h"

void status_init(void);
void status_checkEvent(void);
float status_getBateryVoltage(void);
//#define status_getCurrentThrottle() ctrl_getCurrentThrottle()

#endif // __FILE_APP_STATUS_STATUS_H__
