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

// app/engine/engine1.h
// 2013-2-21 22:27:05
// js200300953

#ifndef __FILE_APP_ENGINE_ENGINE1_H__
#define __FILE_APP_ENGINE_ENGINE1_H__

#include <stdint.h>

int32_t engine1_init(void);
int32_t engine1_loop(void);
int32_t engine1_onCmd(const uint8_t *param,int32_t length);
int32_t engine1_onWriteFlash(const uint8_t *param,int32_t length);

#endif // __FILE_APP_ENGINE_ENGINE1_H__
