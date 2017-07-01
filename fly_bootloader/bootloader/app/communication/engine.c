//     Copyright (c) 2013 js200300953@qq.com All rights reserved.
//         ==================================================
//         ========Բ�㲩ʿ΢����������������������========
//         ==================================================
//     Բ�㲩ʿ΢������������������������λ��������λ��Bootloader
// ����λ��App�������ǵ�Դ���룬�����ܳơ��������
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

// app/communication/engine.c
// 2013-6-1 16:13:09
// js200300953

#include <bsp/led.h>
#include "protocol.h"
#include "engine.h"
#include "cmd.h"
#include "serial.h"
#include "nrf24l01p.h"

static enum
{
    ENGINE_SERIAL = 1,
    ENGINE_NRF
} engine_lastReceiveSource = ENGINE_NRF;

int32_t engine_init(void);
void    engine_checkEvent(void);
int32_t engine_handlePacket_serial(const uint8_t * packet,int32_t length);
int32_t engine_handlePacket_nrf(const uint8_t * packet,int32_t length);
int32_t engine_transmitBegin(uint8_t packetType);
int32_t engine_transmitContent(const void * part,int32_t length);
int32_t engine_transmitEnd(void);
int32_t engine_maxTransmitContentSize(void);
//
static int32_t engine_handlePacket(const uint8_t * packet,int32_t length);

int32_t engine_init(void)
{
    com_serial_init();
    com_nrf24l01p_init();
    //
    return 0;
}

void engine_checkEvent(void)
{
    com_serial_checkEvent();
    com_nrf24l01p_checkEvent();
}

int32_t engine_transmitBegin(uint8_t packetType)
{
    switch(engine_lastReceiveSource)
    {
        case ENGINE_SERIAL :
            return com_serial_transmitBegin(packetType);
        case ENGINE_NRF :
            return com_nrf24l01p_transmitBegin(packetType);
    }
    return 0;
}

int32_t engine_transmitContent(const void * part,int32_t length)
{
    switch(engine_lastReceiveSource)
    {
        case ENGINE_SERIAL :
            return com_serial_transmitContent(part,length);
        case ENGINE_NRF :
            return com_nrf24l01p_transmitContent(part,length);
    }
    return 0;
}

int32_t engine_transmitEnd(void)
{
    switch(engine_lastReceiveSource)
    {
        case ENGINE_SERIAL :
            return com_serial_transmitEnd();
        case ENGINE_NRF :
            return com_nrf24l01p_transmitEnd();
    }
    return 0;
}

int32_t engine_maxTransmitContentSize(void)
{
    switch(engine_lastReceiveSource)
    {
        case ENGINE_SERIAL :
            return COM_SERIAL_CONTENT_SIZE_MAX;
        case ENGINE_NRF :
            return COM_NRF24L01P_CONTENT_SIZE_MAX;
    }
    return 0;
}

// packet:|version|type|content|��
int32_t engine_handlePacket_serial(const uint8_t * packet,int32_t length)
{
    engine_lastReceiveSource = ENGINE_SERIAL;
    return engine_handlePacket(packet,length);
}

// packet:|version|type|content|��
int32_t engine_handlePacket_nrf(const uint8_t * packet,int32_t length)
{
    engine_lastReceiveSource = ENGINE_NRF;
    return engine_handlePacket(packet,length);
}

// packet:|version|type|content|��
int32_t engine_handlePacket(const uint8_t * packet,int32_t length)
{
    if(packet[0] != PROTOCOL_VERSION)
        return 1;
    led_reverse();
    //
    const uint8_t * param = packet + 2;
    int32_t param_length = length - 2;
    switch(packet[1])
    {
    case PROTOCOL_BOOTLOADER_CMD :
        {
            if(param_length < 1)
                break;
            cmd_bootloaderCmd(param,param_length);
        }
        break;
		default :
				{
					  if(engine_transmitBegin(0xff) == 0)
					  {
						    engine_transmitContent(packet,length);
						    engine_transmitEnd();	
					  }
				}
				break;
    }
    return 0;
}
