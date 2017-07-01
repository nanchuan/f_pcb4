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

// bsp/nrf24l01p.c
// 2012-03-30 08:56:35

#include <stdint.h>
#include "rf.h"
#include "rf_hal.h"

#define _BV(n) (1<<(n))

/*
 * address��*/
#define RF_LOCAL_ADDRESS ((uint32_t)0x5AB76476)

/*
 * command��*/
#define RF_CMD_R_REGISTER   0x00/*0b00000000*/  /* ���Ĵ�����0b000aaaaa��aΪ��ַ�� */
#define RF_CMD_W_REGISTER   0x20/*0b00100000*/  /* д�Ĵ�����0b001aaaaa��aΪ��ַ�� */
    #define RF_CMD_RWR_MASK 0x1F/*0b00011111*/  /* �Ĵ�����ַ���룬0bcccaaaaa */
#define RF_CMD_R_RX_PAYLOAD 0x61/*0b01100001*/  /* ��ȡ���ݡ� */
#define RF_CMD_W_TX_PAYLOAD 0xA0/*0b10100000*/  /* �������ݡ� */
#define RF_CMD_FLUSH_TX     0xE1/*0b11100001*/  /* ��շ��ͻ��档 */
#define RF_CMD_FLUSH_RX     0xE2/*0b11100010*/  /* ��ս��ջ��档 */
#define RF_CMD_R_RX_PL_WID  0x60/*0b01100000*/  /* ��ȡ���յ������ݵĳ��ȡ� */
#define RF_CMD_NOP          0xFF/*0b11111111*/  /* �ղ�����������STATUS�Ĵ����� */

/*
 * regester map*/
#define RF_REG_CONFIG 0x00
    #define RF_REG_CONFIG_MASK_RX_DR    _BV(6) /* �����жϡ� */
    #define RF_REG_CONFIG_MASK_TX_DS    _BV(5) /* �����жϡ� */
    #define RF_REG_CONFIG_MASK_MAX_RT   _BV(4) /* �ط�ʧ���жϡ� */
    #define RF_REG_CONFIG_EN_CRC        _BV(3) /* ʹ��CRC�� */
    #define RF_REG_CONFIG_CRCO          _BV(2) /* ʹ��2λCRC�� */
    #define RF_REG_CONFIG_PWR_UP        _BV(1) /* ���Ͻ���Standby1״̬�� */
    #define RF_REG_CONFIG_PRIM_RX       _BV(0) /* �Ƿ���Ϊ�������� */
    #define RF_REG_CONFIG_DEFAULT_R        /* Ĭ�ϡ� */ \
        (RF_REG_CONFIG_CRCO | RF_REG_CONFIG_EN_CRC | \
        RF_REG_CONFIG_PWR_UP | RF_REG_CONFIG_PRIM_RX)
    #define RF_REG_CONFIG_DEFAULT_T        /* Ĭ�ϡ� */ \
        (RF_REG_CONFIG_CRCO | RF_REG_CONFIG_EN_CRC | \
        RF_REG_CONFIG_PWR_UP)
#define RF_REG_EN_AA 0x01
    #define RF_REG_EN_AA_ENAA_P0    _BV(0) /* 0ͨ���Զ�ACK�� */
    #define RF_REG_EN_AA_DEFAULT    RF_REG_EN_AA_ENAA_P0 /* Ĭ�ϡ� */
#define RF_REG_EN_RXADDR 0x02
    #define RF_REG_EN_RXADDR_ERX_P0 _BV(0) /* ʹ��0ͨ���� */
    #define RF_REG_EN_RXADDR_DEFAULT RF_REG_EN_RXADDR_ERX_P0 /* Ĭ�ϡ� */
#define RF_REG_SETUP_AW 0x03
    #define RF_REG_SETUP_AW_N(n)    (n-2)  /* ʹ��nλ��ַ��n:[3,5] */
    #define RF_REG_SETUP_AW_DEFAULT RF_REG_SETUP_AW_N(4) /* Ĭ�� */
#define RF_REG_SETUP_RETR 0x04
    #define RF_REG_SETUP_RETR_ARD(n)    ((uint8_t)n<<4)   /* (n+1)*250��sû�յ�ACK���ط��� */
    #define RF_REG_SETUP_RETR_ARC(n)    ((uint8_t)n&0x0F) /* ����ط�n�Ρ� */
    #define RF_REG_SETUP_RETR_DEFAULT   /* Ĭ�� */ \
        (RF_REG_SETUP_RETR_ARD(3) | RF_REG_SETUP_RETR_ARC(4))
#define RF_REG_RF_CH 0x05
    #define RF_REG_RF_CH_MHZ(n)      ((uint8_t)(n-2400)&(~_BV(7))) /* ����Ƶ�ʣ�Mhz��[2400,2525]��*/
    #define RF_REG_RF_CH_DEFAULT     RF_REG_RF_CH_MHZ(2400) /* Ĭ�� */
#define RF_REG_RF_SETUP 0x06
    #define RF_REG_RF_SETUP_CONT_WAVE   _BV(7) /* �����������͡� */
    #define RF_REG_RF_SETUP_RATE_250K   _BV(5) /* �������ʣ�250kBps�� */
    #define RF_REG_RF_SETUP_RATE_1M     0      /* �������ʣ�1MBps�� */
    #define RF_REG_RF_SETUP_RATE_2M     _BV(3) /* �������ʣ�2MBps�� */
    #define RF_REG_RF_SETUP_PWR(n)      (n<<1) /* ���÷��书�ʣ�(-6*(3-n))dBm�� */
    #define RF_REG_RF_SETUP_DEFAULT     /* Ĭ�� */ \
        (RF_REG_RF_SETUP_RATE_2M | RF_REG_RF_SETUP_PWR(3))
#define RF_REG_STATUS 0x07
    #define RF_REG_STATUS_RX_DR     _BV(6) /* ���յ����ݡ� */
    #define RF_REG_STATUS_TX_DS     _BV(5) /* ���������ݡ� */
    #define RF_REG_STATUS_MAX_RT    _BV(4) /* �ط������������� */
    #define RF_REG_STATUS_TX_FULL   _BV(0) /* ���ͻ��������� */
#define RF_REG_FIFO_STATUS 0x17
    #define RF_REG_FIFO_STATUS_TX_REUSE     _BV(6) /*  */
    #define RF_REG_FIFO_STATUS_TX_FULL      _BV(5) /* 1:���Ͷ������� */
    #define RF_REG_FIFO_STATUS_TX_EMPTY     _BV(4) /* 1:���Ͷ��пա� */
    #define RF_REG_FIFO_STATUS_RX_FULL      _BV(1) /* 1:���ն������� */
    #define RF_REG_FIFO_STATUS_RX_EMPTY     _BV(0) /* 1:���ն��пա� */
#define RF_REG_DYNPD 0x1C
    #define RF_REG_DYNPD_DPL_P0 _BV(0) /* ����ͨ��0Ϊ��̬���ݳ��ȡ� */
    #define RF_REG_DYNPD_DEFAULT RF_REG_DYNPD_DPL_P0 /* Ĭ�ϡ� */
#define RF_REG_FEATURE 0x1D
    #define RF_REG_FEATURE_EN_DPL   _BV(2) /* ����̬���ݳ��ȡ� */
    #define RF_REG_FEATURE_DEFAULT RF_REG_FEATURE_EN_DPL /* Ĭ�ϡ� */
#define RF_REG_RX_ADDR_P0 0x0A /* ���յĵ�ַ */
#define RF_REG_TX_ADDR    0x10 /* ����Ŀ�ĵ�ַ */
#define RF_REG_RX_PW_P0   0x11 /* 0����ͨ���յ������ݵĳ��ȡ� */

int32_t     rf_init(void);
int32_t     rf_transmit(const uint8_t * data,uint32_t len);
void        rf_startReceive(void);
void        rf_stopReceive(void);
void        rf_checkEvent(void);
static void     rf_writeRegester(uint8_t addr,uint8_t val);
static uint8_t  rf_readRegester(uint8_t addr);
//static void     rf_writeAddress(uint8_t addr_reg,uint32_t addr_rf);
static void     rf_writeData(const uint8_t * data,uint8_t len);
static uint8_t  rf_readData(uint8_t * data,uint8_t * len);
static void     rf_getStatus(uint8_t * status,uint8_t * fifo_status);
static uint8_t  rf_getReceivedDataLength(void);
static void     rf_flushTxFifo(void);
static void     rf_flushRxFifo(void);

// ����0��ʾ�ɹ���
int32_t rf_init(void)
{
    // ��ʼ����š�
    rf_hal_init();
    //
    // ��ʼ���Ĵ�����
    const static uint8_t reg_value[][2] =
    {
        {RF_REG_CONFIG      ,RF_REG_CONFIG_DEFAULT_R  },
        {RF_REG_EN_RXADDR   ,RF_REG_EN_RXADDR_DEFAULT },
        {RF_REG_EN_AA       ,RF_REG_EN_AA_DEFAULT     },
        {RF_REG_SETUP_AW    ,RF_REG_SETUP_AW_DEFAULT  },
        {RF_REG_SETUP_RETR  ,RF_REG_SETUP_RETR_DEFAULT},
        {RF_REG_RF_CH       ,RF_REG_RF_CH_DEFAULT     },
        {RF_REG_RF_SETUP    ,RF_REG_RF_SETUP_DEFAULT  },
        {RF_REG_DYNPD       ,RF_REG_DYNPD_DEFAULT     },
        {RF_REG_FEATURE     ,RF_REG_FEATURE_DEFAULT   },
    };
    for(uint32_t i=0;i<sizeof(reg_value)/2;i++)
        rf_writeRegester(reg_value[i][0],reg_value[i][1]);
    for(uint32_t i=0;i<sizeof(reg_value)/2;i++)
        if(rf_readRegester(reg_value[i][0]) != reg_value[i][1])
            return 1 + reg_value[i][0] * 1e3 + reg_value[i][1]*1e6;
    //
    rf_flushTxFifo();
    rf_flushRxFifo();
    //
    return 0;
}

/*
 * �������ݡ�
 * ����ֵ��
 *  0���Ѿ��͵�RFоƬ��
 *  1��RFоƬ������������*/
int32_t rf_transmit(const uint8_t * data,uint32_t len)
{
    /*
     * ��ȡ״̬��*/
    rf_hal_spiBegin();
    uint8_t status = rf_hal_spiSwap(RF_CMD_NOP);
    rf_hal_spiEnd();
    /*
     * �ж��Ƿ��п�λ��*/
    if(status & RF_REG_STATUS_TX_FULL)
        return 1;
    //
    rf_hal_ceLow();
    rf_writeRegester(RF_REG_CONFIG,RF_REG_CONFIG_DEFAULT_T);
    rf_writeData(data,len);
    rf_hal_ceHigh();
    //
    return 0;
}

void rf_startReceive(void)
{
    rf_hal_ceLow();
    rf_writeRegester(RF_REG_CONFIG,RF_REG_CONFIG_DEFAULT_R);
    rf_hal_ceHigh();
}

void rf_stopReceive(void)
{
    rf_hal_ceLow();
}

void rf_writeRegester(uint8_t addr,uint8_t val)
{
    rf_hal_spiBegin();
    //
    rf_hal_spiSwap(RF_CMD_W_REGISTER | (addr & RF_CMD_RWR_MASK));
    rf_hal_spiSwap(val);
    //
    rf_hal_spiEnd();
}

uint8_t rf_readRegester(uint8_t addr)
{
    rf_hal_spiBegin();
    //
    rf_hal_spiSwap(RF_CMD_R_REGISTER | (addr & RF_CMD_RWR_MASK));
    uint8_t rt = rf_hal_spiSwap(0);
    //
    rf_hal_spiEnd();
    //
    return rt;
}

/*
void rf_writeAddress(uint8_t addr_reg,uint32_t addr_rf)
{
    rf_hal_spiBegin();
    //
    rf_hal_spiSwap(RF_CMD_W_REGISTER | (addr_reg & RF_CMD_RWR_MASK));
    rf_hal_spiSwap((uint8_t)addr_rf);
    addr_rf >>= 8;
    rf_hal_spiSwap((uint8_t)addr_rf);
    addr_rf >>= 8;
    rf_hal_spiSwap((uint8_t)addr_rf);
    addr_rf >>= 8;
    rf_hal_spiSwap((uint8_t)addr_rf);
    //
    rf_hal_spiEnd();
}
*/
extern uint8_t uart2_transmit(const void * data,uint32_t len);
void rf_checkEvent(void)
{
    /*
     * �ж��߱����ͣ���ʾ���жϡ�*/
    if(! rf_hal_irqIsHigh())
    {
        /*
         * ��ȡ״̬��*/
        uint8_t status = 0;
        uint8_t fifo_status = 0;
        rf_getStatus(&status,&fifo_status);
        /*
         * �յ����ݡ� */
        if(status & RF_REG_STATUS_RX_DR)
        {
            rf_stopReceive();
            //
			uint8_t max_times = 3;
            while(!(fifo_status & RF_REG_FIFO_STATUS_RX_EMPTY) && max_times--)
            {
                /*
                 * ȡ�����������ݡ�*/
                uint8_t buf[32];
                uint8_t len = 0;
                if(rf_readData(buf,&len) != 0)
                    break;
      							rf_handleReceivedData(buf, len);
                /*
                 * ��ȡ״̬��*/
                rf_getStatus(&status,&fifo_status);
            }
            //
            rf_startReceive();
        }
        /*
         * ���������ݡ� */
        if(status & RF_REG_STATUS_TX_DS)
        {
            /*
             * û�еȴ������ݾ�ֹͣ���͡� */
            if(fifo_status & RF_REG_FIFO_STATUS_TX_EMPTY)
                rf_startReceive();
        }
        /*
         * �ط�������� */
        if(status & RF_REG_STATUS_MAX_RT)
        {
            /*
             * ��ջ�������*/
            rf_flushTxFifo();
            rf_startReceive();
        }
        /*
         * ȡ���жϱ�ʶ��*/
        rf_writeRegester(RF_REG_STATUS,status);
    }
}

void rf_writeData(const uint8_t * data,uint8_t len)
{
    rf_hal_spiBegin();
    //
    rf_hal_spiSwap(RF_CMD_W_TX_PAYLOAD);
    for(uint8_t i=0;i<len;i++)
        rf_hal_spiSwap(data[i]);
    //
    rf_hal_spiEnd();
}

/*
 * ȡ�����ݡ�
 * ����ֵ��
 *  0���ɹ���
 *  1��ʧ�ܣ������Ѿ���ս��ջ�������*/
uint8_t rf_readData(uint8_t * p_data,uint8_t * p_len)
{
    /*
     * ������ݳ�����Ч�ԡ�*/
    uint8_t len = rf_getReceivedDataLength();
    if(len > 32)
    {
        /*
         * ������Ч����ջ�������*/
         rf_flushRxFifo();
        return 1;
    }
    /*
     * ȡ�����ݡ�*/
    rf_hal_spiBegin();
    rf_hal_spiSwap(RF_CMD_R_RX_PAYLOAD);
    for(uint8_t i=0;i<len;i++)
        p_data[i] = rf_hal_spiSwap(0);
    rf_hal_spiEnd();
    //
    *p_len = len;
    return 0;
}

uint8_t rf_getReceivedDataLength(void)
{
    uint8_t rt = 0;
    //
    rf_hal_spiBegin();
    rf_hal_spiSwap(RF_CMD_R_RX_PL_WID);
    rt = rf_hal_spiSwap(0);
    rf_hal_spiEnd();
    //
    return rt;
}

void rf_getStatus(uint8_t * status,uint8_t * fifo_status)
{
    rf_hal_spiBegin();
    *status = rf_hal_spiSwap(RF_CMD_R_REGISTER | (RF_REG_FIFO_STATUS & RF_CMD_RWR_MASK));
    *fifo_status = rf_hal_spiSwap(0);
    rf_hal_spiEnd();
}

void rf_flushRxFifo(void)
{
    rf_hal_spiBegin();
    rf_hal_spiSwap(RF_CMD_FLUSH_RX);
    rf_hal_spiEnd();
}

void rf_flushTxFifo(void)
{
    rf_hal_spiBegin();
    rf_hal_spiSwap(RF_CMD_FLUSH_TX);
    rf_hal_spiEnd();
}
