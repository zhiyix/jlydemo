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
/**
  * @brief  Description ��¼�ǹ���״̬����
  * @param  ��  		
  * @retval ��		
  */
static void WorkornotMode(void)
{
    if(Conf.Jly.WorkStatueIsStop <1)//ֹͣ����
    {
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
        
//         SaveData();
        Flag.IsDisplayRightNow=1;
        
    } 
}
/**
  * @brief  Description ��¼��������ʽ����
  * @param  ��  		
  * @retval ��		
  * ˵����
		 0FF:����(�ֶ�)ͣ��״̬
	    00FF:��¼�Ǽ���ͣ��
		10FF:����������ʽ����δ��ʼ��¼
		20FF:����ֹͣ��ʽ���ѵ�ʱͣ��
		30FF:��ʱ������ʽ����δ��ʼ��¼
		40FF:
		50FF:��ʱ������ʽ����δ��ʼ��¼
		90FF:���ֹ���ͣ��
	�ֶ�������λ�����úͰ�����������
  */
void RecorderBootModeHandle(void)
{
    if(JlyParam.LastErrorCode != 0)
    {
        if(Conf.Jly.WorkStatueIsStop == 1)	/* ����״̬ */
        {
            
            Conf.Jly.WorkStatueIsStop = 0;	/*ֹͣ����*/
            Fram_Write(&Conf.Jly.WorkStatueIsStop,FRAM_WorkStatueIsStopAddr,1);
			
            JlyParam.ShowOffCode = 0x09;	/*���ֹ���*/
        }
        return;
    }
    if(Conf.Jly.RecBootMode == 0x00)	/* ��ʱ����(Ĭ����ʱʱ��0������������)*/
    {
		if(((JlyParam.delay_start_time--) <= 0) && (!Conf.Jly.WorkStatueIsStop))	
        {
            JlyParam.delay_start_time = -1;
            Conf.Jly.WorkStatueIsStop = 1;	/*��������*/
        }
        if((JlyParam.delay_start_time)>0)
        {
            Conf.Jly.WorkStatueIsStop = 0;	/* ֹͣ���� */
			
            JlyParam.ShowOffCode = 0x05;	/*��ʾ��ʱ������ʽ����δ��ʼ��¼*/
        }
    }
	else if(Conf.Jly.RecBootMode == 0x01)	/* ʱ��㶨ʱ���� */
	{
		if(!Conf.Jly.WorkStatueIsStop)	/* ֹͣ���� */
		{
			/*��ȡʱ��ʱ��*/
			read_time();
			if((Rtc.Year == Conf.Jly.MixBoot_Year)&&(Rtc.Month == Conf.Jly.MixBoot_Month)&&(Rtc.Day == Conf.Jly.MixBoot_Day)\
				&&(Rtc.Hour == Conf.Jly.MixBoot_Hour)&&(Rtc.Minute == Conf.Jly.MixBoot_Min))
			{
				Conf.Jly.WorkStatueIsStop = 1;	/*��ʱ��㿪������*/				
			}
			else
			{
				Conf.Jly.WorkStatueIsStop = 0;	/* ֹͣ���� */
				JlyParam.ShowOffCode = 0x03;	/*��ʾ��ʱ������ʽ����δ��ʼ��¼*/
			}
		}
	}
	else if(Conf.Jly.RecBootMode == 0x02)	/* ʱ��㶨����ͣ */
	{
		if(!Conf.Jly.WorkStatueIsStop) /* ʱ��㶨������ */
		{
			/*��ȡʱ��ʱ��*/
			read_time();
			if((Rtc.Year == Conf.Jly.MixBoot_Year)&&(Rtc.Month == Conf.Jly.MixBoot_Month)&&(Rtc.Day == Conf.Jly.MixBoot_Day)\
				&&(Rtc.Hour == Conf.Jly.MixBoot_Hour)&&(Rtc.Minute == Conf.Jly.MixBoot_Min))
			{
				Conf.Jly.WorkStatueIsStop = 1;	/*��ʱ��㿪������*/
			}
			else
			{
				Conf.Jly.WorkStatueIsStop = 0;	/* ֹͣ���� */
				JlyParam.ShowOffCode = 0x01;	/*��ʾ����������ʽ����δ��ʼ��¼*/
			}
		}
		if(Conf.Jly.WorkStatueIsStop) /* ʱ��㶨��ֹͣ */
		{
			/*��ȡʱ��ʱ��*/
			read_time();
			if((Rtc.Year == Conf.Jly.FixedStop_Year)&&(Rtc.Month == Conf.Jly.FixedStop_Month)&&(Rtc.Day == Conf.Jly.FixedStop_Day)\
				&&(Rtc.Hour == Conf.Jly.FixedStop_Hour)&&(Rtc.Minute == Conf.Jly.FixedStop_Min))
			{
				Conf.Jly.WorkStatueIsStop = 0;	/* ��ʱ���ֹͣ���� */
				JlyParam.ShowOffCode = 0x02;	/*��ʾ����ֹͣ��ʽ ���ѵ�ʱͣ��*/
			}
			else
			{
				Conf.Jly.WorkStatueIsStop = 1;	
			}
		}
	}
	else if((Conf.Jly.RecBootMode == 0x03) && (Conf.Jly.WorkStatueIsStop))	/* �ֶ�����*/
    {
        Conf.Jly.WorkStatueIsStop = 0;	/* ֹͣ���� */
            //д��fram
		Fram_Write(&Conf.Jly.WorkStatueIsStop,FRAM_WorkStatueIsStopAddr,1);
		JlyParam.ShowOffCode = 0xFF;
    }
    else if(Conf.Jly.RecBootMode == 0x10)	/* �쳣�������� */
    {
        
    }
}
/**
  * @brief  Description ��¼��ֹͣ��ʽ����
  * @param  ��  		
  * @retval ��		
  */
void   RecorderStopModeHandle(void)
{
    uint8_t  IsWriteLasestStopTimeToFram;
    IsWriteLasestStopTimeToFram = 0;
    
    if(Conf.Jly.WorkStatueIsStop)
    {
        if(Conf.Jly.RecStopMode == 0){};	/* �Ƚ��ȳ��ļ�¼ֹͣ��ʽ */
        if(Conf.Jly.RecStopMode == 1)		/* �洢�������ļ�¼ֹͣ��ʽ */
        {
            if((Queue.RecorderFlashPoint >= Flash_MAX_NUM)) //&&IsReadingI2c==0,��������
            {
                Conf.Jly.WorkStatueIsStop = 0;	/* ֹͣ���� */
				
				Fram_Write(&Conf.Jly.WorkStatueIsStop,FRAM_WorkStatueIsStopAddr,1);
                IsWriteLasestStopTimeToFram = 1;
                JlyParam.ShowOffCode = 0;	/*��¼�Ǽ���ͣ��*/
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
  * @brief  Description ��¼��1s�����־
  * @param  ��  		
  * @retval ��		
  */
static void OneSec_Timedeal(void)
{
    
    //LCD��ʾ����
    display_ct++;          
    if(display_ct>=36)
    {
        display_ct = 0;
		Display_Signal(2);/*��ʾ�ź�ǿ��*/
        Display_Mem();	/*��ʾ�洢����*/
         
    }
   /*�����ӵ����*/
	if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_Deal_ACtest) == 0)         
    {
        PManage.HaveExternalPower++;
        if(PManage.HaveExternalPower>=ExternalPowerchecktime)
        {
            Flag.Powerdowncountflag=1;
            PManage.HaveExternalPower=0;
        }
    }    
	/*����ص�Դ�Ƿ����*/
	if(Flag.ExPwOn == 1)	/*������ӵ������¼���Ƿ������*/
	{
		if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_Deal_CHGtest) == 1)         
		{
			PManage.BatChargeFullCount++;
			if(PManage.BatChargeFullCount>=ExternalPowerchecktime)/*���60s*/
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
  * @brief  Description ��¼��
  * @param  ��  		
  * @retval ��		
  */
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
/**
  * @brief  Description ��¼�Ǵ�����
  * @param  ��  		
  * @retval ��		
  */
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
        
        WorkornotMode();
        
		SaveDataOnTimeDeal();
    }
	if(Conf.Jly.WorkStatueIsStop == 0) /*ֹͣ����*/
    {
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
