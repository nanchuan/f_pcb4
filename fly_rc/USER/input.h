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

// app/input/input.h
// 2013-6-9 16:13:14
// js200300953

#ifndef __FILE_APP_INPUT_INPUT_H__
#define __FILE_APP_INPUT_INPUT_H__

#include <stdint.h>

#ifndef _BV
    #define _BV(n) (1<<(n))
#endif

// �������룬��ͬ����ռ��ͬ��λ��
typedef enum
{
    INPUT_KEY_1                 = _BV(0),
    INPUT_KEY_2                 = _BV(1),
    /*INPUT_KEY_3                 = _BV(2),
    INPUT_KEY_4                 = _BV(3),
    INPUT_KEY_JOYSTICK_A_BUTTON = _BV(4),
    INPUT_KEY_JOYSTICK_B_BUTTON = _BV(5),*/
    //
    INPUT_KEY_JOYSTICK_A_RIGHT  = _BV(6),
    INPUT_KEY_JOYSTICK_A_UP     = _BV(7),
    INPUT_KEY_JOYSTICK_A_LEFT   = _BV(8),
    INPUT_KEY_JOYSTICK_A_DOWN   = _BV(9),
    //
    INPUT_KEY_JOYSTICK_B_RIGHT  = _BV(10),
    INPUT_KEY_JOYSTICK_B_UP     = _BV(11),
    INPUT_KEY_JOYSTICK_B_LEFT   = _BV(12),
    INPUT_KEY_JOYSTICK_B_DOWN   = _BV(13),
}input_key_t;

// �¼����롣
typedef enum
{
    INPUT_EVENT_DOWN = 1,
    INPUT_EVENT_UP,
    INPUT_EVENT_DOUBLE_CLICK,
    INPUT_EVENT_PRESS,
}input_event_t;

typedef int32_t (* input_keyEventCallback_t)(int32_t act,int32_t key,int32_t event,int32_t param);

int32_t input_init(void);
int32_t input_checkEvent(void);
int32_t input_setKeyEventCallback(int32_t this_,input_keyEventCallback_t callback);
joystick_t joystickFilter_getValue(void);

#endif // __FILE_APP_INPUT_INPUT_H__
