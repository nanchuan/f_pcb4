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

// bsp/motor.c
// 2013-1-8 11:43:58
// js200300953

#include "stm32f10x.h"
#include "motor.h"

#define MOTOR_ARR 72*100        // CCR��װֵ�������趨���ڣ�72M/(72*100)==10k��
#define MOTOR_CCR_MIN 0         // ��С���Ŷ�Ӧ��CCRֵ��
#define MOTOR_CCR_MAX MOTOR_ARR // ������Ŷ�Ӧ��CCRֵ��

#define ESC_PSC     (1-1) // 72M��Ƶ��1��Ƶ��

void motor_init(void)
{
    // ����GPIO���ܡ�
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //
    // ��λ��ʱ����
    TIM_DeInit(TIM2);
    //
    // ���ü�ʱ����
    TIM_TimeBaseInitTypeDef param;
    TIM_TimeBaseStructInit(&param);
    param.TIM_Prescaler = ESC_PSC;
    param.TIM_Period = MOTOR_ARR;
    param.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2,&param);
    //
    // ����PWM��
    TIM_OCInitTypeDef pwmParam;
    TIM_OCStructInit(&pwmParam);
    pwmParam.TIM_OCMode = TIM_OCMode_PWM1;
    pwmParam.TIM_OutputState = TIM_OutputState_Enable;
    pwmParam.TIM_Pulse = MOTOR_CCR_MIN;
    pwmParam.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2,&pwmParam);
    TIM_OC2Init(TIM2,&pwmParam);
    TIM_OC3Init(TIM2,&pwmParam);
    TIM_OC4Init(TIM2,&pwmParam);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
    //
    // ������ʱ����
    TIM_Cmd(TIM2,ENABLE);
}

void motor_setForce(const float force[4])
{
    uint16_t f[4];
    for(int32_t i=0;i<4;i++)
    {
        if(force[i] < MOTOR_FORCE_MIN)
            f[i] = MOTOR_CCR_MIN;
        else if(force[i] > MOTOR_FORCE_MAX)
            f[i] = MOTOR_CCR_MAX;
        else
            f[i] = (uint16_t)(force[i] * (MOTOR_CCR_MAX - MOTOR_CCR_MIN)) + MOTOR_CCR_MIN;
    }
    //
		
     TIM_SetCompare1(TIM2,f[2]);
     TIM_SetCompare2(TIM2,f[3]);
     TIM_SetCompare3(TIM2,f[0]);
     TIM_SetCompare4(TIM2,f[1]);
}
