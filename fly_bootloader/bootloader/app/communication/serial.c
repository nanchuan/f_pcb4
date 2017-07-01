//     Copyright (c) 2013 js200300953@qq.com All rights reserved.
//         ==================================================
//         ========圆点博士微型四轴飞行器配套软件声明========
//         ==================================================
//     圆点博士微型四轴飞行器配套软件包括上位机程序、下位机Bootloader
// 和下位机App，及它们的源代码，以下总称“软件”。
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

// app/communication/serial.c
// 2013-6-1 18:13:12
// js200300953

#include <bsp/led.h>
#include <bsp/uart1.h>
#include <app/math/interface.h>
#include "protocol.h"
#include "engine.h"
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
            if(uart1_receiveValidBufferSize() < 1)
                break;
            uint8_t byte = 0;
            uart1_readReceiveBuffer(&byte,1);
            //
            if(byte == (uint8_t)(0x55))
                com_serial_receiveStatus = NEED_0XAA;
        }
        break;
        case NEED_0XAA :
        {
            if(uart1_receiveValidBufferSize() < 1)
                break;
            uint8_t byte = 0;
            uart1_readReceiveBuffer(&byte,1);
            //
            if(byte == (uint8_t)(0xAA))
                com_serial_receiveStatus = NEED_LENGTH;
            else if(byte == (uint8_t)(0x55))
                break; // 连续两个0x55的情况。
            else
                com_serial_receiveStatus = FINDING_0X55;
        }
        break;
        case NEED_LENGTH :
        {
            if(uart1_receiveValidBufferSize() < 1)
                break;
            uint8_t length = 0;
            uart1_readReceiveBuffer(&length,1);
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
            if(uart1_receiveValidBufferSize() < com_serial_receiveLength)
                break;
            //
            uart1_readReceiveBuffer(com_serial_receiveBuffer,com_serial_receiveLength);
            if(math_crc16(0,com_serial_receiveBuffer,com_serial_receiveLength) == 0)
                engine_handlePacket_serial(com_serial_receiveBuffer,com_serial_receiveLength-2);
            //
            com_serial_receiveStatus = FINDING_0X55;
        }
        break;
    }
}

int32_t com_serial_transmitBegin(uint8_t packetType)
{
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
    // 判断是否有足够空间。-2以保留CRC的位置。
    if(com_serial_transmitLength+length > TRANSMIT_BUFFER_SIZE-2)
        return 1;
    //
    // 复制数据。
    for(int32_t i=0;i<length;i++)
        com_serial_transmitBuffer[com_serial_transmitLength+i] = ((uint8_t *)part)[i];
    com_serial_transmitLength += length;
    //
    // 更新CRC。
    com_serial_transmitCrc = math_crc16(com_serial_transmitCrc,part,length);
    //
    return 0;
}

int32_t com_serial_transmitEnd(void)
{
    //
    // 把CRC放到包后面。
    com_serial_transmitBuffer[com_serial_transmitLength ++] = ((uint8_t *)&com_serial_transmitCrc)[1];
    com_serial_transmitBuffer[com_serial_transmitLength ++] = ((uint8_t *)&com_serial_transmitCrc)[0];
    //
    // 发送包。
    com_serial_transmitBuffer[2] = com_serial_transmitLength-(1+1+1);
    uart1_transmit(com_serial_transmitBuffer,com_serial_transmitLength);
    com_serial_transmitLength = 0;
    //
    return 0;
}
