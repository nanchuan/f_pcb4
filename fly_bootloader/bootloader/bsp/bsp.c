//     Copyright (c) 2013 js200300953@qq.com All rights reserved.
//         ==================================================
//         ========圆点博士微型四轴飞行器配套软件声明========
//         ==================================================
//     圆点博士微型四轴飞行器配套软件包括上位机程序、下位机Bootloader
// 和下位机App，及它们的源代码，以下总称“软件”。
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
#include "uart1.h"
#include "pin.h"

void bsp_init(void)
{
    /*RCC_AHBPeriphClockCmd(
        RCC_AHBPeriph_DMA1,ENABLE);
    RCC_APB1PeriphClockCmd(
        RCC_APB1Periph_TIM2 |
        RCC_APB1Periph_I2C1 |
        RCC_APB1Periph_USB,ENABLE);*/
    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOA    |
        RCC_APB2Periph_GPIOB    |
        RCC_APB2Periph_GPIOC    |
        RCC_APB2Periph_AFIO/*     |
        RCC_APB2Periph_USART1*/   |
        RCC_APB2Periph_SPI1/*     |
        RCC_APB2Periph_ADC1*/,ENABLE);
    //
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    //
    time_init();
    time_waitMs(100);
    //
    led_init();
    pin_init();
    //
    //uart1_init();
    int32_t rs = rf_init();
    if(rs != 0) 
		{
				led_on();
				while(1);
    }
    rf_startReceive();
}

void assert_failed(uint8_t* file, uint32_t line)
{
    while(1)
    {
    }
}
