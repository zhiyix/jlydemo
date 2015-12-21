/**
  ******************************************************************************
  * @file              : bsp_LowPowerDeal.c
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
#ifndef __BSPLOWPOWER_C
#define __BSPLOWPOWER_C

/*============================ INCLUDES ======================================*/
/* Includes ------------------------------------------------------------------*/

#include "main.h"

/******************************************************************************
  * @brief  Description 进入低功耗前的处理
  * @param  None
  * @retval None
  * @note 	
  1.adc PowerDown
  2.
  *****************************************************************************/
static void EnterStopModePowerDeal(void)
{
	if((ReadRX8025Control2() & RX8025_Control2DAFG))//(ReadRX8025Control2() & RX8025_Control2CTFG)||
	{
		RTC8025_Reset(true);
	}
//	read_time();
//	if(Rtc.Second >= 0x30)
//	{
//	  
//		/* Check and Clear the Wakeup flag */
//		if (PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)
//		{
//			PWR_ClearFlag(PWR_FLAG_WU);
//		}
//		{
//			
//			printf("\r\n ... \r\n");
//			printf("\r\n Enter StopMode \r\n");
//			Display_LOW();
//			OffPowerSupply();
//			
//			/* Deselect the FLASH: Chip Select high */
//			SPI_FLASH_CS_HIGH();
//			ADC_Cmd(ADC1, DISABLE);	//关adc
//			
//			//关闭滴答定时器
//			SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
//			//使能电源管理单元时钟
//			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
//			PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);
//		}
//		if(JlyParam.WakeUpSource != 2)
//		{
//			SysClock_ReConfig();
//		}
//		Display_SN();//显示SN号
//		//rtc_deel();
//		printf("\r\n Exit StopMode \r\n");
//	}
}

/******************************************************************************
  * @brief  Description 进入低功耗
  * @param  None
  * @retval None
  *****************************************************************************/
void  EnterLowPower(void)
{
	
	if(JlyParam.WakeUpSource == 2)
	{
		if(Flag.WakeUpStopModeOnTime == 1)
		{
			JlyParam.WakeUpSource =0;
			JlyParam.WakeUpCount = 0; //清0
			Flag.WakeUpStopModeOnTime = 0;
			EnterStopModePowerDeal();
		}
	}else{
		EnterStopModePowerDeal();
	}
}
	
#endif /* __BSPLOWPOWER_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
