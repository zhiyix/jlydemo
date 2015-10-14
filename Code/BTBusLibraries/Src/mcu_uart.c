/**
  ******************************************************************************
  * @file    mcu_uart.c
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
#ifndef __MCU_UART_C
#define __MCU_UART_C

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef	UartHandle;

/* Prescaler declaration */
#define            RECV_BUF_SIZE   9
uint8_t RecvBuffer[RECV_BUF_SIZE + 1];

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

void UART_Config(void)
{
    /*##-1- Configure the UART peripheral ######################################*/
    /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
#if (0)
    /* UART configured as follows:
        - Word Length = 8 Bits (7 data bit + 1 parity bit)
        - Stop Bit    = One Stop bit
        - Parity      = ODD parity
        - BaudRate    = 9600 baud
        - Hardware flow control disabled (RTS and CTS signals) */
    UartHandle.Instance        = USARTx;

    UartHandle.Init.BaudRate   = 9600;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits   = UART_STOPBITS_1;
    UartHandle.Init.Parity     = UART_PARITY_ODD;
    UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode       = UART_MODE_TX_RX;
#else
    /* UART configured as follows:
        - Word Length = 8 Bits (8 data bit + 0 parity bit)
        - Stop Bit    = One Stop bit
        - Parity      = None parity
        - BaudRate    = 9600 baud
        - Hardware flow control disabled (RTS and CTS signals) */
    UartHandle.Instance        = USARTx;

    UartHandle.Init.BaudRate   = 9600;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits   = UART_STOPBITS_1;
    UartHandle.Init.Parity     = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode       = UART_MODE_TX_RX;
#endif
    if (HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }

    /* Output a message on Hyperterminal using printf function */
    printf("\r\n UART Printf Example: retarget the C library printf function to the UART\r\n");

    HAL_UART_Receive_IT(&UartHandle, RecvBuffer, RECV_BUF_SIZE);
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
    HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);
    ITM_SendChar((uint32_t)ch);
    return ch;
}

#endif /* __MCU_UART_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
