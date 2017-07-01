/**********************************************************
* @ File name -> led.h
* @ Version   -> V1.0
* @ Date      -> 10-31-2013
* @ Brief     -> LED控制相关函数头文件

 V1.*
* @ Revise    ->
**********************************************************/

#ifndef _led_h_
#define _led_h_

/**********************************************************
                     外部函数头文件
         应用到不同的外设头文件请在这里修改即可                        
**********************************************************/

#include "sys.h"

/**********************************************************
                         定义接口
**********************************************************/

#define LED0					PBout(10)
#define LED1					PBout(12)
#define LED2					PBout(11)

#define LED0_ON  			GPIO_SetBits(GPIOB, GPIO_Pin_10)	//蜂鸣器
#define LED0_OFF 			GPIO_ResetBits(GPIOB, GPIO_Pin_10)

#define LED1_OFF  		GPIO_SetBits(GPIOB, GPIO_Pin_12)		//PB12	red
#define LED1_ON 			GPIO_ResetBits(GPIOB, GPIO_Pin_12)

#define LED2_OFF  		GPIO_SetBits(GPIOB, GPIO_Pin_11)		//PB11	green
#define LED2_ON 			GPIO_ResetBits(GPIOB, GPIO_Pin_11)

#define LED0_CHANGE 	GPIOB->ODR ^= GPIO_Pin_10
#define LED1_CHANGE 	GPIOB->ODR ^= GPIO_Pin_12
#define LED2_CHANGE 	GPIOB->ODR ^= GPIO_Pin_11

#define LEDALL_OFF  	LED0_ON;LED1_OFF;LED2_OFF
#define LEDALL_ON 		LED0_OFF;LED1_ON;LED2_ON

/**********************************************************
                       外部功能函数                      
**********************************************************/

void LED_Init(void);	//初始化LED接口

#endif

