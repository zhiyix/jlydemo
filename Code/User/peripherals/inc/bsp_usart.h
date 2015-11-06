#ifndef __BSPUSART_H
#define __BSPUSART_H

#include "stm32l1xx.h"

#define Usart1_DefaultBaudRate 115200

void USART1_Config(uint32_t BaudRats);

//void fputc(int ch,FILE *f);

#endif
