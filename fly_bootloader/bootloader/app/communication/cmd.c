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

// app/communication/cmd.c
// 2013-1-8 14:23:01
// js200300953

#include <stdint.h>
#include <app/engine/interface.h>
#include "protocol.h"
#include "engine.h"
#include "cmd.h"

int32_t cmd_returnMessage(const char * msg);
void    cmd_bootloaderCmd(const uint8_t param[],int32_t param_length);
void    cmd_returnBootloaderInfo(void);

int32_t cmd_returnMessage(const char * msg)
{
    // 确定发送数据的长度，取字符串长度和最大包长中最小的一个。
    int32_t len = 0;
    while(len < engine_maxTransmitContentSize() && msg[len] != '\0')
        len ++;
    //
    if(engine_transmitBegin(PROTOCOL_RETURN_MESSAGE) != 0)
        return 1;
    engine_transmitContent(msg,len);
    engine_transmitEnd();
    //
    return 0;
}

void cmd_bootloaderCmd(const uint8_t param[],int32_t param_length)
{
    engine1_onCmd(param,param_length);
}

void cmd_returnBootloaderInfo(void)
{
    cmd_returnMessage("Bootloader v2.1");
    //cmd_returnMessage(__DATE__);
    //cmd_returnMessage(__TIME__);
}

