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

// app/attitude/magfilter.c
// 2013-1-8 15:52:35
// js200300953

#include "magfilter.h"

static float g_mag[3];

void magfilter_init(void);
void magfilter_input(const float val[3]);
const float * magfilter_getCurrent(void);

void magfilter_init(void)
{
    for(int i=0;i<3;i++)
        g_mag[i] = 0;
}

void magfilter_input(const float val[3])
{
    for(int i=0;i<3;i++)
        g_mag[i] = val[i];
}

const float * magfilter_getCurrent(void)
{
    return g_mag;
}
