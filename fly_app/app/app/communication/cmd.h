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

// app/communication/cmd.h
// 2013-1-8 14:22:09
// js200300953

#ifndef __FILE_APP_COMMUNICATION_CMD_H__
#define __FILE_APP_COMMUNICATION_CMD_H__

#include <stdint.h>
#include "app/control/interface.h"

void    cmd_getVector(uint8_t type);
void    cmd_getAttitude(void);
void    cmd_setControlMode(uint8_t mode);
int32_t cmd_returnMessage(const char * msg);
void    cmd_getStatus(const uint8_t which[],int32_t amount);
void    cmd_bootloaderCmd(uint8_t cmd,const uint8_t param[],int32_t param_length);
void    cmd_returnAppInfo(void);
void    cmd_parameter(const uint8_t param[],int32_t param_length);
int32_t cmd_lockAttitude(const uint8_t param[],int32_t param_length);

#define cmd_lockThrottle_setThrottle(th)        ctrl_lockThrottle_setThrottle(th)

#endif // __FILE_APP_COMMUNICATION_CMD_H__
