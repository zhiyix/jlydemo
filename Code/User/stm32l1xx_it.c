/**
  ******************************************************************************
  * @file    Project/STM32L1xx_StdPeriph_Templates/stm32l1xx_it.c 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    16-May-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_it.h"
#include "main.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
	SysTickTestCount++;
	
	if(SysTickTestCount >= 1000000)
	{
		SysTickTestCount = 0;
	}
//	MsCount++;
//	if(MsCount >=60000)
//	{
//		MsCount = 0;
//		Flag.SysTickSec = 1;
//	}
}

/******************************************************************************/
/*            STM32L1xx Peripherals Interrupt Handlers                        */
/******************************************************************************/
/**
  * @brief  This function handles USART1 interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	uint8_t ch;
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
    {
        //ch = USART1->DR;
        ch = USART_ReceiveData(USART1);
        printf("%c",ch);
    }

}
/**
  * @brief  This function handles External lines 0 interrupt request.
  * @param  None
  * @retval None
  */

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) //PA0唤醒引脚
	{

		if(JlyParam.NormalRecInterval >= 60)//记录间隔大于60s
		{
			Flag.StorageData = 1;	//存储标志
			Flag.StartSample = 1;	//采集标志
			
		}else if((JlyParam.NormalRecInterval >0) && (JlyParam.NormalRecInterval < 60))
		{
			JlyParam.RtcSecCount++;
			if(JlyParam.RtcSecCount >= JlyParam.NormalRecInterval)//到秒记录间隔
			{
				JlyParam.RtcSecCount = 0;
				
				Flag.StorageData = 1;	//存储标志
				Flag.StartSample = 1;	//采集标志
			}
		}
		
		
		/* Clear the EXTI line 0 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}
/*
void RTC中断(void)
{
	存储标志=1;
	采集标志=1;
}
*/
/**
  * @brief  This function handles External lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line13) != RESET) //机械按键key1
	{
		//SysClock_ReConfig();
		printf("\r\n Sys Clock ReConfig Exit StopMode \r\n");
		TOUCHKEY_POWER(ON);		  //开触摸按键电源
		JlyParam.WakeUpSource = 2;//表示按键唤醒
        Flag.Key1AnXia = 1;
		Flag.AlarmXiaoYin = 1;	//按键消音标志
		Flag.LcdBackLightOn = 1; //Lcd背光点亮
		LcdBackLight(ON);
		BEEP(OFF);
		// Toggle LED1 
		LED1_TOGGLE;
		/* Clear the EXTI line 13 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	if(EXTI_GetITStatus(EXTI_Line14) != RESET) //触摸按键key2
	{
		AlarmLed2_TOGGLE;
		Flag.TouchKey2DuanAn = 1;
		JlyParam.WakeUpCount = 0;
		/* Clear the EXTI line 14 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	
	if(EXTI_GetITStatus(EXTI_Line15) != RESET) //触摸按键key1
	{
		LED1_TOGGLE;
		Flag.TouchKey1DuanAn = 1;
		/* Clear the EXTI line 15 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}
/**
  * @brief  This function handles TIM2 interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
    {
//        time++;
        Flag.Sec = 1;//1s定时时间到
		
		
		if((--JlyParam.SampleTimeCount) <=0)//采样时间
		{
			JlyParam.SampleTimeCount = JlyParam.SampleInterval;//采集时间 单位:s
			Flag.StartSample = 1;
		}
		if(JlySensor.SensorStableCount >0)//传感器稳定时间
		{
			JlySensor.SensorStableCount--;
		}
		
		
		
		JlyParam.AlarmTimeDelayCount ++;
		if(JlyParam.AlarmTimeDelayCount >= JlyParam.SoundLightAlarmTimeDelay)
		{
			JlyParam.AlarmTimeDelayCount = 0;
			Flag.AlarmTimeDelayIsOut = 1;//声光报警延时时间到
		}
		//控制进入低功耗
		if(JlyParam.WakeUpSource == 2)
		{
			JlyParam.WakeUpCount ++;
			if(JlyParam.WakeUpCount >= WakeUpTime)
			{
				JlyParam.WakeUpCount = 0;
				Flag.WakeUpStopModeOnTime = 1;
			}
		}
		
		
        TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
    }
}
/*
void TIM2中断(void)
{
	if (采集计数器-- == 0)
	{
		采集标志=1;
	}
	if ( 休眠计数器 > 0)
	{
		休眠计数器  --;
	}
	if (传感器稳定计数器 > 0)
	{
		传感器稳定计数器 --;
	}
}*/
/******************************************************************************/
/*                 STM32L1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l1xx_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
