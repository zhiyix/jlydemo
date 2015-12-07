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
    if((Conf.Jly.PowerMode >= 1)&&(Conf.Jly.WorkStatueIsStop >= 1)) /*低功耗模式*/
    {
		//开启adc电压后(AVCC1_POWER(ON)),由于电压不稳没有立即去采集，而是利用程序执行延时后在这个地方开启采集
		if(Flag.StartSample==1)
        {
            Flag.StartSample=0;
            //Flag.EndSample=1;
			Flag.IsDisplayRightNow = 1; //第一次采样完显示
			
            Dealing_Gather(Started_Channel);
			
            AVCC1_POWER(OFF);
            DoGatherChannelDataFloat(Started_Channel);
			
        }
        if(JlyParam.SampleTime == JlyParam.SampleInterval-1)//
        {
			/*判断传感器接口类型 模拟/数字*/
			for(i=0;i<JlyParam.ChannelNumOld;i++)
			{
				if(Conf.Sensor[i].SensorInterfaceType==0x00)	/*模拟*/
				{
					Flag.StartSample=1;
				}
				else if(Conf.Sensor[i].SensorInterfaceType==0x01)/*数字*/
				{
					
				}
			}
			if(Flag.StartSample == 1 )
			{
				AVCC1_POWER(ON);	/*打开传感器电源*/
			}
        }
		
//        if(Flag.EndSample==1)
//        {
//            Flag.EndSample=0;
//            
//            if(Flag.MucReset==1)//???? MCU???,???????????
//            {
//                Flag.MucReset=0;
//            }
//        }
    }//END 
    else if(Conf.Jly.WorkStatueIsStop >= 1)//正常模式
    {
		
		if(Flag.StartSample==1)
        {
            Flag.StartSample=0;
            //Flag.EndSample=1;
                
			
			Dealing_Gather(Started_Channel);
            AVCC1_POWER(OFF);
            DoGatherChannelDataFloat(Started_Channel);
        }
        if((JlyParam.SampleTime>1) && JlyParam.SampleTime%2==0)//2s采集一次
        {
			/*判断传感器接口类型 模拟/数字*/
			for(i=0;i<JlyParam.ChannelNumOld;i++)
			{
				if(Conf.Sensor[i].SensorInterfaceType==0x00)	/*模拟*/
				{
					Flag.StartSample=1;
				}
				else if(Conf.Sensor[i].SensorInterfaceType==0x01)/*数字*/
				{
					
				}
			}
			if(Flag.StartSample == 1 )
			{
				AVCC1_POWER(ON);	/*打开传感器电源*/
			}
        }
		
//        if(Flag.EndSample==1)
//        {
//            Flag.EndSample=0;
//            
//            if(Flag.MucReset==1)
//            {
//                Flag.MucReset=0;
//            }
//        }
    }
	
	if((--JlyParam.SampleTime)<=0)
    {
        JlyParam.SampleTime = JlyParam.SampleInterval;//采集时间 单位:s
    }
}
/******************************************************************************
  * @brief  Description 记录仪数据采集、数据显示、通道报警处理
  * @param  无  		
  * @retval 无		
  *****************************************************************************/
static void WorkornotMode(void)
{
    if(Conf.Jly.WorkStatueIsStop >= 1)
    {

		//当实际通道数 >0 时开启采样 存储 显示
        if(JlyParam.ChannelNumActual >0)
		{
			LOWorNomal_Mode();
			
	//        if((Flag.buttonS2flag==0)&&(Flag.buttonS3flag==0)&&(Flag.buttonS4flag==0))
	//        {
				Display_ChannelValue(StartedChannelForDisplay);  //LCD 
	//        }
			
			Flag.IsDisplayRightNow=1;
			Flag.StopRecording = 0;	//停止记录标志清0
        }
		
    }else{
		
		if(Flag.StopRecording == 0)
		{
			Flag.StopRecording = 1;	//停止记录
			
			OffPowerSupply();//关设备电源
			if(JlyParam.FramErrorCode!=0)
			{
				/*!< Wait Until the last LCD RAM update finish */
				while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET); 
				displayErr(JlyParam.FramErrorCode);
				/*!< Requesy LCD RAM update */
				LCD_UpdateDisplayRequest();  
			}else{
				lcd_OFF(JlyParam.ShowOffCode);
			}
		}
	}
}


/******************************************************************************
  * @brief  Description 记录仪 1s处理变量/标志
  * @param  无  		
  * @retval 无		
  *****************************************************************************/
static void OneSecTimedeal(void)
{
    
    //LCD显示处理
    display_ct++;          
    if(display_ct >= 36)//----------耗时80ms,调整显示的位置
    {
        display_ct = 0;
		   
        Display_Mem();	  //36s刷新 存储容量 显示
		//Display_Signal(2);/*显示信号强度*/
		
    }
	/*****************************************************************/
	/*机械按键 key1短按长按检测*/
	//短按1s点亮lcd背光，消除报警声音
	//长按5s开关机
	/*
	if((GPIO_ReadInputDataBit(Key_PORT,Key1_PIN) == 0)&&(Flag.Key1AnXia == 1))
	{//判断是否有键按下
		if(++Key.DuanAnCount >=1)//1s计数消抖
		{
			Key.DuanAnCount = 0;
			
			if(++Key.ChangAnCount >=3)//3s长按识别
			{
				 Flag.Keyflag = 0;//短按无效
				 Key.ChangAnCount = 3;//锁定长按
				 switch(Key.KeyNum)
				 {
					  case 1:Flag.Key1ChangAn = 1;BellNn(1);rtc_deel();break;
					  //case 2:Flag.key2anxia  = 1;break;
					  //case 3:Flag.key3anxia  = 1;break;
					  //case 4:key_ret = 8;break;
				 } 
			}
			else//短按有效
			{
				 Flag.Keyflag = 1;//短按标志位
				 if(GPIO_ReadInputDataBit(Key_PORT,Key1_PIN) == 0)Key.KeyNum = 1;//以下用于识别按键
				 //if((~KeyIn)&Key2)key_num = 2;
                 //if((~KeyIn)&Key3)key_num = 3;
			}  
		} 
	}
	else//松手之后
	{
		Key.ChangAnCount = 0;
		if(Flag.Keyflag)//松手
		{
			Flag.Keyflag = 0;
			switch(Key.KeyNum)
			{
				case 1: Flag.Key1DuanAn = 1;
						while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){}
						clearJINBAO;
						LCD_UpdateDisplayRequest();	// 清除喇叭符号	
						BEEP(OFF);
						LcdBackLight(ON);
						break;
				//case 2:Flag.key2anxia  = 1;break;
				//case 3:Flag.key3anxia  = 1;break;
				//case 4:key_ret = 4;break;    
			}
		 }
	}
	
	if(Flag.Key1DuanAn == 1)
	{
		Flag.Key1DuanAn = 0;
		
		Flag.AlarmXiaoYin = 1;	//按键消音标志
		Flag.LcdBackLightOn = 1; //Lcd背光点亮
		
	}
	if(Flag.Key1ChangAn == 1)
	{
		Flag.Key1ChangAn = 0;
		
		Conf.Jly.RecBootMode = 0x03;// 机械按键手动启动
	}
	*/
	/*****************************************************************/
	//Lcd背光
	if(Flag.LcdBackLightOn == 1)
	{
		JlyParam.LcdBackLightCount++;
		if(JlyParam.LcdBackLightCount >= LcdBackLightTime)
		{
			Flag.LcdBackLightOn = 0;
			JlyParam.LcdBackLightCount = 0;
			LcdBackLight(OFF);
		}
	}
	//机械按键长按
	if((GPIO_ReadInputDataBit(Key_PORT,Key1_PIN) == 0)&&(Flag.Key1AnXia == 1))
	{
		Delay_ms(10);//10ms 延时消抖
		if(GPIO_ReadInputDataBit(Key_PORT,Key1_PIN) == 0)
		{
			Key1ChangAnCount++;
			if(Key1ChangAnCount >= 3)
			{
				Key1ChangAnCount = 0;
				Flag.Key1AnXia = 0;
				
				Conf.Jly.RecBootMode = 0x03;// 机械按键手动启动
				BellNn(1);
				rtc_deel();
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

        OneSecTimedeal();

        rtc_deel();

		ExternalPowerDetection();

        BatteryVoltageDetection();//BatteryVoltageDetection
    
		RecorderBootModeHandle();

        WorkornotMode();

		StorageHistoryData();
		
    }
	
	if(Conf.Jly.WorkStatueIsStop < 1) /*停止工作*/
    {	
		if(Flag.StopRecording == 0)
		{
			Flag.StopRecording = 1;	//停止记录,只执行一次
			
			RecorderBootModeHandle();
			OffPowerSupply();//关设备电源
			if(JlyParam.FramErrorCode!=0)
			{
				/*!< Wait Until the last LCD RAM update finish */
				while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET); 
				displayErr(JlyParam.FramErrorCode);
				/*!< Requesy LCD RAM update */
				LCD_UpdateDisplayRequest();  
			}else{
				lcd_OFF(JlyParam.ShowOffCode);
			}
		}
    }
}
#endif /* __BSPDATADEAL_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
