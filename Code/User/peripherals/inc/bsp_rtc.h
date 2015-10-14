/**
  ******************************************************************************
  * @file              : 
  * @author            : 
  * @version           : 
  * @date              : 
  * @brief             : h file
  * @description       : Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 201x STMicroelectronics
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSPRTC_H
#define __BSPRTC_H

//#ifdef __cplusplus
// extern "C" {
//#endif

/*============================ INCLUDES ======================================*/
//! @name include
//! @{
#include "stm32l1xx.h"
#include "config.h"
//! @}

/*============================ MACROS ========================================*/
//! @{
//! \brief 
// �豸��ַ
#define        RX8025_ADDR_READ                0x65
#define        RX8025_ADDR_WRITE               0x64
 // �Ĵ�����ַ
#define        RX8025_ADDR_SECONDS             0x00
#define        RX8025_ADDR_WEEK                0x30
#define        RX8025_ADDR_DATES               0x40
#define        RX8025_ADDR_MONTH               0x50
#define        RX8025_ADDR_MINUTES             0x80
#define        RX8025_ADDR_CONTROL1        	   0xE0
#define        RX8025_ADDR_CONTROL2        	   0xF0
 // �豸����ģʽ
#define        RX8025_WRITE_MODE               0xF0
#define        RX8025_READ_MODE                0xF0 
#define        RX8025_SIMP_READ_MODE           0x04

//2009��1��1��0ʱ0��0���𵽵�ǰʱ��������������
#define xMINUTE          (uint32_t)(60                 )//1�ֵ�����
#define xHOUR            (uint32_t)(60*xMINUTE         )//1Сʱ������
#define xDAY             (uint32_t)(24*xHOUR           )//1�������
#define xYEAR            (uint32_t)(365*xDAY           )//1�������

#define BCD_TO_D10(BCD)     (BCD/16)*10+BCD%16
#define D10_TO_BCD(D10)     (D10/10)*16+D10%10

//! @}

/*============================ TYPES =========================================*/
//!


/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/
// MCU:
/*============================ EXTERN FUNCTIONS ==============================*/
void  set_time(void);
void rtc_init(void);
void read_time(void);
void rtc_deel(void);
unsigned long DateToSeconds(struct   RTCRX8025 *Rtc);
void RtcBcdToD10(struct   RTCRX8025 *Rtc);
void RtcD10ToBcd(struct   RTCRX8025 *Rtc);
//#ifdef __cplusplus
//}
//#endif

#endif /* __BSPRTC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
