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

/******************************************************************************
  * @brief  Description 设置RTC出厂时间
  * @param  无
  * @retval 无		
  *****************************************************************************/
static void  Reset_Time(void)
{
	uint8_t setbuf[7];
	setbuf[0] = 0x00;
	RTC8025_Write(setbuf,RX8025_DigitalOffsetAddr,1);	/*不使用精度调整功能*/
	
	setbuf[0] = 0x20;
	RTC8025_Write(setbuf,RX8025_Control1Addr,1);   /*设置24小时制*/
	
	/*写入出厂时间*/
	setbuf[6] = 0x15;	/*年*/
	setbuf[5] = 0x10;	/*月*/
	setbuf[4] = 0x24;	/*日*/
	setbuf[3] = 0x00;	/*星期*/
	setbuf[2] = 0x10;	/*时*/
	setbuf[1] = 0x39;	/*分*/
	setbuf[0] = 0x30;	/*秒*/
	RTC8025_Write(setbuf,RX8025_SecondsAddr,7);	/*写入RX8025*/ 
}
/******************************************************************************
  * @brief  Description RX8025初始化
  * @param  无
  * @retval 无		
  *****************************************************************************/
uint8_t RX8025_RTC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	/*使能PWR和BKP外设时钟*/  
	PWR_RTCAccessCmd(ENABLE);	                   		/*使能后备寄存器访问*/
	/* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();
    /*检查是不是第一次配置时钟 ，这里MCU自带RTC未接备份电池*/
	/*从指定的后备寄存器中读出数据：读出了与写入的指定数据不相乎*/
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5050)//BKP_DR1 
	{
		printf("\r\n\r\n RX8025_RTC configured....");
		Reset_Time();	/*设置时间出厂时间*/
		
		/*向指定的后备寄存器中写入用户程序数据*/
		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x5050);
		return 1;	/*返回1，第一次初始化*/
	}
	else
	{
		/*启动无需设置新时钟*/
		/*检查是否掉电重启*/
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
		  printf("\r\n Power On Reset occurred....\n\r");
		}
		/*检查是否Reset复位*/
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
		  printf("\r\n External Reset occurred....\n\r");
		}

		printf("\n\r No need to configure RX8025_RTC....\n\r");
	}
    return 0;	/*返回0，已经初始化过*/
}
/******************************************************************************
  * @brief  Description 读取RX8025时间
  * @param  无
  * @retval 无		
  *****************************************************************************/
void read_time(void)
{
//    unsigned char i;
	uint8_t readbuf[7];

    //RTC8025_Read(&readbuf[1],(RX8025_ADDR_CONTROL1&RX8025_READ_MODE),1);
	RTC8025_Read(readbuf,RX8025_SecondsAddr,7);	/*读取时钟时间*/
	
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
    Rtc.Day    &= 0x3f;
    Rtc.Week   &= 0x07;
    Rtc.Month  &= 0x9f;
    Rtc.Year   &= 0x7f;
}
/******************************************************************************
  * @brief  Description 设置RX8025时间
  * @param  无
  * @retval 无		
  *****************************************************************************/
void set_time(void)
{
	uint8_t setbuf[7];
	
	setbuf[0] = Conf.Jly.Time_Sec;	 /*秒*/
	setbuf[1] = Conf.Jly.Time_Min;	 /*分*/
	setbuf[2] = Conf.Jly.Time_Hour;	 /*时*/
	setbuf[3] = Conf.Jly.Time_Week;	 /*星期*/
	setbuf[4] = Conf.Jly.Time_Day;	 /*日*/
	setbuf[5] = Conf.Jly.Time_Month; /*月*/
	setbuf[6] = Conf.Jly.Time_Year;	 /*年*/
	RTC8025_Write(setbuf,RX8025_SecondsAddr,7);	/*写入RX8025*/ 
}
/******************************************************************************
  * @brief  Description rtc 处理
  * @param  无
  * @retval 无		
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
//        if(*(char*)(TCP_ENABLE_ADDR)!=1)
//        {
//            clear_GPRS;
//            GSM_POWERCTL_OFF;;
//            Flag.STOP_SendGPRS=1;
//            clearMEM;
//            Pt.IStep=0;
//        }
        
        rtc_pt=0;
        read_time();                     //60s 读一次时间
        while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
		displayTIME(Rtc.Hour,Rtc.Minute);//显示时间
        showTIME;
        LCD_UpdateDisplayRequest();
    }
    if((rtc_pt==20)||(rtc_pt==50))
    {
        while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
		displayDAY(Rtc.Month,Rtc.Day);  //30s 显示一次日期
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
    if((rtc_pt<20)||((rtc_pt>23)&&(rtc_pt<50))||(rtc_pt>53))//时间COL闪,在显示日期,年时,COL不闪
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

/******************************************************************************
  * @brief  Description 转换成秒数 处理
  * @param  无
  * @retval 无		
  *****************************************************************************/
unsigned long DateToSeconds(struct   RTCRX8025 *Rtc)
{
    static uint32_t month[12]=
    {
        /*01月*/xDAY*(0),
        /*02月*/xDAY*(31),
        /*03月*/xDAY*(31+28),
        /*04月*/xDAY*(31+28+31),
        /*05月*/xDAY*(31+28+31+30),
        /*06月*/xDAY*(31+28+31+30+31),
        /*07月*/xDAY*(31+28+31+30+31+30),
        /*08月*/xDAY*(31+28+31+30+31+30+31),
        /*09月*/xDAY*(31+28+31+30+31+30+31+31),
        /*10月*/xDAY*(31+28+31+30+31+30+31+31+30),
        /*11月*/xDAY*(31+28+31+30+31+30+31+31+30+31),
        /*12月*/xDAY*(31+28+31+30+31+30+31+31+30+31+30)
    };
    uint32_t seconds;
    uint8_t year;
    
    year = Rtc->Year-9;                 //不考虑2100年千年虫问题
    seconds = xYEAR*year + xDAY*((year+1)/4);//前几年过去的秒数
    seconds += month[Rtc->Month-1];       //加上今年本月过去的秒数
    if( (Rtc->Month > 2) && (((year+2)%4)==0) )//2008年为闰年
    seconds += xDAY;                       //闰年加上一天秒数
    seconds += xDAY*(Rtc->Day-1);         //加上本天过去的秒数
    seconds += xHOUR*Rtc->Hour;           //加上本小时过去的秒数
    seconds += xMINUTE*Rtc->Minute;       //加上本分钟过去的秒数
    //seconds += Rtc->Second;               //加上当前秒数
    return seconds;
}
/******************************************************************************
  * @brief  Description BCD码转D10 处理
  * @param  无
  * @retval 无		
  *****************************************************************************/
void RtcBcdToD10(struct   RTCRX8025 *Rtc)
{
    Rtc->Second=(uint8_t)(BCD_TO_D10(Rtc->Second));
    Rtc->Minute=(uint8_t)(BCD_TO_D10(Rtc->Minute));
    Rtc->Hour=(uint8_t)(BCD_TO_D10(Rtc->Hour));
    Rtc->Day=(uint8_t)(BCD_TO_D10(Rtc->Day));
    Rtc->Month=(uint8_t)(BCD_TO_D10(Rtc->Month));
    Rtc->Year=(uint8_t)(BCD_TO_D10(Rtc->Year));
}
/******************************************************************************
  * @brief  Description D10转BCD码 处理
  * @param  无
  * @retval 无		
  *****************************************************************************/
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
