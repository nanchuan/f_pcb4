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

// main.c
// 2013-2-21 20:41:35
// js200300953

#include <stdint.h>
#include <bsp/bsp.h>
#include <bsp/time.h>
#include <app/communication/interface.h>
#include <app/refresher/interface.h>
#include <app/engine/interface.h>

int main(void)
{
    bsp_init();
    //
    // ��ʼ��ģ�飬˳���н�����
    refresher_init();
    engine1_init();
    communication_init();
    //
    engine1_loop();
    while(1);
}
