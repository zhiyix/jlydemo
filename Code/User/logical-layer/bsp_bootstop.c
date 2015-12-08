/**
  ******************************************************************************
  * @file              : bsp_bootstop.c
  * @author            : 
  * @version           : 
  * @date              : 
  * @brief             : 记录仪启动方式,停止方式处理 c file
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
#ifndef __BSPBOOTSTOP_C
#define __BSPBOOTSTOP_C

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
	
	按键停止记录后，如果仪器重启则按重启前的启动方式启动
  *****************************************************************************/
void RecorderBootModeHandle(void)
{
    if(JlyParam.FramErrorCode != 0)
    {
        if(Conf.Jly.WorkStatueIsStop == 1)	/* 工作状态 */
        {
            
            Conf.Jly.WorkStatueIsStop = 0;	/*停止工作*/
            Fram_Write(&Conf.Jly.WorkStatueIsStop,FRAM_WorkStatueIsStopAddr,1);
			
            JlyParam.ShowOffCode = 0x09;	/*出现故障*/
			Flag.StopRecording = 0;	//停止记录标志清0
        }
        return;
    }
    if(Conf.Jly.RecBootMode == 0x00)	/* 延时启动(默认延时时间0，即立即启动)*/
    {
		if((JlyParam.delay_start_time--) <= 0 && (!Conf.Jly.WorkStatueIsStop))	//&& (!Conf.Jly.WorkStatueIsStop)
        {
            JlyParam.delay_start_time = -1;
            Conf.Jly.WorkStatueIsStop = 1;	/*开启工作*/
			Conf.Jly.RecBootMode = 0xFF;	//当前记录方式启动后，不会再执行当前程序，提高代码效率
        }
        if((JlyParam.delay_start_time)>0)
        {
            Conf.Jly.WorkStatueIsStop = 0;	/* 停止工作 */
			
            JlyParam.ShowOffCode = 0x05;	/*表示延时启动方式，还未开始记录*/
        }
    }
	else if(Conf.Jly.RecBootMode == 0x01)	/* 时间点定时启动 */
	{
		
		/*读取时钟时间*/
		read_time();
		if((Rtc.Year == Conf.Jly.MixBoot_Year)&&(Rtc.Month == Conf.Jly.MixBoot_Month)&&(Rtc.Day == Conf.Jly.MixBoot_Day)\
			&&(Rtc.Hour == Conf.Jly.MixBoot_Hour)&&(Rtc.Minute == Conf.Jly.MixBoot_Min))
		{
			Conf.Jly.WorkStatueIsStop = 1;	/*到时间点开启工作*/	
			Conf.Jly.RecBootMode = 0xFF;	//当前记录方式启动后，不会再执行当前程序，提高代码效率
		}else{
			Conf.Jly.WorkStatueIsStop = 0;	/* 停止工作 */
			JlyParam.ShowOffCode = 0x03;	/*表示定时启动方式，还未开始记录*/
		}
	}
	else if(Conf.Jly.RecBootMode == 0x02)	/* 时间点定点启停 */
	{
		if(Flag.RecTimeDingDianBoot == 0) /* 时间点定点启动 */
		{
			/*读取时钟时间*/
			read_time();
			if((Rtc.Year == Conf.Jly.MixBoot_Year)&&(Rtc.Month == Conf.Jly.MixBoot_Month)&&(Rtc.Day == Conf.Jly.MixBoot_Day)\
				&&(Rtc.Hour == Conf.Jly.MixBoot_Hour)&&(Rtc.Minute == Conf.Jly.MixBoot_Min))
			{
				Conf.Jly.WorkStatueIsStop = 1;	/*到时间点开启工作*/
				Flag.RecTimeDingDianBoot = 1;	/* 时间点定点已启动 */
			}else{
				Conf.Jly.WorkStatueIsStop = 0;	/* 停止工作 */
				JlyParam.ShowOffCode = 0x01;	/*表示定点启动方式，还未开始记录*/
				Flag.RecTimeDingDianBoot = 0;
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
				
				Flag.RecTimeDingDianBoot = 0;
				Conf.Jly.RecBootMode = 0xFF;	//当前记录方式启动停止后，不会再执行当前程序，提高代码效率
			}else{
				
				Conf.Jly.WorkStatueIsStop = 1;	
			}
		}
	}
	else if(Conf.Jly.RecBootMode == 0x03 )	/* 机械按键手动启动*/
    {
		if(Conf.Jly.WorkStatueIsStop >= 1)//由工作到停止
		{
			Conf.Jly.WorkStatueIsStop = 0;	/* 停止工作 */
			Conf.Jly.RecBootMode = 0xFF;
            //写入fram
			Fram_Write(&Conf.Jly.WorkStatueIsStop,FRAM_WorkStatueIsStopAddr,1);
			JlyParam.ShowOffCode = 0xFF;
			Flag.StopRecording = 0;	//停止记录标志清0  这样的话在别的启动方式未生效前任可修改到别的记录方式
		}else{//由停止到工作
			Conf.Jly.WorkStatueIsStop = 1;
			Conf.Jly.RecBootMode = 0xFF;
			Flag.StopRecording = 0;	//停止记录标志清0  这样的话在别的启动方式未生效前任可修改到别的记录方式
			Fram_Write(&Conf.Jly.WorkStatueIsStop,FRAM_WorkStatueIsStopAddr,1);
		}
    }
    else if(Conf.Jly.RecBootMode == 0x10)	/* 异常条件启动 */
    {
        
    }
}
/******************************************************************************
  * @brief  Description 记录仪停止方式处理
  * @param  无  		
  * @retval 无		
  *****************************************************************************/
void RecorderStopModeHandle(void)
{
    uint8_t  IsWriteLasestStopTimeToFram;
    IsWriteLasestStopTimeToFram = 0;
    
    if(Conf.Jly.WorkStatueIsStop)
    {
        if(Conf.Jly.RecStopMode == 0){};	/* 先进先出的记录停止方式 */
        if(Conf.Jly.RecStopMode == 1)		/* 存储器记满的记录停止方式 */
        {
            if((Queue.WriteFlashDataPointer >= Queue.FlashRecActualStorage)) //&&IsReadingI2c==0,下载数据
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

#endif /* __BSPBOOTSTOP_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
