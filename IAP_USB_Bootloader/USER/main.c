/**********************************************************
                   STM32 DFU移植实验

* @ 硬件平台：战舰STM32开发板

**********************************************************/

#include "stm32_config.h"

#include "led.h"
#include "key.h"
//#include "lcd_tft.h"
#include "stmflash.h"


#include "hw_config.h"	//USB相关头文件
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_pwr.h"
#include "dfu_mal.h"

//u8 Dis_buffer[16];	//显示缓存

typedef  void (*pFunction)(void);

uint8_t DeviceState;
uint8_t DeviceStatus[6];
pFunction Jump_To_Application;
uint32_t JumpAddress;

/**********************************************************
                           主函数
**********************************************************/
int main(void)
{
	u8 i=0;

	delay_init(72);	//初始化延时函数
	//USART1_Init(9600);	//初始化串口1
	LED_Init();	//初始化LED接口
	//LCDTFT_Init();	//初始化LCD接口

	DFU_Button_Config();	//初始化跳转APP程序按键

	delay_ms(10);

	//检测是否进入DFU模式按键，开机没有按下则跳转到APP程序中执行
	if(DFU_Button_Read() == 1)
	{
		if(((*(__IO uint32_t*)ApplicationAddress) & 0x2FFFA000 ) == 0x20000000)	//检测APP地址是否合法
		{
			//跳转到APP地址开始执行，地址+4位置是复位中断入口
			JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
			Jump_To_Application = (pFunction) JumpAddress;

			//设置APP程序堆栈指针
			__set_MSP(*(__IO uint32_t*) ApplicationAddress);
			
			//跳转到APP程序中执行
			Jump_To_Application();
		}
	}

	#if 0
	if(DFU_Button_Read() == 1)
	{
	/******************************************************
	                      显示基本信息
	                     从LCD顶部开始 
	******************************************************/
	LCD_Display_String_BK(30, 0, "Software Compiled Time:", 16, MAGENTA, WHITH);
	LCD_Display_String_BK(30, 16, __DATE__, 16, MAGENTA, WHITH);
	LCD_Display_String_BK(150, 16, __TIME__, 16, MAGENTA, WHITH);

	LCD_Display_String_BK(30, 32, "WarShip STM32.", 16, ORANGE, WHITH);
	LCD_Display_String_BK(30, 48, "STM32 DFU Test.", 16, ORANGE, WHITH);
	LCD_Display_String_BK(30, 64, "2014/03/14 week5", 16, ORANGE, WHITH);
	LCD_Display_String_BK(30, 80, "By@Sam Chan.", 16, ORANGE, WHITH);	
	
	sprintf((char*)Dis_buffer, "LCD ID:%04X", LCD_Manage.ID);//将LCD ID打印到lcd_id数组
	LCD_Display_String_BK(72, 96, Dis_buffer, 16, RED, WHITH);	//显示LCDID到显示屏上

	printf("Software Compiled Time: %s, %s.\r\n",__DATE__, __TIME__);	//获取软件编译时间

	/*******************我是美丽的分割线******************/
	}
	#endif

	//按键已经按下，则进入APP升级模式
	DeviceState = STATE_dfuERROR;
	DeviceStatus[0] = STATUS_ERRFIRMWARE;
	DeviceStatus[4] = DeviceState;
	
	Set_System();
	Set_USBClock();
	USB_Init();
	
	while(1)
	{
		i++;
		delay_ms(10);
		if(i == 20)
		{
			LED0 = ~LED0;
			i = 0;
		}
	}
} 


