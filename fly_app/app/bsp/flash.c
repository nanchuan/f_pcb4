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
