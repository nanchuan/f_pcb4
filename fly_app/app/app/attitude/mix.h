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

// app/attitude/mix.h
// 2013-1-8 15:51:33
// js200300953

#ifndef __FILE_ATTITUDE_MIX_H__
#define __FILE_ATTITUDE_MIX_H__

#include <stdint.h>
#include "../math/quaternion.h"

void mix_init(void);
void mix_gyr(quaternion * attitude,const float gyr[3],float interval);
void mix_AccMag(quaternion * attitude,const float acc[3],const float mag[3]);
void mix_AccMag_steepestDescentMethod(quaternion * attitude,const float acc[3],const float mag[3]);
void mix_AccMag_rotateMethod(quaternion * attitude,const float acc[3],const float mag[3]);
void mix_Acc_steepestDescentMethod(quaternion * attitude,const float acc[3]);
void mix_gyrAcc_crossMethod(quaternion * attitude,const float gyr[3],const float acc[3],float interval);
void mix_gyrAccMag_crossMethod(quaternion * attitude,const float gyr[3],const float acc[3],const float mag[3],float interval);

#endif // __FILE_ATTITUDE_MIX_H__
