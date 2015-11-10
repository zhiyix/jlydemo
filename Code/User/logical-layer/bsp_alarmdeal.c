/**
  ******************************************************************************
  * @file              : bsp_alarmdeal.c
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
#ifndef __BSPALARM_C
#define __BSPALARM_C

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
  * @brief  Description 蜂鸣器响短的声音
  * @param  无  		
  * @retval 无		
  *****************************************************************************/
void BellNn(uint8_t n)
{
    while(n--)
    {
        BELL;
        if(n>0)
        {
            Delay_ms(50);
        }
    }
}
/******************************************************************************
  * @brief  Description 蜂鸣器响长的声音
  * @param  无  		
  * @retval 无		
  *****************************************************************************/
void BellNn_longer(uint8_t n)
{
    while(n--)
    {
        BELL_longer;
        if(n>0)
        {
            Delay_ms(250);
        }
    }
}

/******************************************************************************
  * @brief  Description 关报警
  * @param  无  		
  * @retval 无		
  *****************************************************************************/
static void CloseAlarm(void)
{
	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){}
	clearJINBAO;
	LCD_UpdateDisplayRequest();	// 清除喇叭符号 	
	BEEP(OFF);
	//Alarmflag = Alarmflag&(~channel_BITx);
}
/******************************************************************************
  * @brief  Description 报警时具体动作
  * @param  无  		
  * @retval 无		
  *****************************************************************************/
static void AlarmAction(uint8_t channel)
{
	//这里注意
	if((Conf.Sensor[channel-1].AlarmSwitch & 0x01) && (Conf.Sensor[channel-1].AlarmSwitch & 0x02))
	{
		BellNn_longer(1);// 表示响500ms一次 //buzzer_on();
		while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){}
		showJINBAO;
		LCD_UpdateDisplayRequest();
	}
	else if(Conf.Sensor[channel-1].AlarmSwitch & 0x01)	//报警声音打开
	{
		BellNn_longer(1);// 表示响500ms一次 //buzzer_on();
	}
	else if(Conf.Sensor[channel-1].AlarmSwitch & 0x02) //报警显示打开
	{
		while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){}
		showJINBAO;
		LCD_UpdateDisplayRequest();
	}
	else
	{
		while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){}
		clearJINBAO;
		LCD_UpdateDisplayRequest();	
		BEEP(OFF);
	}
	/*!< 短信 */
	//Alarmflag = Alarmflag | channel_BITx;	// 短信报警相关
}
/******************************************************************************
  * @brief  Description 记录仪报警处理函数,需要完善
消音按键，报警自恢复,报警时按下消音按键声音关闭，温度恢复到正常再超标时会报警
注意：Conf.Sensor[channel-1].SensorAlarm_High 注意进制转换	
开启报警：
	(1)开报警开关
	(2)每个通道声音开关
	(3)每个通道开显示开关
  * @param  无  		
  * @retval 无		
  *****************************************************************************/
void AlarmDeal(uint8_t channel)
{
	if(Conf.Alarm.AlarmSwitch ==1)	// 报警总开关 
	{
		if((ChannelDataFloat[channel-1] >= Conf.Sensor[channel-1].SensorAlarm_High.wd)||(ChannelDataFloat[channel-1] <= Conf.Sensor[channel-1].SensorAlarm_Low.wd))
        {
			if(Flag.AlarmXiaoYin == 1)
			{
				Flag.AlarmHuiFu[channel-1] = 1;//报警关闭
				
                CloseAlarm();
			}else if(Flag.AlarmHuiFu[channel-1] ==0)
			{
				if(Conf.Alarm.AlarmTime_Mode & 0x01)	// 上班时间 
				{
					read_time();
					if((Rtc.Minute < Conf.Alarm.OnWork_Min && Rtc.Hour < Conf.Alarm.OnWork_Hour)||(Rtc.Minute > Conf.Alarm.OffWork_Min && Rtc.Hour > Conf.Alarm.OffWork_Hour))
					{
						AlarmAction(channel);
					}else{
						CloseAlarm();
					}
				}else if(Conf.Alarm.AlarmTime_Mode & 0x02)	// 下班时间 
				{
					AlarmAction(channel);
				}
			} 
        }
        else
        {
			Flag.AlarmXiaoYin = 0;
            Flag.AlarmHuiFu[channel-1] = 0;//消音恢复
			
            CloseAlarm();
        }        
	}
    else
    {
        CloseAlarm();
    }   
}

#endif /* __BSPALARM_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
