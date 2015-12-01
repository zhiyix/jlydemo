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
  * @brief  Description ������ʡ��ģʽ����
						ʡ��ģʽ���ɼ������С10s
						����ģʽ���ɼ������С2s
  * @param  ��  		
  * @retval ��		
  *****************************************************************************/
static void LOWorNomal_Mode(void)
{
	uint8_t i=0;
    if((Conf.Jly.PowerMode >= 1)&&(Conf.Jly.WorkStatueIsStop >= 1)) /*�͹���ģʽ*/
    {
		
		if(Flag.StartSample==1)//����λ�ò����Ҷ����ᵼ��adc�ɼ���׼
        {
            Flag.StartSample=0;
            //Flag.EndSample=1;
			Flag.IsDisplayRightNow = 1; //��һ�β�������ʾ
			
            Dealing_Gather(Started_Channel);
			
            AVCC1_POWER(OFF);
            DoGatherChannelDataFloat(Started_Channel);
			
        }
        if(JlyParam.SampleTime == JlyParam.SampleInterval-1)//
        {
			/*�жϴ������ӿ����� ģ��/����*/
			for(i=0;i<JlyParam.ChannelNumOld;i++)
			{
				if(Conf.Sensor[i].SensorInterfaceType==0x00)	/*ģ��*/
				{
					Flag.StartSample=1;
				}
				else if(Conf.Sensor[i].SensorInterfaceType==0x01)/*����*/
				{
					
				}
			}
			if(Flag.StartSample == 1 )
			{
				AVCC1_POWER(ON);	/*�򿪴�������Դ*/
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
    else if(Conf.Jly.WorkStatueIsStop >= 1)//����ģʽ
    {
		
		if(Flag.StartSample==1)
        {
            Flag.StartSample=0;
            //Flag.EndSample=1;
                
			
			Dealing_Gather(Started_Channel);
            AVCC1_POWER(OFF);
            DoGatherChannelDataFloat(Started_Channel);
        }
        if((JlyParam.SampleTime>1) && JlyParam.SampleTime%2==0)//2s�ɼ�һ��
        {
			/*�жϴ������ӿ����� ģ��/����*/
			for(i=0;i<JlyParam.ChannelNumOld;i++)
			{
				if(Conf.Sensor[i].SensorInterfaceType==0x00)	/*ģ��*/
				{
					Flag.StartSample=1;
				}
				else if(Conf.Sensor[i].SensorInterfaceType==0x01)/*����*/
				{
					
				}
			}
			if(Flag.StartSample == 1 )
			{
				AVCC1_POWER(ON);	/*�򿪴�������Դ*/
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
        JlyParam.SampleTime = JlyParam.SampleInterval;//�ɼ�ʱ�� ��λ:s
    }
}
/******************************************************************************
  * @brief  Description ��¼�ǹ���״̬����
  * @param  ��  		
  * @retval ��		
  *****************************************************************************/
static void WorkornotMode(void)
{
    if(Conf.Jly.WorkStatueIsStop < 1)//ֹͣ����
    {
		OffPowerSupply();//���豸��Դ
		
        if(JlyParam.FramErrorCode!=0)
        {
            displayErr(JlyParam.FramErrorCode);
		}else{
			lcd_OFF(JlyParam.ShowOffCode);
		}
        if(Flag.MucReset==1)
        {
            Flag.MucReset=0;
            
        }
        return;
    }
    else
    {
		//��ʵ��ͨ���� >0 ʱ�������� �洢 ��ʾ
        if(JlyParam.ChannelNumActual >0)
		{
			
			LOWorNomal_Mode();
			
			//SaveDataOnTimeDeal();
	//        if((Flag.buttonS2flag==0)&&(Flag.buttonS3flag==0)&&(Flag.buttonS4flag==0))
	//        {
				Display_ChannelValue(StartedChannelForDisplay);  //LCD 
	//        }
			
			
			Flag.IsDisplayRightNow=1;
        }
    } 
}


/******************************************************************************
  * @brief  Description ��¼��1s�����־
  * @param  ��  		
  * @retval ��		
  *****************************************************************************/
static void OneSec_Timedeal(void)
{
    
    //LCD��ʾ����
    display_ct++;          
    if(display_ct>=36)//----------��ʱ80ms,������ʾ��λ��
    {
		
        display_ct = 0;
		   
        Display_Mem();	  //��ʾ�洢���� 
		Display_Signal(2);/*��ʾ�ź�ǿ��*/
		//LED1(ON);//LED1(OFF);
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
	}else{
		Key1ChangAnCount = 0;
	}
	
	
	/*****************************************************************/
   /*�����ӵ����*/
	if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_ACtest_PIN) == 0)         
    {
        PManage.HaveExternalPower++;
        if(PManage.HaveExternalPower >= ExternalPowerchecktime)
        {
            Flag.Powerdowncountflag=1;
            PManage.HaveExternalPower=0;
        }
    }    
	/*****************************************************************
	 *����ӵ�����
	 *(1)��﮵�أ�����������
	 *(2)δ��﮵��
	 *****************************************************************/
	if(Flag.ExPwOn == 1)	
	{
		if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_CHGtest_PIN) == 1)         
		{
			PManage.BatChargeFullCount++;
			
			if(PManage.BatChargeFullCount >= ExternalPowerchecktime)/*���60s*/
			{
				PManage.BatChargeFullCount=0;
				Flag.BatChargeFull=1;/*����ӵ磬��س�����־*/
				
				Flag.BatCharging = 0;/*����ӵ�δ�ӵ�أ����δ���*/
			}			
		}
		else
		{
			Flag.BatChargeFull=0;
			Flag.BatCharging = 1;/*����ӵ磬������ڳ����*/
		}
	}
	else
	{
		Flag.BatChargeFull=0;
		Flag.BatCharging = 0;/*��ӵ�δ��*/
	}
	
}

/******************************************************************************
  * @brief  Description ��¼�Ǵ�����
  * @param  ��  		
  * @retval ��		
  *****************************************************************************/
void JlySecDeal(void)
{
    if(Flag.Sec == 1)   //1��
    {
        Flag.Sec = 0;
		
		//LED1(ON);
        OneSec_Timedeal();
        
        rtc_deel();
        
		OutpowerShan();
		
        VoltageTest();
        
		RecorderBootModeHandle();
		
        WorkornotMode();
		
		StorageHistoryData();
		
		//LED1(OFF);
		//----------------------����
//		read_time();
    }
	if(Conf.Jly.WorkStatueIsStop < 1) /*ֹͣ����*/
    {
		OffPowerSupply();//���豸��Դ
		
        if(JlyParam.FramErrorCode!=0)
        {
            displayErr(JlyParam.FramErrorCode);
        }else{
			lcd_OFF(JlyParam.ShowOffCode);
		}
    }
}
#endif /* __BSPDATADEAL_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
