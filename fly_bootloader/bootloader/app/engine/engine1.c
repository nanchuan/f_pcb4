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

// app/engine/engine1.c
// 2013-2-21 22:27:05
// js200300953

#include <bsp/time.h>
#include <bsp/pin.h>
#include <app/refresher/interface.h>
#include <app/communication/interface.h>
#include "engine1.h"

// ������ʱ���飬�㹻��
#define ENGINE1_PUBLIC_BUFFER_SIZE_BLOCK 16
#define ENGINE1_PUBLIC_BUFFER_SIZE_BYTE (ENGINE1_PUBLIC_BUFFER_SIZE_BLOCK*MAP_BLOCK_SIZE_BYTE)

static enum
{
    ENGINE1_STATUS_INIT = 1,
    ENGINE1_STATUS_STAY_AT_BOOTLOADER,
    ENGINE1_STATUS_READY_TO_REFRESH_APP,
    ENGINE1_STATUS_WRITING_APP,
    ENGINE1_STATUS_WRITING_APP_COMPLETE_CRC_ERROR,
    ENGINE1_STATUS_WRITE_APP_SUCCEED,
}engine1_status = ENGINE1_STATUS_INIT;

static uint32_t engine1_publicBuffer_u32[ENGINE1_PUBLIC_BUFFER_SIZE_BLOCK];
static uint8_t *engine1_publicBuffer = (uint8_t *)engine1_publicBuffer_u32;

int32_t engine1_init(void);
int32_t engine1_loop(void);
int32_t engine1_onCmd(const uint8_t *param,int32_t length);
static int32_t engine1_onWriteFlash(const uint8_t *param,int32_t length);
static int32_t engine1_reportStatus(uint8_t returnStatus);

int32_t engine1_init(void)
{
    return 0;
}

int32_t engine1_loop(void)
{
		// ��У��app�����appУ�鲻ͨ��������bootloader�ȴ����
    if(refresher_checkApp() != 0)
        engine1_status = ENGINE1_STATUS_STAY_AT_BOOTLOADER;
    //
    // ���ͨ�ţ��ж��Ƿ���Ҫˢapp��
    uint32_t timeout = time_nowMs() + 500;
    // �ȴ�������ʱ�����յ����
    while(timeout > time_nowMs() && engine1_status == ENGINE1_STATUS_INIT)
    {
        // �����Ҫˢapp��ͨ��ģ���յ����������engine1_stayAtBootloader()��
        communication_checkEvent();
        //
        TIME_EXECUTE_PERIODICALLY_BEGIN(10)
        {
            if(! pin_usbDmIsHigh())
            {
                engine1_status = ENGINE1_STATUS_STAY_AT_BOOTLOADER;
                break;
            }
        }TIME_EXECUTE_PERIODICALLY_END();
    }
    //
    // ����Ƿ���Ҫˢapp������Ҫ�͵���app��
    if(engine1_status == ENGINE1_STATUS_INIT)
    {
        refresher_jumpToApp();
        return 0;
    }
    //
    // ���������ʾҪͣ��bootloader�ˣ�����Ҫˢapp��
    while(1)
    {
        communication_checkEvent();
        //
        TIME_EXECUTE_PERIODICALLY_BEGIN(100)
        {
            switch(engine1_status)
            {
            case ENGINE1_STATUS_WRITING_APP :
                engine1_reportStatus(PROTOCOL_BOOTLOADER_STATUS_WRITING_APP);
                break;
            }
        } TIME_EXECUTE_PERIODICALLY_END()
    }
}

int32_t engine1_onCmd(const uint8_t *param,int32_t length)
{
    int32_t cmdCode = param[0];
    switch(cmdCode)
    {
        // ����bootloaderģʽ��
        case PROTOCOL_BOOTLOADER_CMD_ENTER_BOOTLOADER :
        {
            engine1_status = ENGINE1_STATUS_STAY_AT_BOOTLOADER;
            engine1_reportStatus(PROTOCOL_BOOTLOADER_STATUS_STAY_AT_BOOTLOADER);
        }
        break;
        //
        // ��ʼˢapp��
        case PROTOCOL_BOOTLOADER_CMD_BEGIN_REFRESH_APP :
        {
            if(engine1_status != ENGINE1_STATUS_STAY_AT_BOOTLOADER
                && engine1_status != ENGINE1_STATUS_READY_TO_REFRESH_APP)
                break;
            engine1_status = ENGINE1_STATUS_READY_TO_REFRESH_APP;
            engine1_reportStatus(PROTOCOL_BOOTLOADER_STATUS_READY_TO_REFRESH_APP);
        }
        break;
        //
        // �յ�app�ĳ��Ⱥ�crc
        case PROTOCOL_BOOTLOADER_CMD_APP_LENGTH_CRC :
        {
            if(engine1_status == ENGINE1_STATUS_WRITING_APP)
            {
                engine1_reportStatus(PROTOCOL_BOOTLOADER_STATUS_WRITING_APP);
                break;
            }
            if(engine1_status != ENGINE1_STATUS_READY_TO_REFRESH_APP)
                break;
            //
            uint32_t length_block = 0;
            uint32_t crc = 0;
            for(int32_t i=0;i<4;i++)
            {
                ((uint8_t *)&length_block)[i] = param[i+1];
                ((uint8_t *)&crc         )[i] = param[i+5];
            }
            //
            if(refresher_setAppSizeCrc(length_block,crc) != 0)
                break;
            if(refresher_eraseApp() != 0)
                break;
            engine1_status = ENGINE1_STATUS_WRITING_APP;
            engine1_reportStatus(PROTOCOL_BOOTLOADER_STATUS_WRITING_APP);
        }
        break;
        //
        // дapp��
        case PROTOCOL_BOOTLOADER_CMD_WRITE_APP :
        {
            if(engine1_status != ENGINE1_STATUS_WRITING_APP)
                break;
            engine1_onWriteFlash(param+1,length-1);
            //
            if(refresher_isWriteComplete())
            {
                if(refresher_checkApp() == 0)
                {
                    engine1_status = ENGINE1_STATUS_WRITE_APP_SUCCEED;
                    engine1_reportStatus(PROTOCOL_BOOTLOADER_STATUS_WRITE_APP_SUCCEED);
                }
                else
                {
                    engine1_status = ENGINE1_STATUS_WRITING_APP_COMPLETE_CRC_ERROR;
                    engine1_reportStatus(PROTOCOL_BOOTLOADER_STATUS_APP_CRC_ERROR);
                }
            }
            /*
            // ����Ҫ����Ϊ������ENGINE1_STATUS_WRITING_APP״̬ʱ��ÿ100ms���Զ�����һ��״̬��
            else
            {
                engine1_reportStatus(PROTOCOL_BOOTLOADER_STATUS_WRITING_APP);
            }
            */
        }
        break;
        //
        // ����ʣ��δд�Ŀ顣
        case PROTOCOL_BOOTLOADER_CMD_QUEST_REMAIN_BLOCK :
        {
            // ���д���Ѿ�����������д�����״̬��������������δд�Ŀ顣
            if(engine1_status == ENGINE1_STATUS_WRITE_APP_SUCCEED)
                engine1_reportStatus(PROTOCOL_BOOTLOADER_STATUS_WRITE_APP_SUCCEED);
            else if(engine1_status == ENGINE1_STATUS_WRITING_APP_COMPLETE_CRC_ERROR)
                engine1_reportStatus(PROTOCOL_BOOTLOADER_STATUS_APP_CRC_ERROR);
            else
                engine1_reportStatus(PROTOCOL_BOOTLOADER_STATUS_BLOCK_REMAIN);
            //
        }
        break;
        //
        // ����app��
        case PROTOCOL_BOOTLOADER_CMD_ENTER_APP :
        {
            refresher_jumpToApp();
        }
        break;
    }
    //
    return 0;
}

static int32_t engine1_onWriteFlash(const uint8_t *param,int32_t length)
{
    if(engine1_status != ENGINE1_STATUS_WRITING_APP || length < (2+4))
        return -1;
    //
    // ��ȡ��ţ�ע���ֽڶ������⡣
    uint16_t blockIndex = 0;
    ((uint8_t *)&blockIndex)[0] = param[0];
    ((uint8_t *)&blockIndex)[1] = param[1];
    //
    // ��ȡapp���ݣ���Ϊ�ֽڶ������⣬����Ҫ����һ�Ρ�
    int32_t len_block = (length - 2)/4;
    if(len_block > ENGINE1_PUBLIC_BUFFER_SIZE_BLOCK)
        len_block = ENGINE1_PUBLIC_BUFFER_SIZE_BLOCK;
    int32_t len_byte = len_block * MAP_BLOCK_SIZE_BYTE;
    for(int i=0;i<len_byte;i++)
        ((uint8_t *)engine1_publicBuffer_u32)[i] = param[i+2];
    //
    if(refresher_write(engine1_publicBuffer_u32,len_block,blockIndex) != 0)
        return -2;
    //
    return 0;
}

static int32_t engine1_reportStatus(uint8_t returnStatus)
{
    // ��ʼһ�����ͻỰ��
    if(com_transmitBegin(PROTOCOL_BOOTLOADER_STATUS) != 0)
        return -1;
    //
    // ״̬�Ǳ��뷢�ġ�
    com_transmitContent(&returnStatus,1);
    //
    // ���ݲ�ͬ��״̬���в�ͬ�Ĳ�����
    switch(returnStatus)
    {
        case PROTOCOL_BOOTLOADER_STATUS_WRITING_APP :
        {
            uint32_t blocksWrittenAmount = refresher_getWrittenAmount_block();
            com_transmitContent(&blocksWrittenAmount,4);
        }
        break;
        case PROTOCOL_BOOTLOADER_STATUS_BLOCK_REMAIN:
        {
            int32_t validLen = refresher_getRemainBlock(engine1_publicBuffer,com_maxTransmitContentSize()-1);
            if(validLen < 0)
                return -2;
            //
            com_transmitContent(engine1_publicBuffer,validLen);
        }
        break;
    }
    //
    // �����Ự��
    com_transmitEnd();
    return 0;
}
