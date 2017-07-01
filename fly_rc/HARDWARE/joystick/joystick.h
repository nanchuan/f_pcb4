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

// bsp/joystick.h
// 2013-6-7 15:05:35
// js200300953

#ifndef __FILE_BSP_JOYSTICK_H__
#define __FILE_BSP_JOYSTICK_H__

#include <stdint.h>

// ҡ�˵�ֵ����Լ��׼����[-1,1]��
typedef struct
{
    float a_horizontal;
    float a_vertical;
    float b_horizontal;
    float b_vertical;
}joystick_t;

#ifndef _BV
    #define _BV(n) (1<<(n))
#endif

typedef enum
{
    KEY_1                   = _BV(0),
    KEY_2                   = _BV(1),
}key_t;

typedef enum
{
    KEY_UP   = 0,
    KEY_DOWN = 1,
}keyStatus_t;


int32_t key_getKeyValue(void);
void joystick_init(void);
void joystick_zero(void);
joystick_t joystick_getValue(void);
float battery_getRcVoltage(void);

#endif // __FILE_BSP_JOYSTICK_H__
