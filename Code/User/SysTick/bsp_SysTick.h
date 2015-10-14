#ifndef __BSPSYSTICK_H
#define __BSPSYSTICK_H

#include "stm32l1xx.h"

//extern __IO u32 TimingDelay;

void SysTick_Init(void);
void Delay_ms(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
#endif
