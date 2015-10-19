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
/**
  * @brief  Description ��������̵�����
  * @param  ��  		
  * @retval ��		
  */
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
/**
  * @brief  Description �������쳤������
  * @param  ��  		
  * @retval ��		
  */
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
/**
  * @brief  Description ��¼�Ǳ���������
  * @param  ��  		
  * @retval ��		
  */
void AlarmDeal(uint8_t channel)
{
    uint8_t channel_BITx;
    
    channel_BITx = 1<<(channel-1);
	if(Conf.Alarm.AlarmSwitch ==1)	/*!< �����ܿ��� */
	{
		if((ChannelDataFloat[channel-1] >= Conf.Sensor[channel-1].SensorAlarm_High.ft)||(ChannelDataFloat[channel-1] <= Conf.Sensor[channel-1].SensorAlarm_Low.ft))
        {
			if(Conf.Alarm.AlarmTime_Mode & 0x01)	/*!< �ϰ�ʱ�� */
			{
			
			}
			if(Conf.Alarm.AlarmTime_Mode & 0x02)	/*!< �°�ʱ�� */
			{
			
			}
			if(Conf.Sensor[channel-1].AlarmSwitch & 0x01)
			{
				while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){}; 
				showJINBAO;
				LCD_UpdateDisplayRequest();  	/*!< ��ʾ���ȷ��� */
			}
			else{
				while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){};
				clearJINBAO;
				LCD_UpdateDisplayRequest();	/*!< ������ȷ��� */	
			}
			if(Conf.Sensor[channel-1].AlarmSwitch & 0x02)
				BellNn_longer(1);/*!< ��ʾ��500msһ�� //buzzer_on();*/
			else
				BEEP(OFF);
			/*!< ���� */
            //Alarmflag = Alarmflag | channel_BITx;	/*!< ���ű������ */
            
        }
        else
        {
            while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){};
			clearJINBAO;
			LCD_UpdateDisplayRequest();	/*!< ������ȷ��� */	
            BEEP(OFF);
            //Alarmflag = Alarmflag&(~channel_BITx);
        }        
	}
    else
    {
        while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){};
		clearJINBAO;
		LCD_UpdateDisplayRequest();	/*!< ������ȷ��� */	
        BEEP(OFF);
        //Alarmflag = Alarmflag&(~channel_BITx);//????????
    }
    
}

#endif /* __BSPALARM_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
