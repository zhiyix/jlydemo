/**
  ******************************************************************************
  * @file              : bsp_bootstop.c
  * @author            : 
  * @version           : 
  * @date              : 
  * @brief             : ¼ÇÂ¼ÒÇÆô¶¯·½Ê½,Í£Ö¹·½Ê½´¦Àí c file
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
  * @brief  Description ¼ÇÂ¼ÒÇÆô¶¯·½Ê½´¦Àí
  * @param  ÎÞ  		
  * @retval ÎÞ		
  * ËµÃ÷£º
		 0FF:¿ØÖÆ(ÊÖ¶¯)Í£»ú×´Ì¬
	    00FF:¼ÇÂ¼ÒÇ¼ÇÂúÍ£»ú
		10FF:¶¨µãÆô¶¯·½Ê½£¬»¹Î´¿ªÊ¼¼ÇÂ¼
		20FF:¶¨µãÍ£Ö¹·½Ê½£¬ÒÑµ½Ê±Í£»ú
		30FF:¶¨Ê±Æô¶¯·½Ê½£¬»¹Î´¿ªÊ¼¼ÇÂ¼
		40FF:
		50FF:ÑÓÊ±Æô¶¯·½Ê½£¬»¹Î´¿ªÊ¼¼ÇÂ¼
		90FF:³öÏÖ¹ÊÕÏÍ£»ú
	ÊÖ¶¯Æô¶¯ÉÏÎ»»úÅäÖÃºÍ°´¼ü¿ªÆô¹¤×÷¡
	
	°´¼üÍ£Ö¹¼ÇÂ¼ºó£¬Èç¹ûÒÇÆ÷ÖØÆôÔò°´ÖØÆôÇ°µÄÆô¶¯·½Ê½Æô¶¯
  *****************************************************************************/
void RecorderBootModeHandle(void)
{
    if(JlyParam.FramErrorCode != 0)
    {
        if(Conf.Jly.WorkStatueIsStop == 1)	/* ¹¤×÷×´Ì¬ */
        {
            
            Conf.Jly.WorkStatueIsStop = 0;	/*Í£Ö¹¹¤×÷*/
            Fram_Write(&Conf.Jly.WorkStatueIsStop,FRAM_WorkStatueIsStopAddr,1);
			
            JlyParam.ShowOffCode = 0x09;	/*³öÏÖ¹ÊÕÏ*/
			Flag.StopRecording = 0;	//Í£Ö¹¼ÇÂ¼±êÖ¾Çå0
        }
        return;
    }
    if(Conf.Jly.RecBootMode == 0x00)	/* ÑÓÊ±Æô¶¯(Ä¬ÈÏÑÓÊ±Ê±¼ä0£¬¼´Á¢¼´Æô¶¯)*/
    {
		if((JlyParam.delay_start_time--) <= 0 && (!Conf.Jly.WorkStatueIsStop))	//&& (!Conf.Jly.WorkStatueIsStop)
        {
            JlyParam.delay_start_time = -1;
            Conf.Jly.WorkStatueIsStop = 1;	/*¿ªÆô¹¤×÷*/
			Conf.Jly.RecBootMode = 0xFF;	//µ±Ç°¼ÇÂ¼·½Ê½Æô¶¯ºó£¬²»»áÔÙÖ´ÐÐµ±Ç°³ÌÐò£¬Ìá¸ß´úÂëÐ§ÂÊ
        }
        if((JlyParam.delay_start_time)>0)
        {
            Conf.Jly.WorkStatueIsStop = 0;	/* Í£Ö¹¹¤×÷ */
			
            JlyParam.ShowOffCode = 0x05;	/*±íÊ¾ÑÓÊ±Æô¶¯·½Ê½£¬»¹Î´¿ªÊ¼¼ÇÂ¼*/
        }
    }
	else if(Conf.Jly.RecBootMode == 0x01)	/* Ê±¼äµã¶¨Ê±Æô¶¯ */
	{
		
		/*¶ÁÈ¡Ê±ÖÓÊ±¼ä*/
		read_time();
		if((Rtc.Year == Conf.Jly.MixBoot_Year)&&(Rtc.Month == Conf.Jly.MixBoot_Month)&&(Rtc.Day == Conf.Jly.MixBoot_Day)\
			&&(Rtc.Hour == Conf.Jly.MixBoot_Hour)&&(Rtc.Minute == Conf.Jly.MixBoot_Min))
		{
			Conf.Jly.WorkStatueIsStop = 1;	/*µ½Ê±¼äµã¿ªÆô¹¤×÷*/	
			Conf.Jly.RecBootMode = 0xFF;	//µ±Ç°¼ÇÂ¼·½Ê½Æô¶¯ºó£¬²»»áÔÙÖ´ÐÐµ±Ç°³ÌÐò£¬Ìá¸ß´úÂëÐ§ÂÊ
		}else{
			Conf.Jly.WorkStatueIsStop = 0;	/* Í£Ö¹¹¤×÷ */
			JlyParam.ShowOffCode = 0x03;	/*±íÊ¾¶¨Ê±Æô¶¯·½Ê½£¬»¹Î´¿ªÊ¼¼ÇÂ¼*/
		}
	}
	else if(Conf.Jly.RecBootMode == 0x02)	/* Ê±¼äµã¶¨µãÆôÍ£ */
	{
		if(Flag.RecTimeDingDianBoot == 0) /* Ê±¼äµã¶¨µãÆô¶¯ */
		{
			/*¶ÁÈ¡Ê±ÖÓÊ±¼ä*/
			read_time();
			if((Rtc.Year == Conf.Jly.MixBoot_Year)&&(Rtc.Month == Conf.Jly.MixBoot_Month)&&(Rtc.Day == Conf.Jly.MixBoot_Day)\
				&&(Rtc.Hour == Conf.Jly.MixBoot_Hour)&&(Rtc.Minute == Conf.Jly.MixBoot_Min))
			{
				Conf.Jly.WorkStatueIsStop = 1;	/*µ½Ê±¼äµã¿ªÆô¹¤×÷*/
				Flag.RecTimeDingDianBoot = 1;	/* Ê±¼äµã¶¨µãÒÑÆô¶¯ */
			}else{
				Conf.Jly.WorkStatueIsStop = 0;	/* Í£Ö¹¹¤×÷ */
				JlyParam.ShowOffCode = 0x01;	/*±íÊ¾¶¨µãÆô¶¯·½Ê½£¬»¹Î´¿ªÊ¼¼ÇÂ¼*/
				Flag.RecTimeDingDianBoot = 0;
			}
		}
		
		if(Conf.Jly.WorkStatueIsStop) /* Ê±¼äµã¶¨µãÍ£Ö¹ */
		{
			/*¶ÁÈ¡Ê±ÖÓÊ±¼ä*/
			read_time();
			if((Rtc.Year == Conf.Jly.FixedStop_Year)&&(Rtc.Month == Conf.Jly.FixedStop_Month)&&(Rtc.Day == Conf.Jly.FixedStop_Day)\
				&&(Rtc.Hour == Conf.Jly.FixedStop_Hour)&&(Rtc.Minute == Conf.Jly.FixedStop_Min))
			{
				Conf.Jly.WorkStatueIsStop = 0;	/* µ½Ê±¼äµãÍ£Ö¹¹¤×÷ */
				JlyParam.ShowOffCode = 0x02;	/*±íÊ¾¶¨µãÍ£Ö¹·½Ê½ £¬ÒÑµ½Ê±Í£»ú*/
				
				Flag.RecTimeDingDianBoot = 0;
				Conf.Jly.RecBootMode = 0xFF;	//µ±Ç°¼ÇÂ¼·½Ê½Æô¶¯Í£Ö¹ºó£¬²»»áÔÙÖ´ÐÐµ±Ç°³ÌÐò£¬Ìá¸ß´úÂëÐ§ÂÊ
			}else{
				
				Conf.Jly.WorkStatueIsStop = 1;	
			}
		}
	}
	else if(Conf.Jly.RecBootMode == 0x03 )	/* »úÐµ°´¼üÊÖ¶¯Æô¶¯*/
    {
		if(Conf.Jly.WorkStatueIsStop >= 1)//ÓÉ¹¤×÷µ½Í£Ö¹
		{
			Conf.Jly.WorkStatueIsStop = 0;	/* Í£Ö¹¹¤×÷ */
			Conf.Jly.RecBootMode = 0xFF;
            //Ð´Èëfram
			Fram_Write(&Conf.Jly.WorkStatueIsStop,FRAM_WorkStatueIsStopAddr,1);
			JlyParam.ShowOffCode = 0xFF;
			Flag.StopRecording = 0;	//Í£Ö¹¼ÇÂ¼±êÖ¾Çå0  ÕâÑùµÄ»°ÔÚ±ðµÄÆô¶¯·½Ê½Î´ÉúÐ§Ç°ÈÎ¿ÉÐÞ¸Äµ½±ðµÄ¼ÇÂ¼·½Ê½
		}else{//ÓÉÍ£Ö¹µ½¹¤×÷
			Conf.Jly.WorkStatueIsStop = 1;
			Conf.Jly.RecBootMode = 0xFF;
			Flag.StopRecording = 0;	//Í£Ö¹¼ÇÂ¼±êÖ¾Çå0  ÕâÑùµÄ»°ÔÚ±ðµÄÆô¶¯·½Ê½Î´ÉúÐ§Ç°ÈÎ¿ÉÐÞ¸Äµ½±ðµÄ¼ÇÂ¼·½Ê½
			Fram_Write(&Conf.Jly.WorkStatueIsStop,FRAM_WorkStatueIsStopAddr,1);
		}
    }
    else if(Conf.Jly.RecBootMode == 0x10)	/* Òì³£Ìõ¼þÆô¶¯ */
    {
        
    }
}
/******************************************************************************
  * @brief  Description ¼ÇÂ¼ÒÇÍ£Ö¹·½Ê½´¦Àí
  * @param  ÎÞ  		
  * @retval ÎÞ		
  *****************************************************************************/
void RecorderStopModeHandle(void)
{
    uint8_t  IsWriteLasestStopTimeToFram;
    IsWriteLasestStopTimeToFram = 0;
    
    if(Conf.Jly.WorkStatueIsStop)
    {
        if(Conf.Jly.RecStopMode == 0){};	/* ÏÈ½øÏÈ³öµÄ¼ÇÂ¼Í£Ö¹·½Ê½ */
        if(Conf.Jly.RecStopMode == 1)		/* ´æ´¢Æ÷¼ÇÂúµÄ¼ÇÂ¼Í£Ö¹·½Ê½ */
        {
            if((Queue.WriteFlashDataPointer >= Queue.FlashRecActualStorage)) //&&IsReadingI2c==0,ÏÂÔØÊý¾Ý
            {
                Conf.Jly.WorkStatueIsStop = 0;	/* Í£Ö¹¹¤×÷ */
				
				Fram_Write(&Conf.Jly.WorkStatueIsStop,FRAM_WorkStatueIsStopAddr,1);
                IsWriteLasestStopTimeToFram = 1;
                JlyParam.ShowOffCode = 0;	/*¼ÇÂ¼ÒÇ¼ÇÂúÍ£»ú*/
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
