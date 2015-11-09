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
/*设备地址*/
#define RX8025_ReadAddr          	0x65
#define RX8025_WriteAddr         	0x64
/*寄存器地址*/
#define RX8025_SecondsAddr       	0x00
#define RX8025_MinutesAddr       	0x01
#define RX8025_HoursAddr         	0x02
#define RX8025_WeekdaysAddr      	0x03
#define RX8025_DaysAddr      		0x04
#define RX8025_MonthsAddr         	0x05
#define RX8025_YearsAddr         	0x06
#define RX8025_DigitalOffsetAddr	0x07
/*控制寄存器*/
#define RX8025_Control1Addr        	   0x0E
#define RX8025_Control2Addr        	   0x0F

//2009年1月1日0时0分0秒起到当前时间所经过的秒数
#define xMINUTE          (uint32_t)(60                 )//1分的秒数
#define xHOUR            (uint32_t)(60*xMINUTE         )//1小时的秒数
#define xDAY             (uint32_t)(24*xHOUR           )//1天的秒数
#define xYEAR            (uint32_t)(365*xDAY           )//1年的秒数

#define BCD_TO_D10(BCD)     (BCD/16)*10+BCD%16
#define D10_TO_BCD(D10)     (D10/10)*16+D10%10

//! @}

/*============================ TYPES =========================================*/
//!


/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/
// MCU:
/*============================ EXTERN FUNCTIONS ==============================*/
bool RX8025_RTC_Init(void);

void read_time(void);
void serialread_time(void);
void rtc_deel(void);
void set_time(void);
unsigned long DateToSeconds(struct   RTCRX8025 *Rtc);
void RtcBcdToD10(struct   RTCRX8025 *Rtc);
void RtcD10ToBcd(struct   RTCRX8025 *Rtc);
//#ifdef __cplusplus
//}
//#endif

#endif /* __BSPRTC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
