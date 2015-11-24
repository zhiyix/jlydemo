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
  * @brief  Description ��������̵�����
  * @param  ��  		
  * @retval ��		
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
  * @brief  Description �������쳤������
  * @param  ��  		
  * @retval ��		
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
  * @brief  Description �ر���
  * @param  ��  		
  * @retval ��		
  *****************************************************************************/
static void CloseAlarm(void)
{
	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){}
	clearJINBAO;
	LCD_UpdateDisplayRequest();	// ������ȷ��� 	
	BEEP(OFF);
	//Alarmflag = Alarmflag&(~channel_BITx);
}
/******************************************************************************
  * @brief  Description ����ʱ���嶯��
  * @param  ��  		
  * @retval ��		
  *****************************************************************************/
static void AlarmAction(uint8_t channel)
{
	//����ע��
	if((Conf.Sensor[channel-1].AlarmSwitch & 0x01) && (Conf.Sensor[channel-1].AlarmSwitch & 0x02))
	{
		BellNn_longer(1);// ��ʾ��500msһ�� //buzzer_on();
		while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){}
		showJINBAO;
		LCD_UpdateDisplayRequest();
	}
	else if(Conf.Sensor[channel-1].AlarmSwitch & 0x01)	//����������
	{
		BellNn_longer(1);// ��ʾ��500msһ�� //buzzer_on();
	}
	else if(Conf.Sensor[channel-1].AlarmSwitch & 0x02) //������ʾ��
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
	/*!< ���� */
	//Alarmflag = Alarmflag | channel_BITx;	// ���ű������
}
/******************************************************************************
  * @brief  Description ��¼�Ǳ���������,��Ҫ����
���������������Իָ�,����ʱ�����������������رգ��¶Ȼָ��������ٳ���ʱ�ᱨ��
ע�⣺Conf.Sensor[channel-1].SensorAlarm_High ע�����ת��	
����������
	(1)����������
	(2)ÿ��ͨ����������
	(3)ÿ��ͨ������ʾ����
  * @param  ��  		
  * @retval ��		
  *****************************************************************************/
void AlarmDeal(uint8_t channel)
{
	if(Conf.Alarm.AlarmSwitch ==1)	// �����ܿ��� 
	{
		if(ChannelDataFloat[channel-1] >= Conf.Sensor[channel-1].SensorAlarm_High.wd)//���޳���
        {
			if(Flag.AlarmXiaoYin == 1)//����������
			{
				Flag.AlarmHuiFu[channel-1] = 1;//�����ر�
				
				JlyParam.ContinueExcessiveUpperLimitCount = 0;//���޳������ ��0
                CloseAlarm();
			}else if(Flag.AlarmHuiFu[channel-1] ==0)
			{
				JlyParam.ContinueExcessiveUpperLimitCount++;
				if(JlyParam.ContinueExcessiveUpperLimitCount >= JlyParam.ContinueExcessiveTimes)//����������
				{
					JlyParam.ContinueExcessiveUpperLimitCount = 0;//���޳������ ��0
					if(Flag.AlarmTimeDelayIsOut == 1)
					{
						Flag.AlarmTimeDelayIsOut =0;
						if(Conf.Alarm.AlarmTime_Mode & 0x01)	// �ϰ�ʱ�� 
						{
							read_time();
							if((Rtc.Minute < Conf.Alarm.OnWork_Min && Rtc.Hour < Conf.Alarm.OnWork_Hour)||(Rtc.Minute > Conf.Alarm.OffWork_Min && Rtc.Hour > Conf.Alarm.OffWork_Hour))
							{
								AlarmAction(channel);
							}else{
								CloseAlarm();
							}
						}else if(Conf.Alarm.AlarmTime_Mode & 0x02)	// �°�ʱ�� 
						{
							AlarmAction(channel);
						}

					}
				}
			} 
        }else if(ChannelDataFloat[channel-1] <= Conf.Sensor[channel-1].SensorAlarm_Low.wd){//���޳���
			if(Flag.AlarmXiaoYin == 1)
			{
				Flag.AlarmHuiFu[channel-1] = 1;//�����ر�
				
				JlyParam.ContinueExcessiveLowerLimitCount =0;//���޳������ ��0
                CloseAlarm();
			}else if(Flag.AlarmHuiFu[channel-1] ==0)
			{
				JlyParam.ContinueExcessiveLowerLimitCount++;
				if(JlyParam.ContinueExcessiveLowerLimitCount >= JlyParam.ContinueExcessiveTimes)//����������
				{
					JlyParam.ContinueExcessiveLowerLimitCount =0;//���޳������ ��0
					if(Flag.AlarmTimeDelayIsOut == 1)
					{
						Flag.AlarmTimeDelayIsOut =0;
						if(Conf.Alarm.AlarmTime_Mode & 0x01)	// �ϰ�ʱ�� 
						{
							read_time();
							if((Rtc.Minute < Conf.Alarm.OnWork_Min && Rtc.Hour < Conf.Alarm.OnWork_Hour)||(Rtc.Minute > Conf.Alarm.OffWork_Min && Rtc.Hour > Conf.Alarm.OffWork_Hour))
							{
								AlarmAction(channel);
							}else{
								CloseAlarm();
							}
						}else if(Conf.Alarm.AlarmTime_Mode & 0x02)	// �°�ʱ�� 
						{
							AlarmAction(channel);
						}
					}
				}
			}
		}else{
			Flag.AlarmXiaoYin = 0;
            Flag.AlarmHuiFu[channel-1] = 0;//�����ָ�
			
			JlyParam.ContinueExcessiveUpperLimitCount = 0;//���޳������ ��0
			JlyParam.ContinueExcessiveLowerLimitCount =0;//���޳������ ��0
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
