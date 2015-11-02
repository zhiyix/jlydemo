/**
  ******************************************************************************
  * @file    bsp_exti_key.c
  * @author  
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   key应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:
  *
  ******************************************************************************
  */
#include "main.h"

/**
  * @brief  初始化控制KEY的IO
  * @param  无
  * @retval 无
  */
void KEY_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*机械按键 key1 PF13*/
	RCC_AHBPeriphClockCmd(Key_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = Key1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(Key_PORT,&GPIO_InitStructure);
	
	/************************GPIOC***************************/
	/*触摸按键*/
	RCC_AHBPeriphClockCmd(TouchKey_CLK ,ENABLE);
	/*触摸按键 touchkey1 配置*/
	GPIO_InitStructure.GPIO_Pin = TouchKey1_PIN | TouchKey2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(TouchKey_PORT,&GPIO_InitStructure);
	/********************************************************/
}
/**
  * @brief  Configure PF13 in interrupt mode
  * @param  None
  * @retval None
  */
void EXTI15_10_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;	
  
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
    /**************************************************************/
	/* Connect EXTI13 Line to PF13 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource13);
	/* Configure EXTI13 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;

	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&EXTI_InitStructure);
	/**************************************************************/
	
	 /**************************************************************/
	/* Connect EXTI14 Line to PC14 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource14);
	/* Configure EXTI14 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Connect EXTI15 Line to PC15 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource15);
	/* Configure EXTI15 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
  
	/* Enable and set EXTI15_10 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
	/**************************************************************/
}
