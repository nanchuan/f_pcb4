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

// app/engine/engine1.c
// 2013-2-21 22:27:05
// js200300953

#include <bsp/time.h>
#include <bsp/pin.h>
#include <app/refresher/interface.h>
#include <app/communication/interface.h>
#include "engine1.h"

// 公共临时数组，足够大。
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
		// 先校验app，如果app校验不通过，留在bootloader等待命令。
    if(refresher_checkApp() != 0)
        engine1_status = ENGINE1_STATUS_STAY_AT_BOOTLOADER;
    //
    // 检查通信，判断是否需要刷app。
    uint32_t timeout = time_nowMs() + 500;
    // 等待发生超时或者收到命令。
    while(timeout > time_nowMs() && engine1_status == ENGINE1_STATUS_INIT)
    {
        // 如果需要刷app，通信模块收到命令后会调用engine1_stayAtBootloader()。
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
    // 检查是否需要刷app，不需要就调用app。
    if(engine1_status == ENGINE1_STATUS_INIT)
    {
        refresher_jumpToApp();
        return 0;
    }
    //
    // 来到这里表示要停在bootloader了，或许要刷app。
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
        // 进入bootloader模式。
        case PROTOCOL_BOOTLOADER_CMD_ENTER_BOOTLOADER :
        {
            engine1_status = ENGINE1_STATUS_STAY_AT_BOOTLOADER;
            engine1_reportStatus(PROTOCOL_BOOTLOADER_STATUS_STAY_AT_BOOTLOADER);
        }
        break;
        //
        // 开始刷app。
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
        // 收到app的长度和crc
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
        // 写app。
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
            // 不需要，因为当处于ENGINE1_STATUS_WRITING_APP状态时，每100ms会自动返回一次状态。
            else
            {
                engine1_reportStatus(PROTOCOL_BOOTLOADER_STATUS_WRITING_APP);
            }
            */
        }
        break;
        //
        // 请求剩余未写的块。
        case PROTOCOL_BOOTLOADER_CMD_QUEST_REMAIN_BLOCK :
        {
            // 如果写入已经结束，返回写入完成状态，否则按正常返回未写的块。
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
        // 进入app。
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
    // 获取块号，注意字节对齐问题。
    uint16_t blockIndex = 0;
    ((uint8_t *)&blockIndex)[0] = param[0];
    ((uint8_t *)&blockIndex)[1] = param[1];
    //
    // 获取app数据，因为字节对齐问题，所以要复制一次。
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
    // 开始一个发送会话。
    if(com_transmitBegin(PROTOCOL_BOOTLOADER_STATUS) != 0)
        return -1;
    //
    // 状态是必须发的。
    com_transmitContent(&returnStatus,1);
    //
    // 根据不同的状态，有不同的参数。
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
    // 结束会话。
    com_transmitEnd();
    return 0;
}
