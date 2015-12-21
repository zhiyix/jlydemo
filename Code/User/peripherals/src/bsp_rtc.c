/**
  ******************************************************************************
  * @file              : bsp_rtc.c
  * @author            : 
  * @version           : 
  * @date              : 
  * @brief             : c file
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
#ifndef __BSPRTC_C
#define __BSPRTC_C

/*============================ INCLUDES ======================================*/
/* Includes ------------------------------------------------------------------*/

#include "main.h"

//	<<< end of configuration section >>>

/*============================ MACRO =========================================*/

/*============================ TYPES =========================================*/

/*============================ LOCAL VARIABLES ===============================*/

/*============================ GLOBAL VARIABLES ==============================*/

/*============================ EXTERN FUNCTIONS ==============================*/
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* ******************************* �·ݶ�Ӧ�������� ************************************ */
uint16_t month_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/* Private functions ---------------------------------------------------------*/

/******************************************************************************
  * @brief  Description ����RTC����ʱ��
  * @param  ��
  * @retval ��		
  *****************************************************************************/
void  Reset_Time(void)
{
	uint8_t setbuf[7];
	setbuf[0] = 0x00;
	RTC8025_Write(setbuf,RX8025_DigitalOffsetAddr,1);	/*��ʹ�þ��ȵ�������*/
	
	setbuf[0] = (0x20 + 0x05);// 0011 0101  1��һ��
	RTC8025_Write(setbuf,RX8025_Control1Addr,1);   /*����24Сʱ��,����BIT4 Ϊ1��ʾ���ù�RX8025*/
	
	setbuf[0] = 0x20;//0x20
	RTC8025_Write(setbuf,RX8025_Control2Addr,1);  //���RX8025 PONλ������BIT3 Ϊ1��ʾ���ù�RX8025
	
	/*д�����ʱ��*/
//	setbuf[6] = 0x15;	/*��*/
//	setbuf[5] = 0x10;	/*��*/
//	setbuf[4] = 0x24;	/*��*/
//	setbuf[3] = 0x00;	/*����*/
//	setbuf[2] = 0x10;	/*ʱ*/
//	setbuf[1] = 0x39;	/*��*/
//	setbuf[0] = 0x30;	/*��*/
//	RTC8025_Write(setbuf,RX8025_SecondsAddr,7);	/*д��RX8025*/ 
}

/******************************************************************************
  * @brief  Description ��ȡ���ù�RX8025 Control1��־
  * @param  ��
  * @retval ��		
  *****************************************************************************/
/*
static int8_t ReadRX8025Control1(void)
{
	uint8_t TempBuf[1];
	int8_t  RX8025Flag;
	
	RTC8025_Read(TempBuf,RX8025_Control1Addr,1);
	
	RX8025Flag = TempBuf[0];
	return RX8025Flag;
}
*/
/******************************************************************************
  * @brief  Description ��ȡ���ù�RX8025 Control2��־
  * @param  ��
  * @retval ��		
  *****************************************************************************/
int8_t ReadRX8025Control2(void)
{
	uint8_t TempBuf[1];
	int8_t  RX8025Flag;
	
	RTC8025_Read(TempBuf,RX8025_Control2Addr,1);
	
	RX8025Flag = TempBuf[0];
	return RX8025Flag;
}

/******************************************************************************
  * @brief  Description ��ȡRX8025ʱ��
  * @param  ��
  * @retval ��		
  *****************************************************************************/
void read_time(void)
{
//    unsigned char i;
	uint8_t readbuf[16];//16 7

    //RTC8025_Read(&readbuf[1],(RX8025_ADDR_CONTROL1&RX8025_READ_MODE),1);
	RTC8025_Read(readbuf,RX8025_SecondsAddr,16);	/*��ȡʱ��ʱ��*/
	
	Rtc.Second = readbuf[0];
    Rtc.Minute = readbuf[1];
    Rtc.Hour   = readbuf[2];
	Rtc.Week   = readbuf[3];
    Rtc.Day    = readbuf[4];
    Rtc.Month  = readbuf[5];
    Rtc.Year   = readbuf[6];
	
    Rtc.Second &= 0x7f;
    Rtc.Minute &= 0x7f;
    Rtc.Hour   &= 0x3f;
    Rtc.Week   &= 0x07;
    Rtc.Day    &= 0x3f;
    Rtc.Month  &= 0x9f;
    Rtc.Year   &= 0x7f;
}
/******************************************************************************
  * @brief  Description ͨ������(Э��)��ȡRX8025ʱ��
  * @param  ��
  * @retval ��		
  *****************************************************************************/
void serialread_time(void)
{
	uint8_t readbuf[7];
	
	read_time();
	
	readbuf[0] = Rtc.Second;
    readbuf[1] = Rtc.Minute; 
    readbuf[2] = Rtc.Hour;
	readbuf[3] = Rtc.Week; 
    readbuf[4] = Rtc.Day;
    readbuf[5] = Rtc.Month; 
    readbuf[6] = Rtc.Year;  
	/*д����ʱ�䵽Fram�У�����û���޸�cen������*/
	Fram_Write(readbuf,FRAM_JlyTimeConfAddr,7);
}
/******************************************************************************
  * @brief  Description ����RX8025ʱ��
  * @param  ��
  * @retval ��		
  *****************************************************************************/
void set_time(void)
{
	uint8_t setbuf[7];
	
	setbuf[0] = Conf.Time.Time_Sec;	  /*��*/
	setbuf[1] = Conf.Time.Time_Min;	  /*��*/
	setbuf[2] = Conf.Time.Time_Hour;  /*ʱ*/
	setbuf[3] = Conf.Time.Time_Week;  /*����*/
	setbuf[4] = Conf.Time.Time_Day;	  /*��*/
	setbuf[5] = Conf.Time.Time_Month; /*��*/
	setbuf[6] = Conf.Time.Time_Year;  /*��*/
	RTC8025_Write(setbuf,RX8025_SecondsAddr,7);	/*д��RX8025*/ 
}
/******************************************************************************
  * @brief  Description rtc ��ʾ����
  * @param  ��
  * @retval ��		
  *****************************************************************************/
void rtc_deel(void)
{
    rtc_pt++;
    /*
    if((rtc_pt%5)==0)
    {
        read_time();                     
    }*/
    if(rtc_pt>59)
    {
        //printf("rtc_pt%d,",rtc_pt);
        rtc_pt=0;
        read_time();                     //60s ��һ��ʱ��
        while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
		displayTIME(Rtc.Hour,Rtc.Minute);//��ʾʱ��
        LCD_UpdateDisplayRequest();
		
    }
    if((rtc_pt==20)||(rtc_pt==50))
    {
        while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
		displayDAY(Rtc.Month,Rtc.Day);  //30s ��ʾһ������
        showP;
        LCD_UpdateDisplayRequest();
    }
    if((rtc_pt==22)||(rtc_pt==52))
    {
        while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
		displayYEAR(Conf.Time.Time_Cen,Rtc.Year);
        clearP;
        LCD_UpdateDisplayRequest();
    }
    if((rtc_pt==24)||(rtc_pt==54))
    {
        while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
		displayTIME(Rtc.Hour,Rtc.Minute);
        LCD_UpdateDisplayRequest();
    }
    if((rtc_pt<20)||((rtc_pt>23)&&(rtc_pt<50))||(rtc_pt>53))//ʱ��COL��,����ʾ����,��ʱ,COL����
    {
        if(Flag.RtcShan==1)
        {
            Flag.RtcShan=0;
            while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
            showCOL;
            LCD_UpdateDisplayRequest();
        }
        else
        {
            Flag.RtcShan=1;
            while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
            clearCOL;
            LCD_UpdateDisplayRequest();
        }
    }
}

/******************************************************************************
  * @brief  Description ת�������� ����
  * @param  ��
  * @retval ��		
  *****************************************************************************/
unsigned long DateToSeconds(struct   RTCRX8025Str *Rtc)
{
    static uint32_t month[12]=
    {
        /*01��*/xDAY*(0),
        /*02��*/xDAY*(31),
        /*03��*/xDAY*(31+28),
        /*04��*/xDAY*(31+28+31),
        /*05��*/xDAY*(31+28+31+30),
        /*06��*/xDAY*(31+28+31+30+31),
        /*07��*/xDAY*(31+28+31+30+31+30),
        /*08��*/xDAY*(31+28+31+30+31+30+31),
        /*09��*/xDAY*(31+28+31+30+31+30+31+31),
        /*10��*/xDAY*(31+28+31+30+31+30+31+31+30),
        /*11��*/xDAY*(31+28+31+30+31+30+31+31+30+31),
        /*12��*/xDAY*(31+28+31+30+31+30+31+31+30+31+30)
    };
    uint32_t seconds;
    uint8_t year;
    
    year = Rtc->Year-9;                 //������2100��ǧ�������
    seconds = xYEAR*year + xDAY*((year+1)/4);//ǰ�����ȥ������
    seconds += month[Rtc->Month-1];       //���Ͻ��걾�¹�ȥ������
    if( (Rtc->Month > 2) && (((year+2)%4)==0) )//2008��Ϊ����
    seconds += xDAY;                       //�������һ������
    seconds += xDAY*(Rtc->Day-1);         //���ϱ����ȥ������
    seconds += xHOUR*Rtc->Hour;           //���ϱ�Сʱ��ȥ������
    seconds += xMINUTE*Rtc->Minute;       //���ϱ����ӹ�ȥ������
    //seconds += Rtc->Second;               //���ϵ�ǰ����
    return seconds;
}
/******************************************************************************
  * @brief  Description BCD��תD10 ����
  * @param  ��
  * @retval ��		
  *****************************************************************************/
void RtcBcdToD10(struct   RTCRX8025Str *Rtc)
{
    Rtc->Second=(uint8_t)(BCD_TO_D10(Rtc->Second));
    Rtc->Minute=(uint8_t)(BCD_TO_D10(Rtc->Minute));
    Rtc->Hour=(uint8_t)(BCD_TO_D10(Rtc->Hour));
    Rtc->Day=(uint8_t)(BCD_TO_D10(Rtc->Day));
    Rtc->Month=(uint8_t)(BCD_TO_D10(Rtc->Month));
    Rtc->Year=(uint8_t)(BCD_TO_D10(Rtc->Year));
}
/******************************************************************************
  * @brief  Description D10תBCD�� ����
  * @param  ��
  * @retval ��		
  *****************************************************************************/
void RtcD10ToBcd(struct   RTCRX8025Str *Rtc)
{
    Rtc->Second=(uint8_t)(D10_TO_BCD(Rtc->Second));
    Rtc->Minute=(uint8_t)(D10_TO_BCD(Rtc->Minute));
    Rtc->Hour=(uint8_t)(D10_TO_BCD(Rtc->Hour));
    Rtc->Day=(uint8_t)(D10_TO_BCD(Rtc->Day));
    Rtc->Month=(uint8_t)(D10_TO_BCD(Rtc->Month));
    Rtc->Year=(uint8_t)(D10_TO_BCD(Rtc->Year));
}

/* **********************************************************************************
 * ��������Date_Time_To_Second
 * ����  ��������ʱ������ת����������
 * ����  ��ʱ���������ݽṹ
 * ���  ��������
 * **********************************************************************************/
uint32_t RTC_Date_Time_To_Second(struct   RTCRX8025Str *Rtc)
{
	Rtc->Year += STARTOFTIME;
	if (Rtc->Month > 2) 			/* 1..12 -> 11,12,1..10 */
	{								
		Rtc->Month -= 2;
	}else
	{
		Rtc->Month += 10;		/* Puts Feb last since it has leap day */
		Rtc->Year -= 1;
	}

	return (((
		(uint32_t) (Rtc->Year/4 - Rtc->Year/100 + Rtc->Year/400 + 367*Rtc->Month/12 + Rtc->Day) +
			Rtc->Year*365 - 730456
	    )*24 + Rtc->Hour /* now have hours 719499 */
	  )*60 + Rtc->Minute /* now have minutes */
	)*60 + Rtc->Second; /* finally seconds */
	
}

/* **********************************************************************************
 * ��������Second_To_Date_Time
 * ����  ����������ת���ɹ�����ʱ������
 * ����  ����ǰ�������ݡ�ʱ�����ݽṹ��
 * ���  ����
 * **********************************************************************************/
void RTC_Second_To_Date_Time(uint32_t tim,struct   RTCRX8025Str *Rtc)
{
	register uint32_t    i;
	register long   hms, day;

	day = tim / SECDAY;
	hms = tim % SECDAY;

	/* Hours, minutes, seconds are easy */
	Rtc->Hour = hms / 3600;
	Rtc->Minute = (hms % 3600) / 60;
	Rtc->Second = (hms % 3600) % 60;

	/* Number of years in days */ /*�����ǰ��ݣ���ʼ�ļ������Ϊ1970��*/
	for (i = STARTOFTIME; day >= days_in_year(i); i++) {
		day -= days_in_year(i);
	}
	Rtc->Year = i;

	/* Number of months in days left */ /*���㵱ǰ���·�*/
	if (leapyear(Rtc->Year)) {
		days_in_month(FEBRUARY) = 29;
	}
	for (i = 1; day >= days_in_month(i); i++) {
		day -= days_in_month(i);
	}
	days_in_month(FEBRUARY) = 28;
	Rtc->Month = i;

	/* Days are what is left over (+1) from all that. *//*���㵱ǰ����*/
	Rtc->Day = day + 1;
	
	Rtc->Year -= STARTOFTIME;
}


#endif /* __BSPRTC_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
