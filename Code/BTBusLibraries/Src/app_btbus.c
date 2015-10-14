/**
  ******************************************************************************
  * @file              : app_btbus.c
  * @author            : HZ Zeda Team
  * @version           : Ver 5.1
  * @date              : 27/09/2014 17:41:57
  * @brief             : h file
  * @description       : Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2014 STMicroelectronics
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BTBUS_C
#define BTBUS_C

#ifdef __cplusplus
//extern "C" {
#endif

/*============================ INCLUDES ======================================*/
//! @name include
//! @{
#include <stdio.h>
#include <stdio.h>
#include <string.h>
//! @}

/*============================ MACROS ========================================*/
/*######--------------------------------######--------------------------------*/

/*============================ TYPES =========================================*/

/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/
// MCU:STM32F103C(8-B)Tx

/*============================ EXTERN FUNCTIONS ==============================*/
/* Includes ------------------------------------------------------------------*/
#include "app_btbus.h"

//!< FreeModBus Output
#include "./btbus.h"


/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
//����Ĵ�������
uint16_t usRegInputBuf[REG_INPUT_NREGS];

//�Ĵ�����ʼ��ַ
uint16_t usRegInputStart = REG_INPUT_START;

//���ּĴ�������
uint8_t  ucRegHoldingBuf[REG_HOLDING_NREGS];

//���ּĴ�����ʼ��ַ
uint16_t usRegHoldingStart = REG_HOLDING_START;

//��Ȧ״̬
uint8_t ucRegCoilsBuf [REG_COILS_SIZE / 8] = {0x01, 0x00, };
uint8_t ucRegCoilsRerv[REG_COILS_SIZE / 8] = {0x5A, 0x5F, };

//��������״̬
uint8_t ucRegDiscreteBuf [REG_DISCRETE_SIZE / 8] = {0x00, 0x01, };
uint8_t ucRegDiscreteRerv[REG_DISCRETE_SIZE / 8] = {0xA5, 0xAF, };

/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
/**
  * @brief  FreeModBus ��ʼ��
  * @param  None
  * @retval None
  */
int
btbus_init(void)
{
    //Relay_Config();
    // ��ʼ��ģʽ��RTUģʽ
    // �ӻ���ַ��Ϊ'1'
    // �˿ڣ�COM1
    // ������9600 ��У��
    eBTInit(BT_RTU, BT_DefaultSlaveAddress, 1, BT_DefaultDeviceBaudRate, BT_PAR_NONE);
    //����FreeModbus
    eBTEnable();
    return 0;
}

/**
  * @brief  FreeModBus ��ѭ��
  * @param  None
  * @retval None
  */
int
btbus_main(void)
{
    do
    {
        //����ѭ��
        eBTPoll();
    }
    while (0);
    return 0;
}

/**
  * @brief  ���ּĴ��������������ּĴ����ɶ����ɶ���д
  * @param  pucRegBuffer  ������ʱ--��������ָ�룬д����ʱ--��������ָ��
  *         usAddress     �Ĵ�����ʼ��ַ
  *         usNRegs       �Ĵ�������
  *         eMode         ������ʽ��������д
  * @retval eStatus       �Ĵ���״̬
  */
eBTErrorCode
eBTRegReportingCB( UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                   eBTRegisterMode eMode )
{
    //����״̬
    eBTErrorCode    eStatus = BT_ENOERR;
    //ƫ����
    int16_t         iRegIndex = 0;
    //���ݳ���
    int16_t         iRegLength = usNRegs;

    //�жϼĴ����ǲ����ڷ�Χ��
    if( ( usAddress >= REG_HOLDING_START ) && \
        ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        //����ƫ����
        iRegIndex = ( int16_t )( usAddress + usRegHoldingStart );

        switch ( eMode )
        {
            //��������
        case BT_REG_READ:
            while( iRegLength > 0 )
            {
                *pucRegBuffer++ = ( uint8_t )( ucRegHoldingBuf[iRegIndex] & 0xFF );
                iRegIndex++;
                iRegLength--;
            }
            break;

            //д������
        case BT_REG_WRITE:
            while( iRegLength > 0 )
            {
                ucRegHoldingBuf[iRegIndex] = *pucRegBuffer++;
                iRegIndex++;
                iRegLength--;
            }
            break;
        }
        printf("Hold\r\n");
    }
    else
    {
        //���ش���״̬
        eStatus = BT_ENOREG;
    }

    return eStatus;
}

#ifdef __cplusplus
//}
#endif

#endif /* BTBUS_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

