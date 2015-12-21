#ifndef __ADC_H
#define	__ADC_H


#include "stm32l1xx.h"

extern __IO uint16_t ADC_ConvertedValue[32];
extern float ChannelDataFloat[32];

void ADC1_Init(void);

//void  Dealing_Gather(unsigned char all_channel_code);

#endif /* __ADC_H */

