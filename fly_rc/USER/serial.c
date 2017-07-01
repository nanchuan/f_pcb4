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

// app/communication/serial.c
// 2013-6-1 18:13:12
// js200300953

#include "protocol.h"
#include "led.h"
#include "nrf24l01p.h"
#include "uart2.h"
#include "serial.h"

#define RECEIVE_BUFFER_SIZE 32
#define TRANSMIT_BUFFER_SIZE (1+1+1+32)

static enum
{
    FINDING_0X55 = 1,
    NEED_0XAA,
    NEED_LENGTH,
    GETTING_DATA
} com_serial_receiveStatus = FINDING_0X55;
static int32_t  com_serial_receiveLength = 0;
static uint8_t  com_serial_receiveBuffer[RECEIVE_BUFFER_SIZE]; // |version|type|content|crc|
static int32_t  com_serial_transmitLength = 0;
static uint16_t com_serial_transmitCrc = 0;
static uint8_t  com_serial_transmitBuffer[TRANSMIT_BUFFER_SIZE]; // |0x55|0xAA|length|version|type|content|crc|

int32_t com_serial_init(void);
void    com_serial_checkEvent(void);
int32_t com_serial_transmitBegin(uint8_t packetType);
int32_t com_serial_transmitContent(const void * part,int32_t length);
int32_t com_serial_transmitEnd(void);
uint16_t math_crc16(uint16_t crc,const void * data,uint16_t len);

int32_t com_serial_init(void)
{
    return 0;
}

void com_serial_checkEvent(void)
{
    switch(com_serial_receiveStatus)
    {
        case FINDING_0X55 :
        {
            if(uart2_receiveValidBufferSize() < 1)
                break;
            uint8_t byte = 0;
            uart2_readReceiveBuffer(&byte,1);
            //
            if(byte == (uint8_t)(0x55))
                com_serial_receiveStatus = NEED_0XAA;
        }
        break;
        case NEED_0XAA :
        {
            if(uart2_receiveValidBufferSize() < 1)
                break;
            uint8_t byte = 0;
            uart2_readReceiveBuffer(&byte,1);
            //
            if(byte == (uint8_t)(0xAA))
                com_serial_receiveStatus = NEED_LENGTH;
            else if(byte == (uint8_t)(0x55))
                break; // ��������0x55�������
            else
                com_serial_receiveStatus = FINDING_0X55;
        }
        break;
        case NEED_LENGTH :
        {
            if(uart2_receiveValidBufferSize() < 1)
                break;
            uint8_t length = 0;
            uart2_readReceiveBuffer(&length,1);
            //
            if(length < 4 || length > RECEIVE_BUFFER_SIZE)
                break;
            //
            com_serial_receiveLength = length;
            com_serial_receiveStatus = GETTING_DATA;
        }
        break;
        case GETTING_DATA :
        {
            if(uart2_receiveValidBufferSize() < com_serial_receiveLength)
                break;
            //
            uart2_readReceiveBuffer(com_serial_receiveBuffer,com_serial_receiveLength);
            if(math_crc16(0,com_serial_receiveBuffer,com_serial_receiveLength) == 0)
            {						
                if(com_nrf24l01p_transmitBegin(com_serial_receiveBuffer[1]) == 0)
						    {    
										com_nrf24l01p_transmitContent((com_serial_receiveBuffer+2),(com_serial_receiveLength-4));
										com_nrf24l01p_transmitEnd();	
								}									
								
								LED2_CHANGE;
            }
            //
            com_serial_receiveStatus = FINDING_0X55;
        }
        break;
    }
}

extern  __IO uint32_t bDeviceState; /* USB device status */
int32_t com_serial_transmitBegin(uint8_t packetType)
{
		if(bDeviceState != 5/*CONFIGURED*/)	//δ����
			return 1;
	
    com_serial_transmitBuffer[0] = 0x55;
    com_serial_transmitBuffer[1] = 0xAA;
    com_serial_transmitBuffer[2] = 0;
    com_serial_transmitBuffer[3] = PROTOCOL_VERSION;
    com_serial_transmitBuffer[4] = packetType;
    com_serial_transmitLength = 5;
    //
    uint8_t tmp[] = {PROTOCOL_VERSION,packetType};
    com_serial_transmitCrc = math_crc16(0,tmp,sizeof(tmp));
    //
    return 0;
}

int32_t com_serial_transmitContent(const void * part,int32_t length)
{
    //
    // �ж��Ƿ����㹻�ռ䡣-2�Ա���CRC��λ�á�
    if(com_serial_transmitLength+length > TRANSMIT_BUFFER_SIZE-2)
        return 1;
    //
    // �������ݡ�
    for(int32_t i=0;i<length;i++)
        com_serial_transmitBuffer[com_serial_transmitLength+i] = ((uint8_t *)part)[i];
    com_serial_transmitLength += length;
    //
    // ����CRC��
    com_serial_transmitCrc = math_crc16(com_serial_transmitCrc,part,length);
    //
    return 0;
}

int32_t com_serial_transmitEnd(void)
{
    //
    // ��CRC�ŵ������档
    com_serial_transmitBuffer[com_serial_transmitLength ++] = ((uint8_t *)&com_serial_transmitCrc)[1];
    com_serial_transmitBuffer[com_serial_transmitLength ++] = ((uint8_t *)&com_serial_transmitCrc)[0];
    //
    // ���Ͱ���
    com_serial_transmitBuffer[2] = com_serial_transmitLength-(1+1+1);
    uart2_transmit(com_serial_transmitBuffer,com_serial_transmitLength);
    com_serial_transmitLength = 0;
    //
    return 0;
}


// ����CRC-16-CCITT��
// http://www.dzjs.net/html/qianrushixitong/2007/0530/2162.html
// crcΪ�ϴεĽ������ʼʱ��Ϊ0�����ڷֶμ��㡣
uint16_t math_crc16(uint16_t crc,const void * data,uint16_t len)
{
    const static uint16_t crc_tab[16] =
    {
        0x0000 , 0x1021 , 0x2042 , 0x3063 , 0x4084 , 0x50A5 , 0x60C6 , 0x70E7 ,
        0x8108 , 0x9129 , 0xA14A , 0xB16B , 0xC18C , 0xD1AD , 0xE1CE , 0xF1EF
    };
    uint8_t h_crc;
    const uint8_t * ptr = (const uint8_t *)data;
    //
    while(len --)
    {
        h_crc = (uint8_t)(crc >> 12);
        crc <<= 4;
        crc ^= crc_tab[h_crc ^ ((*ptr) >> 4)];
        //
        h_crc = crc >> 12;
        crc <<= 4;
        crc ^= crc_tab[h_crc ^ ((*ptr) & 0x0F)];
        //
        ptr ++;
    }
    //
    return crc;
}


