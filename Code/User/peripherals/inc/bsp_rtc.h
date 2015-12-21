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
/*�豸��ַ*/
#define RX8025_ReadAddr          	0x65
#define RX8025_WriteAddr         	0x64
/*�Ĵ�����ַ*/
#define RX8025_SecondsAddr       	0x00
#define RX8025_MinutesAddr       	0x01
#define RX8025_HoursAddr         	0x02
#define RX8025_WeekdaysAddr      	0x03
#define RX8025_DaysAddr      		0x04
#define RX8025_MonthsAddr         	0x05
#define RX8025_YearsAddr         	0x06
#define RX8025_DigitalOffsetAddr	0x07
#define RX8025_Alarm_W_MinuteAddr	0x08
#define RX8025_Alarm_W_HourAddr		0x09
#define RX8025_Alarm_W_WeekdayAddr	0x0A
#define RX8025_Alarm_D_MinuteAddr	0x0B
#define RX8025_Alarm_D_HourAddr		0x0C
/*���ƼĴ���*/
#define RX8025_Control1Addr        	   0x0E
#define RX8025_Control2Addr        	   0x0F
#define RX8025_Control2CTFG			   0x04
#define RX8025_Control2DAFG			   0x01

//2009��1��1��0ʱ0��0���𵽵�ǰʱ��������������
#define xMINUTE          (uint32_t)(60                 )//1�ֵ�����
#define xHOUR            (uint32_t)(60*xMINUTE         )//1Сʱ������
#define xDAY             (uint32_t)(24*xHOUR           )//1�������
#define xYEAR            (uint32_t)(365*xDAY           )//1�������

#define BCD_TO_D10(BCD)     (BCD/16)*10+BCD%16
#define D10_TO_BCD(D10)     (D10/10)*16+D10%10
//------------------------------------


/* ***************************** �������빫��ʱ�任�� ********************************** */
/* ��׼ʱ����� */
#define FEBRUARY		(2u)
#define	STARTOFTIME		(2000u)
#define SECDAY			(86400uL)

#define	STARTYEAR		STARTOFTIME
#define	ENDYEAR			(2100u)
#define	MONTH			(12u)
#define	DAY				(28u)

#define	HOUR			(23u)
#define	MINUTE			(59u)
#define	SECOND			(59u)

#define SECYR				(SECDAY * 365)
#define	leapyear(year)		((year) % 4 == 0)
#define	days_in_year(a) 	(leapyear(a) ? 366 : 365)  //�ж�a % 4 == 0������1��0����Ӧ���366��365
#define	days_in_month(a) 	(month_days[(a) - 1])

//! @}

/*============================ TYPES =========================================*/
//!


/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/
// MCU:
/*============================ EXTERN FUNCTIONS ==============================*/

void  Reset_Time(void);
int8_t ReadRX8025Control2(void);


void read_time(void);
void serialread_time(void);
void rtc_deel(void);
void set_time(void);
unsigned long DateToSeconds(struct   RTCRX8025Str *Rtc);
void RtcBcdToD10(struct   RTCRX8025Str *Rtc);
void RtcD10ToBcd(struct   RTCRX8025Str *Rtc);

uint32_t RTC_Date_Time_To_Second(struct   RTCRX8025Str *Rtc);
void RTC_Second_To_Date_Time(uint32_t tim,struct   RTCRX8025Str *Rtc);
//#ifdef __cplusplus
//}
//#endif

#endif /* __BSPRTC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
