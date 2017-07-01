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

// main.c
// 2013-1-7 10:51:49
// js200300953

#include "bsp/bsp.h"
#include "app/communication/interface.h"
#include "app/sensor/interface.h"
#include "app/exception/interface.h"
#include "app/attitude/interface.h"
#include "app/control/interface.h"
#include "app/status/interface.h"
#include "app/param/interface.h"

int main(void)
{
    bsp_init();
    param_init();
    //
    communication_init();
    sensor_init();
    attitude_init();
    exception_init();
    status_init();
    ctrl_init();
    //
    while(1)
    {
        communication_checkEvent();
        sensor_checkEvent();
        ctrl_checkEvent();
        exception_checkEvent();
        status_checkEvent();
    }
}
