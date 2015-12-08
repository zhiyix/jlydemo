/**
  ******************************************************************************
  * @file              : bsp_bootstop.c
  * @author            : 
  * @version           : 
  * @date              : 
  * @brief             : ��¼��������ʽ,ֹͣ��ʽ���� c file
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
	
	����ֹͣ��¼�������������������ǰ��������ʽ����
  *****************************************************************************/
void RecorderBootModeHandle(void)
{
    if(JlyParam.FramErrorCode != 0)
    {
        if(Conf.Jly.WorkStatueIsStop == 1)	/* ����״̬ */
        {
            
            Conf.Jly.WorkStatueIsStop = 0;	/*ֹͣ����*/
            Fram_Write(&Conf.Jly.WorkStatueIsStop,FRAM_WorkStatueIsStopAddr,1);
			
            JlyParam.ShowOffCode = 0x09;	/*���ֹ���*/
			Flag.StopRecording = 0;	//ֹͣ��¼��־��0
        }
        return;
    }
    if(Conf.Jly.RecBootMode == 0x00)	/* ��ʱ����(Ĭ����ʱʱ��0������������)*/
    {
		if((JlyParam.delay_start_time--) <= 0 && (!Conf.Jly.WorkStatueIsStop))	//&& (!Conf.Jly.WorkStatueIsStop)
        {
            JlyParam.delay_start_time = -1;
            Conf.Jly.WorkStatueIsStop = 1;	/*��������*/
			Conf.Jly.RecBootMode = 0xFF;	//��ǰ��¼��ʽ�����󣬲�����ִ�е�ǰ������ߴ���Ч��
        }
        if((JlyParam.delay_start_time)>0)
        {
            Conf.Jly.WorkStatueIsStop = 0;	/* ֹͣ���� */
			
            JlyParam.ShowOffCode = 0x05;	/*��ʾ��ʱ������ʽ����δ��ʼ��¼*/
        }
    }
	else if(Conf.Jly.RecBootMode == 0x01)	/* ʱ��㶨ʱ���� */
	{
		
		/*��ȡʱ��ʱ��*/
		read_time();
		if((Rtc.Year == Conf.Jly.MixBoot_Year)&&(Rtc.Month == Conf.Jly.MixBoot_Month)&&(Rtc.Day == Conf.Jly.MixBoot_Day)\
			&&(Rtc.Hour == Conf.Jly.MixBoot_Hour)&&(Rtc.Minute == Conf.Jly.MixBoot_Min))
		{
			Conf.Jly.WorkStatueIsStop = 1;	/*��ʱ��㿪������*/	
			Conf.Jly.RecBootMode = 0xFF;	//��ǰ��¼��ʽ�����󣬲�����ִ�е�ǰ������ߴ���Ч��
		}else{
			Conf.Jly.WorkStatueIsStop = 0;	/* ֹͣ���� */
			JlyParam.ShowOffCode = 0x03;	/*��ʾ��ʱ������ʽ����δ��ʼ��¼*/
		}
	}
	else if(Conf.Jly.RecBootMode == 0x02)	/* ʱ��㶨����ͣ */
	{
		if(Flag.RecTimeDingDianBoot == 0) /* ʱ��㶨������ */
		{
			/*��ȡʱ��ʱ��*/
			read_time();
			if((Rtc.Year == Conf.Jly.MixBoot_Year)&&(Rtc.Month == Conf.Jly.MixBoot_Month)&&(Rtc.Day == Conf.Jly.MixBoot_Day)\
				&&(Rtc.Hour == Conf.Jly.MixBoot_Hour)&&(Rtc.Minute == Conf.Jly.MixBoot_Min))
			{
				Conf.Jly.WorkStatueIsStop = 1;	/*��ʱ��㿪������*/
				Flag.RecTimeDingDianBoot = 1;	/* ʱ��㶨�������� */
			}else{
				Conf.Jly.WorkStatueIsStop = 0;	/* ֹͣ���� */
				JlyParam.ShowOffCode = 0x01;	/*��ʾ����������ʽ����δ��ʼ��¼*/
				Flag.RecTimeDingDianBoot = 0;
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
				
				Flag.RecTimeDingDianBoot = 0;
				Conf.Jly.RecBootMode = 0xFF;	//��ǰ��¼��ʽ����ֹͣ�󣬲�����ִ�е�ǰ������ߴ���Ч��
			}else{
				
				Conf.Jly.WorkStatueIsStop = 1;	
			}
		}
	}
	else if(Conf.Jly.RecBootMode == 0x03 )	/* ��е�����ֶ�����*/
    {
		if(Conf.Jly.WorkStatueIsStop >= 1)//�ɹ�����ֹͣ
		{
			Conf.Jly.WorkStatueIsStop = 0;	/* ֹͣ���� */
			Conf.Jly.RecBootMode = 0xFF;
            //д��fram
			Fram_Write(&Conf.Jly.WorkStatueIsStop,FRAM_WorkStatueIsStopAddr,1);
			JlyParam.ShowOffCode = 0xFF;
			Flag.StopRecording = 0;	//ֹͣ��¼��־��0  �����Ļ��ڱ��������ʽδ��Чǰ�ο��޸ĵ���ļ�¼��ʽ
		}else{//��ֹͣ������
			Conf.Jly.WorkStatueIsStop = 1;
			Conf.Jly.RecBootMode = 0xFF;
			Flag.StopRecording = 0;	//ֹͣ��¼��־��0  �����Ļ��ڱ��������ʽδ��Чǰ�ο��޸ĵ���ļ�¼��ʽ
			Fram_Write(&Conf.Jly.WorkStatueIsStop,FRAM_WorkStatueIsStopAddr,1);
		}
    }
    else if(Conf.Jly.RecBootMode == 0x10)	/* �쳣�������� */
    {
        
    }
}
/******************************************************************************
  * @brief  Description ��¼��ֹͣ��ʽ����
  * @param  ��  		
  * @retval ��		
  *****************************************************************************/
void RecorderStopModeHandle(void)
{
    uint8_t  IsWriteLasestStopTimeToFram;
    IsWriteLasestStopTimeToFram = 0;
    
    if(Conf.Jly.WorkStatueIsStop)
    {
        if(Conf.Jly.RecStopMode == 0){};	/* �Ƚ��ȳ��ļ�¼ֹͣ��ʽ */
        if(Conf.Jly.RecStopMode == 1)		/* �洢�������ļ�¼ֹͣ��ʽ */
        {
            if((Queue.WriteFlashDataPointer >= Queue.FlashRecActualStorage)) //&&IsReadingI2c==0,��������
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

#endif /* __BSPBOOTSTOP_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
