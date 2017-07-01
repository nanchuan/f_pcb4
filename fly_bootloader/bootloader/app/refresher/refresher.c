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

// app/refresher/refresher.c
// 2013-2-21 21:04:21
// js200300953

#include <stm32f10x.h>
#include <app/math/interface.h>
#include "refresher.h"
#include "flash_map.h"

// app�εĿ�ӳ�䡣
// app��ÿһ���鶼��Ӧrefresher_appWrittenBitMap��һ��λ��
int32_t refresher_appBlockBitMap[MAP_APP_SIZE_BLOCK/(sizeof(int32_t)*8) + 1];
int32_t refresher_appSize_block = 0; // app�ĳ��ȣ��Կ�Ϊ��λ��
int32_t refresher_appCrc = 0; // app��У���롣
int32_t refresher_checkCompleteStartIndex = 0; // ����Ƿ����ʱ����Ϊ��ʼ������
int32_t refresher_writtenAmount_block = 0; // �ۼ���д��Ŀ�����

#define STM32_SRAM_BITMAP_BASE 0x22000000
#define REFRESHER_APP_BLOCK_BITMAP_BASE_INT32 ((int32_t *)((int32_t)refresher_appBlockBitMap | STM32_SRAM_BITMAP_BASE))
// #define refresher_setBit(index) REFRESHER_APP_BLOCK_BITMAP_BASE_INT32[index]=1
// #define refresher_getBit(index) REFRESHER_APP_BLOCK_BITMAP_BASE_INT32[index]
#define refresher_setBit(index) refresher_appBlockBitMap[(index)/32] |= (1<<(index)%32)
#define refresher_getBit(index) (refresher_appBlockBitMap[(index)/32] & (1<<(index)%32))

int32_t refresher_init(void);
int32_t refresher_checkApp(void);
int32_t refresher_setAppSizeCrc(uint32_t size_block,uint32_t crc);
int32_t refresher_eraseApp(void);
int32_t refresher_write(const uint32_t *data,int32_t length,int32_t blockIndexOfFirst);
static int32_t refresher_skipWrittenBlock(void);
int32_t refresher_isWriteComplete(void);
int32_t refresher_getRemainBlock(uint8_t * buffer,int32_t bufferLength);
int32_t refresher_getWrittenAmount_block(void);
int32_t refresher_jumpToApp(void);

int32_t refresher_init(void)
{
    return 0;
}

// ���app�Ƿ���Ч��
// ����ֵ��
//    0 => app��Ч��
//   -1 => app���Ȳ�����
//   -2 => У��ʧ�ܡ�
int32_t refresher_checkApp(void)
{
    // ���app��Ч���ȵ���Ч�ԡ�
    uint32_t appValidBlockAmount = *MPA_APP_VALID_BLOCK_AMOUNT_ADDR_UINT32;
    if(appValidBlockAmount <= 0 || appValidBlockAmount > MAP_APP_SIZE_BLOCK)
        return -1;
    //
    // ����app��crc������MPA_APPCRC_ADDR_INT32���ֵ�Ƚϡ�
    uint32_t crc_cal = math_crc16(0,MAP_APP_BASE_BYTE,appValidBlockAmount*MAP_BLOCK_SIZE_BYTE);
    if(crc_cal != (uint32_t)*MPA_APP_CRC_ADDR_UINT32)
        return -2;
    //
    return 0;
}

int32_t refresher_setAppSizeCrc(uint32_t size_block,uint32_t crc)
{
    if(size_block > MAP_APP_SIZE_BYTE)
        return -1;
    //
    refresher_appSize_block = size_block;
    refresher_appCrc = crc;
    //
    return 0;
}

int32_t refresher_eraseApp(void)
{
    // �ж�refresher_appSizeByte�Ƿ����
    if(refresher_appSize_block < 0 || refresher_appSize_block > MAP_APP_SIZE_BLOCK)
        return -1;
    //
    // ����λӳ�䡣�����ȫ���鶼δд�롣
    for(int32_t i=0;i<sizeof(refresher_appBlockBitMap)/sizeof(refresher_appBlockBitMap[0]);i++)
        refresher_appBlockBitMap[i] = 0;
    refresher_writtenAmount_block = 0;
    refresher_checkCompleteStartIndex = 0;
    //
    // ����flash��
    FLASH_Unlock();
    //
    // ����refresher_appSizeByte������Ҫ�Ŀ飬���ȫ�������˷�ʱ�䡣
    int32_t pageAmount = (refresher_appSize_block / MPA_FLASH_PAGE_SIZE_BLOCK) + 1;
    for(int32_t i=0;i<pageAmount;i++)
        FLASH_ErasePage((uint32_t)(MAP_APP_BASE_PAGE+i));
    //
    // ����app���һ��page�����ڴ��len��crc��
    if(pageAmount < MAP_APP_SIZE_PAGE)
        FLASH_ErasePage((uint32_t)(MAP_APP_BASE_PAGE+MAP_APP_SIZE_PAGE-1));
    //
    // ˳��д��len��crc��
    FLASH_ProgramWord((uint32_t)MPA_APP_VALID_BLOCK_AMOUNT_ADDR_UINT32   ,refresher_appSize_block);
    FLASH_ProgramWord((uint32_t)MPA_APP_CRC_ADDR_UINT32                  ,refresher_appCrc);
    //
    FLASH_Lock();
    //
    return 0;
}

int32_t refresher_write(const uint32_t *data,int32_t length,int32_t blockIndexOfFirst)
{
    FLASH_Unlock();
    //
    for(int32_t i=0;i<length;i++)
    {
        // �Ѿ�д���Ͳ���д�ˡ�
        if(refresher_getBit(blockIndexOfFirst+i))
            continue;
        //
        // д��flash��
        FLASH_ProgramWord((uint32_t)(MAP_APP_BASE_BLOCK + (blockIndexOfFirst+i)),data[i]);
        //
        // ���λӳ��Ϊ��д��
        refresher_setBit(blockIndexOfFirst+i);
        refresher_writtenAmount_block ++;
    }
    //
    FLASH_Lock();
    //
    return 0;
}

// ���app�Ƿ�д����ɡ�
// ����refresher_checkCompleteStartIndex����Ŀ�����
// ���δ��ɣ����refresher_checkCompleteStartIndexָ����һ��δ��ɵĿ顣
// ������߶�β�ѯ��Ч�ʡ�
static int32_t refresher_skipWrittenBlock(void)
{
    int32_t old_index = refresher_checkCompleteStartIndex;
    //
    for(;
        refresher_checkCompleteStartIndex < refresher_appSize_block;
        refresher_checkCompleteStartIndex ++)
    {
        if(! refresher_getBit(refresher_checkCompleteStartIndex))
            return refresher_checkCompleteStartIndex-old_index;
    }
    for(refresher_checkCompleteStartIndex = 0;
        refresher_checkCompleteStartIndex<old_index;
        refresher_checkCompleteStartIndex++)
    {
        if(! refresher_getBit(refresher_checkCompleteStartIndex))
            return ((refresher_appSize_block-old_index) + refresher_checkCompleteStartIndex);
    }
    //
    return refresher_appSize_block;
}

int32_t refresher_isWriteComplete(void)
{
    return refresher_skipWrittenBlock() >= refresher_appSize_block;
}

// ��[{index_uint16,length_uint16},...]���δд���򡣷�������buffer���ֽ�����
int32_t refresher_getRemainBlock(uint8_t * buffer,int32_t bufferLength)
{
    if(bufferLength < 4)
        return -1;
    //
    if(refresher_isWriteComplete())
        return 0;
    //
    int32_t curLen = 0; // ����buffer������δд�������Ŀ������ֵ��
    int32_t step   = 0; // �ۼӲ����������жϽ�����
    //
    while(1)
    {
        // ������д���Ŀ顣
        step += refresher_skipWrittenBlock();
        if(step >= refresher_appSize_block)
            break;
        //
        // ���������ʾ��δд�Ŀ飬����refresher_checkCompleteStartIndexָ����һ��δд�Ŀ顣
        // ����refresher_checkCompleteStartIndex����Ѱ����һ����д�Ŀ顣
        uint16_t index = (uint16_t)refresher_checkCompleteStartIndex;
        for(;
            refresher_checkCompleteStartIndex < refresher_appSize_block;
            refresher_checkCompleteStartIndex ++)
        {
            if(refresher_getBit(refresher_checkCompleteStartIndex))
                break;
        }
        step += refresher_checkCompleteStartIndex - index;
        // refresher_checkCompleteStartIndex�Ѿ�ָ����һ����д�Ŀ飬���ߵ���ĩβ��
        //
        // ����ո�������δд�Ŀ�ĳ��ȡ�
        uint16_t len = refresher_checkCompleteStartIndex - index;
        //
        // ���refresher_checkCompleteStartIndex����ĩβ��Ҫ�ص�ǰ�档
        if(refresher_checkCompleteStartIndex >= refresher_appSize_block)
            refresher_checkCompleteStartIndex = 0;
        //
        // ����index��len��buffer��
        buffer[curLen ++] = ((uint8_t *)&index)[0];
        buffer[curLen ++] = ((uint8_t *)&index)[1];
        buffer[curLen ++] = ((uint8_t *)&len)[0];
        buffer[curLen ++] = ((uint8_t *)&len)[1];
        //
        // �ж��Ƿ����㹻�ռ�װ��һ����
        if(bufferLength < (curLen+2+2))
            break;
    }
    //
    return curLen;
}

int32_t refresher_getWrittenAmount_block(void)
{
    return refresher_writtenAmount_block;
}

int32_t refresher_jumpToApp(void)
{
    //
    //while(1);
    // "void foo(void)"���͵ĺ���ָ�롣
    // +4��Ϊ��һ������ջ��ַ���ڶ����ֲ��Ǹ�λ�����ĺ���ָ�루ע�����Ǻ�����ַ��
    //(* ((void (*)(void))(MAP_FLASH_BASE_BYTE+4)) )();
    //NVIC_SetVectorTable(NVIC_VectTab_FLASH,(uint32_t)MAP_APP_BASE_BYTE);
    int32_t pReset = *(int32_t *)(MAP_APP_BASE_BYTE+4);
    __disable_irq();
    ((void (*)(void))(pReset))();
    //
    return 0;
}
