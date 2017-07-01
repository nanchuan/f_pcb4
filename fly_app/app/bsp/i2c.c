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

// bsp/i2c.c
// 2013-1-7 18:08:35
// js200300953

#include "stm32f10x.h"
#include "i2c.h"

void i2c_bw_init(void)
{
    //
    // ��ʼ�����š�
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // B6 => CLK , B7 => DAT
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB,GPIO_Pin_6 | GPIO_Pin_7);
    //
    // ��ʼ��I2C��
    I2C_InitTypeDef i2c_param;
    I2C_StructInit(&i2c_param);
    i2c_param.I2C_ClockSpeed    = 400000;
    i2c_param.I2C_Mode          = I2C_Mode_I2C;
    i2c_param.I2C_DutyCycle     = I2C_DutyCycle_2;
    i2c_param.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1,&i2c_param);
}

// ����д����ֽ�����
uint32_t i2c_bw_write(uint8_t dev,uint8_t reg,const void * data,uint32_t len)
{
    //
    // Start��
    while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
    //
    // I2C��ַ��
    I2C_Send7bitAddress(I2C1,dev,I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    //
    // �Ĵ�����ַ��
    I2C_SendData(I2C1, reg);  
    while(! I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    //
    // ���ݡ�
    uint8_t * buf = (uint8_t *)data;
    for(int i=0;i<len;i++)
    {
        I2C_SendData(I2C1,buf[i]); 
        while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }
    //
    // Stop��
    I2C_GenerateSTOP(I2C1,ENABLE);
    //
    //
    return 0;
}

// ���ض������ֽ�����
uint32_t i2c_bw_read(uint8_t dev,uint8_t reg,void * data,uint32_t len)
{
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008

    /* Send START condition */
    I2C_GenerateSTART(I2C1, ENABLE);
    //*((u8 *)0x4001080c) &=~0x80;

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C1, dev, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* Clear EV6 by setting again the PE bit */
    I2C_Cmd(I2C1, ENABLE);

    /* Send the EEPROM's internal address to write to */
    I2C_SendData(I2C1, reg);  

    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send STRAT condition a second time */  
    I2C_GenerateSTART(I2C1, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send EEPROM address for read */
    I2C_Send7bitAddress(I2C1, dev, I2C_Direction_Receiver);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    int32_t bytesToRead = len;
    uint8_t * buf = (uint8_t *)data;
    while(bytesToRead)
    {
        if(len==1)
        {
            I2C_AcknowledgeConfig(I2C1, DISABLE);	        
            /* Send STOP Condition */
            I2C_GenerateSTOP(I2C1, ENABLE);
        } 
        /* Test on EV7 and clear it */
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));   
        {
            *buf = I2C_ReceiveData(I2C1);
            buf++;
            bytesToRead--;
        }
    }
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    //
    return 0;
}
