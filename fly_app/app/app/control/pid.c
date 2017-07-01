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

// app/control/pid.c
// 2013-3-2 12:58:36
// js200300953

#include <app/param/interface.h>
#include "pid.h"

static float pid_pre[3] = {0,0,0};
static float pid_accumulate[3] = {0,0,0};

int32_t pid_init(void)
{
		int i = 0;

		for (i = 0; i < 3; i++)
		{
				pid_pre[i] = 0;
				pid_accumulate[i] = 0;
		}
		
    return 0;
}

int32_t pid_iterate(const float input[3],float output[3])
{
    const param_t *p = param_getRamParam();
    //
    for(int i=0;i<3;i++)
    {
        float p_o = input[i] * p->xyz_pid[i].p;
        pid_accumulate[i] += input[i] * p->xyz_pid[i].i;
        float i_o = pid_accumulate[i];
        if(i_o < -0.1)
            i_o = -0.1;
        if(i_o > 0.1)
            i_o = 0.1;
        float d_o = (input[i] - pid_pre[i]) * p->xyz_pid[i].d;
        //
        output[i] = p_o + i_o + d_o;
        //
        pid_pre[i] = input[i];
    }
    //
    return 0;
}
