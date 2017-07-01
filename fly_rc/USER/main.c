/**********************************************************
                   USBģ�⴮��ʵ��

* @ Ӳ��ƽ̨��ս��STM32������

**********************************************************/

#include "stm32_config.h"

#include "led.h"
#include "joystick.h"
#include "input.h"
#include "conductor.h"
//#include "lcd.h"

//USB���ͷ�ļ�
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"

#include "rf.h"
#include "serial.h"
#include "nrf24l01p.h"

//u8 Dis_buffer[16];	//��ʾ����

/**********************************************************
* �������� ---> USB����/�Ͽ�
* ��ڲ��� ---> usb_sw��USB����/�رտ��ơ�0���ر�
*                                         1����
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/	   
void USB_Port_Set(u8 usb_sw)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //ʹ��PORTAʱ��
		   	 
	if(usb_sw)	_SetCNTR(_GetCNTR()&(~(1<<1)));//�˳��ϵ�ģʽ
	else
	{	  
		_SetCNTR(_GetCNTR()|(1<<1));  // �ϵ�ģʽ
		GPIOA->CRH &= 0XFFF00FFF;
		GPIOA->CRH |= 0X00033000;
		PAout(12) = 0;	    		  
	}
}
#if 0
/**********************************************************
* �������� ---> ��ʾ�������ò���
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void Display_COMValue(void)
{
	LCD_Display_String_BK(30, 128, "The USART Parameter:", 16, BLUE, WHITH);	
	/* ��ʾ������ */
	LCD_Display_String_BK(30, 144, "BaudRate  :", 16, RED, WHITH);
	LCD_Display_Array_No0(126, 144, USART_InitStructure.USART_BaudRate, 6, 0);
	/* ��ʾ����λ */
	if(USART_InitStructure.USART_WordLength == USART_WordLength_8b)	//8λ����
	{	LCD_Display_String_BK(30, 160, "WordLength: 8bits", 16, RED, WHITH);	}
	if(USART_InitStructure.USART_WordLength == USART_WordLength_9b)	//9λ����
	{	LCD_Display_String_BK(30, 160, "WordLength: 9bits", 16, RED, WHITH);	}
	/* ��ʾ����ֹͣλ */
	if(USART_InitStructure.USART_StopBits == USART_StopBits_1)	//1λֹͣλ
	{	LCD_Display_String_BK(30, 176, "StopBits  : 1bit  ", 16, RED, WHITH);	}
	if(USART_InitStructure.USART_StopBits == USART_StopBits_1_5)	//1.5λֹͣλ
	{	LCD_Display_String_BK(30, 176, "StopBits  : 1.5bit", 16, RED, WHITH);	}
	if(USART_InitStructure.USART_StopBits == USART_StopBits_2)	//2λֹͣλ
	{	LCD_Display_String_BK(30, 176, "StopBits  : 2bits ", 16, RED, WHITH);	}
	/* ��ʾ������żУ��λ */
	if(USART_InitStructure.USART_Parity == USART_Parity_No)
	{	LCD_Display_String_BK(30, 192, "Parity    : No  ", 16, RED, WHITH);	}
	if(USART_InitStructure.USART_Parity == USART_Parity_Even)
	{	LCD_Display_String_BK(30, 192, "Parity    : Even", 16, RED, WHITH);	}
	if(USART_InitStructure.USART_Parity == USART_Parity_Odd)
	{	LCD_Display_String_BK(30, 192, "Parity    : Odd ", 16, RED, WHITH);	}
	/* Ӳ�������� */
	if(USART_InitStructure.USART_HardwareFlowControl == USART_HardwareFlowControl_None)
	{	LCD_Display_String_BK(30, 208, "HWFlowCtrl: None    ", 16, RED, WHITH);	}
	if(USART_InitStructure.USART_HardwareFlowControl == USART_HardwareFlowControl_RTS)
	{	LCD_Display_String_BK(30, 208, "HWFlowCtrl: RTS     ", 16, RED, WHITH);	}
	if(USART_InitStructure.USART_HardwareFlowControl == USART_HardwareFlowControl_CTS)
	{	LCD_Display_String_BK(30, 208, "HWFlowCtrl: CTS     ", 16, RED, WHITH);	}
	if(USART_InitStructure.USART_HardwareFlowControl == USART_HardwareFlowControl_RTS_CTS)
	{	LCD_Display_String_BK(30, 208, "HWFlowCtrl: RTS&CTS ", 16, RED, WHITH);	}
	/* �ա���ģʽ */
	if(USART_InitStructure.USART_Mode == USART_Mode_Rx)	//����ģʽ
	{	LCD_Display_String_BK(30, 224, "USART Mode: RXD    ", 16, RED, WHITH);	}
	if(USART_InitStructure.USART_Mode == USART_Mode_Tx)	//����ģʽ
	{	LCD_Display_String_BK(30, 224, "USART Mode: TXD    ", 16, RED, WHITH);	}
	if(USART_InitStructure.USART_Mode == (USART_Mode_Rx | USART_Mode_Tx))	//�շ�ģʽ
	{	LCD_Display_String_BK(30, 224, "USART Mode: RXD&TXD", 16, RED, WHITH);	}
}
#endif

/**********************************************************
                           ������
**********************************************************/
int main(void)
{
//	u16 i;

//	u16 srl_rxlen = 0;
//	u8 srl_rxbuf[32] = {0};
	
	//u8 USB_STA;	//USB״̬
	//u8 Divece_STA;	//�豸״̬
	
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,(uint32_t)0x5000);

	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2);	//�����жϷ���
	delay_init(72);	//��ʼ����ʱ����
	USARTx_Init(9600);	//��ʼ������1
	LED_Init();	//��ʼ��LED�ӿ�
	joystick_init();
	//LCD_Init();	//��ʼ��LCD�ӿ�

	#if 0
	/******************************************************
	                      ��ʾ������Ϣ
	                     ��LCD������ʼ 
	******************************************************/
	LCD_Display_String_BK(30, 0, "Software Compiled Time:", 16, MAGENTA, WHITH);
	LCD_Display_String_BK(30, 16, __DATE__, 16, MAGENTA, WHITH);
	LCD_Display_String_BK(150, 16, __TIME__, 16, MAGENTA, WHITH);

	LCD_Display_String_BK(30, 32, "WarShip STM32.", 16, ORANGE, WHITH);
	LCD_Display_String_BK(30, 48, "STM32 Virtual COMPort.", 16, ORANGE, WHITH);
	LCD_Display_String_BK(30, 64, "2014/03/02 week7", 16, ORANGE, WHITH);
	LCD_Display_String_BK(30, 80, "By@Sam Chan.", 16, ORANGE, WHITH);	
	
	sprintf((char*)Dis_buffer, "LCD ID:%04X", LCD_Manage.ID);//��LCD ID��ӡ��lcd_id����
	LCD_Display_String_BK(72, 96, Dis_buffer, 16, RED, WHITH);	//��ʾLCDID����ʾ����
	#endif

	printf("Software Compiled Time: %s, %s.\r\n",__DATE__, __TIME__);	//��ȡ�������ʱ��

	/*******************���������ķָ���******************/

	delay_ms(300);
 	USB_Port_Set(0); 	//USB�ȶϿ�
	delay_ms(300);
	USB_Port_Set(1);	//USB�ٴ�����
	//LCD_Display_String_BK(60, 240, "USB Connecting... ", 16, RED, WHITH);

	USB_Interrupts_Config();	//��ʼ��USB����ж�
	Set_USBClock();	//����USBʱ��
	USB_Init();	//USB��ʼ��

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
		//����״̬��ʾ
		if(USB_STA != USB_COMPort_STA)	//״̬�ı��ˣ������
		{
			//LCD_Display_String_BK(60, 256, "                    ", 16, BLUE, WHITH);	//�����ʾ����

			if(USB_COMPort_STA&0x01)	//�������ݵ�USB
			{
				//LCD_Display_String_BK(60, 256, "Send Data To USB... ", 16, BLUE, WHITH);	//��ʾ���ڷ������ݵ�USB
				LED1 = !LED1;
			}
			if(USB_COMPort_STA&0x02)	//��������USB����
			{
				//LCD_Display_String_BK(60, 256, "Rev Data From USB...", 16, BLUE, WHITH);	//��ʾ���ڽ�������USB������
				LED1 = !LED1;
			}
			USB_STA = USB_COMPort_STA;	//��¼USB����״̬
		}

		//����ָʾ
		if(Divece_STA != bDeviceState) 
		{
			if(bDeviceState == CONFIGURED)	//��ʾ�����ӳɹ�
			{	
				//LCD_Display_String_BK(60, 240, "USB Connected    ", 16, BLUE, WHITH);	
				LED1 = !LED1;
			}
			else	//��ʾ�Ѿ��Ͽ�����
			{	
				//LCD_Display_String_BK(60, 240, "USB DisConnected ", 16, RED, WHITH);	
				LED1 = !LED1;
			}
		
			Divece_STA = bDeviceState;	//��¼USB�豸״̬
		}

		//Display_COMValue();	//��ʾ���ڲ���
		//ָʾ�ƺ����Ӽ��
		i++;
		if(i == 10)
		{
			i = 0;
			//LED0 = !LED0;	//��ʾϵͳ����

			USB_COMPort_STA = 0;	//���USB״̬��־
		}
		#endif
	}
}

