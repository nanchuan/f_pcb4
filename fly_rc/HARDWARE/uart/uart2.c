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

// bsp/uart1.h
// 2013-6-1 13:47:44
// js200300953

#include <stm32f10x.h>
#include "uart2.h"
#include "hw_config.h"

#define UART2_BUFFER_SIZE_TRANSMIT 256
#define UART2_BUFFER_SIZE_RECEIVE 256

static struct
{
    volatile uint32_t head_transmit;
    volatile uint32_t tail_transmit;
    volatile uint32_t head_receive;
    volatile uint32_t tail_receive;
    volatile uint8_t buffer_transmit[UART2_BUFFER_SIZE_TRANSMIT];
    volatile uint8_t buffer_receive[UART2_BUFFER_SIZE_RECEIVE];
}uart;

void    uart2_init(void);
uint8_t uart2_transmit(const void * data,uint32_t len);
int32_t uart2_receiveValidBufferSize(void);
int32_t uart2_readReceiveBuffer(void * outputBuffer,int32_t outputBufferLength);
//
//static int32_t  uart2_transmitIdleBufferSize(void);
//static int32_t  uart2_transmitValidBufferSize(void);
//static void     uart2_transmitAByte(void);

//extern u8 etootle_4xiii_hh_mode;

void uart2_init(void)
{
	#if 0
    //
    // ���ô��ڡ�
    USART_DeInit(USART1);
    //
    // ���ñ���
    uart.head_transmit = 0;
    uart.tail_transmit = 0;
    uart.head_receive = 0;
    uart.tail_receive = 0;
    //
    // ��ͨʱ�ӡ�
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    //
    // ���ö˿ڡ�
 	GPIO_InitTypeDef pin_param;
  	// �������ţ�����Ϊ���������ˢ��Ƶ��2MHz��
  	pin_param.GPIO_Pin = GPIO_Pin_9;
 	pin_param.GPIO_Mode = GPIO_Mode_AF_PP;
  	pin_param.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(GPIOA, &pin_param);
  	// �������ţ����ø������롣
  	pin_param.GPIO_Pin = GPIO_Pin_10;
  	pin_param.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOA, &pin_param);
    //
    // ���ô��ڡ�
    USART_InitTypeDef init_param;
    init_param.USART_BaudRate = 115200;
    init_param.USART_WordLength = USART_WordLength_8b;
    init_param.USART_StopBits = USART_StopBits_1;
    init_param.USART_Parity = USART_Parity_No;
    init_param.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    init_param.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1,&init_param);
    //
    // �����жϡ�
    USART_ClearFlag(USART1,USART_FLAG_TC);
    USART_ITConfig(USART1,USART_IT_TC,ENABLE);
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
    //
    NVIC_InitTypeDef nvicParam;
    nvicParam.NVIC_IRQChannel = USART1_IRQn;
    nvicParam.NVIC_IRQChannelPreemptionPriority = 8;
    nvicParam.NVIC_IRQChannelSubPriority = 8;
    nvicParam.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicParam);
    //
    // �������ڡ�
    USART_Cmd(USART1,ENABLE);
		#endif

	uart.head_transmit = 0;
	uart.tail_transmit = 0;
	uart.head_receive = 0;
	uart.tail_receive = 0;
	
}

#if 0
// ��ȡ���ͻ��������пռ��С��
int32_t uart2_transmitIdleBufferSize(void)
{
	if(uart.tail_transmit <= uart.head_transmit)
		return UART2_BUFFER_SIZE_TRANSMIT - uart.head_transmit + uart.tail_transmit;
	return uart.tail_transmit - uart.head_transmit - 1; // -1:������������
}

// ��ȡ���ͻ������е����ݳ��ȡ�
int32_t uart2_transmitValidBufferSize(void)
{
	if(uart.tail_transmit <= uart.head_transmit)
		return uart.head_transmit - uart.tail_transmit;
	return uart.head_transmit + (UART2_BUFFER_SIZE_TRANSMIT - uart.tail_transmit);
}

// �ӷ��ͻ�����ȡ��һ���ֽڣ����ͳ�ȥ��
// ����黺�����Ƿ�գ�����鷢��״̬��
void uart2_transmitAByte(void)
{
    //USART_SendData(USART1,uart.buffer_transmit[uart.tail_transmit]);
    
	uart.tail_transmit ++;
	if(uart.tail_transmit >= UART2_BUFFER_SIZE_TRANSMIT)
		uart.tail_transmit = 0;
}
#endif

extern uint8_t USART_Rx_Buffer[];
extern uint32_t USART_Rx_ptr_in;

// ��һ�����ݷ��뷢�ͻ�������
// ����ֵ��{UART1_SUCCEED,UART1_FAILED}
uint8_t uart2_transmit(const void * data,uint32_t len)
{
  for (int i=0; i < len; i++)
  {
		USART_Rx_Buffer[USART_Rx_ptr_in] = ((uint8_t *)data)[i];

		USART_Rx_ptr_in++;

		/* To avoid buffer overflow */
		if(USART_Rx_ptr_in == USART_RX_DATA_SIZE)
		{
			USART_Rx_ptr_in = 0;
		}
  }
	
	#if 0
	if(uart2_transmitIdleBufferSize() < len)
	{
		return UART2_FAILED; // �ռ䲻����
	}
	//
	for(int i=0;i<len;i++)
	{
		uart.buffer_transmit[uart.head_transmit] = ((uint8_t *)data)[i];
		//
		uart.head_transmit ++;
		if(uart.head_transmit >= UART2_BUFFER_SIZE_TRANSMIT)
			uart.head_transmit = 0;
		//
	}
	//
	// �������û�����ڽ��У����������͡�
	if(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == SET)
        uart2_transmitAByte();
	#endif
	//	
	return UART2_SUCCEED;
}

// ��ȡ���ջ������е����ݳ��ȡ�
int32_t uart2_receiveValidBufferSize(void)
{
	if(uart.tail_receive <= uart.head_receive)
		return uart.head_receive - uart.tail_receive;
	return uart.head_receive + (UART2_BUFFER_SIZE_RECEIVE - uart.tail_receive);
}

// ��һ���ֽڷ�����ջ�������
// ����������ʧ�ܡ�
// ����ֵ��{UART1_SUCCEED,UART1_FAILED}��
uint8_t uart2_receivePushToBuffer(uint8_t data)
{
    uint32_t newHead = uart.head_receive + 1;
	if(newHead == UART2_BUFFER_SIZE_RECEIVE)
		newHead = 0;
    //
    if(newHead == uart.tail_receive)
        return UART2_FAILED;
    //
    uart.buffer_receive[uart.head_receive] = data;
    uart.head_receive = newHead;
    //
    return UART2_SUCCEED;
}

// �ӽ��ջ�����ȡ�����ݣ�����ȡ���ĳ��ȡ�
// ȡ���ĳ���ΪoutputBufferLength��uart2_receiveValidBufferSize()����С�ߡ�
int32_t uart2_readReceiveBuffer(void * outputBuffer,int32_t outputBufferLength)
{
    // ����outputBufferLength��uart1_receiveValidBufferSize()����Сֵ��
    uint32_t returnLength = uart2_receiveValidBufferSize();
    if(outputBufferLength < returnLength)
        returnLength = outputBufferLength;
    //
    // �������ݣ��ƽ�ָ�롣
    for(uint32_t i=0;i<returnLength;i++)
    {
        ((uint8_t *)outputBuffer)[i] = uart.buffer_receive[uart.tail_receive];
        //
        uart.tail_receive ++;
        if(uart.tail_receive == UART2_BUFFER_SIZE_RECEIVE)
            uart.tail_receive = 0;
    }
    //
    return returnLength;
}

#if 0
void USART2_IRQHandler(void)
{
	
if (etootle_4xiii_hh_mode==0)		
{	
    // ������ɡ�
    if(USART_GetFlagStatus(USART1,USART_FLAG_TC) == SET)
    {
        USART_ClearFlag(USART1,USART_FLAG_TC);
        //
        if(uart1_transmitValidBufferSize() > 0)
            uart1_transmitAByte();
    }
    //
    // ���յ����ݡ�
    if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
    {
        USART_ClearFlag(USART1,USART_FLAG_RXNE);
        //
        uint8_t data = (uint8_t)USART_ReceiveData(USART1);
        //
        uart1_receivePushToBuffer(data);
    }
		
	}
	else
	{
		  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
			USART_To_USB_Send_Data();
  }

  if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
  {
    (void)USART_ReceiveData(USART1);
  }
}
}
#endif
