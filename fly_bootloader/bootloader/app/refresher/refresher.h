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

// app/refresher/refresher.h
// 2013-2-21 21:04:21
// js200300953

#ifndef __FILE_APP_REFRESHER_REFRESHER_H__
#define __FILE_APP_REFRESHER_REFRESHER_H__

#include <stdint.h>

int32_t refresher_init(void);
int32_t refresher_checkApp(void);
int32_t refresher_setAppSizeCrc(uint32_t size_block,uint32_t crc);
int32_t refresher_eraseApp(void);
int32_t refresher_write(const uint32_t *data,int32_t length,int32_t blockIndexOfFirst);
int32_t refresher_isWriteComplete(void);
int32_t refresher_getRemainBlock(uint8_t * buffer,int32_t bufferLength);
int32_t refresher_getWrittenAmount_block(void);
int32_t refresher_jumpToApp(void);

#endif // __FILE_APP_REFRESHER_REFRESHER_H__
