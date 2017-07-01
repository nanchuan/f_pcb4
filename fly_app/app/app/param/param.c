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

// app/param/param.c
// 2013-4-8 9:07:23
// js200300953

#include <bsp/flash.h>
#include <app/math/interface.h>
#include <app/communication/interface.h>
#include "param.h"

#define PARAM_FLASH_ADDRESS (0x08000000 + 63*1024)
#define PARAM_VERSION 2

static param_t param_ram;
static const param_t * param_flash = (const param_t *)PARAM_FLASH_ADDRESS;

int32_t         param_init(void);
const param_t * param_getRamParam(void);
param_t *       param_setRamParam(void);
const param_t * param_getFlashParam(void);
int32_t         param_writeToFlash(void);
int32_t         param_resetFlash(void);
static int32_t  param_loadDefault(param_t * param);
static int32_t  param_refreshCrc(param_t * param);
static int32_t  param_checkValid(const param_t * param);

// ��ʼ������ģ�顣
int32_t param_init(void)
{
    if(param_checkValid(param_flash) == 0)
        param_ram = *param_flash;
    else
        param_loadDefault(&param_ram);
    //
    return 0;
}

// ���ز���Ĭ��ֵ��
int32_t param_loadDefault(param_t * param)
{
    // ȫ����0�����ⲻС��û��ʼ����
    for(int i=0;i<sizeof(param);i++)
        ((uint8_t *)param)[i] = 0;
    //
    param->version = PARAM_VERSION;
    param->length  = sizeof(param_t) / 4;
    param->reserve_zero_0 = 0;
    //
    for(int i=0;i<3;i++)
    {
        param->acc_gain[i]   = 9.7883 / 4096.0;
        param->acc_offset[i] = 0;
        param->gyr_gain[i]   = (3.1415926/180.0) / 16.4;
        param->gyr_offset[i] = 0;
        param->mag_gain[i]   = 1;
        param->mag_offset[i] = 0;
    }
    //
    param->horizon.w = 1;
    param->horizon.x = 0;
    param->horizon.y = 0;
    param->horizon.z = 0;
    //
    for(int i=0;i<3;i++)
    {
        param->xyz_pid[i].p = 0.4;
        param->xyz_pid[i].i = 0.01;
        param->xyz_pid[i].d = 12;
    }
    //
    param->haltVoltage = 3.3; // Ĭ�ϵ���3.3Vͣ����
    param->haltInclination_cos = 0.5; // Ĭ��60��ͣ����
    //
    param_refreshCrc(param);
    return 0;
}

// ���������CRC��
int32_t param_refreshCrc(param_t * param)
{
    uint8_t * crc_var = (uint8_t *)&(param->crc);
    crc_var[0] = 0;
    crc_var[1] = 0;
    uint16_t crc16_ccitt = math_crc16(0,param,sizeof(param_t)-2);
    crc_var[2] = ((uint8_t *)&crc16_ccitt)[1];
    crc_var[3] = ((uint8_t *)&crc16_ccitt)[0];
    //
    return 0;
}

// �������Ƿ���Ч��
// �汾���ԣ�CRCУ��ʧ�ܶ��ᵼ�²�����Ч��
int32_t param_checkValid(const param_t * param)
{
    if(math_crc16(0,param,sizeof(param_t)) != 0)
        return -1;
    if(param->version != PARAM_VERSION)
        return -2;
    return 0;
}

// ��ȡRAM��Ĳ�����
const param_t * param_getRamParam(void)
{
    return &param_ram;
}

// ����RAm��Ĳ�����
param_t * param_setRamParam(void)
{
    return &param_ram;
}

// ��ȡFlash��Ĳ�����
const param_t * param_getFlashParam(void)
{
    return param_flash;
}

// ��RAM�еĲ���д�뵽Flash�
int32_t param_writeToFlash(void)
{
    param_refreshCrc(&param_ram);
    if(param_checkValid(&param_ram) != 0)
        return -1;
    //
    // ����Ƿ���Ҫд�������ظ�д�롣
    int32_t same = 1;
    for(int i=0;i<sizeof(param_t);i++)
    {
        if(((const uint8_t *)param_flash)[i] != ((uint8_t *)&param_ram)[i])
        {
            same = 0;
            break;
        }
    }
    if(same)
    {
        cmd_returnMessage(
					"\xE5\x8F\x82\xE6\x95" \
					"\xB0\xE7\x9B\xB8\xE5" \
					"\x90\x8C\xEF\xBC\x8C" \
					"\xE4\xB8\x8D\xE9\x9C" \
					"\x80\xE5\x86\x99\xE5" \
					"\x85\xA5\xE3\x80\x82");//"������ͬ������д��"
        return 0;
    }
    //
    flash_erase((const uint32_t *)param_flash);
    flash_write((const uint32_t *)param_flash,(const uint32_t *)&param_ram,sizeof(param_t)/4);
    //
    if(param_checkValid(param_flash) != 0)
        return -2;
    //
    cmd_returnMessage("\xE5\x8F\x82\xE6\x95\xB0\xE5\xB7\xB2\xE5\x86\x99\xE5\x85\xA5""Flash");//"������д��Flash" Դ�ļ�Ҫ��UTF8���롣
    return 0;
}

int32_t param_resetFlash(void)
{
    param_loadDefault(&param_ram);
    param_writeToFlash();
    //
    return 0;
}
