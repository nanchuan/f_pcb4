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

// bsp/led.c
// 2013-1-7 13:12:46
// js200300953

#include "led.h"
#include "stm32f10x.h"

void led_init(void);
void led_on(void);
void led_off(void);
void led_reverse(void);

void led_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	//ָʾ��
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    GPIO_ResetBits(GPIOB,GPIO_Pin_3);

		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;	//ҹ���
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);

		//�ȹص��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
}

void led_on(void)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_3);
}

void led_off(void)
{
    GPIO_SetBits(GPIOB,GPIO_Pin_3);
}

void led_reverse(void)
{
    GPIOB->ODR ^= GPIO_Pin_3;
}

void led_light_on(void)
{
    GPIO_SetBits(GPIOC,GPIO_Pin_13);
}

void led_light_off(void)
{
    GPIO_ResetBits(GPIOC,GPIO_Pin_13);
}

void led_light_reverse(void)
{
    GPIOC->ODR ^= GPIO_Pin_13;
}


