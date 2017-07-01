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

// bsp/bsp.c
// 2013-1-7 13:13:47
// js200300953

#include "stm32f10x.h"
#include "bsp.h"
#include "led.h"
#include "time.h"
#include "rf.h"
#include "i2c.h"
#include "mpu6050.h"
#include "hmc5883.h"
#include "battery.h"
#include "motor.h"
#include "flash.h"
#include "uart1.h"

void bsp_init(void)
{
    // �����ʼ��NVIC�������ڵ�һ���жϷ���ǰ���á�
    flash_init();
    __enable_irq();
    //
    RCC_AHBPeriphClockCmd(
        RCC_AHBPeriph_DMA1,ENABLE);
    RCC_APB1PeriphClockCmd(
        RCC_APB1Periph_TIM2 |
        RCC_APB1Periph_I2C2,ENABLE);
    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOA    |
        RCC_APB2Periph_GPIOB    |
        RCC_APB2Periph_GPIOC    |
        RCC_APB2Periph_AFIO     |
        RCC_APB2Periph_USART1   |
        RCC_APB2Periph_SPI1     |
        RCC_APB2Periph_ADC1,ENABLE);
    //
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    //
    // ��ʼ��ʱ�ӡ�
    time_init();
    time_waitMs(100);
    //
    // ��ʼ��ͨ�š�
    led_init();
    uart1_init();
    int32_t rs = rf_init();
    if(rs != 0)
    {
        //while(1)
        //{
				//		led_reverse();
				//		time_waitMs(250);
				//}
    }
    else
    {
        // ��ʼ���ɹ���
        rf_startReceive();
    }
    //
    // ��ʼ������ģ�顣
    i2c_init();
    battery_init();
    motor_init();
    //
    // ��ʼ����������
    if(mpu6050_init() != 0)
    {
				while(1)
        {
						led_reverse();
						time_waitMs(250);
				}
    }
    //if(hmc5883_init() != 0)
    //    while(1);
}

void assert_failed(uint8_t* file, uint32_t line)
{
    while(1)
    {
    }
}

void bsp_systemReset(void)
{
    NVIC_SystemReset();
}
