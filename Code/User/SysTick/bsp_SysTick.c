/**
  ******************************************************************************
  * @file    bsp_SysTick.c
  * @author  
  * @version V1.0
  * @date    2015-07-12
  * @brief   Sys_Tick应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:
  *
  ******************************************************************************
  */
#include "bsp_SysTick.h"

__IO uint32_t TimingDelay = 0;

/******************************************************************************
  * @brief  系统滴答定时器 SysTick初始化
  * @param  无
  * @retval 无
  *****************************************************************************/
void SysTick_Init(void)
{
	/* SystemFrequency / 1000 1ms 中断一次
     * SystemFrequency / 100000 10us 中断一次
     * SystemFrequency / 1000000 1us 中断一次
     */
	if(SysTick_Config(SystemCoreClock / 1000))
	{
		while(1);
	}
	//关闭滴答定时器
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

/******************************************************************************
  * @brief ms延时程序,1ms为一个单位
  * @param
  * @arg nTime: Delay_us( 1 ) 则实现延时1 * 1ms = 1ms
  *****************************************************************************/
void Delay_ms(__IO uint32_t nTime)
{
	TimingDelay = nTime;
	//使能滴答定时器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	while(TimingDelay != 0);
}

/******************************************************************************
  * @brief  获取节拍程序
  *	@parram 无
  *	@retval 无
  * @attention 在SysTick 中断函数 sysTick_Handler() 调用
  *****************************************************************************/
void TimingDelay_Decrement(void)
{
	if(TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}
