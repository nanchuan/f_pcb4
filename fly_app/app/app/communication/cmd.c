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

// app/communication/cmd.c
// 2013-1-8 14:23:01
// js200300953

#include <stdint.h>
#include <bsp/bsp.h>
#include <app/sensor/interface.h>
#include <app/attitude/interface.h>
#include <app/control/interface.h>
#include <app/status/interface.h>
#include <app/param/interface.h>
#include <app/math/interface.h>
#include "protocol.h"
#include "engine.h"
#include "cmd.h"
#include "bsp/led.h"

void    cmd_getVector(uint8_t type);
void    cmd_getAttitude(void);
void    cmd_setControlMode(uint8_t mode);
int32_t cmd_returnMessage(const char * msg);
void    cmd_bootloaderCmd(uint8_t cmd,const uint8_t param[],int32_t param_length);
void    cmd_returnAppInfo(void);
void    cmd_parameter(const uint8_t param[],int32_t param_length);
int32_t cmd_lockAttitude(const uint8_t param[],int32_t param_length);
//
static void cmd_returnParameter(uint8_t code,uint8_t pos,uint8_t len,const int32_t param[]);

void cmd_getVector(uint8_t type)
{
    if(engine_transmitBegin(PROTOCOL_RETURN_VECTOR) != 0)
        return;
    //
    float buffer[3];
    const void * vector = 0;
    //
    switch(type)
    {
        case PROTOCOL_VECTOR_TYPE_RAW_ACC:
        {
            int16_t raw[3];
            sensor_getRawAcc(raw);
            for(int i=0;i<3;i++)
                buffer[i] = raw[i];
            vector = buffer;
            //
            break;
        }
        case PROTOCOL_VECTOR_TYPE_RAW_GYR:
        {
            int16_t raw[3];
            sensor_getRawGyr(raw);
            for(int i=0;i<3;i++)
                buffer[i] = raw[i];
            vector = buffer;
            //
            break;
        }
        case PROTOCOL_VECTOR_TYPE_RAW_MAG:
        {
            int16_t raw[3];
            sensor_getRawMag(raw);
            for(int i=0;i<3;i++)
                buffer[i] = raw[i];
            vector = buffer;
            //
            break;
        }
        case PROTOCOL_VECTOR_TYPE_CALIBRATED_ACC :
        {
            sensor_getCalibratedAcc(buffer);
            vector = buffer;
            break;
        }
        case PROTOCOL_VECTOR_TYPE_CALIBRATED_GYR :
        {
            sensor_getCalibratedGyr(buffer);
            vector = buffer;
            break;
        }
        case PROTOCOL_VECTOR_TYPE_CALIBRATED_MAG :
        {
            sensor_getCalibratedMag(buffer);
            vector = buffer;
            break;
        }
    }
    //
    if(vector != 0)
    {
        engine_transmitContent(&type,1);
        engine_transmitContent(vector,3*4);
        engine_transmitEnd();
    }
}

void cmd_getAttitude(void)
{
    if(engine_transmitBegin(PROTOCOL_RETURN_ATTITUDE_QUATERNION) != 0)
        return;
    engine_transmitContent(attitude_getAttitude(),4*4);
    engine_transmitEnd();
}

void cmd_setControlMode(uint8_t mode)
{
    switch(mode)
    {
        case PROTOCOL_SET_CONTROL_MODE_HALT :
        {
            ctrl_enterHaltMode();
						led_light_off();
            break;
        }
        case PROTOCOL_SET_CONTROL_MODE_LOCK_THROTTLE :
        {
            ctrl_enterLockThrottleMode();
            break;
        }
        case PROTOCOL_SET_CONTROL_MODE_LOCK_ATTITUDE :
        {			
            ctrl_enterLockAttitudeMode();
            break;
        }
    }
}

void cmd_getStatus(const uint8_t which[],int32_t amount)
{
    for(int i=0;i<amount;i++)
    {
        switch(which[i])
        {
            case PROTOCOL_STATUS_BATERY_VOLTAGE :
            {
                if(engine_transmitBegin(PROTOCOL_RETURN_STATUS) != 0)
                    return;
                //
                float v = status_getBateryVoltage();
                engine_transmitContent(&which[i],1);
                engine_transmitContent(&v,sizeof(v));
                engine_transmitEnd();
                break;
            }
            case PROTOCOL_STATUS_CURRENT_THROTTLE :
            {
                if(engine_transmitBegin(PROTOCOL_RETURN_STATUS) != 0)
                    return;
                //
                engine_transmitContent(&which[i],1);
                engine_transmitContent(status_getCurrentThrottle(),4*sizeof(float));
                engine_transmitEnd();
                break;
            }
        }
    }
}

int32_t cmd_returnMessage(const char * msg)
{
    // ȷ���������ݵĳ��ȣ�ȡ�ַ������Ⱥ�����������С��һ����
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

void cmd_bootloaderCmd(uint8_t cmd,const uint8_t param[],int32_t param_length)
{
    switch(cmd)
    {
        case PROTOCOL_BOOTLOADER_CMD_ENTER_BOOTLOADER :
        {
            // �������bootloader��
            //int32_t pReset = *(int32_t *)(0+4);
            //((void (*)(void))(pReset))();
            bsp_systemReset();
        }
        break;
        case PROTOCOL_BOOTLOADER_CMD_ENTER_APP :
        {
            // ����������app���ͷ��ء��Ѿ�����app��״̬��
            if(engine_transmitBegin(PROTOCOL_BOOTLOADER_STATUS) != 0)
                return;
            //
            uint8_t status = PROTOCOL_BOOTLOADER_STATUS_RUNNING_APP;
            engine_transmitContent(&status,1);
            engine_transmitEnd();
        }
        break;
    }
}

void cmd_returnAppInfo(void)
{
    //cmd_returnMessage("App\xE2\x85\xA1 v2.3");
		cmd_returnMessage("4X_III_APP v3.3");
    cmd_returnMessage(__DATE__);
    cmd_returnMessage(__TIME__);
}

// param=|content|
void cmd_parameter(const uint8_t param[],int32_t param_length)
{
    //
    // ����������Ҫһ���ֽڡ�
    if(param_length < 1)
        return;
    //
    int32_t * param_ram = (int32_t *)param_getRamParam();
    int32_t * param_flash = (int32_t *)param_getFlashParam();
    switch(param[0])
    {
    case PROTOCOL_PARAMETER_GET_RAM :
        {
            // |pos:8|len:8|
            if(param_length < 2)
                return;
            //
            uint8_t pos = param[1];
            if(pos < 1 || pos >= (sizeof(param_t)/4))
                return;
            uint8_t len = param[2];
            if(len < 1 || pos+len > (sizeof(param_t)/4))
                return;
            //
            cmd_returnParameter(PROTOCOL_PARAMETER_RETURN_RAM,pos,len,param_ram+pos);
        }
        break;
    case PROTOCOL_PARAMETER_SET_RAM :
        {
            // |pos:8|value:32*n|
            if(param_length < 1+4)
                return;
            //
            uint8_t pos = param[1];
            if(pos < 1 || pos >= (sizeof(param_t)/4))
                return;
            uint8_t len = (param_length-1)/4;
            if(len < 1 || pos+len > (sizeof(param_t)/4))
                return;
            //
            int32_t len_b = len * 4; // ��Ϊ�ֽڵ�λ��
            int32_t pos_b = pos * 4; // ��Ϊ�ֽڵ�λ��
            for(int i=0;i<len_b;i++)
            {
                ((uint8_t *)param_ram)[pos_b+i] = param[2+i];
            }
            //
            cmd_returnParameter(PROTOCOL_PARAMETER_RETURN_RAM,pos,len,param_ram+pos);
        }
        break;
    case PROTOCOL_PARAMETER_GET_FLASH :
        {
            // |pos:8|len:8|
            if(param_length < 2)
                return;
            //
            uint8_t pos = param[1];
            if(pos < 1 || pos >= (sizeof(param_t)/4))
                return;
            uint8_t len = param[2];
            if(len < 1 || pos+len > (sizeof(param_t)/4))
                return;
            //
            cmd_returnParameter(PROTOCOL_PARAMETER_RETURN_FLASH,pos,len,param_flash+pos);
        }
        break;
    case PROTOCOL_PARAMETER_WRITE_FLASH :
        //
        // |û�и��Ӳ���|
        param_writeToFlash();
        break;
    case PROTOCOL_PARAMETER_RESET_FLASH :
        //
        // |û�и��Ӳ���|
        param_resetFlash();
        break;
    case PROTOCOL_PARAMETER_OPERATE_RAM :
        //
        // |op:8|wxyz:32*4|
        if(param_length < 17)
            return;
        //
        switch(param[1])
        {
        case PROTOCOL_PARAMETER_OPERATE_RAM_MIX_HORIZONTAL_ATTITUDE :
            {
                quaternion q;
                for(int i=0;i<4*4;i++)
                    ((uint8_t *)&q)[i] = param[2 + i];
                param_t * p = param_setRamParam();
                quaternion old = p->horizon;
                //
                quaternion_mult(&(p->horizon),&old,&q);
                p->horizon.z = 0;
                quaternion_normalize(&(p->horizon));
                cmd_returnParameter(PROTOCOL_PARAMETER_RETURN_RAM,19,4,param_ram+19);
            }
            break;
        }
        break;
    }
}

void cmd_returnParameter(uint8_t code,uint8_t pos,uint8_t len,const int32_t param[])
{
    const static int MAX_LEN_PER_PACKET = 6;
    //
    // �������İ���
    int packetAmount = len / MAX_LEN_PER_PACKET;
    int offset = 0;
    for(int i=0;i<packetAmount;i++)
    {
        if(engine_transmitBegin(PROTOCOL_PARAMETER) != 0)
            return;
        //
        if(engine_transmitContent(&code,1) != 0)
            return;
        uint8_t tmp = pos+offset;
        if(engine_transmitContent(&tmp,1) != 0)
            return;
        if(engine_transmitContent(&param[offset],MAX_LEN_PER_PACKET*4) != 0)
            return;
        if(engine_transmitEnd() != 0)
            return;
        //
        offset += MAX_LEN_PER_PACKET;
    }
    //
    // �����û��ʣ�ಿ�֡�
    if(offset < len)
    {
        if(engine_transmitBegin(PROTOCOL_PARAMETER) != 0)
            return;
        //
        if(engine_transmitContent(&code,1) != 0)
            return;
        uint8_t tmp = pos+offset;
        if(engine_transmitContent(&tmp,1) != 0)
            return;
        if(engine_transmitContent(&param[offset],(len-offset)*4) != 0)
            return;
        if(engine_transmitEnd() != 0)
            return;
    }
}

int32_t cmd_lockAttitude(const uint8_t param[],int32_t param_length)
{
    if(param_length < 1)
        return -1;
    //
    switch(param[0])
    {
    case PROTOCOL_LOCK_ATTITUDE_GET_READY :
        {
            uint8_t cmd = PROTOCOL_LOCK_ATTITUDE_READY;
            if(ctrl_lockAttitude_getReady() == 0)
            {
                cmd = PROTOCOL_LOCK_ATTITUDE_READY;								
								led_light_on();
                cmd_returnMessage("\xE5\x87\x86\xE5\xA4\x87\xE5\xA5\xBD\xE8\xB5\xB7\xE9\xA3\x9E\xE4\xBA\x86");//"׼���������" Դ�ļ�Ҫ��UTF8���롣
            }
            else
                cmd = PROTOCOL_LOCK_ATTITUDE_NOT_READY;
            //
            if(engine_transmitBegin(PROTOCOL_LOCK_ATTITUDE) != 0)
                return -2;
            //
            if(engine_transmitContent(&cmd,1) != 0)
                return -2;
            if(engine_transmitEnd() != 0)
                return -2;
            break;
        }
    case PROTOCOL_LOCK_ATTITUDE_CONTROL :
        {
            if(param_length != (1+4+4*4))
                break;
            //
            float th;
            for(int i=0;i<4;i++)
                ((uint8_t *)&th)[i] = param[1+i];
            //
            quaternion target;
            for(int i=0;i<4*4;i++)
                ((uint8_t *)&target)[i] = param[1+4+i];
            //
            if(ctrl_lockAttitude_setBaseThrottle(th) != 0
                || ctrl_lockAttitude_control(&target) != 0)
            {
            }
            //
            break;
        }
    }
    //
    return 0;
}
