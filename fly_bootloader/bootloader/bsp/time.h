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

// bsp/time.h
// 2013-1-7 13:49:49
// js200300953

#ifndef __FILE_BSP_TIME_H__
#define __FILE_BSP_TIME_H__

#include <stdint.h>

void     time_init(void);
uint64_t time_nowUs(void);
uint32_t time_nowMs(void);
void     time_waitUs(uint32_t delay);
void     time_waitMs(uint32_t delay);

#define TIME_EXECUTE_PERIODICALLY_BEGIN(time_ms) \
{\
    static uint32_t target = 0;\
    if(target <= time_nowMs())\
    {\
        target = time_nowMs() + time_ms;
#define TIME_EXECUTE_PERIODICALLY_END() \
    }\
}

#endif // __FILE_BSP_TIME_H__
