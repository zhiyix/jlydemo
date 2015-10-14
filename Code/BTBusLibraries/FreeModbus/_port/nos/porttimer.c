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
//#include ".\port.h"
#include ".\app_cfg.h"

/* ----------------------- Modbus includes ----------------------------------*/
// #include "mb.h"
// #include "mb_m.h"
// #include "mbport.h"

//STM32���ͷ�ļ�
#if defined(__GSF_VER2)
 #include "./interface.h"
#elif defined(__GSF_VER1)
 #include ".\stm32fwlib_tim.h"
#else
 #include "stm32f10x.h"
 #include "stm32f10x_it.h"
#endif

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR( void );

/* ----------------------- extern functions ---------------------------------*/
extern void Timer4_Configuration(uint16_t period);
extern void Timer4_Enable(bool bEnable);

/* ----------------------- Start implementation -----------------------------*/
/**
  * @brief  ��ʱ����ʼ������
  * @param  None
  * @retval None
  */
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
#if defined(__GSF_VER2)
	Timer4_Configuration(usTim1Timerout50us);
	MB_STATUS_IND(MB_STA_IND_TIMEN, FALSE);
#elif defined(__GSF_VER1)
    CBASETIM basic_timer = TIM4_BASE;
    basic_timer.Config(20000, usTim1Timerout50us);
    //!
    basic_timer.EnableAutoReload(ENABLE);

    basic_timer.ClearUpdate();
    basic_timer.UpdateInterrupt(DISABLE);
    basic_timer.EnableCounter(DISABLE);
#else
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    uint16_t PrescalerValue = 0;

    // ʹ�ܶ�ʱ��4ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    // ��ʱ��ʱ�������˵��
    // HCLKΪ72MHz��APB1����2��ƵΪ36MHz
    // TIM4��ʱ�ӱ�Ƶ��Ϊ72MHz��Ӳ���Զ���Ƶ,�ﵽ���
    // TIM4�ķ�Ƶϵ��Ϊ3599��ʱ���Ƶ��Ϊ72 / (1 + Prescaler) = 20KHz,��׼Ϊ50us
    // TIM������ֵΪusTim1Timerout50u
    PrescalerValue = (uint16_t) (72000000 / 20000) - 1;
    // ��ʱ��1��ʼ��
    TIM_TimeBaseStructure.TIM_Period = (uint16_t) usTim1Timerout50us;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    // Ԥװ��ʹ��
    TIM_ARRPreloadConfig(TIM4, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    // ��ʱ��4�ж����ȼ�
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // �������жϱ�־λ
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
    // ��ʱ��4����жϹر�
    TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
    // ��ʱ��4����
    TIM_Cmd(TIM4,  DISABLE);
#endif
    return TRUE;
}


void
vMBPortTimersEnable(  )
{
#if defined(__GSF_VER2)
	Timer4_Enable(true);
	MB_STATUS_IND(MB_STA_IND_TIMEN, TRUE);
#elif defined(__GSF_VER1)
    CBASETIM basic_timer = TIM4_BASE;
    basic_timer.ClearUpdate();
    basic_timer.UpdateInterrupt(ENABLE);
    basic_timer.SetCounter(0);
    basic_timer.EnableCounter(ENABLE);	
#else
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    // �趨��ʱ��4�ĳ�ʼֵ
    TIM_SetCounter(TIM4, 0x0000);
    // ��ʱ��4����
    TIM_Cmd(TIM4, ENABLE);	
#endif	
}

void
vMBPortTimersDisable(  )
{
#if defined(__GSF_VER2)
	Timer4_Enable(false);
	MB_STATUS_IND(MB_STA_IND_TIMEN, FALSE);
#elif defined(__GSF_VER1)
    CBASETIM basic_timer = TIM4_BASE;
    basic_timer.ClearUpdate();
    basic_timer.UpdateInterrupt(DISABLE);
    basic_timer.SetCounter(0);
    basic_timer.EnableCounter(DISABLE);	
#else	
    /* Disable any pending timers. */
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
    TIM_SetCounter(TIM4, 0x0000);
    // �رն�ʱ��4
    TIM_Cmd(TIM4, DISABLE);
#endif
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
static void prvvTIMERExpiredISR( void )
{
#if defined(__GSF_VER2)
	if (pxMBPortCBTimerExpired)
	{
    ( void )pxMBPortCBTimerExpired();
	}
#elif defined(__GSF_VER1)
#else
    ( void )pxMBPortCBTimerExpired();
#endif 
}

/**
  * @brief  ��ʱ���жϷ�����
  * @param  None
  * @retval None
  */
void prvvTIMERISR(void)
{
#if defined(__GSF_VER2)
	prvvTIMERExpiredISR( );
#elif defined(__GSF_VER1)
    CBASETIM basic_timer = TIM4_BASE;
    if (basic_timer.GetUpdateStatus())
    {
        basic_timer.ClearUpdate();//TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        // �����ʱ��T4����жϱ�־λ
        
        prvvTIMERExpiredISR( );
    }
#else
	
#endif 
}

