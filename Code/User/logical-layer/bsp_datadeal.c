/**
  ******************************************************************************
  * @file              : bsp_datadeal.c
  * @author            : 
  * @version           : 
  * @date              : 
  * @brief             : 数据处理子程序
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
#ifndef __BSPDATADEAL_C
#define __BSPDATADEAL_C

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
  * @brief  Description 正常、省电模式处理
  * @param  无  		
  * @retval 无		
  */
static void LOWorNomal_Mode(void)
{
    if((JlyParam.LowMode == 1)&&(JlyParam.WorkStatueIsStop >= 1))//省电模式
    {
        if(Flag.StartSample==1)
        {
            Flag.StartSample=0;
            Flag.EndSample=1;
            //sht_index=0;
//            #if WD_TYPE==WD_NTC
                Dealing_Gather(Started_Channel);
//            #endif
            
            AVCC1_POWER(OFF);
            DoGatherChannelDataFloat(Started_Channel);
        }
        if(JlyParam.Save_Time == JlyParam.SaveDataTimeOnlyRead-1)
        {
//            SET_CHANNEL0_POWER_IO;
            AVCC1_POWER(ON);
            
            Flag.StartSample=1;
        }
        if(Flag.EndSample==1)
        {
            Flag.EndSample=0;
            
            if(Flag.MucReset==1)//???? MCU???,???????????
            {
                Flag.MucReset=0;
            }
        }
    }//END 
    else if(JlyParam.WorkStatueIsStop >= 1)//正常模式
    {
        if(Flag.StartSample==1)
        {
            Flag.StartSample=0;
            Flag.EndSample=1;
//            #if WD_TYPE==WD_NTC
                Dealing_Gather(Started_Channel);
//            #endif
//            CLR_CHANNEL0_POWER_IO;
            AVCC1_POWER(OFF);
            DoGatherChannelDataFloat(Started_Channel);
            //sht_index=0;
        }
        if((JlyParam.Save_Time>1) && JlyParam.Save_Time%2==0)
        {
//            SET_CHANNEL0_POWER_IO;
            AVCC1_POWER(ON);
            
            Flag.StartSample=1;
        }
        if(Flag.EndSample==1)
        {
            Flag.EndSample=0;
            
            if(Flag.MucReset==1)//
            {
                Flag.MucReset=0;
            }
        }
    }
}
/**
  * @brief  Description 记录仪工作停止状态处理
  * @param  无  		
  * @retval 无		
  */
static void WorkornotMode(void)
{
    if(JlyParam.WorkStatueIsStop <1)//停止工作
    {
//        OpenWDT_IE;
//        CLR_ALL_INPUT_OUTPUT_IO;
//        CLR_ALLCHANNELS_POWER_IO;

//        lcd_OFF(4);
//        buzzer_off();
//        clearJINBAO;
//        
//        CLR_ALARM_LED;
//        GSM_POWERCTL_OFF;
//        clearMEM;
//        Pt.IStep=0;
//        
//        if(LastErrorCode!=0)
//        {
//            displayErr(LastErrorCode,2);//zzz
//        }
//        if(Flag.MucReset==1)
//        {
//            Flag.MucReset=0;
//            
//        }
        return;
    }
    else
    {
//        WDT_CLR;
        
        LOWorNomal_Mode();
        
        //-----------------
//        if((Flag.STOP_SendGPRS==1)||(*(char*)(TCP_ENABLE_ADDR)==0))
//        {
//            LCDMEM[14]&=~0X20;LCDMEM[14]&=~0X10;
//            LCDMEM[14]&=~0X40;LCDMEM[14]&=~0X80;
//            clearMEM;
//            
//            buzzer_off();
//            GSM_POWERCTL_OFF;
//            Flag.STOP_SendGPRS=1;
//            clearMEM;
//            Pt.IStep=0;
//        }
//        if((Flag.buttonS2flag==0)&&(Flag.buttonS3flag==0)&&(Flag.buttonS4flag==0))
//        {
            Display_ChannelValue(StartedChannelForDisplay);  //LCD ????
//        }
        
//         SaveData();
        Flag.IsDisplayRightNow=1;
        
//        GSM_USART1_SendDeal();
    } 
}
/**
  * @brief  Description 记录仪1s处理
  * @param  无  		
  * @retval 无		
  */
static void OneSec_Timedeal(void)
{
    
    //LCD显示处理
    display_ct++;          
    if(display_ct>=36)
    {
        display_ct = 0;
//        display_mem();
         
    }
   //----------------
}
/**
  * @brief  Description 记录仪
  * @param  无  		
  * @retval 无		
  */
void SaveDataOnTimeDeal(void)
{
    
    read_time();
    
    RtcBcdToD10(&Rtc);
    
    Rtc.TMPS=DateToSeconds(&Rtc);
    
    RtcD10ToBcd(&Rtc);
    
    Rtc.TMPS=Rtc.TMPS/60;
        
    if(Rtc.TMPS%JlyParam.SaveHisDataTime ==0)
    {   
        if(Rtc.TMPS!=Rtc.TCPS)
        {
            Rtc.TCPS=Rtc.TMPS;
            
            read_time();
            ChannelDataDeal(Channel_count,Clock_choose,Gps_choose);
            SaveHisDataToFram();
        }
    }
}
/**
  * @brief  Description 记录仪处理函数
  * @param  无  		
  * @retval 无		
  */
void JlySecDeal(void)
{
    if(Flag.Sec == 1)   //1秒
    {
        Flag.Sec = 0;
        
        Display_SN();   //显示SN号
        
        OneSec_Timedeal();
        
        
        rtc_deel();
        
        voltage_test();
        
        WorkornotMode();
        
		SaveDataOnTimeDeal();
    }
}
#endif /* __BSPDATADEAL_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
