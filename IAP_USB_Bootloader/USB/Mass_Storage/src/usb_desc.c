/**
  ******************************************************************************
  * @file    usb_desc.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Descriptors for Device Firmware Upgrade (DFU)
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "usb_desc.h"
#include "platform_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
uint8_t DFU_DeviceDescriptor[DFU_SIZ_DEVICE_DESC] =
  {
    0x12,   /* bLength */
    0x01,   /* bDescriptorType */
    0x00,   /* bcdUSB, version 1.00 */
    0x01,
    0x00,   /* bDeviceClass : See interface */
    0x00,   /* bDeviceSubClass : See interface*/
    0x00,   /* bDeviceProtocol : See interface */
    bMaxPacketSize0, /* bMaxPacketSize0 0x40 = 64 */
    0x83,   /* idVendor     (0483) */
    0x04,
    0x11,   /* idProduct (0xDF11) DFU PiD*/
    0xDF,
    0x00,   /* bcdDevice*/
    0x02,

    0x01,   /* iManufacturer : index of string Manufacturer  */
    0x02,   /* iProduct      : index of string descriptor of product*/
    0x03,   /* iSerialNumber : index of string serial number*/

    0x01    /*bNumConfigurations */
  };


uint8_t DFU_ConfigDescriptor[DFU_SIZ_CONFIG_DESC] =
  {
    0x09,   /* bLength: Configuration Descriptor size */
    0x02,   /* bDescriptorType: Configuration */
    DFU_SIZ_CONFIG_DESC, /* wTotalLength: Bytes returned */
    0x00,
    0x01,   /* bNumInterfaces: 1 interface */
    0x01,   /* bConfigurationValue: */
    /*      Configuration value */
    0x00,   /* iConfiguration: */
    /*      Index of string descriptor */
    /*      describing the configuration */
    0x80,   /* bmAttributes: */
    /*      bus powered */
    0x20,   /* MaxPower 100 mA: this current is used for detecting Vbus */
    /* 09 */

    /************ Descriptor of DFU interface 0 Alternate setting 0 *********/
    0x09,   /* bLength: Interface Descriptor size */
    0x04,   /* bDescriptorType: */
    /*      Interface descriptor type */
    0x00,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x00,   /* bNumEndpoints*/
    0xFE,   /* bInterfaceClass: Application Specific Class Code */
    0x01,   /* bInterfaceSubClass : Device Firmware Upgrade Code */
    0x02,   /* nInterfaceProtocol: DFU mode protocol */
    0x04,   /* iInterface: */
    /* Index of string descriptor */
    /* 18 */

    /************ Descriptor of DFU interface 0 Alternate setting 1  **********/

    0x09,   /* bLength: Interface Descriptor size */
    0x04,   /* bDescriptorType: */
    /*      Interface descriptor type */
    0x00,   /* bInterfaceNumber: Number of Interface */
    0x01,   /* bAlternateSetting: Alternate setting */
    0x00,   /* bNumEndpoints*/
    0xFE,   /* bInterfaceClass: Application Specific Class Code */
    0x01,   /* bInterfaceSubClass : Device Firmware Upgrade Code */
    0x02,   /* nInterfaceProtocol: DFU mode protocol */
    0x05,   /* iInterface: */
    /* Index of string descriptor */
    /* 27 */    
 
    /************ Descriptor of DFU interface 0 Alternate setting 2  **********/

    0x09,   /* bLength: Interface Descriptor size */
    0x04,   /* bDescriptorType: */
    /*      Interface descriptor type */
    0x00,   /* bInterfaceNumber: Number of Interface */
    0x02,   /* bAlternateSetting: Alternate setting */
    0x00,   /* bNumEndpoints*/
    0xFE,   /* bInterfaceClass: Application Specific Class Code */
    0x01,   /* bInterfaceSubClass : Device Firmware Upgrade Code */
    0x02,   /* nInterfaceProtocol: DFU mode protocol */
    0x06,   /* iInterface: */
    /* Index of string descriptor */
    /* 36 */    

    /******************** DFU Functional Descriptor********************/
    0x09,   /*blength = 9 Bytes*/
    0x21,   /* DFU Functional Descriptor*/
    0x0B,   /*bmAttribute

    bitCanDnload             = 1      (bit 0)
    bitCanUpload             = 1      (bit 1)
    bitManifestationTolerant = 0      (bit 2)
    bitWillDetach            = 1      (bit 3)
    Reserved                          (bit4-6)
    bitAcceleratedST         = 0      (bit 7)*/
    0xFF,   /*DetachTimeOut= 255 ms*/
    0x00,
    wTransferSizeB0,
    wTransferSizeB1,          /* TransferSize = 1024 Byte*/
    0x1A,                     /* bcdDFUVersion*/
    0x01
    /***********************************************************/
    /*45*/

  };

uint8_t DFU_StringLangId[DFU_SIZ_STRING_LANGID] =
  {
    DFU_SIZ_STRING_LANGID,
    0x03,
    0x09,
    0x04    /* LangID = 0x0409: U.S. English */
  };

/**********************************************************
       以下是用户修改得到相关的制造商信息和设备名称
**********************************************************/

uint8_t DFU_StringVendor[DFU_SIZ_STRING_VENDOR] =	/* 制造商信息字符串 */
  {
    DFU_SIZ_STRING_VENDOR,
    0x03,
    /* Manufacturer: "STMicroelectronics" */
    'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
    'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
    'c', 0, 's', 0
  };

uint8_t DFU_StringProduct[DFU_SIZ_STRING_PRODUCT] =	/* 设备名称字符串 */
  {
    DFU_SIZ_STRING_PRODUCT,
    0x03,
    /* Product name: "STM32 DFU" */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, ' ', 0, 'D', 0, 'F', 0, 'U', 0
  };

/**********************************************************
                 结束制造商和设备名称设置
**********************************************************/

uint8_t DFU_StringSerial[DFU_SIZ_STRING_SERIAL] =
  {
    DFU_SIZ_STRING_SERIAL,
    0x03,
    /* Serial number */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0  
  };

uint8_t DFU_StringInterface0[DFU_SIZ_STRING_INTERFACE0] =
  {
    DFU_SIZ_STRING_INTERFACE0,
    0x03,
    // Interface 0: "@Internal Flash   /0x08000000/20*001Ka,108*001Kg"
    '@', 0, 'I', 0, 'n', 0, 't', 0, 'e', 0, 'r', 0, 'n', 0, 'a', 0, 'l', 0,  /* 18 */
    ' ', 0, 'F', 0, 'l', 0, 'a', 0, 's', 0, 'h', 0, ' ', 0, ' ', 0, /* 16 */

    '/', 0, '0', 0, 'x', 0, '0', 0, '8', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, /* 22 */

    '/', 0, '2', 0, '0', 0, '*', 0, '0', 0, '0', 0, '1', 0, 'K', 0, 'a', 0, /* 18 */
    ',', 0, '1', 0, '0', 0, '8', 0, '*', 0, '0', 0, '0', 0, '1', 0, 'K', 0, 'g', 0, /* 20 */
  };

uint8_t DFU_StringInterface1[DFU_SIZ_STRING_INTERFACE1] =
  {
    DFU_SIZ_STRING_INTERFACE1,
    0x03,
    // Interface 1: "@ SPI Flash: M25P64  /0x00000000/128*064Kg"
    '@', 0, 'S', 0, 'P', 0, 'I', 0, ' ', 0, 'F', 0, 'l', 0, 'a', 0, 's', 0,
    'h', 0, ' ', 0, ':', 0, ' ', 0, 'M', 0, '2', 0, '5', 0, 'P', 0, '6', 0, '4', 0,
    '/', 0, '0', 0, 'x', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0,
    '/', 0, '1', 0, '2', 0, '8', 0, '*', 0, '6', 0, '4', 0, 'K', 0, 'g', 0
  };

uint8_t DFU_StringInterface2_1[DFU_SIZ_STRING_INTERFACE2] =
  {
    DFU_SIZ_STRING_INTERFACE2,
    0x03,
    // Interface 1: "@ NOR Flash: M29W128 /0x64000000/256*064Kg"
    '@', 0, 'N', 0, 'O', 0, 'R', 0, ' ', 0, 'F', 0, 'l', 0, 'a', 0, 's', 0,
    'h', 0, ' ', 0, ':', 0, ' ', 0, 'M', 0, '2', 0, '9', 0, 'W', 0, '1', 0, '2', 0, '8', 0, 'F', 0,
    '/', 0, '0', 0, 'x', 0, '6', 0, '4', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0,
    '/', 0, '0', 0, '2', 0, '5', 0, '6', 0, '*', 0, '6', 0, '4', 0, 'K', 0, 'g', 0
  };
uint8_t DFU_StringInterface2_2[DFU_SIZ_STRING_INTERFACE2] =
  {
    DFU_SIZ_STRING_INTERFACE2,
    0x03,
    // Interface 1: "@ NOR Flash: M29W128 /0x64000000/128*128Kg"
    '@', 0, 'N', 0, 'O', 0, 'R', 0, ' ', 0, 'F', 0, 'l', 0, 'a', 0, 's', 0,
    'h', 0, ' ', 0, ':', 0, ' ', 0, 'M', 0, '2', 0, '9', 0, 'W', 0, '1', 0, '2', 0, '8', 0, 'G', 0,
    '/', 0, '0', 0, 'x', 0, '6', 0, '4', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0,
    '/', 0, '1', 0, '2', 0, '8', 0, '*', 0, '1', 0, '2', 0, '8', 0, 'K', 0, 'g', 0
  };

uint8_t DFU_StringInterface2_3[DFU_SIZ_STRING_INTERFACE2] =
  {
    DFU_SIZ_STRING_INTERFACE2,
    0x03,
    // Interface 1: "@ NOR Flash:S29GL128 /0x64000000/128*128Kg"
    '@', 0, 'N', 0, 'O', 0, 'R', 0, ' ', 0, 'F', 0, 'l', 0, 'a', 0, 's', 0,
    'h', 0, ' ', 0, ':', 0, ' ', 0, 'S', 0, '2', 0, '9', 0, 'G', 0, 'L', 0 , '1', 0, '2', 0, '8', 0,
    '/', 0, '0', 0, 'x', 0, '6', 0, '4', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0,
    '/', 0, '1', 0, '2', 0, '8', 0, '*', 0, '1', 0, '2', 0, '8', 0, 'K', 0, 'g', 0
  };

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
