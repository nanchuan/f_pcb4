/**********************************************************
                   STM32 DFU��ֲʵ��

* @ Ӳ��ƽ̨��ս��STM32������

**********************************************************/

#include "stm32_config.h"

#include "led.h"
#include "key.h"
//#include "lcd_tft.h"
#include "stmflash.h"


#include "hw_config.h"	//USB���ͷ�ļ�
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_pwr.h"
#include "dfu_mal.h"

//u8 Dis_buffer[16];	//��ʾ����

typedef  void (*pFunction)(void);

uint8_t DeviceState;
uint8_t DeviceStatus[6];
pFunction Jump_To_Application;
uint32_t JumpAddress;

/**********************************************************
                           ������
**********************************************************/
int main(void)
{
	u8 i=0;

	delay_init(72);	//��ʼ����ʱ����
	//USART1_Init(9600);	//��ʼ������1
	LED_Init();	//��ʼ��LED�ӿ�
	//LCDTFT_Init();	//��ʼ��LCD�ӿ�

	DFU_Button_Config();	//��ʼ����תAPP���򰴼�

	delay_ms(10);

	//����Ƿ����DFUģʽ����������û�а�������ת��APP������ִ��
	if(DFU_Button_Read() == 1)
	{
		if(((*(__IO uint32_t*)ApplicationAddress) & 0x2FFFA000 ) == 0x20000000)	//���APP��ַ�Ƿ�Ϸ�
		{
			//��ת��APP��ַ��ʼִ�У���ַ+4λ���Ǹ�λ�ж����
			JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
			Jump_To_Application = (pFunction) JumpAddress;

			//����APP�����ջָ��
			__set_MSP(*(__IO uint32_t*) ApplicationAddress);
			
			//��ת��APP������ִ��
			Jump_To_Application();
		}
	}

	#if 0
	if(DFU_Button_Read() == 1)
	{
	/******************************************************
	                      ��ʾ������Ϣ
	                     ��LCD������ʼ 
	******************************************************/
	LCD_Display_String_BK(30, 0, "Software Compiled Time:", 16, MAGENTA, WHITH);
	LCD_Display_String_BK(30, 16, __DATE__, 16, MAGENTA, WHITH);
	LCD_Display_String_BK(150, 16, __TIME__, 16, MAGENTA, WHITH);

	LCD_Display_String_BK(30, 32, "WarShip STM32.", 16, ORANGE, WHITH);
	LCD_Display_String_BK(30, 48, "STM32 DFU Test.", 16, ORANGE, WHITH);
	LCD_Display_String_BK(30, 64, "2014/03/14 week5", 16, ORANGE, WHITH);
	LCD_Display_String_BK(30, 80, "By@Sam Chan.", 16, ORANGE, WHITH);	
	
	sprintf((char*)Dis_buffer, "LCD ID:%04X", LCD_Manage.ID);//��LCD ID��ӡ��lcd_id����
	LCD_Display_String_BK(72, 96, Dis_buffer, 16, RED, WHITH);	//��ʾLCDID����ʾ����

	printf("Software Compiled Time: %s, %s.\r\n",__DATE__, __TIME__);	//��ȡ�������ʱ��

	/*******************���������ķָ���******************/
	}
	#endif

	//�����Ѿ����£������APP����ģʽ
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


