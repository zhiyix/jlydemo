/**
  ******************************************************************************
  * @file    mcu_tim.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    31-March-2015
  * @brief   
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MCU_TIM_C
#define __MCU_TIM_C

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

TIM_HandleTypeDef 	TimHandle;

/* Prescaler declaration */
uint32_t uwPrescalerValue = 0;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TIM_Config(void)
{

    /*##-1- Configure the TIM peripheral #######################################*/
    /* -----------------------------------------------------------------------
      In this example TIM3 input clock (TIM3CLK)  is set to APB1 clock (PCLK1),
      since APB1 prescaler is equal to 1.
        TIM3CLK = PCLK1
        PCLK1 = HCLK
        => TIM3CLK = HCLK = SystemCoreClock
      To get TIM3 counter clock at 10 KHz, the Prescaler is computed as following:
      Prescaler = (TIM3CLK / TIM3 counter clock) - 1
      Prescaler = (SystemCoreClock /10 KHz) - 1

      Note:
       SystemCoreClock variable holds HCLK frequency and is defined in system_stm32l1xx.c file.
       Each time the core clock (HCLK) changes, user had to update SystemCoreClock
       variable value. Otherwise, any configuration based on this variable will be incorrect.
       This variable is updated in three ways:
        1) by calling CMSIS function SystemCoreClockUpdate()
        2) by calling HAL API function HAL_RCC_GetSysClockFreq()
        3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
    ----------------------------------------------------------------------- */

    /* Compute the prescaler value to have TIMx counter clock equal to 10000 Hz */
    uwPrescalerValue = (uint32_t)(SystemCoreClock / 10000) - 1;

    /* Set TIMx instance */
    TimHandle.Instance = TIMx;

    /* Initialize TIMx peripheral as follows:
         + Period = 10000 - 1
         + Prescaler = (SystemCoreClock/10000) - 1
         + ClockDivision = 0
         + Counter direction = Up
    */
    TimHandle.Init.Period            = 10000 - 1;
    TimHandle.Init.Prescaler         = uwPrescalerValue;
    TimHandle.Init.ClockDivision     = 0;
    TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;

    if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }

    /*##-2- Start the TIM Base generation in interrupt mode ####################*/
    /* Start Channel1 */
    if (HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
    {
        /* Starting Error */
        Error_Handler();
    }
}

#endif /* __MCU_TIM_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
