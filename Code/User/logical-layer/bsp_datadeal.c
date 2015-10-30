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

/******************************************************************************
  * @brief  Description 正常、省电模式处理
						省电模式：采集间隔最小10s
						正常模式：采集间隔最小2s
  * @param  无  		
  * @retval 无		
  *****************************************************************************/
static void LOWorNomal_Mode(void)
{
	uint8_t i=0;
    if((Conf.Jly.PowerMode == 1)&&(Conf.Jly.WorkStatueIsStop >= 1)) /*低功耗模式*/
    {
        if(Flag.StartSample==1)
        {
            Flag.StartSample=0;
            Flag.EndSample=1;
			
			
            Dealing_Gather(Started_Channel);
            AVCC1_POWER(OFF);
            DoGatherChannelDataFloat(Started_Channel);
        }
        if(JlyParam.SampleTime == JlyParam.SampleInterval-1)//
        {
			/*判断传感器接口类型 模拟/数字*/
			for(i=0;i<Conf.Jly.ChannelNum;i++)
			{
				if(Conf.Sensor[i].SensorInterfaceType==0x00)	/*模拟*/
				{
					Flag.StartSample=1;
					AVCC1_POWER(ON);	/*打开传感器电源*/
				}
				else if(Conf.Sensor[i].SensorInterfaceType==0x01)/*数字*/
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
        if((JlyParam.SampleTime>1) && JlyParam.SampleTime%2==0)//2s采集一次
        {
			/*判断传感器接口类型 模拟/数字*/
			for(i=0;i<Conf.Jly.ChannelNum;i++)
			{
				if(Conf.Sensor[i].SensorInterfaceType==0x00)	/*模拟*/
				{
					Flag.StartSample=1;
					AVCC1_POWER(ON);	/*打开传感器电源*/
				}
				else if(Conf.Sensor[i].SensorInterfaceType==0x01)/*数字*/
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
	
	if((--JlyParam.SampleTime)<=0)
    {
        JlyParam.SampleTime = JlyParam.SampleInterval;//采集时间 单位:s
    }
}
/******************************************************************************
  * @brief  Description 记录仪工作状态处理
  * @param  无  		
  * @retval 无		
  *****************************************************************************/
static void WorkornotMode(void)
{
    if(Conf.Jly.WorkStatueIsStop <1)//停止工作
    {
		/*关掉外设电源*/
		AVCC1_POWER(OFF);	/* 关传感器电源 */
        BEEP(OFF);			/* 关蜂鸣器 */
		AlarmLed2_OFF;		/* 关报警灯 */
		
		lcd_OFF(JlyParam.ShowOffCode);
		
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
            Display_ChannelValue(StartedChannelForDisplay);  //LCD 
//        }
        Display_Signal(2);/*显示信号强度*/
        Display_Mem();	  /*显示存储容量*/
//         SaveData();
        Flag.IsDisplayRightNow=1;
        
    } 
}


/******************************************************************************
  * @brief  Description 记录仪1s处理标志
  * @param  无  		
  * @retval 无		
  *****************************************************************************/
static void OneSec_Timedeal(void)
{
    
    //LCD显示处理
    display_ct++;          
    if(display_ct>=36)
    {
        display_ct = 0;
         
    }
   /*检测外接电接入*/
	if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_Deal_ACtest) == 0)         
    {
        PManage.HaveExternalPower++;
        if(PManage.HaveExternalPower >= ExternalPowerchecktime)
        {
            Flag.Powerdowncountflag=1;
            PManage.HaveExternalPower=0;
        }
    }    
	/*****************************************************************
	 *有外接电的情况
	 *(1)接锂电池，检测其充满电
	 *(2)未接锂电池
	 *****************************************************************/
	if(Flag.ExPwOn == 1)	
	{
		if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_Deal_CHGtest) == 1)         
		{
			PManage.BatChargeFullCount++;
			
			if(PManage.BatChargeFullCount >= ExternalPowerchecktime)/*检测60s*/
			{
				PManage.BatChargeFullCount=0;
				Flag.BatChargeFull=1;/*接外接电，电池充满标志*/
				
				Flag.BatCharging = 0;/*接外接电未接电池，电池未充电*/
			}			
		}
		else
		{
			Flag.BatChargeFull=0;
			Flag.BatCharging = 1;/*接外接电，电池正在充电中*/
		}
	}
	else
	{
		Flag.BatChargeFull=0;
		Flag.BatCharging = 0;/*外接电未接*/
	}
	
}
/******************************************************************************
  * @brief  Description 记录仪
  * @param  无  		
  * @retval 无		
  *****************************************************************************/
void SaveDataOnTimeDeal(void)
{
    
    read_time();
    
    RtcBcdToD10(&Rtc);
    
    Rtc.TMPS=DateToSeconds(&Rtc);
    
    RtcD10ToBcd(&Rtc);
    
	if((JlyParam.NormalRecInterval > 0) && (JlyParam.NormalRecInterval < 60))//1s-60s内数据记录
	{
		Rtc.SCount++;
		if(Rtc.SCount >= 60)
		{
			Rtc.SCount = 0;
		}
		if(Rtc.SCount % JlyParam.NormalRecInterval ==0)
		{
			
			read_time();
			ChannelDataDeal(Channel_count,Clock_choose,Gps_choose);
			SaveHisDataToFram();
		}
	}
	else
	{
		Rtc.TMPS = Rtc.TMPS/60;	//分钟
		if(Rtc.TMPS % JlyParam.NormalRecIntervalMin ==0)//1分钟-
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
	
}
/******************************************************************************
  * @brief  Description 记录仪处理函数
  * @param  无  		
  * @retval 无		
  *****************************************************************************/
void JlySecDeal(void)
{
    if(Flag.Sec == 1)   //1秒
    {
        Flag.Sec = 0;
        
        Display_SN();   /*显示SN号*/
        
        OneSec_Timedeal();
        
        
        rtc_deel();
        
		OutpowerShan();
		
        VoltageTest();
        
		RecorderBootModeHandle();
		RecorderStopModeHandle();
		
        WorkornotMode();
        
		SaveDataOnTimeDeal();
    }
	if(Conf.Jly.WorkStatueIsStop == 0) /*停止工作*/
    {
		/*关掉外设电源*/
        AVCC1_POWER(OFF);	/* 关传感器电源 */
        BEEP(OFF);			/* 关蜂鸣器 */
		AlarmLed2_OFF;		/* 关报警灯 */
		
        lcd_OFF(JlyParam.ShowOffCode);

        if(JlyParam.LastErrorCode!=0)
        {
            displayErr(JlyParam.LastErrorCode);
        }
    }
}
#endif /* __BSPDATADEAL_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
