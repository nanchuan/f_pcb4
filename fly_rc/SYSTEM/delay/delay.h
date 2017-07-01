/**********************************************************
* @ File name -> delay.h
* @ Version   -> V1.0.1
* @ Date      -> 02-26-2014
* @ Brief     -> ϵͳ��ʱ��صĺ���ͷ�ļ�

* @ ��ϸ˵����ο���Cortex-M3Ȩ��ָ��(����)����133ҳ ��8�� SysTick��ʱ������

 V1.0.1
* @ Revise    -> ��������һ����ʱ���㷽��
**********************************************************/

#ifndef _delay_h_
#define _delay_h_

#include "time.h"

#define delay_init(a) time_init()
#define delay_us(a) time_waitUs(a)
#define delay_ms(a) time_waitMs(a)

#if 0

/**********************************************************
                     �ⲿ����ͷ�ļ�                        
**********************************************************/

#include "sys.h"

/**********************************************************
                  ��ʱ����������Դ����
**********************************************************/

#define _USER_SysTick					0	/* �����Ƿ�ʹ��SysTick��ʱ������ʱ�������� */
											//0��ʹ��������ʽ
											//1��ʹ��SysTick��ʱ������ʱ��������

/**********************************************************
                   ����������ʱ������Դ
**********************************************************/

#if !_USER_SysTick	//ʹ��������ʽ����ʱ��������

#define DWT_CTRL				*(volatile uint32_t*)0xe0001000	//DWT���ƼĴ���
#define DWT_CYCCNT				*(volatile uint32_t*)0xe0001004	//DWT��ǰPC�������ڼ����Ĵ���
																//��ϸ������������CM3�����ο��ֲᡷ��129ҳ����
#define DWT_CPICNT				*(volatile uint32_t*)0xe0001008	//DWT��ǰCPI�����Ĵ���
#define DEM_CTRL				*(volatile uint32_t*)0xe000edfc	//�����쳣�ͼ�ؿ��ƼĴ���

#endif

/**********************************************************
                     �ⲿ���ܺ���
**********************************************************/

#if _USER_SysTick	//��������ʹ��SysTick��ʱ������ʱ��������

void delay_init(uint8_t SYSCLK);	//��ʼ����ʱ����
void delay_us(uint32_t nus);	//��ʱn��us
void delay_ms(uint16_t nms);	//��ʱn��ms

#else	//����ʹ��������ʽ

void delay_init(uint8_t SYSCLK);	//��ʼ����ʱ����
void delay_us(uint32_t nus);	//��ʱn��us

//��ʱms�����壬��ʱ��Χ��1 ~ 65535ms����ʱ���ֵ�ɱ䣬������uint32_t/1000��Χ����
#define delay_ms(nms)			delay_us((uint16_t)nms*1000)

#endif	//end _USER_SysTick


#endif

#endif
