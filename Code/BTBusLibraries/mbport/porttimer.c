/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttimer.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "./app_cfg.h"
#include "./porttype.h"
//!< FreeModBus Output
#include "./modbus.h"

/* ----------------------- Modbus includes ----------------------------------*/
// #include "mb.h"
// #include "mb_m.h"
// #include "mbport.h"

//STM32相关头文件
#include "bt_main.h"

/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
static TIM_HandleTypeDef TimHandle;

/* Prescaler declaration */
static uint32_t uwPrescalerValue = 0;

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR( void );

/* ----------------------- extern functions ---------------------------------*/
extern void Timer4_Configuration(uint16_t period);
extern void Timer4_Enable(bool bEnable);

/* ----------------------- Start implementation -----------------------------*/
/**
  * @brief  定时器初始化函数
  * @param  None
  * @retval None
  */
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
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
    uwPrescalerValue = (uint32_t)(SystemCoreClock / 20000) - 1;

    /* Set TIMx instance */
    TimHandle.Instance = TIMx;

    /* Initialize TIMx peripheral as follows:
         + Period = 10000 - 1
         + Prescaler = (SystemCoreClock/10000) - 1
         + ClockDivision = 0
         + Counter direction = Up
    */
    TimHandle.Init.Period            = (usTim1Timerout50us * 20) - 1;
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
	
	return TRUE;
}


void
vMBPortTimersEnable(  )
{
    /* Stop Channel1 */
    if (HAL_TIM_Base_Stop_IT(&TimHandle) != HAL_OK)
    {
        /* Stoping Error */
        Error_Handler();
    }
    /* Start Channel1 */
    if (HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
    {
        /* Starting Error */
        Error_Handler();
    }
}

void
vMBPortTimersDisable(  )
{
    /* Stop Channel1 */
    if (HAL_TIM_Base_Stop_IT(&TimHandle) != HAL_OK)
    {
        /* Stoping Error */
        Error_Handler();
    }
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCMBimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
static void prvvTIMERExpiredISR( void )
{
	extern BOOL( *pxMBPortCBTimerExpired ) ( void );
	
	if (pxMBPortCBTimerExpired)
	{
		( void )pxMBPortCBTimerExpired();
	}
}

/**
  * @brief  定时器中断服务函数
  * @param  None
  * @retval None
  */
void prvvTIMERISR(void)
{
	prvvTIMERExpiredISR( );
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	prvvTIMERExpiredISR( );
}

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void TIMx_IRQHandler( void )
{
	HAL_TIM_IRQHandler(&TimHandle);
}
