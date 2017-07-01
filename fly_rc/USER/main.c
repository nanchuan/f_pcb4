/**********************************************************
                   USB模拟串口实验

* @ 硬件平台：战舰STM32开发板

**********************************************************/

#include "stm32_config.h"

#include "led.h"
#include "joystick.h"
#include "input.h"
#include "conductor.h"
//#include "lcd.h"

//USB相关头文件
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"

#include "rf.h"
#include "serial.h"
#include "nrf24l01p.h"

//u8 Dis_buffer[16];	//显示缓存

/**********************************************************
* 函数功能 ---> USB连接/断开
* 入口参数 ---> usb_sw：USB开启/关闭控制。0：关闭
*                                         1：打开
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/	   
void USB_Port_Set(u8 usb_sw)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //使能PORTA时钟
		   	 
	if(usb_sw)	_SetCNTR(_GetCNTR()&(~(1<<1)));//退出断电模式
	else
	{	  
		_SetCNTR(_GetCNTR()|(1<<1));  // 断电模式
		GPIOA->CRH &= 0XFFF00FFF;
		GPIOA->CRH |= 0X00033000;
		PAout(12) = 0;	    		  
	}
}
#if 0
/**********************************************************
* 函数功能 ---> 显示串口设置参数
* 入口参数 ---> none
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/
void Display_COMValue(void)
{
	LCD_Display_String_BK(30, 128, "The USART Parameter:", 16, BLUE, WHITH);	
	/* 显示波特率 */
	LCD_Display_String_BK(30, 144, "BaudRate  :", 16, RED, WHITH);
	LCD_Display_Array_No0(126, 144, USART_InitStructure.USART_BaudRate, 6, 0);
	/* 显示数据位 */
	if(USART_InitStructure.USART_WordLength == USART_WordLength_8b)	//8位数据
	{	LCD_Display_String_BK(30, 160, "WordLength: 8bits", 16, RED, WHITH);	}
	if(USART_InitStructure.USART_WordLength == USART_WordLength_9b)	//9位数据
	{	LCD_Display_String_BK(30, 160, "WordLength: 9bits", 16, RED, WHITH);	}
	/* 显示串口停止位 */
	if(USART_InitStructure.USART_StopBits == USART_StopBits_1)	//1位停止位
	{	LCD_Display_String_BK(30, 176, "StopBits  : 1bit  ", 16, RED, WHITH);	}
	if(USART_InitStructure.USART_StopBits == USART_StopBits_1_5)	//1.5位停止位
	{	LCD_Display_String_BK(30, 176, "StopBits  : 1.5bit", 16, RED, WHITH);	}
	if(USART_InitStructure.USART_StopBits == USART_StopBits_2)	//2位停止位
	{	LCD_Display_String_BK(30, 176, "StopBits  : 2bits ", 16, RED, WHITH);	}
	/* 显示串口奇偶校验位 */
	if(USART_InitStructure.USART_Parity == USART_Parity_No)
	{	LCD_Display_String_BK(30, 192, "Parity    : No  ", 16, RED, WHITH);	}
	if(USART_InitStructure.USART_Parity == USART_Parity_Even)
	{	LCD_Display_String_BK(30, 192, "Parity    : Even", 16, RED, WHITH);	}
	if(USART_InitStructure.USART_Parity == USART_Parity_Odd)
	{	LCD_Display_String_BK(30, 192, "Parity    : Odd ", 16, RED, WHITH);	}
	/* 硬件流控制 */
	if(USART_InitStructure.USART_HardwareFlowControl == USART_HardwareFlowControl_None)
	{	LCD_Display_String_BK(30, 208, "HWFlowCtrl: None    ", 16, RED, WHITH);	}
	if(USART_InitStructure.USART_HardwareFlowControl == USART_HardwareFlowControl_RTS)
	{	LCD_Display_String_BK(30, 208, "HWFlowCtrl: RTS     ", 16, RED, WHITH);	}
	if(USART_InitStructure.USART_HardwareFlowControl == USART_HardwareFlowControl_CTS)
	{	LCD_Display_String_BK(30, 208, "HWFlowCtrl: CTS     ", 16, RED, WHITH);	}
	if(USART_InitStructure.USART_HardwareFlowControl == USART_HardwareFlowControl_RTS_CTS)
	{	LCD_Display_String_BK(30, 208, "HWFlowCtrl: RTS&CTS ", 16, RED, WHITH);	}
	/* 收、发模式 */
	if(USART_InitStructure.USART_Mode == USART_Mode_Rx)	//接收模式
	{	LCD_Display_String_BK(30, 224, "USART Mode: RXD    ", 16, RED, WHITH);	}
	if(USART_InitStructure.USART_Mode == USART_Mode_Tx)	//发送模式
	{	LCD_Display_String_BK(30, 224, "USART Mode: TXD    ", 16, RED, WHITH);	}
	if(USART_InitStructure.USART_Mode == (USART_Mode_Rx | USART_Mode_Tx))	//收发模式
	{	LCD_Display_String_BK(30, 224, "USART Mode: RXD&TXD", 16, RED, WHITH);	}
}
#endif

/**********************************************************
                           主函数
**********************************************************/
int main(void)
{
//	u16 i;

//	u16 srl_rxlen = 0;
//	u8 srl_rxbuf[32] = {0};
	
	//u8 USB_STA;	//USB状态
	//u8 Divece_STA;	//设备状态
	
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,(uint32_t)0x5000);

	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2);	//设置中断分组
	delay_init(72);	//初始化延时函数
	USARTx_Init(9600);	//初始化串口1
	LED_Init();	//初始化LED接口
	joystick_init();
	//LCD_Init();	//初始化LCD接口

	#if 0
	/******************************************************
	                      显示基本信息
	                     从LCD顶部开始 
	******************************************************/
	LCD_Display_String_BK(30, 0, "Software Compiled Time:", 16, MAGENTA, WHITH);
	LCD_Display_String_BK(30, 16, __DATE__, 16, MAGENTA, WHITH);
	LCD_Display_String_BK(150, 16, __TIME__, 16, MAGENTA, WHITH);

	LCD_Display_String_BK(30, 32, "WarShip STM32.", 16, ORANGE, WHITH);
	LCD_Display_String_BK(30, 48, "STM32 Virtual COMPort.", 16, ORANGE, WHITH);
	LCD_Display_String_BK(30, 64, "2014/03/02 week7", 16, ORANGE, WHITH);
	LCD_Display_String_BK(30, 80, "By@Sam Chan.", 16, ORANGE, WHITH);	
	
	sprintf((char*)Dis_buffer, "LCD ID:%04X", LCD_Manage.ID);//将LCD ID打印到lcd_id数组
	LCD_Display_String_BK(72, 96, Dis_buffer, 16, RED, WHITH);	//显示LCDID到显示屏上
	#endif

	printf("Software Compiled Time: %s, %s.\r\n",__DATE__, __TIME__);	//获取软件编译时间

	/*******************我是美丽的分割线******************/

	delay_ms(300);
 	USB_Port_Set(0); 	//USB先断开
	delay_ms(300);
	USB_Port_Set(1);	//USB再次连接
	//LCD_Display_String_BK(60, 240, "USB Connecting... ", 16, RED, WHITH);

	USB_Interrupts_Config();	//初始化USB相关中断
	Set_USBClock();	//开启USB时钟
	USB_Init();	//USB初始化

	if (0 != rf_init())
	{
		while(1)
		{
			LED1_CHANGE;
			delay_ms(100);
		}
	}
	rf_startReceive();
	
	(void)input_init();
	(void)com_serial_init();
	(void)com_nrf24l01p_init();
	
	while (1)
	{

		com_nrf24l01p_checkEvent();
		com_serial_checkEvent();
		(void)input_checkEvent();
		(void)actFly_onCheckEvent();
		
		TIME_EXECUTE_PERIODICALLY_BEGIN(1000)
    {
        LED2_CHANGE;
				
				if (conductor_getBatteryVoltage() < 3.4)
						LED0_CHANGE;
				else
						LED0_OFF;
				
				if (battery_getRcVoltage() < 3.5)
						LED1_CHANGE;
				else
						LED1_OFF;						
    }TIME_EXECUTE_PERIODICALLY_END()
		
		#if 0
		//操作状态显示
		if(USB_STA != USB_COMPort_STA)	//状态改变了，有情况
		{
			//LCD_Display_String_BK(60, 256, "                    ", 16, BLUE, WHITH);	//清除显示区域

			if(USB_COMPort_STA&0x01)	//发送数据到USB
			{
				//LCD_Display_String_BK(60, 256, "Send Data To USB... ", 16, BLUE, WHITH);	//提示正在发送数据到USB
				LED1 = !LED1;
			}
			if(USB_COMPort_STA&0x02)	//接收来自USB数据
			{
				//LCD_Display_String_BK(60, 256, "Rev Data From USB...", 16, BLUE, WHITH);	//提示正在接收来自USB的数据
				LED1 = !LED1;
			}
			USB_STA = USB_COMPort_STA;	//记录USB操作状态
		}

		//连接指示
		if(Divece_STA != bDeviceState) 
		{
			if(bDeviceState == CONFIGURED)	//提示已连接成功
			{	
				//LCD_Display_String_BK(60, 240, "USB Connected    ", 16, BLUE, WHITH);	
				LED1 = !LED1;
			}
			else	//提示已经断开连接
			{	
				//LCD_Display_String_BK(60, 240, "USB DisConnected ", 16, RED, WHITH);	
				LED1 = !LED1;
			}
		
			Divece_STA = bDeviceState;	//记录USB设备状态
		}

		//Display_COMValue();	//显示串口参数
		//指示灯和连接检测
		i++;
		if(i == 10)
		{
			i = 0;
			//LED0 = !LED0;	//提示系统运行

			USB_COMPort_STA = 0;	//清空USB状态标志
		}
		#endif
	}
}

