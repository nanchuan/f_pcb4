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

// app/attitude/accfilter.h
// 2013-1-8 15:49:15
// js200300953

#ifndef __FILE_ATTITUDE_ACCFILTER_H__
#define __FILE_ATTITUDE_ACCFILTER_H__

#include <stdint.h>

void accfilter_init(void);
void accfilter_input(const float val[3]);
const float * accfilter_getCurrent(void);

#endif // __FILE_ATTITUDE_ACCFILTER_H__