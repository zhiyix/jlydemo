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
/**
  * @brief  Description 记录仪工作状态处理
  * @param  无  		
  * @retval 无		
  */
static void WorkornotMode(void)
{
    if(Conf.Jly.WorkStatueIsStop <1)//停止工作
    {
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
        
//         SaveData();
        Flag.IsDisplayRightNow=1;
        
    } 
}
/**
  * @brief  Description 记录仪启动方式处理
  * @param  无  		
  * @retval 无		
  * 说明：
		 0FF:控制(手动)停机状态
	    00FF:记录仪记满停机
		10FF:定点启动方式，还未开始记录
		20FF:定点停止方式，已到时停机
		30FF:定时启动方式，还未开始记录
		40FF:
		50FF:延时启动方式，还未开始记录
		90FF:出现故障停机
	手动启动上位机配置和按键开启工作
  */
void RecorderBootModeHandle(void)
{
    if(JlyParam.LastErrorCode != 0)
    {
        if(Conf.Jly.WorkStatueIsStop == 1)	/* 工作状态 */
        {
            
            Conf.Jly.WorkStatueIsStop = 0;	/*停止工作*/
            Fram_Write(&Conf.Jly.WorkStatueIsStop,FRAM_WorkStatueIsStopAddr,1);
			
            JlyParam.ShowOffCode = 0x09;	/*出现故障*/
        }
        return;
    }
    if(Conf.Jly.RecBootMode == 0x00)	/* 延时启动(默认延时时间0，即立即启动)*/
    {
		if(((JlyParam.delay_start_time--) <= 0) && (!Conf.Jly.WorkStatueIsStop))	
        {
            JlyParam.delay_start_time = -1;
            Conf.Jly.WorkStatueIsStop = 1;	/*开启工作*/
        }
        if((JlyParam.delay_start_time)>0)
        {
            Conf.Jly.WorkStatueIsStop = 0;	/* 停止工作 */
			
            JlyParam.ShowOffCode = 0x05;	/*表示延时启动方式，还未开始记录*/
        }
    }
	else if(Conf.Jly.RecBootMode == 0x01)	/* 时间点定时启动 */
	{
		if(!Conf.Jly.WorkStatueIsStop)	/* 停止工作 */
		{
			/*读取时钟时间*/
			read_time();
			if((Rtc.Year == Conf.Jly.MixBoot_Year)&&(Rtc.Month == Conf.Jly.MixBoot_Month)&&(Rtc.Day == Conf.Jly.MixBoot_Day)\
				&&(Rtc.Hour == Conf.Jly.MixBoot_Hour)&&(Rtc.Minute == Conf.Jly.MixBoot_Min))
			{
				Conf.Jly.WorkStatueIsStop = 1;	/*到时间点开启工作*/				
			}
			else
			{
				Conf.Jly.WorkStatueIsStop = 0;	/* 停止工作 */
				JlyParam.ShowOffCode = 0x03;	/*表示定时启动方式，还未开始记录*/
			}
		}
	}
	else if(Conf.Jly.RecBootMode == 0x02)	/* 时间点定点启停 */
	{
		if(!Conf.Jly.WorkStatueIsStop) /* 时间点定点启动 */
		{
			/*读取时钟时间*/
			read_time();
			if((Rtc.Year == Conf.Jly.MixBoot_Year)&&(Rtc.Month == Conf.Jly.MixBoot_Month)&&(Rtc.Day == Conf.Jly.MixBoot_Day)\
				&&(Rtc.Hour == Conf.Jly.MixBoot_Hour)&&(Rtc.Minute == Conf.Jly.MixBoot_Min))
			{
				Conf.Jly.WorkStatueIsStop = 1;	/*到时间点开启工作*/
			}
			else
			{
				Conf.Jly.WorkStatueIsStop = 0;	/* 停止工作 */
				JlyParam.ShowOffCode = 0x01;	/*表示定点启动方式，还未开始记录*/
			}
		}
		if(Conf.Jly.WorkStatueIsStop) /* 时间点定点停止 */
		{
			/*读取时钟时间*/
			read_time();
			if((Rtc.Year == Conf.Jly.FixedStop_Year)&&(Rtc.Month == Conf.Jly.FixedStop_Month)&&(Rtc.Day == Conf.Jly.FixedStop_Day)\
				&&(Rtc.Hour == Conf.Jly.FixedStop_Hour)&&(Rtc.Minute == Conf.Jly.FixedStop_Min))
			{
				Conf.Jly.WorkStatueIsStop = 0;	/* 到时间点停止工作 */
				JlyParam.ShowOffCode = 0x02;	/*表示定点停止方式 ，已到时停机*/
			}
			else
			{
				Conf.Jly.WorkStatueIsStop = 1;	
			}
		}
	}
	else if((Conf.Jly.RecBootMode == 0x03) && (Conf.Jly.WorkStatueIsStop))	/* 手动启动*/
    {
        Conf.Jly.WorkStatueIsStop = 0;	/* 停止工作 */
            //写入fram
		Fram_Write(&Conf.Jly.WorkStatueIsStop,FRAM_WorkStatueIsStopAddr,1);
		JlyParam.ShowOffCode = 0xFF;
    }
    else if(Conf.Jly.RecBootMode == 0x10)	/* 异常条件启动 */
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
        if(Conf.Jly.RecStopMode == 0){};	/* 先进先出的记录停止方式 */
        if(Conf.Jly.RecStopMode == 1)		/* 存储器记满的记录停止方式 */
        {
            if((Queue.RecorderFlashPoint >= Flash_MAX_NUM)) //&&IsReadingI2c==0,下载数据
            {
                Conf.Jly.WorkStatueIsStop = 0;	/* 停止工作 */
				
				Fram_Write(&Conf.Jly.WorkStatueIsStop,FRAM_WorkStatueIsStopAddr,1);
                IsWriteLasestStopTimeToFram = 1;
                JlyParam.ShowOffCode = 0;	/*记录仪记满停机*/
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
  * @brief  Description 记录仪1s处理标志
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
		Display_Signal(2);/*显示信号强度*/
        Display_Mem();	/*显示存储容量*/
         
    }
   /*检测外接电接入*/
	if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_Deal_ACtest) == 0)         
    {
        PManage.HaveExternalPower++;
        if(PManage.HaveExternalPower>=ExternalPowerchecktime)
        {
            Flag.Powerdowncountflag=1;
            PManage.HaveExternalPower=0;
        }
    }    
	/*检测电池电源是否充满*/
	if(Flag.ExPwOn == 1)	/*在有外接电的情况下检测是否充满电*/
	{
		if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_Deal_CHGtest) == 1)         
		{
			PManage.BatChargeFullCount++;
			if(PManage.BatChargeFullCount>=ExternalPowerchecktime)/*检测60s*/
			{
				Flag.BatChargeFull=1;
				PManage.BatChargeFullCount=0;
			}
		}
		else
		{
			Flag.BatChargeFull=0;
		}
	}
	else
	{
		Flag.BatChargeFull=0;
	}
	
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
        
        Display_SN();   /*显示SN号*/
        
        OneSec_Timedeal();
        
        
        rtc_deel();
        
		OutpowerShan();
		
        VoltageTest();
        
        WorkornotMode();
        
		SaveDataOnTimeDeal();
    }
	if(Conf.Jly.WorkStatueIsStop == 0) /*停止工作*/
    {
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
