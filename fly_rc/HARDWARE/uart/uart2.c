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
    // 重置串口。
    USART_DeInit(USART1);
    //
    // 设置变量
    uart.head_transmit = 0;
    uart.tail_transmit = 0;
    uart.head_receive = 0;
    uart.tail_receive = 0;
    //
    // 接通时钟。
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    //
    // 设置端口。
 	GPIO_InitTypeDef pin_param;
  	// 发送引脚，配置为复用输出，刷新频率2MHz。
  	pin_param.GPIO_Pin = GPIO_Pin_9;
 	pin_param.GPIO_Mode = GPIO_Mode_AF_PP;
  	pin_param.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(GPIOA, &pin_param);
  	// 接收引脚，配置浮地输入。
  	pin_param.GPIO_Pin = GPIO_Pin_10;
  	pin_param.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOA, &pin_param);
    //
    // 配置串口。
    USART_InitTypeDef init_param;
    init_param.USART_BaudRate = 115200;
    init_param.USART_WordLength = USART_WordLength_8b;
    init_param.USART_StopBits = USART_StopBits_1;
    init_param.USART_Parity = USART_Parity_No;
    init_param.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    init_param.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1,&init_param);
    //
    // 设置中断。
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
    // 启动串口。
    USART_Cmd(USART1,ENABLE);
		#endif

	uart.head_transmit = 0;
	uart.tail_transmit = 0;
	uart.head_receive = 0;
	uart.tail_receive = 0;
	
}

#if 0
// 获取发送缓冲区空闲空间大小。
int32_t uart2_transmitIdleBufferSize(void)
{
	if(uart.tail_transmit <= uart.head_transmit)
		return UART2_BUFFER_SIZE_TRANSMIT - uart.head_transmit + uart.tail_transmit;
	return uart.tail_transmit - uart.head_transmit - 1; // -1:不可以填满。
}

// 获取发送缓冲区中的数据长度。
int32_t uart2_transmitValidBufferSize(void)
{
	if(uart.tail_transmit <= uart.head_transmit)
		return uart.head_transmit - uart.tail_transmit;
	return uart.head_transmit + (UART2_BUFFER_SIZE_TRANSMIT - uart.tail_transmit);
}

// 从发送缓冲区取出一个字节，发送出去。
// 不检查缓冲区是否空，不检查发送状态。
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

// 把一段数据放入发送缓冲区。
// 返回值：{UART1_SUCCEED,UART1_FAILED}
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
		return UART2_FAILED; // 空间不够。
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
	// 如果发送没有正在进行，就启动发送。
	if(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == SET)
        uart2_transmitAByte();
	#endif
	//	
	return UART2_SUCCEED;
}

// 获取接收缓冲区中的数据长度。
int32_t uart2_receiveValidBufferSize(void)
{
	if(uart.tail_receive <= uart.head_receive)
		return uart.head_receive - uart.tail_receive;
	return uart.head_receive + (UART2_BUFFER_SIZE_RECEIVE - uart.tail_receive);
}

// 把一个字节放入接收缓冲区。
// 缓冲区满则失败。
// 返回值：{UART1_SUCCEED,UART1_FAILED}。
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

// 从接收缓冲区取出数据，返回取出的长度。
// 取出的长度为outputBufferLength和uart2_receiveValidBufferSize()的最小者。
int32_t uart2_readReceiveBuffer(void * outputBuffer,int32_t outputBufferLength)
{
    // 计算outputBufferLength和uart1_receiveValidBufferSize()的最小值。
    uint32_t returnLength = uart2_receiveValidBufferSize();
    if(outputBufferLength < returnLength)
        returnLength = outputBufferLength;
    //
    // 复制数据，推进指针。
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
    // 发送完成。
    if(USART_GetFlagStatus(USART1,USART_FLAG_TC) == SET)
    {
        USART_ClearFlag(USART1,USART_FLAG_TC);
        //
        if(uart1_transmitValidBufferSize() > 0)
            uart1_transmitAByte();
    }
    //
    // 接收到数据。
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
