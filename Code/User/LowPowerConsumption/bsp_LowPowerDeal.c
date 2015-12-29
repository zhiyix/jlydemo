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
  * @brief  Description ����͹���ǰ�Ĵ���
  * @param  None
  * @retval None
  * @note 	
  1.adc PowerDown
  2.
  *****************************************************************************/
static void EnterStopModePowerDeal(void)
{
	RTC8025_Reset(true);
	if(JlySensor.bAdcComplete)
	{
//		if((ReadRX8025Control2() & RX8025_Control2DAFG))//(ReadRX8025Control2() & RX8025_Control2CTFG)||
//		{
//			
//			printf("\r\n Exit StopMode \r\n");
//		}
		
		RTC8025_Reset(true);
		// Check and Clear the Wakeup flag 
		if (PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)
		{
			PWR_ClearFlag(PWR_FLAG_WU);
		}
		{
			
			PowerCount++;
			printf("\r\n PowerCount:%d \r\n",PowerCount);
			printf("\r\n Enter StopMode \r\n");
			Display_LOW();
			OffPowerSupply();
			
			// Deselect the FLASH: Chip Select high 
			//SPI_FLASH_CS_HIGH();
//			SPI_Flash_PowerDown(); 
//			
//			DMA_Cmd(DMA1_Channel1, DISABLE);
//			ADC_Cmd(ADC1, DISABLE);	//��adc
			
			//�رյδ�ʱ��
			SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
			//ʹ�ܵ�Դ����Ԫʱ��
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
			PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);
		}
		if(JlyParam.WakeUpSource == 0)
		{
			SysClock_ReConfig();
		}
		//RTC8025_ClearDALE(true);
//		DMA_Cmd(DMA1_Channel1, ENABLE);
//		ADC_DMACmd(ADC1, ENABLE);
//		ADC_Cmd(ADC1, ENABLE);	//����adc
		
			
		//ADC1_Init();
		
		printf("\r\n Exit StopMode \r\n");
	}
}

/******************************************************************************
  * @brief  Description ����͹���
  * @param  None
  * @retval None
  *****************************************************************************/
void  EnterLowPower(void)
{
	
	if(JlyParam.WakeUpSource != 0)
	{
		if(Flag.WakeUpStopModeOnTime == 1)
		{
			JlyParam.WakeUpSource =0;
			JlyParam.WakeUpCount = 0; //��0
			Flag.WakeUpStopModeOnTime = 0;
			EnterStopModePowerDeal();
		}
		
	}else
	{
		EnterStopModePowerDeal();
	}
}
	
#endif /* __BSPLOWPOWER_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
