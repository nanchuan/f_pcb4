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

// bsp/time/time.c
// 2012-11-3 12:01:00
// js200300953

#include "time.h"
#include "stm32f10x.h"

static struct 
{
    volatile uint32_t msPeriod;    // �����ڵ�ʱ�䣬ms��
    uint32_t ticksPerUs;  // ÿus���ڵ�tick����
    uint32_t ticksPerMs;  // ÿms���ڵ�tick����
    uint32_t msPerPeriod; // ÿ���ڵ�ms����
}time;

// ��ʼ��ʱ�䡣
void time_init(void)
{
    time.msPeriod = 0;
    time.ticksPerUs = SystemCoreClock / 1e6;
    time.ticksPerMs = SystemCoreClock / 1e3;
    time.msPerPeriod = 10;
    //
    SysTick_Config(SystemCoreClock/(1000/time.msPerPeriod));
}

// SysTick�жϡ�
void SysTick_Handler(void)
{
    time.msPeriod += time.msPerPeriod;
}

// ��ȡ��ǰʱ�䣬us��
uint64_t time_nowUs(void)
{
    return time.msPeriod * (uint64_t)1000 + (SysTick->LOAD - SysTick->VAL) / time.ticksPerUs;
}

// ��ȡ��ǰʱ�䣬ms��
uint32_t time_nowMs(void)
{
    return time.msPeriod + (SysTick->LOAD - SysTick->VAL) / time.ticksPerMs;
}

// ��ʱdelay us��delay>=4ʱ��׼ȷ��
void time_waitUs(uint32_t delay)
{
    uint64_t target = time_nowUs() + delay - 2;
    while(time_nowUs() <= target)
        ; // ��ѭ����
}

// ��ʱdelay ms��
void time_waitMs(uint32_t delay)
{
    time_waitUs(delay * 1000);
}

