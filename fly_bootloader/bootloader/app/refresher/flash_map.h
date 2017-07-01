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

// app/refresher/flash_map.h
// 2013-2-21 21:13:53
// js200300953

#ifndef __FILE_APP_REFRESHER_FLASHMAP_H__
#define __FILE_APP_REFRESHER_FLASHMAP_H__

/*

Flashһ��64kB���ֳ�5�ݣ���ͷΪ15kB��bootloader����������1kB��param��ʣ�µ���48kB��app+applen+appcrc��
paramΪ��appʹ�õ�Flash��
  --------------------------
        Bootloader
   [0,16kB)          : 16kB
  --------------------------
          AppCode
   [16kB,63kB-8B)    : 47kB-8B
  --------------------------
           AppLen
   [63kB-8B,63kB-4B)  : 4B
  --------------------------
            AppCrc
   [63kB-4B,63kB)     : 4B
  --------------------------
           Param
   [63kB,64kB)       : 1kB
  --------------------------

�����ͳ��������һ����Ϊ��������
 _BIT   => λ����
 _BYTE  => �ֽڼ���
 _BLOCK => �����
 _PAGE  => ҳ����

*/

#include <stdint.h>

typedef struct {uint8_t bytes[1024];} page_t;

// FLASH��ʼ��ַ��
#define MAP_FLASH_BASE_PAGE     ((page_t *)0x08000000)
#define MAP_FLASH_BASE_BLOCK    ((uint32_t *)MAP_FLASH_BASE_PAGE)
#define MAP_FLASH_BASE_BYTE     ((uint8_t *) MAP_FLASH_BASE_PAGE)

// ���С���̶�Ϊ4�ֽڣ����ܸı䣬��ΪFlashд������Ϊ��λ��
#define MAP_BLOCK_SIZE_BYTE (4)
#define MAP_BLOCK_SIZE_BIT  (MAP_BLOCK_SIZE_BYTE*8)

// FLASHҳ��С��������ҳΪ��λ��STM32F103R8��Ϊ1kB��
/*С������Ʒ��ָ����洢��������16K��32K�ֽ�֮���STM32F101xx��STM32F102xx��STM32F103xx΢��������
��������Ʒ��ָ����洢��������64K��128K�ֽ�֮���STM32F101xx��STM32F102xx��STM32F103xx΢��������
��������Ʒ��ָ����洢��������256K��512K�ֽ�֮���STM32F101xx��STM32F103xx΢��������
��������Ʒ��FLASHÿ����2K�ֽڣ���������С������FLASHÿ����1K�ֽڡ�*/
#define MPA_FLASH_PAGE_SIZE_BYTE (sizeof(page_t))
#define MPA_FLASH_PAGE_SIZE_BLOCK (MPA_FLASH_PAGE_SIZE_BYTE/MAP_BLOCK_SIZE_BYTE)

// bootloader�εĻ�ַ�ʹ�С��
#define MAP_BOOTLOADER_BASE_PAGE    (MAP_FLASH_BASE_PAGE)
#define MAP_BOOTLOADER_BASE_BLOCK   ((uint32_t *)MAP_BOOTLOADER_BASE_PAGE)
#define MAP_BOOTLOADER_BASE_BYTE    ((uint8_t  *)MAP_BOOTLOADER_BASE_PAGE)
#define MAP_BOOTLOADER_SIZE_PAGE    (16) // 16page = 16kB
#define MAP_BOOTLOADER_SIZE_BLOCK   (MAP_BOOTLOADER_SIZE_PAGE*MPA_FLASH_PAGE_SIZE_BLOCK)
#define MAP_BOOTLOADER_SIZE_BYTE    (MAP_BOOTLOADER_SIZE_PAGE*MPA_FLASH_PAGE_SIZE_BYTE)

// app�εĻ�ַ�ʹ�С��
#define MAP_APP_BASE_PAGE    (MAP_BOOTLOADER_BASE_PAGE+MAP_BOOTLOADER_SIZE_PAGE)
#define MAP_APP_BASE_BLOCK   ((uint32_t *)MAP_APP_BASE_PAGE)
#define MAP_APP_BASE_BYTE    ((uint8_t  *)MAP_APP_BASE_PAGE)
#define MAP_APP_SIZE_PAGE    (47) // 47page = 47kB
#define MAP_APP_SIZE_BLOCK   (MAP_APP_SIZE_PAGE*MPA_FLASH_PAGE_SIZE_BLOCK)
#define MAP_APP_SIZE_BYTE    (MAP_APP_SIZE_PAGE*MPA_FLASH_PAGE_SIZE_BYTE)

// app����Ч���Ⱥ�CRC�Ĵ�ŵĵ�ַ
#define MPA_APP_VALID_BLOCK_AMOUNT_ADDR_UINT32 ((uint32_t *)(MAP_FLASH_BASE_BYTE + 63*1024 - 8))
#define MPA_APP_CRC_ADDR_UINT32                ((uint32_t *)(MAP_FLASH_BASE_BYTE + 63*1024 - 4))

// param�εĻ�ַ�ʹ�С��
#define MAP_PARAM_BASE_PAGE    (MAP_FLASH_BASE_PAGE+63)
#define MAP_PARAM_BASE_BLOCK   ((uint32_t *)MAP_PARAM_BASE_PAGE)
#define MAP_PARAM_BASE_BYTE    ((uint8_t  *)MAP_PARAM_BASE_PAGE)
#define MAP_PARAM_SIZE_PAGE    (1) // 1page = 1kB
#define MAP_PARAM_SIZE_BLOCK   (MAP_PARAM_SIZE_PAGE*MPA_FLASH_PAGE_SIZE_BLOCK)
#define MAP_PARAM_SIZE_BYTE    (MAP_PARAM_SIZE_PAGE*MPA_FLASH_PAGE_SIZE_BYTE)

#endif // __FILE_APP_REFRESHER_FLASHMAP_H__
