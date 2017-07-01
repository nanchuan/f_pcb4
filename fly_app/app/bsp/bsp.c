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
    // 里面初始化NVIC，必须在第一次中断发生前调用。
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
    // 初始化时钟。
    time_init();
    time_waitMs(100);
    //
    // 初始化通信。
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
        // 初始化成功。
        rf_startReceive();
    }
    //
    // 初始化其他模块。
    i2c_init();
    battery_init();
    motor_init();
    //
    // 初始化传感器。
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
