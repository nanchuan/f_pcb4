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

// bsp/joystick.c
// 2013-6-7 15:11:24
// js200300953

#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "joystick.h"

#define CHANNEL_AMOUNT 8
#define REV_RANGE (1.0f/2048.0f)

static int16_t volatile g_adcValue[CHANNEL_AMOUNT] = {0,0,0,0,0,0,0,0};
static int16_t volatile g_zero[CHANNEL_AMOUNT] = {0,0,0,0,2048,2048,2048,2048};

void joystick_init(void)
{		
		/* 使能各个用到的外设时钟 */
  	RCC_APB2PeriphClockCmd(
  	RCC_APB2Periph_GPIOA | 
  	RCC_APB2Periph_GPIOB |
  	RCC_APB2Periph_ADC1 | 
  	RCC_APB2Periph_AFIO, ENABLE); //使能ADC1通道时钟，各个管脚时钟
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能DMA传输
		
    // 调整ADC时钟。
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    //
    // 配置GPIO。
  	GPIO_InitTypeDef GPIO_InitStructure;	
  	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
			GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_Init(GPIOA , &GPIO_InitStructure);

		/* KEY */	
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;	//MODE
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;	//FUN
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    //
    // 设置DMA。
    DMA_DeInit(DMA1_Channel1);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_adcValue;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = CHANNEL_AMOUNT;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);
    //
    // 设置ADC。
    ADC_InitTypeDef ADC_InitStructure;
	
		ADC_DeInit(ADC1); //将外设 ADC1 的全部寄存器重设为缺省值
		/*	
		*	独立工作模式;
		*	多通道扫描模式;
		*	连续模数转换模式;
		*	转换触发方式：转换由软件触发启动;
		*	ADC 数据右对齐 ;
		*	进行规则转换的 ADC 通道的数目为8; 
		*/
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_ScanConvMode = ENABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//////////////////////////////////////
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfChannel = CHANNEL_AMOUNT;
		ADC_Init(ADC1, &ADC_InitStructure);
		
//	/* 设置 ADC1 使用8转换通道，在规则组里的转换顺序--->1，采样时间为 239 周期 */ 
		ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, ADC_SampleTime_239Cycles5 );    
    //
    // 启动ADC。
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    //
    // 校正ADC1。
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1))
        ; // 空等。
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1))
        ; // 空等。
    //
    // 启动转换。
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

// 更新零点。
void joystick_zero(void)
{
    const static float FACTOR = 0.1;
    //
    if(g_zero[0] == 0)
    {
        for(int i=0;i<CHANNEL_AMOUNT;i++)
            g_zero[i] = g_adcValue[i];
    }
    else
    {
        for(int i=0;i<CHANNEL_AMOUNT;i++)
            g_zero[i] = (int16_t)(g_zero[i] * (1 - FACTOR) + g_adcValue[i] * FACTOR);
    }
}

// 获取摇杆的值，大约标准化到[-1,1]。
joystick_t joystick_getValue(void)
{
    joystick_t value;
     value.a_horizontal =    (g_adcValue[5] - g_zero[5]) * REV_RANGE;
     value.a_vertical   =    (g_adcValue[4] - g_zero[4]) * REV_RANGE;
     value.b_horizontal =    (g_adcValue[6] - g_zero[6]) * REV_RANGE;
     value.b_vertical   =    (g_adcValue[7] - g_zero[7]) * REV_RANGE;
//    int i=4;
//    value.a_horizontal =    (g_adcValue[(1+i)%CHANNEL_AMOUNT] - g_zero[(1+i)%CHANNEL_AMOUNT]) * REV_RANGE;
//    value.a_vertical   =    (g_adcValue[(0+i)%CHANNEL_AMOUNT] - g_zero[(0+i)%CHANNEL_AMOUNT]) * REV_RANGE;
//    value.b_horizontal =    (g_adcValue[(2+i)%CHANNEL_AMOUNT] - g_zero[(2+i)%CHANNEL_AMOUNT]) * REV_RANGE;
//    value.b_vertical   =    (g_adcValue[(3+i)%CHANNEL_AMOUNT] - g_zero[(3+i)%CHANNEL_AMOUNT]) * REV_RANGE;
    //
    return value;
}

// 返回按键的状态，按键对应位为1表示按键按下。
int32_t key_getKeyValue(void)
{
    int32_t keyStatus = 0;
    //
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1 ) == 0)
        keyStatus |= KEY_1;
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9 ) == 0)
        keyStatus |= KEY_2;
    return keyStatus;
}

float battery_getRcVoltage(void)
{		
		 return (g_adcValue[3] - g_zero[3]) * 6.6 / 4096;
}

