/**
  ******************************************************************************
  * @file    mcu_itm.c
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
#ifndef __MCU_ITM_C
#define __MCU_ITM_C

/* Includes ------------------------------------------------------------------*/
#include "mcu_itm.h"
#include "stdio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Exported functions ------------------------------------------------------- */
void ITM_Config(void)
{
	DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN;
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
    ITM_SendChar((uint32_t)ch);
	
    return ch;
}

#endif /* __MCU_ITM_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
