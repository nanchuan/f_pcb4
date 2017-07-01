//     Copyright (c) 2013 js200300953@qq.com All rights reserved.
//         ==================================================
//         ========圆点博士微型四轴飞行器配套软件声明========
//         ==================================================
//     圆点博士微型四轴飞行器配套软件包括上位机程序、下位机Bootloader
// 、下位机App和遥控程序，及它们的源代码，以下总称“软件”。
//     软件仅提供参考，js200300953不对软件作任何担保，不对因使用该软件
// 而出现的损失负责。
//     软件可以以学习为目的修改和使用，但不允许以商业的目的使用该软件。
//     修改该软件时，必须保留原版权声明。
// 
//     更多资料见：
// http://blog.sina.com.cn/js200300953
// http://www.etootle.com/
// http://www.eeboard.com/bbs/forum-98-1.html#separatorline
// 圆点博士微型四轴飞行器QQ群：276721324

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
