/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
//#include ".\port.h"
#include ".\app_cfg.h"

/* ----------------------- Modbus includes ----------------------------------*/
// #include "mb.h"
// #include "mbport.h"

// STM32�������ͷ�ļ�
#if defined(__GSF_VER2)
 #include ".\hal_usart.h"
 #include "./interface.h"
#elif defined(__GSF_VER1)
 #include ".\bsp_gpio.h"
 #include ".\bsp_serial.h"
#else
 #include "stm32f10x.h"
 #include "stm32f10x_it.h"
#endif

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void* parameter );
static void prvvUARTRxISR( void* parameter );

/* ----------------------- Start implementation -----------------------------*/
/**
  * @brief  ���ڳ�ʼ��
  * @param  ucPORT      ���ں�
  *         ulBaudRate  ������
  *         ucDataBits  ����λ
  *         eParity     У��λ
  * @retval None
  */
BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
#if defined(__GSF_VER2)
    hal_usart_cfg cfg =
    {
        true,
        USART_PARITY_NO,
        USART_STOPBITS_1B,
        USART_HWFlowControl_None,
        115200,
		{ prvvUARTTxReadyISR, 0, },
		{ prvvUARTRxISR, 0, },
		{ 0, 0, },
    };
	cfg.ulBaudRate = ulBaudRate;
    USART1.Config(cfg);
#elif defined(__GSF_VER1)
	USART1_TX.Init();
	USART1_RX.Init();
	
	Serial1.Init();
	Serial1.ReceiveInterrupt();
	Serial1.TransCompInterrupt();
	Serial1.TransmittDMA();
	
	MCU_NVIC.Config(USART1_IRQn, ENABLE, 0, 0);
#else
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    (void)ucPORT;     //���޸Ĵ���
    (void)ucDataBits; //���޸�����λ����
    (void)eParity;    //���޸�У���ʽ

    //�������485���ͺͽ���ģʽ
    RCC_APB2PeriphClockCmd(MB_RS485_DIR_PERPH, ENABLE);
    //GPIOD.8
    GPIO_InitStructure.GPIO_Pin = MB_RS485_DIR_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MB_RS485_DIR_PORT, &GPIO_InitStructure);

    //return TRUE;

    //ʹ��USART1��GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
              RCC_APB2Periph_USART1, ENABLE);

    //GPIOA9 USART1_Tx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //�������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //GPIOA.10 USART1_Rx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = ulBaudRate;            //ֻ�޸Ĳ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    //���ڳ�ʼ��
    USART_Init(USART1, &USART_InitStructure);
    //ʹ��USART1
    USART_Cmd(USART1, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    //�趨USART1 �ж����ȼ�
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif
    return TRUE;
}

/**
  * @brief  ���ƽ��պͷ���״̬
  * @param  xRxEnable ����ʹ�ܡ�
  *         xTxEnable ����ʹ��
  * @retval None
  */
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
#if defined(__GSF_VER2)
    if(xRxEnable)	//ʹ�ܽ��պͽ����ж�
    {
        USART1.IntEn(RXNEIT, true);
		//MAX485���� �͵�ƽΪ����ģʽ
		RS485_DIR_RX();
		MB_STATUS_IND(MB_STA_IND_UDIR, FALSE);
    }
    else
    {
        USART1.IntEn(RXNEIT, false);
        //MAX485���� �ߵ�ƽΪ����ģʽ
		RS485_DIR_TX();
		MB_STATUS_IND(MB_STA_IND_UDIR, TRUE);
    }
    if(xTxEnable)	//ʹ�ܷ�������ж�
        USART1.IntEn(TCIT, true);
    else			//��ֹ��������ж�
        USART1.IntEn(TCIT, false);
#elif defined(__GSF_VER1)
    if(xRxEnable)	//ʹ�ܽ��պͽ����ж�
    {
        Serial1.InterruptConfig(RXNE_INT_EN, ENABLE);
		//MAX485���� �͵�ƽΪ����ģʽ
    }
    else
    {
        Serial1.InterruptConfig(RXNE_INT_EN, DISABLE);
        //MAX485���� �ߵ�ƽΪ����ģʽ
    }
    if(xTxEnable)	//ʹ�ܷ�������ж�
        Serial1.InterruptConfig(TC_INT_EN, ENABLE);
    else			//��ֹ��������ж�
        Serial1.InterruptConfig(TC_INT_EN, DISABLE);
#else
    if(xRxEnable)	//ʹ�ܽ��պͽ����ж�
    {
		USART_ITConfig(MB_USARTx, USART_IT_RXNE, ENABLE);
		//MAX485���� �͵�ƽΪ����ģʽ
		GPIO_ResetBits(MB_RS485_DIR_PORT, MB_RS485_DIR_PIN);
    }
    else
    {
		USART_ITConfig(MB_USARTx, USART_IT_RXNE, DISABLE);
        //MAX485���� �ߵ�ƽΪ����ģʽ
		GPIO_SetBits(MB_RS485_DIR_PORT, MB_RS485_DIR_PIN);
    }
    if(xTxEnable) 	//ʹ�ܷ�������ж�
		USART_ITConfig(MB_USARTx, USART_IT_TC, ENABLE);
    else 			//��ֹ��������ж�
		USART_ITConfig(MB_USARTx, USART_IT_TC, DISABLE);
#endif
}

/**
  * @brief  ͨ�����ڷ�������
  * @param  None
  * @retval None
  */
BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    //��������
#if defined(__GSF_VER2)
    USART1.Write(ucByte);
#elif defined(__GSF_VER1)
    Serial1.Write(ucByte);
#else
	USART_SendData(MB_USARTx, ucByte);
#endif
    return TRUE;
}

/**
  * @brief  ͨ�����ڷ�������
  * @param  None
  * @retval None
  */
BOOL
xMBPortSerialPutByteWithTxe( CHAR ucByte )
{
    //��������
#if defined(__GSF_VER2)
    USART1.PutChar(ucByte);
#elif defined(__GSF_VER1)
	Serial1.PutChar(ucByte);
#else
	USART_SendData(MB_USARTx, ucByte);
	while (USART_GetFlagStatus(MB_USARTx, USART_FLAG_TXE) == RESET)
	{};
#endif
    return TRUE;
}

/**
  * @brief  �Ӵ��ڻ������
  * @param  None
  * @retval None
  */
BOOL
xMBPortSerialGetByte( CHAR *pucByte )
{
    //��������
#if defined(__GSF_VER2)
	*pucByte = USART1.Read();
#elif defined(__GSF_VER1)
	*pucByte = Serial1.Read();
#else
	*pucByte = USART_ReceiveData(MB_USARTx);
#endif
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void* parameter )
{
	//mb.c eMBInit������
    //pxMBFrameCBTransmitterEmpty = xMBRTUTransmitFSM
    //����״̬��
    pxMBFrameCBTransmitterEmpty();
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void* parameter )
{	
	MBMasterStatusIndicate(MB_STA_IND_TRIG, false);
    //mb.c eMBInit������
    //pxMBFrameCBByteReceived = xMBRTUReceiveFSM
    //����״̬��
	if (USART1.Event(USART_FLAG_RXNE))
	{
		pxMBFrameCBByteReceived();
	}
}

/**
  * @brief  MB_USARTx�жϷ�����
  * @param  None
  * @retval None
  */
ARMAPI void BSP_IntHandlerUSART1_0(void)
{
#if defined(__GSF_VER2)
    //���������ж�
    if (USART1.Event(USART_FLAG_RXNE))
    {
        prvvUARTRxISR(NULL);
		//����жϱ�־λ
		USART1.Clear(USART_FLAG_RXNE);
    }
    //��������ж�
    if (USART1.Event(USART_FLAG_TC))
    {
        prvvUARTTxReadyISR(NULL);
		//����жϱ�־
		USART1.Clear(USART_FLAG_TC);
    }
	//�������
    if (USART1.Event(USART_FLAG_ORE))
    {
		//����жϱ�־
		USART1.Clear(USART_FLAG_ORE);
    }
#elif defined(__GSF_VER1)
    //���������ж�
    if (Serial1.Received())
    {
//		Serial1.Read_ISR();
        prvvUARTRxISR();
		//����жϱ�־λ
		Serial1.ClearITPendingBit(RXNE_FLAG);
    }
    //��������ж�
    if (Serial1.Transmitted())
    {
        prvvUARTTxReadyISR();
		//����жϱ�־
		Serial1.ClearITPendingBit(TC_FLAG);
    }
#else
	if(USART_GetITStatus(MB_USARTx, USART_IT_RXNE) == SET)
	{
		prvvUARTRxISR();
		//����жϱ�־λ
		USART_ClearITPendingBit(MB_USARTx, USART_IT_RXNE);
	}
    //��������ж�
	if(USART_GetITStatus(MB_USARTx, USART_IT_TC) == SET)
	{
		prvvUARTTxReadyISR();
		//����жϱ�־
		USART_ClearITPendingBit(MB_USARTx, USART_IT_TC);
	}
    //���Կ��Ƿ����ȥ�� 2012-07-23
    //���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ��� �������������жϵ�����
    /*
    if(USART_GetFlagStatus(MB_USARTx,USART_FLAG_ORE)==SET)
    {
      USART_ClearFlag(MB_USARTx,USART_FLAG_ORE); 	//��SR
      USART_ReceiveData(MB_USARTx);              	//��DR
    }
    */
#endif
}

