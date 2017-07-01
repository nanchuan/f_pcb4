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

// app/communication/nrf24l01p.c
// 2013-6-1 15:50:22
// js200300953

#include <bsp/rf.h>
#include <bsp/time.h>
#include <app/math/interface.h>
#include "protocol.h"
#include "engine.h"
#include "nrf24l01p.h"

#define TRANSMIT_PACKET_SIZE_MAX 32
#define TRANSMIT_BUFFER_AMOUNT 16

int32_t com_nrf24l01p_init(void);
void    com_nrf24l01p_checkEvent(void);
int32_t com_nrf24l01p_transmitBegin(uint8_t packetType);
int32_t com_nrf24l01p_transmitContent(const void * part,int32_t length);
int32_t com_nrf24l01p_transmitEnd(void);

static uint8_t  transmit_packet[TRANSMIT_BUFFER_AMOUNT] \
                    [TRANSMIT_PACKET_SIZE_MAX];
static uint32_t transmit_length[TRANSMIT_BUFFER_AMOUNT];
static int32_t  transmit_head; // ָ����һ�յ�Ԫ��
static int32_t  transmit_tail; // ָ�����һ����Ч��Ԫ��
static int32_t  transmit_isFull;
static int32_t  transmit_isPacketing;
static uint16_t transmit_currentCrc;

static void com_nrf24l01p_transmitTry(void);

int32_t com_nrf24l01p_init(void)
{
    transmit_head           = 0;
    transmit_tail           = 0;
    transmit_isFull         = 0;
    transmit_isPacketing    = 0;
    transmit_currentCrc     = 0;
    //
    for(int32_t i=0;i<TRANSMIT_BUFFER_AMOUNT;i++)
    {
        for(int32_t j=0;j<TRANSMIT_PACKET_SIZE_MAX;j++)
            transmit_packet[i][j] = 0;
        transmit_length[i] = 0;
    }
    //
    return 0;
}

void com_nrf24l01p_checkEvent(void)
{
    rf_checkEvent();
    //
    TIME_EXECUTE_PERIODICALLY_BEGIN(1)
    {
        com_nrf24l01p_transmitTry();
    }TIME_EXECUTE_PERIODICALLY_END()
}

void com_nrf24l01p_transmitTry(void)
{
    if(transmit_head == transmit_tail
            && ! transmit_isFull)
        return; // �������ա�
    //
    // �������ݣ����ƽ�β������
    int32_t rs = rf_transmit(transmit_packet[transmit_tail],
            transmit_length[transmit_tail]);
    if(rs)
        return;
    //
    // �ѷŵ�rf���ͻ��������ƽ����С�
    transmit_tail = (transmit_tail + 1) % TRANSMIT_BUFFER_AMOUNT;
    transmit_isFull = 0;
}

// ��ʼһ���µİ���
int32_t com_nrf24l01p_transmitBegin(uint8_t packetType)
{
    if(transmit_isFull)
        return 1;
    //
    transmit_packet[transmit_head][0] = PROTOCOL_VERSION;
    transmit_packet[transmit_head][1] = packetType;
    transmit_length[transmit_head] = 2;
    //
    uint8_t tmp[] = {PROTOCOL_VERSION,packetType};
    transmit_currentCrc = math_crc16(0,tmp,sizeof(tmp));
    //
    transmit_isPacketing = 1;
    return 0;
}

// �������ݡ��ɶ�ε��á�
int32_t com_nrf24l01p_transmitContent(const void * part,int32_t length)
{
    if(! transmit_isPacketing)
        return 1;
    //
    int32_t length_before = transmit_length[transmit_head];
    //
    // �ж��Ƿ����-2�Ա���CRC��λ�á�
    if((! transmit_isPacketing) || length_before+length > TRANSMIT_PACKET_SIZE_MAX-2)
        return 1;
    //
    // �������ݡ�
    for(int32_t i=0;i<length;i++)
        transmit_packet[transmit_head][length_before+i] = ((uint8_t *)part)[i];
    transmit_length[transmit_head] += length;
    //
    // ����CRC��
    transmit_currentCrc = math_crc16(transmit_currentCrc,part,length);
    //
    return 0;
}

// ������ǰ���������뵽���Ͷ����
int32_t com_nrf24l01p_transmitEnd(void)
{
    if(! transmit_isPacketing)
        return 1;
    //
    // ��CRC�ŵ������档
    int32_t length_before = transmit_length[transmit_head];
    transmit_packet[transmit_head][length_before + 0]
        = ((uint8_t *)&transmit_currentCrc)[1];
    transmit_packet[transmit_head][length_before + 1] 
        = ((uint8_t *)&transmit_currentCrc)[0];
    transmit_length[transmit_head] += 2;
    //
    // �ƽ����У��൱�ڰѵ�ǰ�����뵽���С�
    transmit_head = (transmit_head + 1) % TRANSMIT_BUFFER_AMOUNT;
    if(transmit_head == transmit_tail)
        transmit_isFull = 1;
    //
    transmit_isPacketing = 0;
    return 0;
}

// packet:|version|type|content|crc|��
void rf_handleReceivedData(const uint8_t * packet,int32_t length)
{
    if(length < 4)
        return;
    if(math_crc16(0,packet,length) != 0)
        return;
    //
    engine_handlePacket_nrf(packet,length-2);
}
