/**
 * @brief USART1 GPIO ���ã�����ģʽ����115200 8-N-1
 * @param ��
 * @tetval ��
 */
#include "bsp_usart.h"
#include <stdio.h>

static void NVIC_Configuration(void);


void USART1_Config(uint32_t BaudRats)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    
	/* config GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    /* config USART1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);
    //stm32f103 ����Ҫ��������
	/* Connect PXx to USARTx_Tx */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
	/* Connect PXx to USARTx_Rx */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
	
    /* USART1 GPIO config */
    /* Configure USART1 Tx (PB.06) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* Configure USART1 Rx (PB.07) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* USART1 mode config */
    USART_InitStructure.USART_BaudRate = BaudRats;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    
    /*ʹ�ܴ���1�����ж�,,,ע��*/
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
    
	NVIC_Configuration();
	
    USART_Cmd(USART1, ENABLE);
}

/**
 * @brief ����USART1�����ж�
 * @param ��
 * @tetval ��
 */
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    
    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
 * @brief �ض���c�⺯��printf��usart1
 * @param ��
 * @tetval ��
 */
int fputc(int ch,FILE *f)
{
    /*����һ���ֽ����ݵ�USART1*/
    USART_SendData(USART1,(uint8_t) ch);
    
    /*�ȴ��������*/
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
    
    return(ch);
}

/**
 * @brief �ض���c�⺯��scanf��usart1
 * @param ��
 * @tetval ��
 */
int fgetc(FILE *f)
{
    /*�ȴ�����1��������*/
    while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET);
    
    return (int)USART_ReceiveData(USART1);
}
