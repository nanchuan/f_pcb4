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

// bsp/hal.c
// 2012-9-2 9:39:32
// js200300953

#include <stm32f10x.h>
#include "rf_hal.h"

void    rf_hal_init(void);
void    rf_hal_ceHigh(void);
void    rf_hal_ceLow(void);
void    rf_hal_spiBegin(void);
void    rf_hal_spiEnd(void);
uint8_t rf_hal_spiSwap(uint8_t val);

void rf_hal_init(void)
{
    //
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,  ENABLE);		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//SPI2 
	
    // ��ʼ����š�
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; // C13 => CSN
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC,GPIO_Pin_13);
    //
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; // B13 => CLK ,B14 => MISO, B15 => MOSI
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; // C14 => CE
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC,GPIO_Pin_14);
    //
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; // C15 => IRQ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //		
    // ��ʼ��SPI��
    SPI_I2S_DeInit(SPI2);
    SPI_Cmd(SPI2,DISABLE);
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL      = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS       = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial     = 7;
    SPI_Init(SPI2,&SPI_InitStructure);
    SPI_Cmd(SPI2,ENABLE);
}

void rf_hal_ceHigh(void)
{
    GPIO_SetBits(GPIOC,GPIO_Pin_14);
}

void rf_hal_ceLow(void)
{
    GPIO_ResetBits(GPIOC,GPIO_Pin_14);
}

int32_t rf_hal_irqIsHigh(void)
{
    return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15);
}

void rf_hal_spiBegin(void)
{
    GPIO_ResetBits(GPIOC,GPIO_Pin_13);
}

void rf_hal_spiEnd(void)
{
    GPIO_SetBits(GPIOC,GPIO_Pin_13);
}

/* ͨ��SPI���Ͳ�����һ���ֽڡ�*/
uint8_t rf_hal_spiSwap(uint8_t val)
{
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
    {
        // ��ѭ����
    }
    SPI_I2S_SendData(SPI2,val);
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
    {
        // ��ѭ����
    }
    return (uint8_t)SPI_I2S_ReceiveData(SPI2);
}
