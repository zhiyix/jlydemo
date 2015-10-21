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
	uint8_t i=0;
    if((Conf.Jly.PowerMode == 1)&&(Conf.Jly.WorkStatueIsStop >= 1))//省电模式
    {
        if(Flag.StartSample==1)
        {
            Flag.StartSample=0;
            Flag.EndSample=1;
			
			
            Dealing_Gather(Started_Channel);
            AVCC1_POWER(OFF);
            DoGatherChannelDataFloat(Started_Channel);
        }
        if(JlyParam.Save_Time == JlyParam.SaveDataTimeOnlyRead-1)
        {
//            SET_CHANNEL0_POWER_IO;
			//判断传感器接口类型 模拟/数字
			for(i=0;i<Conf.Jly.ChannelNum;i++)
			{
				if(Conf.Sensor[i].SensorInterfaceType==0x00)//模拟
				{
					Flag.StartSample=1;
					AVCC1_POWER(ON);
				}
				else if(Conf.Sensor[i].SensorInterfaceType==0x01)//数字
				{
					
				}
			}
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
    else if(Conf.Jly.WorkStatueIsStop >= 1)//正常模式
    {
        if(Flag.StartSample==1)
        {
            Flag.StartSample=0;
            Flag.EndSample=1;
                
			
			Dealing_Gather(Started_Channel);
            AVCC1_POWER(OFF);
            DoGatherChannelDataFloat(Started_Channel);
        }
        if((JlyParam.Save_Time>1) && JlyParam.Save_Time%2==0)
        {
			//判断传感器接口类型 模拟/数字
			for(i=0;i<Conf.Jly.ChannelNum;i++)
			{
				if(Conf.Sensor[i].SensorInterfaceType==0x00)//模拟
				{
					Flag.StartSample=1;
					AVCC1_POWER(ON);
				}
				else if(Conf.Sensor[i].SensorInterfaceType==0x01)//数字
				{
					
				}
			}
        }
        if(Flag.EndSample==1)
        {
            Flag.EndSample=0;
            
            if(Flag.MucReset==1)
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
    if(Conf.Jly.WorkStatueIsStop <1)//停止工作
    {
		AVCC1_POWER(OFF);	/* 关传感器电源  */
        BEEP(OFF);			/* 关蜂鸣器 */
		AlarmLed2_OFF;		/* 关报警灯 */
		
		lcd_OFF(4);
		
        if(JlyParam.LastErrorCode!=0)
        {
            displayErr(JlyParam.LastErrorCode);
		}
        if(Flag.MucReset==1)
        {
            Flag.MucReset=0;
            
        }
        return;
    }
    else
    {
        
        LOWorNomal_Mode();
        
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
  * @brief  Description 记录仪启动方式处理
  * @param  无  		
  * @retval 无		
  */
void RecorderBootModeHandle(void)
{
    if(JlyParam.LastErrorCode!=0)
    {
        if(Conf.Jly.WorkStatueIsStop==1)	/*!< 工作状态 */
        {
            
            Conf.Jly.WorkStatueIsStop = 0;
            //写入fram
//            ShowOffCode = 0x09;
        }
        return;
    }
    if(Conf.Jly.RecBootMode == 0)	/*!< 延时启动(默认延时时间0，即立即启动)*/
    {
		if(((JlyParam.delay_start_time--)<=0) && (!Conf.Jly.WorkStatueIsStop))	/*!< 工作 */
        {
            JlyParam.delay_start_time = -1;
            Conf.Jly.WorkStatueIsStop = 1;
            Conf.Jly.RecBootMode = 0;
        }
        if((JlyParam.delay_start_time)>0)
        {
            
            Conf.Jly.WorkStatueIsStop = 0;	/*!< 停止工作 */
			
//            ShowOffCode=5;
        }
    }
	else if(Conf.Jly.RecBootMode == 1)	/*!< 时间点定时启动 */
	{
		
	}
	else if(Conf.Jly.RecBootMode == 2)	/*!< 时间点定点启停 */
	{
		
	}
	else if((Conf.Jly.RecBootMode == 3) && (Conf.Jly.WorkStatueIsStop))	/*!< 手动启动*/
    {
        Conf.Jly.WorkStatueIsStop = 0;	/*!< 停止工作 */
            //写入fram
		Fram_Write(&Conf.Jly.WorkStatueIsStop,FRAM_WorkStatueIsStopAddr,1);
//		ShowOffCode = 0xFF;
    }
    else if(Conf.Jly.RecBootMode == 0x10)	/*!< 异常条件启动 */
    {
        
    }
}
/**
  * @brief  Description 记录仪停止方式处理
  * @param  无  		
  * @retval 无		
  */
void   RecorderStopModeHandle(void)
{
    uint8_t  IsWriteLasestStopTimeToFram;
    IsWriteLasestStopTimeToFram = 0;
    
    if(Conf.Jly.WorkStatueIsStop)
    {
        if(Conf.Jly.RecStopMode == 0){};	/*!< 先进先出的记录停止方式 */
        if(Conf.Jly.RecStopMode ==1)	/*!< 存储器记满的记录停止方式 */
        {
            if((Queue.RecorderFlashPoint >= Flash_MAX_NUM)) //&&IsReadingI2c==0,下载数据
            {
                
                Conf.Jly.WorkStatueIsStop = 1;	/*!< 停止工作 */
				
				//写Fram
				Fram_Write(&Conf.Jly.WorkStatueIsStop,FRAM_WorkStatueIsStopAddr,1);
                IsWriteLasestStopTimeToFram = 1;
//                ShowOffCode = 0;
            }
        }
        
    }
    if(IsWriteLasestStopTimeToFram)
    {
//        Word_to_Char(Current_Year);
//        Write24c02(S_E_A,E2PROM_LastestStopTime_FA,word00.high);
//        Write24c02(S_E_A,E2PROM_LastestStopTime_FA+1,word00.low);
//        Write24c02(S_E_A,E2PROM_LastestStopTime_FA+2,Current_Month);
//        Write24c02(S_E_A,E2PROM_LastestStopTime_FA+3,Current_Day);
//        Write24c02(S_E_A,E2PROM_LastestStopTime_FA+4,Current_Hour);
//        Write24c02(S_E_A,E2PROM_LastestStopTime_FA+5,Current_Min);
//        Write24c02(S_E_A,E2PROM_LastestStopTime_FA+6,Current_Sec);
//        
//        Write24c02(S_E_A,E2PROM_LastestStopTime_FA+7,ShowOffCode);
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
