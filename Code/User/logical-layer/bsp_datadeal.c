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
    if((Conf.Jly.PowerMode == 1)&&(Conf.Jly.WorkStatueIsStop >= 1)) /*�͹���ģʽ*/
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
			/*�жϴ������ӿ����� ģ��/����*/
			for(i=0;i<Conf.Jly.ChannelNum;i++)
			{
				if(Conf.Sensor[i].SensorInterfaceType==0x00)	/*ģ��*/
				{
					Flag.StartSample=1;
					AVCC1_POWER(ON);	/*�򿪴�������Դ*/
				}
				else if(Conf.Sensor[i].SensorInterfaceType==0x01)/*����*/
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
    else if(Conf.Jly.WorkStatueIsStop >= 1)//����ģʽ
    {
        if(Flag.StartSample==1)
        {
            Flag.StartSample=0;
            Flag.EndSample=1;
                
			
			Dealing_Gather(Started_Channel);
            AVCC1_POWER(OFF);
            DoGatherChannelDataFloat(Started_Channel);
        }
        if((JlyParam.SampleTime>1) && JlyParam.SampleTime%2==0)//2s�ɼ�һ��
        {
			/*�жϴ������ӿ����� ģ��/����*/
			for(i=0;i<Conf.Jly.ChannelNum;i++)
			{
				if(Conf.Sensor[i].SensorInterfaceType==0x00)	/*ģ��*/
				{
					Flag.StartSample=1;
					AVCC1_POWER(ON);	/*�򿪴�������Դ*/
				}
				else if(Conf.Sensor[i].SensorInterfaceType==0x01)/*����*/
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
    if(Conf.Jly.WorkStatueIsStop <1)//ֹͣ����
    {
		/*�ص������Դ*/
		AVCC1_POWER(OFF);	/* �ش�������Դ */
        BEEP(OFF);			/* �ط����� */
		AlarmLed2_OFF;		/* �ر����� */
		
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
        Display_Signal(2);/*��ʾ�ź�ǿ��*/
        Display_Mem();	  /*��ʾ�洢����*/
//         SaveData();
        Flag.IsDisplayRightNow=1;
        
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
    if(display_ct>=36)
    {
        display_ct = 0;
         
    }
   /*�����ӵ����*/
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
	 *����ӵ�����
	 *(1)��﮵�أ�����������
	 *(2)δ��﮵��
	 *****************************************************************/
	if(Flag.ExPwOn == 1)	
	{
		if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_Deal_CHGtest) == 1)         
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
  * @brief  Description ��¼��
  * @param  ��  		
  * @retval ��		
  *****************************************************************************/
void SaveDataOnTimeDeal(void)
{
    
    read_time();
    
    RtcBcdToD10(&Rtc);
    
    Rtc.TMPS=DateToSeconds(&Rtc);
    
    RtcD10ToBcd(&Rtc);
    
	if((JlyParam.NormalRecInterval > 0) && (JlyParam.NormalRecInterval < 60))//1s-60s�����ݼ�¼
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
		Rtc.TMPS = Rtc.TMPS/60;	//����
		if(Rtc.TMPS % JlyParam.NormalRecIntervalMin ==0)//1����-
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
  * @brief  Description ��¼�Ǵ�����
  * @param  ��  		
  * @retval ��		
  *****************************************************************************/
void JlySecDeal(void)
{
    if(Flag.Sec == 1)   //1��
    {
        Flag.Sec = 0;
        
        Display_SN();   /*��ʾSN��*/
        
        OneSec_Timedeal();
        
        
        rtc_deel();
        
		OutpowerShan();
		
        VoltageTest();
        
		RecorderBootModeHandle();
		RecorderStopModeHandle();
		
        WorkornotMode();
        
		SaveDataOnTimeDeal();
    }
	if(Conf.Jly.WorkStatueIsStop == 0) /*ֹͣ����*/
    {
		/*�ص������Դ*/
        AVCC1_POWER(OFF);	/* �ش�������Դ */
        BEEP(OFF);			/* �ط����� */
		AlarmLed2_OFF;		/* �ر����� */
		
        lcd_OFF(JlyParam.ShowOffCode);

        if(JlyParam.LastErrorCode!=0)
        {
            displayErr(JlyParam.LastErrorCode);
        }
    }
}
#endif /* __BSPDATADEAL_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
