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

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Description RX8025��ʼ��
  * @param  ��
  * @retval ��		
  */
void rtc_init(void)
{
	uint8_t buf[7];
	buf[0] = 0x20;
    RTC8025_Write(buf,(RX8025_ADDR_CONTROL1&RX8025_WRITE_MODE),1);   //24Сʱ��
	
	RTC8025_Write(&buf[1],(RX8025_ADDR_CONTROL1&RX8025_WRITE_MODE),1);   //24Сʱ��
}
/**
  * @brief  Description ��ȡRX8025ʱ��
  * @param  ��
  * @retval ��		
  */
void read_time(void)
{
//    unsigned char i;
	uint8_t buf[7];

    
	RTC8025_Read(buf,(RX8025_ADDR_SECONDS&RX8025_READ_MODE),7);
	
	Rtc.Second = buf[0];
    Rtc.Minute = buf[1];
    Rtc.Hour = buf[2];
    Rtc.Day = buf[4];
    Rtc.Week = buf[3];
    Rtc.Month = buf[5];
    Rtc.Year = buf[6];
	
    Rtc.Second &= 0x7f;
    Rtc.Minute &= 0x7f;
    Rtc.Hour &= 0x3f;
    Rtc.Day &= 0x3f;
    Rtc.Week &= 0x07;
    Rtc.Month &= 0x9f;
    Rtc.Year &= 0x7f;
    
}
/**
  * @brief  Description ����RTCʱ��
  * @param  ��
  * @retval ��		
  */
void  set_time(void)
{
	uint8_t buf[]={0x30,0x36,0x17,0x00,0x19,0x10,0x15};
	
	RTC8025_Write(buf,(RX8025_ADDR_SECONDS&RX8025_WRITE_MODE),7); 
   
}
/**
  * @brief  Description rtc ����
  * @param  ��
  * @retval ��		
  */
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
//        if(*(char*)(TCP_ENABLE_ADDR)!=1)
//        {
//            clear_GPRS;
//            GSM_POWERCTL_OFF;;
//            Flag.STOP_SendGPRS=1;
//            clearMEM;
//            Pt.IStep=0;
//        }
        
        rtc_pt=0;
        read_time();                     //60s ��һ��ʱ��
        while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
		displayTIME(Rtc.Hour,Rtc.Minute);//��ʾʱ��
        showTIME;
        LCD_UpdateDisplayRequest();
    }
    if((rtc_pt==20)||(rtc_pt==50))
    {
        while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
		displayDAY(Rtc.Month,Rtc.Day);  //30s ��ʾһ������
        showP;
        showTIME;        
        LCD_UpdateDisplayRequest();
    }
    if((rtc_pt==22)||(rtc_pt==52))
    {
        while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
		displayYEAR(Rtc.Year);
        clearP;
        showTIME;
        LCD_UpdateDisplayRequest();
    }
    if((rtc_pt==24)||(rtc_pt==54))
    {
        while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
		displayTIME(Rtc.Hour,Rtc.Minute);
        showTIME;
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
        while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
        showTIME;
        LCD_UpdateDisplayRequest();
    }
}

/**
  * @brief  Description ת�������� ����
  * @param  ��
  * @retval ��		
  */
unsigned long DateToSeconds(struct   RTCRX8025 *Rtc)
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
/**
  * @brief  Description BCD��תD10 ����
  * @param  ��
  * @retval ��		
  */
void RtcBcdToD10(struct   RTCRX8025 *Rtc)
{
    Rtc->Second=(uint8_t)(BCD_TO_D10(Rtc->Second));
    Rtc->Minute=(uint8_t)(BCD_TO_D10(Rtc->Minute));
    Rtc->Hour=(uint8_t)(BCD_TO_D10(Rtc->Hour));
    Rtc->Day=(uint8_t)(BCD_TO_D10(Rtc->Day));
    Rtc->Month=(uint8_t)(BCD_TO_D10(Rtc->Month));
    Rtc->Year=(uint8_t)(BCD_TO_D10(Rtc->Year));
}
/**
  * @brief  Description D10תBCD�� ����
  * @param  ��
  * @retval ��		
  */
void RtcD10ToBcd(struct   RTCRX8025 *Rtc)
{
    Rtc->Second=(uint8_t)(D10_TO_BCD(Rtc->Second));
    Rtc->Minute=(uint8_t)(D10_TO_BCD(Rtc->Minute));
    Rtc->Hour=(uint8_t)(D10_TO_BCD(Rtc->Hour));
    Rtc->Day=(uint8_t)(D10_TO_BCD(Rtc->Day));
    Rtc->Month=(uint8_t)(D10_TO_BCD(Rtc->Month));
    Rtc->Year=(uint8_t)(D10_TO_BCD(Rtc->Year));
}
#endif /* __BSPRTC_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
