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

// bsp/flash.c
// 2013-4-11 10:21:02
// js200300953

#include <cmsis/stm32f10x.h>
#include "flash.h"

int32_t flash_init(void);
int32_t flash_erase(const uint32_t * page);
int32_t flash_write(const uint32_t * target,const uint32_t * source,int32_t length);

int32_t flash_init(void)
{
    NVIC_SetVectorTable(NVIC_VectTab_FLASH,(uint32_t)0x4000);
    return 0;
}

int32_t flash_erase(const uint32_t * page)
{
    int32_t rt = FLASH_COMPLETE;
    //
    FLASH_Unlock();
    rt = FLASH_ErasePage((uint32_t)page);
    FLASH_Lock();
    //
    if(rt != FLASH_COMPLETE)
        return -1;
    return 0;
}

int32_t flash_write(const uint32_t * target,const uint32_t * source,int32_t length)
{
    int32_t ok = 1;
    //
    FLASH_Unlock();
    for(int32_t i=0;i<length;i++)
    {
        if(FLASH_ProgramWord((uint32_t)(target+i),source[i]) != FLASH_COMPLETE)
        {
            ok = 0;
            break;
        }
    }
    FLASH_Lock();
    //
    if(! ok)
        return -1;
    return 0;
}
