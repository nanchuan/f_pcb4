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

// app/attitude/gyrfilter.c
// 2013-1-8 15:52:31
// js200300953

#include "gyrfilter.h"

static float g_gyr[3];

void gyrfilter_init(void);
void gyrfilter_input(const float val[3]);
const float * gyrfilter_current(void);

void gyrfilter_init(void)
{
    for(int i=0;i<3;i++)
        g_gyr[i] = 0;
}

void gyrfilter_input(const float val[3])
{
    for(int i=0;i<3;i++)
        g_gyr[i] = val[i];
}

const float * gyrfilter_current(void)
{
    return g_gyr;
}
