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

// app/attitude/attitude.h
// 2013-1-8 15:49:21
// js200300953

#ifndef __FILE_ATTITUDE_ATTITUDE_H__
#define __FILE_ATTITUDE_ATTITUDE_H__

#include <stdint.h>
#include "../math/interface.h"

void attitude_init(void);
const quaternion * attitude_getAttitude(void);
void attitude_inputAcc(const float acc[3]);
void attitude_inputGyr(const float gyr[3]);
void attitude_inputMag(const float mag[3]);
void attitude_mixGyrAccMag(void);

#endif // __FILE_ATTITUDE_ATTITUDE_H__
