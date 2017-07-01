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

// app/control/interface.h
// 2013-1-8 16:29:51
// js200300953

#ifndef __FILE_APP_CONTROL_INTERFACE_H__
#define __FILE_APP_CONTROL_INTERFACE_H__

#include "app/math/interface.h"
#include "pid.h"

void    ctrl_init(void);
void    ctrl_checkEvent(void);
void    ctrl_enterHaltMode(void);
void    ctrl_enterLockAttitudeMode(void);
int32_t ctrl_lockAttitude_getReady(void);
int32_t ctrl_lockAttitude_control(const quaternion * delta);
int32_t ctrl_lockAttitude_setTarget(const quaternion * target);
int32_t ctrl_lockAttitude_setBaseThrottle(float baseThrottle);
void ctrl_enterLockThrottleMode(void);
void ctrl_lockThrottle_setThrottle(const float th[4]);
const float * ctrl_getCurrentThrottle(void);

#endif // __FILE_APP_CONTROL_INTERFACE_H__
