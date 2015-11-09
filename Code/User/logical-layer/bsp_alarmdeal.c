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
  * @brief  Description 记录仪报警处理函数,需要完善
  * @param  无  		
  * @retval 无		
  *****************************************************************************/
void AlarmDeal(uint8_t channel)
{
	float temp1;
	float temp2;
	if(Conf.Alarm.AlarmSwitch ==1)	/*!< 报警总开关 */
	{
		temp1 = Conf.Sensor[channel-1].SensorAlarm_High.ft;
		temp2 = Conf.Sensor[channel-1].SensorAlarm_Low.ft;
		if((ChannelDataFloat[channel-1] >= Conf.Sensor[channel-1].SensorAlarm_High.ft)||(ChannelDataFloat[channel-1] <= Conf.Sensor[channel-1].SensorAlarm_Low.ft))
        {
			if(Flag.AlarmXiaoYin == 1)
			{
				Flag.AlarmHuiFu[channel-1] = 1;
				
                while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){}
				clearJINBAO;
				LCD_UpdateDisplayRequest();	/*!< 清除喇叭符号 */	
				BEEP(OFF);
                //Alarmflag = Alarmflag&(~channel_BITx);
			}else if(Flag.AlarmHuiFu[channel-1] ==0)
			{
				if(Conf.Alarm.AlarmTime_Mode & 0x01)	/*!< 上班时间 */
				{
				
				}
				if(Conf.Alarm.AlarmTime_Mode & 0x02)	/*!< 下班时间 */
				{
				
				}
				if(Conf.Sensor[channel-1].AlarmSwitch & 0x01)	/*报警声音打开*/
				{
					BellNn_longer(1);/*!< 表示响500ms一次 //buzzer_on();*/
				}
				else{
					BEEP(OFF);
				}
				if(Conf.Sensor[channel-1].AlarmSwitch & 0x02) /*报警显示打开*/
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
				}
				/*!< 短信 */
				//Alarmflag = Alarmflag | channel_BITx;	/*!< 短信报警相关 */
			} 
        }
        else
        {
			Flag.AlarmXiaoYin=0;
            Flag.AlarmHuiFu[channel-1]=0;
			
            while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){}
			clearJINBAO;
			LCD_UpdateDisplayRequest();	/*!< 清除喇叭符号 */	
            BEEP(OFF);
            //Alarmflag = Alarmflag&(~channel_BITx);
        }        
	}
    else
    {
        while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){}
		clearJINBAO;
		LCD_UpdateDisplayRequest();	/*!< 清除喇叭符号 */	
        BEEP(OFF);
        //Alarmflag = Alarmflag&(~channel_BITx);
    }
    
}

#endif /* __BSPALARM_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
