//     Copyright (c) 2013 js200300953@qq.com All rights reserved.
//         ==================================================
//         ========圆点博士微型四轴飞行器配套软件声明========
//         ==================================================
//     圆点博士微型四轴飞行器配套软件包括上位机程序、下位机Bootloader
// 、下位机App和遥控程序，及它们的源代码，以下总称“软件”。
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

// 初始化参数模块。
int32_t param_init(void)
{
    if(param_checkValid(param_flash) == 0)
        param_ram = *param_flash;
    else
        param_loadDefault(&param_ram);
    //
    return 0;
}

// 加载参数默认值。
int32_t param_loadDefault(param_t * param)
{
    // 全部清0，避免不小心没初始化。
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
    param->haltVoltage = 3.3; // 默认低于3.3V停机。
    param->haltInclination_cos = 0.5; // 默认60°停机。
    //
    param_refreshCrc(param);
    return 0;
}

// 计算参数的CRC。
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

// 检查参数是否有效。
// 版本不对，CRC校验失败都会导致参数无效。
int32_t param_checkValid(const param_t * param)
{
    if(math_crc16(0,param,sizeof(param_t)) != 0)
        return -1;
    if(param->version != PARAM_VERSION)
        return -2;
    return 0;
}

// 获取RAM里的参数。
const param_t * param_getRamParam(void)
{
    return &param_ram;
}

// 设置RAm里的参数。
param_t * param_setRamParam(void)
{
    return &param_ram;
}

// 获取Flash里的参数。
const param_t * param_getFlashParam(void)
{
    return param_flash;
}

// 把RAM中的参数写入到Flash里。
int32_t param_writeToFlash(void)
{
    param_refreshCrc(&param_ram);
    if(param_checkValid(&param_ram) != 0)
        return -1;
    //
    // 检查是否需要写，避免重复写入。
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
					"\x85\xA5\xE3\x80\x82");//"参数相同，不需写入"
        return 0;
    }
    //
    flash_erase((const uint32_t *)param_flash);
    flash_write((const uint32_t *)param_flash,(const uint32_t *)&param_ram,sizeof(param_t)/4);
    //
    if(param_checkValid(param_flash) != 0)
        return -2;
    //
    cmd_returnMessage("\xE5\x8F\x82\xE6\x95\xB0\xE5\xB7\xB2\xE5\x86\x99\xE5\x85\xA5""Flash");//"参数已写入Flash" 源文件要用UTF8编码。
    return 0;
}

int32_t param_resetFlash(void)
{
    param_loadDefault(&param_ram);
    param_writeToFlash();
    //
    return 0;
}
