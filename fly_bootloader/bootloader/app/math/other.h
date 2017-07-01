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

/* math/other.h
 * 2012-11-7 20:07:51
 * js200300953
 */

#ifndef __FILE_MATH_OTHER_H__
#define __FILE_MATH_OTHER_H__

#include <stdint.h>

float math_rsqrt(float number);
float math_vector_length(const float v[3]);
float math_vector_dot(const float left[3],const float right[3]);
void math_vector_cross(float result[3],const float left[3],const float right[3]);
uint16_t math_crc16(uint16_t crc,const void * data,uint16_t len);
void math_itoa(int32_t val,char * str);

#endif // __FILE_MATH_OTHER_H__
