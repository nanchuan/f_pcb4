/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

#include "delay.h"
#include "led.h"
#include "sys.h"
#include "lcd_tft.h"

#include "hw_config.h"	//USB相关头文件
#include "usb_lib.h"
#include "usb_istr.h"
#include "usb_prop.h"
#include "usb_pwr.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**********************************************************
* 函数功能 ---> USB低优先级中断处理
* 入口参数 ---> none
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  USB_Istr();
}
/**********************************************************
* 函数功能 ---> USB唤醒中断处理
* 入口参数 ---> none
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/
void USBWakeUp_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line18);	//清除USB中断标志
}
/**********************************************************
* 函数功能 ---> 外部中断线0中断处理
* 入口参数 ---> none
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		if(pInformation->Current_Feature & 0x20) //Remote wake-up enabled
		{
			Resume(RESUME_INTERNAL);
		}

		/* Clear the EXTI line 9 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
//	LCD_Clear(WHITH);	//清除屏幕
//	printf("\r\nHardFault_Handler ERROR\r\n");
	while (1)
	{
//		LCD_Display_String_BK(0, 303, "HardFault_Handler ERROR", 16, RED, WHITH);
		delay_ms(200);
//		LCD_Display_String_BK(0, 303, "                       ", 16, RED, WHITH);
		delay_ms(200);
		LED1 = !LED1;	//硬件出错
	}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  	/* Go to infinite loop when Memory Manage exception occurs */
///	LCD_Clear(WHITH);	//清除屏幕
//	printf("\r\nMemManage_Handler ERROR\r\n");
	while (1)
	{
//		LCD_Display_String_BK(0, 303, "MemManage_Handler ERROR", 16, RED, WHITH);
		delay_ms(200);
//		LCD_Display_String_BK(0, 303, "                       ", 16, RED, WHITH);
		delay_ms(200);
		LED1 = !LED1;	//硬件出错
	}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  	/* Go to infinite loop when Bus Fault exception occurs */
//	LCD_Clear(WHITH);	//清除屏幕
//	printf("\r\nBusFault_Handler ERROR\r\n");
	while (1)
	{
//		LCD_Display_String_BK(0, 303, "BusFault_Handler ERROR", 16, RED, WHITH);
		delay_ms(200);
//		LCD_Display_String_BK(0, 303, "                      ", 16, RED, WHITH);
		delay_ms(200);
		LED1 = !LED1;	//硬件出错
	}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
//	LCD_Clear(WHITH);	//清除屏幕
//	printf("\r\nUsageFault_Handler ERROR\r\n");
	while (1)
	{
//		LCD_Display_String_BK(0, 303, "UsageFault_Handler ERROR", 16, RED, WHITH);
		delay_ms(200);
//		LCD_Display_String_BK(0, 303, "                        ", 16, RED, WHITH);
		delay_ms(200);
		LED1 = !LED1;	//硬件出错
	}
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
