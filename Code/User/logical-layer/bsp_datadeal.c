/**
  ******************************************************************************
  * @file              : bsp_datadeal.c
  * @author            : 
  * @version           : 
  * @date              : 
  * @brief             : ���ݴ����ӳ���
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
  * @brief  Description 
  * @param  ��  		
  * @retval ��		
  *****************************************************************************/
static void LOWorNomalMode(void)
{
	if(Conf.Jly.PowerMode >= 1) /*�͹���ģʽ*/
    {
		JlySensor.bAdcComplete = FALSE;
		
		if(Flag.StartSample ==1)//��ʱ�����ɼ���־
        {
			JlySensor.bAdcComplete = SensorDataSampleDealAndBatteryVoltageDeal();//���������ݲɼ�
			
			if(JlySensor.bAdcComplete)
			{
				Flag.StartSample =0;//�ɼ���־����
				
				Flag.IsDisplayRightNow = 1;//adc���ݲɼ��� ������ʾ
			}
		}
		
	}
}
/******************************************************************************
  * @brief  Description ��¼��ֹͣ��¼��Ĵ���
  * @param  ��  		
  * @retval ��		
  *****************************************************************************/
static void JlyOffDeal(void)
{
	if(Flag.StopRecording == 0)
	{
		Flag.StopRecording = 1;	//ֹͣ��¼,ִֻ��һ��
		
		RecorderBootModeHandle();
		OffPowerSupply();//���豸��Դ
		if(JlyParam.FramErrorCode!=0)
		{
			/*!< Wait Until the last LCD RAM update finish */
			while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET); 
			displayErr(JlyParam.FramErrorCode);
			/*!< Requesy LCD RAM update */
			LCD_UpdateDisplayRequest();  
			
		}else
		{
			lcd_OFF(JlyParam.ShowOffCode);
		}
	}
}
/******************************************************************************
  * @brief  Description ��¼�����ݲɼ���������ʾ��ͨ����������
  * @param  ��  		
  * @retval ��		
  *****************************************************************************/
static void WorkOrNotMode(void)
{
    if((Conf.Jly.WorkStatueIsOrNotStop >= 1)&&(JlyParam.ChannelNumActual >0))
    {
		//��ʵ��ͨ���� >0 ʱ�������� 
        
		LOWorNomalMode();

		Flag.StopRecording = 0;	//ֹͣ��¼��־��0
		
    }else
	{
		
		JlyOffDeal();
	}
	
}


/******************************************************************************
  * @brief  Description ��¼�� 1s�������/��־
  * @param  ��  		
  * @retval ��		
  *****************************************************************************/
static void OneSecTimedeal(void)
{
    
    //LCD��ʾ����
    display_ct++;          
    if(display_ct >= 36)//
    {
        display_ct = 0;
		   
        Display_Mem();	  //36sˢ�� �洢���� ��ʾ
		//Display_Signal(2);/*��ʾ�ź�ǿ��*/
		
    }
	/*****************************************************************/
	/*��е���� key1�̰��������*/
	//�̰�1s����lcd���⣬������������
	//����5s���ػ�
	/*
	if((GPIO_ReadInputDataBit(Key_PORT,Key1_PIN) == 0)&&(Flag.Key1AnXia == 1))
	{//�ж��Ƿ��м�����
		if(++Key.DuanAnCount >=1)//1s��������
		{
			Key.DuanAnCount = 0;
			
			if(++Key.ChangAnCount >=3)//3s����ʶ��
			{
				 Flag.Keyflag = 0;//�̰���Ч
				 Key.ChangAnCount = 3;//��������
				 switch(Key.KeyNum)
				 {
					  case 1:Flag.Key1ChangAn = 1;BellNn(1);rtc_deel();break;
					  //case 2:Flag.key2anxia  = 1;break;
					  //case 3:Flag.key3anxia  = 1;break;
					  //case 4:key_ret = 8;break;
				 } 
			}
			else//�̰���Ч
			{
				 Flag.Keyflag = 1;//�̰���־λ
				 if(GPIO_ReadInputDataBit(Key_PORT,Key1_PIN) == 0)Key.KeyNum = 1;//��������ʶ�𰴼�
				 //if((~KeyIn)&Key2)key_num = 2;
                 //if((~KeyIn)&Key3)key_num = 3;
			}  
		} 
	}
	else//����֮��
	{
		Key.ChangAnCount = 0;
		if(Flag.Keyflag)//����
		{
			Flag.Keyflag = 0;
			switch(Key.KeyNum)
			{
				case 1: Flag.Key1DuanAn = 1;
						while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){}
						clearJINBAO;
						LCD_UpdateDisplayRequest();	// ������ȷ���	
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
		
		Flag.AlarmXiaoYin = 1;	//����������־
		Flag.LcdBackLightOn = 1; //Lcd�������
		
	}
	if(Flag.Key1ChangAn == 1)
	{
		Flag.Key1ChangAn = 0;
		
		Conf.Jly.RecBootMode = 0x03;// ��е�����ֶ�����
	}
	*/
	/*****************************************************************/
	//Lcd����
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
	//��е��������
	if((GPIO_ReadInputDataBit(Key_PORT,Key1_PIN) == 0)&&(Flag.Key1AnXia == 1))
	{
		Delay_ms(10);//10ms ��ʱ����
		if(GPIO_ReadInputDataBit(Key_PORT,Key1_PIN) == 0)
		{
			Key1ChangAnCount++;
			if(Key1ChangAnCount >= 3)
			{
				Key1ChangAnCount = 0;
				Flag.Key1AnXia = 0;
				
				Conf.Jly.RecBootMode = 0x03;// ��е�����ֶ�����
				BellNn(1);
				rtc_deel();
			}
		}
	}
	
}

/******************************************************************************
  * @brief  Description ��¼�����崦����
  * @param  ��  		
  * @retval ��		
  *****************************************************************************/
void JlySecDeal(void)
{
    if(Flag.Sec == 1)   //1��
    {
        Flag.Sec = 0;

        OneSecTimedeal();

        rtc_deel();

		ExternalPowerDetection();
		
		RecorderBootModeHandle();

		Display_ChannelValue(StartedChannelForDisplay);
		
		//����
		//Lcd_ChannelValue(3,25.5);
		
    }
	
	WorkOrNotMode();
	
	StorageHistoryData();
	
	if(Conf.Jly.WorkStatueIsOrNotStop < 1) /*ֹͣ����*/
    {	
		JlyOffDeal();
    }
	
}
#endif /* __BSPDATADEAL_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
