/**
  ******************************************************************************
  * @file    portserial.c
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
#ifndef __PORT_SERIAL_C
#define __PORT_SERIAL_C

/* Includes ------------------------------------------------------------------*/
#include "bt_main.h"
#include "porttype.h"
//!< FreeModBus Output
#include "./modbus.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#define HAL_UART_DIR_RX()
#define HAL_UART_DIR_TX()

/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
static UART_HandleTypeDef UartHandle;

/* Sencond Buffer for Receive */
#define                      RECV_BUF_SIZE   9
static uint8_t SndRecvBuffer[RECV_BUF_SIZE + 1];

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void* parameter );
static void prvvUARTRxISR( void* parameter );

/* ----------------------- Start implementation ------------------------------*/
BOOL xMBPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits,
        eMBParity eParity)
{
    /*##-1- Configure the UART peripheral ######################################*/
    /* Put the USART peripheral in the Asynchronous mode (UART Mode) */

    /* UART configured as follows:
        - Word Length = 8 Bits (8 data bit + 0 parity bit)
        - Stop Bit    = One Stop bit
        - Parity      = None parity
        - BaudRate    = 9600 baud
        - Hardware flow control disabled (RTS and CTS signals) */
    UartHandle.Instance        = USARTx;

    UartHandle.Init.BaudRate   = ulBaudRate;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits   = UART_STOPBITS_1;
    UartHandle.Init.Parity     = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode       = UART_MODE_TX_RX;

    if (HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }
	return TRUE;
}

/**
  * @brief  控制接收和发送状态
  * @param  xRxEnable 接收使能、
  *         xTxEnable 发送使能
  * @retval None
  */
void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
	UART_HandleTypeDef *huart = &UartHandle;
	
    if (xRxEnable)
    {
		HAL_UART_Receive_IT(huart, SndRecvBuffer, RECV_BUF_SIZE);
		HAL_UART_DIR_RX();
    }
    else
    {
		HAL_UART_Receive_End(huart);
		HAL_UART_DIR_TX();
    }
    if (xTxEnable)
    {
        /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
        __HAL_UART_ENABLE_IT(huart, UART_IT_ERR);

        /* Enable the UART Transmit data register empty Interrupt */
        __HAL_UART_ENABLE_IT(huart, UART_IT_TC);
    }
    else
    {
		/* Disable TXE, RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts for the interrupt process */
		__HAL_UART_DISABLE_IT(huart, UART_IT_TC);
		__HAL_UART_DISABLE_IT(huart, UART_IT_PE);
		__HAL_UART_DISABLE_IT(huart, UART_IT_ERR);
    }
}

/**
  * @brief  发送数据
  * @param  None
  * @retval None
  */
BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
	UART_HandleTypeDef *huart = &UartHandle;
	
    if(huart->Init.WordLength == UART_WORDLENGTH_9B)
    {
		huart->Instance->DR = (ucByte & (uint16_t)0x01FF);
    }
	else
	{
		huart->Instance->DR = (ucByte & (uint8_t)0xFF);
	}
	
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCMBransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void* parameter )
{
	//mb.c eMBInit函数中
    //pxMBFrameCMBransmitterEmpty = xMBRTUTransmitFSM
    //发送状态机
	if ( NULL != parameter )
	{
		pxMBFrameCBTransmitterEmpty();
	}
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void* parameter )
{
    //mb.c eMBInit函数中
    //pxMBFrameCBByteReceived = xMBRTUReceiveFSM
    //接收状态机
	if ( NULL != parameter )
	{
		pxMBFrameCBByteReceived();
	}
}


/**
  * @brief  从串口获得数据
  * @param  None
  * @retval None
  */
BOOL
xMBPortSerialGetByte( CHAR *pucByte )
{
    //接收数据
    uint32_t tmp_state = 0;

	UART_HandleTypeDef *huart = &UartHandle;
	
    tmp_state = huart->State;
    if((tmp_state == HAL_UART_STATE_BUSY_RX) || (tmp_state == HAL_UART_STATE_BUSY_TX_RX))
    {
        if(huart->Init.WordLength == UART_WORDLENGTH_9B)
        {
            if(huart->Init.Parity == UART_PARITY_NONE)
            {
                *pucByte = (uint16_t)(huart->Instance->DR & (uint16_t)0x01FF);
            }
            else
            {
                *pucByte = (uint16_t)(huart->Instance->DR & (uint16_t)0x00FF);
            }
        }
        else
        {
            if(huart->Init.Parity == UART_PARITY_NONE)
            {
                *pucByte = (uint8_t)(huart->Instance->DR & (uint8_t)0x00FF);
            }
            else
            {
                *pucByte = (uint8_t)(huart->Instance->DR & (uint8_t)0x007F);
            }
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
  * @brief  This function handles USART interrupt request.
  * @param  None
  * @retval None
  */
void USARTx_IRQHandler(void)
{
    uint32_t tmp_flag = 0, tmp_it_source = 0;

	UART_HandleTypeDef *huart = &UartHandle;
	
    tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_PE);
    tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_PE);
    /* UART parity error interrupt occurred ------------------------------------*/
    if((tmp_flag != RESET) && (tmp_it_source != RESET))
    {
        __HAL_UART_CLEAR_PEFLAG(huart);

        huart->ErrorCode |= HAL_UART_ERROR_PE;
    }

    tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_FE);
    tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR);
    /* UART frame error interrupt occurred -------------------------------------*/
    if((tmp_flag != RESET) && (tmp_it_source != RESET))
    {
        __HAL_UART_CLEAR_FEFLAG(huart);

        huart->ErrorCode |= HAL_UART_ERROR_FE;
    }

    tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_NE);
    /* UART noise error interrupt occurred -------------------------------------*/
    if((tmp_flag != RESET) && (tmp_it_source != RESET))
    {
        __HAL_UART_CLEAR_NEFLAG(huart);

        huart->ErrorCode |= HAL_UART_ERROR_NE;
    }

    tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_ORE);
    /* UART Over-Run interrupt occurred ----------------------------------------*/
    if((tmp_flag != RESET) && (tmp_it_source != RESET))
    {
        __HAL_UART_CLEAR_OREFLAG(huart);

        huart->ErrorCode |= HAL_UART_ERROR_ORE;
    }

    tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE);
    tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE);
    /* UART in mode Receiver ---------------------------------------------------*/
    if((tmp_flag != RESET) && (tmp_it_source != RESET))
    {
        prvvUARTRxISR(huart);
    }

    tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_TXE);
    tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_TXE);
    /* UART in mode Transmitter ------------------------------------------------*/
    if((tmp_flag != RESET) && (tmp_it_source != RESET))
    {
//		UART_Transmit_IT(huart);
    }

    tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_TC);
    tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_TC);
    /* UART in mode Transmitter end --------------------------------------------*/
    if((tmp_flag != RESET) && (tmp_it_source != RESET))
    {
        prvvUARTTxReadyISR(huart);
    }

    if(huart->ErrorCode != HAL_UART_ERROR_NONE)
    {
        /* Set the UART state ready to be able to start again the process */
        huart->State = HAL_UART_STATE_READY;

        HAL_UART_ErrorCallback(huart);
    }
}

#endif /* __PORT_SERIAL_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
