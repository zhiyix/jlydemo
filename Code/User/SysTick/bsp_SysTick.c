/**
  ******************************************************************************
  * @file    bsp_SysTick.c
  * @author  
  * @version V1.0
  * @date    2015-07-12
  * @brief   Sys_TickӦ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:
  *
  ******************************************************************************
  */
#include "bsp_SysTick.h"

__IO uint32_t TimingDelay = 0;

/******************************************************************************
  * @brief  ϵͳ�δ�ʱ�� SysTick��ʼ��
  * @param  ��
  * @retval ��
  *****************************************************************************/
void SysTick_Init(void)
{
	/* SystemFrequency / 1000 1ms �ж�һ��
     * SystemFrequency / 100000 10us �ж�һ��
     * SystemFrequency / 1000000 1us �ж�һ��
     */
	if(SysTick_Config(SystemCoreClock / 1000))
	{
		while(1);
	}
	//�رյδ�ʱ��
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

/******************************************************************************
  * @brief ms��ʱ����,1msΪһ����λ
  * @param
  * @arg nTime: Delay_us( 1 ) ��ʵ����ʱ1 * 1ms = 1ms
  *****************************************************************************/
void Delay_ms(__IO uint32_t nTime)
{
	TimingDelay = nTime;
	//ʹ�ܵδ�ʱ��
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	while(TimingDelay != 0);
}

/******************************************************************************
  * @brief  ��ȡ���ĳ���
  *	@parram ��
  *	@retval ��
  * @attention ��SysTick �жϺ��� sysTick_Handler() ����
  *****************************************************************************/
void TimingDelay_Decrement(void)
{
	if(TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}
