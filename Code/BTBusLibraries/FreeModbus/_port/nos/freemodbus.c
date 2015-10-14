/**
  ******************************************************************************
  * @file              : freemodbus.c
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
#ifndef FREEMODBUS_C
#define FREEMODBUS_C

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
#include ".\freemodbus.h"
// #include "mb.h"
// #include "mbutils.h"
#include "./interface.h"

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif

/* Private macro -------------------------------------------------------------*/
//LED����
#define LED1_OFF()          LED1.Switch(true)
#define LED1_ON()           LED1.Switch(true)

#define LED2_OFF()          LED2.Switch(true)
#define LED2_ON()           LED2.Switch(true)

#define LED3_OFF()          LED3.Switch(true)
#define LED3_ON()           LED3.Switch(true)

#define LED4_OFF()          LED4.Switch(true)
#define LED4_ON()           LED4.Switch(true)

//���뿪�ؿ���
#define BUTTON1_READ()      (BOOL)KEY1
#define BUTTON2_READ()      (BOOL)KEY2
#define BUTTON3_READ()      (BOOL)KEY3
#define BUTTON4_READ()      (BOOL)KEY4
#define BUTTON5_READ()      false
#define BUTTON6_READ()      false
#define BUTTON7_READ()      false
#define BUTTON8_READ()      false

//�̵�������
#define Relay1_ON()
#define Relay1_OFF()

#define Relay2_ON()
#define Relay2_OFF()

#define Relay3_ON()
#define Relay3_OFF()

#define Relay4_ON()
#define Relay4_OFF()

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
//����Ĵ�������
uint16_t usRegInputBuf[REG_INPUT_NREGS];

//�Ĵ�����ʼ��ַ
uint16_t usRegInputStart = REG_INPUT_START;

//���ּĴ�������
uint16_t usRegHoldingBuf[REG_HOLDING_NREGS];

//���ּĴ�����ʼ��ַ
uint16_t usRegHoldingStart = REG_HOLDING_START;

//��Ȧ״̬
uint8_t ucRegCoilsBuf [REG_COILS_SIZE / 8] = {0x01, 0x00, };
uint8_t ucRegCoilsRerv[REG_COILS_SIZE / 8] = {0x5A, 0x5F, };

//��������״̬
uint8_t ucRegDiscreteBuf [REG_DISCRETE_SIZE / 8] = {0x00, 0x01, };
uint8_t ucRegDiscreteRerv[REG_DISCRETE_SIZE / 8] = {0xA5, 0xAF, };

/* Private function prototypes -----------------------------------------------*/
//!
void LED_Config(void);
void LED_Poll(void);
//!
void Button_Config(void);
void Button_Poll(void);
//!
void Relay_Config(void);
void Relay_Poll(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  FreeModBus ��ʼ��
  * @param  None
  * @retval None
  */
int
freemodbus_init(void)
{
    LED_Config();
    Button_Config();
    //Relay_Config();
    // ��ʼ��ģʽ��RTUģʽ
    // �ӻ���ַ��Ϊ'1'
    // �˿ڣ�COM1
    // ������9600 ��У��
    eMBInit(MB_RTU, MB_DefaultSlaveAddress, 1, MB_DefaultDeviceBaudRate, MB_PAR_NONE);
    //����FreeModbus
    eMBEnable();
    //
	EEPROM_DATA_READ((uint8_t *)&usRegHoldingBuf[0],
			usRegHoldingStart*2, REG_HOLDING_NREGS*2);
    return 0;
}
/**
  * @brief  FreeModBus ��ѭ��
  * @param  None
  * @retval None
  */
int
freemodbus_main(void)
{
    do
    {
        //����ѭ��
        eMBPoll();
        //LED ����ѭ��
        LED_Poll();
        //
        Button_Poll();
        //
        Relay_Poll();
    }
    while (0);
    return 0;
}

/**
  * @brief  ����Ĵ���������������Ĵ����ɶ���������д��
  * @param  pucRegBuffer  ��������ָ��
  *         usAddress     �Ĵ�����ʼ��ַ
  *         usNRegs       �Ĵ�������
  * @retval eStatus       �Ĵ���״̬
  */
eMBErrorCode
eMBRegInputCB( UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int16_t         iRegIndex;

    //��ѯ�Ƿ��ڼĴ�����Χ��
    //Ϊ�˱��⾯�棬�޸�Ϊ�з�������
    if( ( (int16_t)usAddress >= REG_INPUT_START ) \
            && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        //��ò���ƫ���������β�����ʼ��ַ-����Ĵ����ĳ�ʼ��ַ
        iRegIndex = ( int16_t )( usAddress - usRegInputStart );
		MB_STATUS_IND(MB_STA_IND_RESV1, TRUE);
		FRAM_DATA_READ((uint8_t *)&usRegInputBuf[iRegIndex], usAddress*2, usNRegs*2);
		MB_STATUS_IND(MB_STA_IND_RESV1, FALSE);
        //�����ֵ
        while( usNRegs > 0 )
        {
            //��ֵ���ֽ�
            *pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] >> 8 );
            //��ֵ���ֽ�
            *pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] & 0xFF );
            //ƫ��������
            iRegIndex++;
            //�������Ĵ��������ݼ�
            usNRegs--;
        }
        printf("Input\r\n");
    }
    else
    {
        //���ش���״̬���޼Ĵ���
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

/**
  * @brief  ���ּĴ��������������ּĴ����ɶ����ɶ���д
  * @param  pucRegBuffer  ������ʱ--��������ָ�룬д����ʱ--��������ָ��
  *         usAddress     �Ĵ�����ʼ��ַ
  *         usNRegs       �Ĵ�������
  *         eMode         ������ʽ��������д
  * @retval eStatus       �Ĵ���״̬
  */
eMBErrorCode
eMBRegHoldingCB( UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
    //����״̬
    eMBErrorCode    eStatus = MB_ENOERR;
    //ƫ����
    int16_t         iRegIndex;
    //���ݳ���
    int16_t         iRegLength = usNRegs;

    //�жϼĴ����ǲ����ڷ�Χ��
    if( ( (int16_t)usAddress >= REG_HOLDING_START ) \
            && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        //����ƫ����
        iRegIndex = ( int16_t )( usAddress - usRegHoldingStart );

        switch ( eMode )
        {
            //��������
        case MB_REG_READ:
            //CPAL_Read((uint8_t *)&usRegHoldingBuf[usAddress - usRegHoldingStart],
            //          usAddress, iRegLength);
			EEPROM_DATA_READ((uint8_t *)&usRegHoldingBuf[usAddress - usRegHoldingStart],
					usAddress*2, iRegLength*2);
            while( usNRegs > 0 )
            {
                *pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] >> 8 );
                *pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
            break;

            //д������
        case MB_REG_WRITE:
            while( usNRegs > 0 )
            {
                usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
            //CPAL_Write((uint8_t *)&usRegHoldingBuf[usAddress - usRegHoldingStart],
            //           usAddress, iRegLength);
			EEPROM_DATA_WRITE((uint8_t *)&usRegHoldingBuf[usAddress - usRegHoldingStart],
					usAddress*2, iRegLength*2);
            break;
        }
        printf("Hold\r\n");
    }
    else
    {
        //���ش���״̬
        eStatus = MB_ENOREG;
    }

    return eStatus;
}


/**
  * @brief  ��Ȧ�Ĵ�������������Ȧ�Ĵ����ɶ����ɶ���д
  * @param  pucRegBuffer  ������---��������ָ�룬д����--��������ָ��
  *         usAddress     �Ĵ�����ʼ��ַ
  *         usNRegs       �Ĵ�������
  *         eMode         ������ʽ��������д
  * @retval eStatus       �Ĵ���״̬
  */
eMBErrorCode
eMBRegCoilsCB( UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
    //����״̬
    eMBErrorCode    eStatus = MB_ENOERR;
    //�Ĵ�������
    int16_t         iNCoils = ( int16_t )usNCoils;
    //�Ĵ���ƫ����
    int16_t         usBitOffset;

    //���Ĵ����Ƿ���ָ����Χ��
    if( ( (int16_t)usAddress >= REG_COILS_START ) &&
            ( usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE ) )
    {
        //����Ĵ���ƫ����
        usBitOffset = ( int16_t )( usAddress - REG_COILS_START );
        switch ( eMode )
        {
            //������
        case MB_REG_READ:
            while( iNCoils > 0 )
            {
                *pucRegBuffer++ = xMBUtilGetBits( ucRegCoilsBuf, usBitOffset,
                                                  ( uint8_t )( iNCoils > 8 ? 8 : iNCoils ) );
                iNCoils -= 8;
                usBitOffset += 8;
            }
            break;

            //д����
        case MB_REG_WRITE:
            while( iNCoils > 0 )
            {
                xMBUtilSetBits( ucRegCoilsBuf, usBitOffset,
                                ( uint8_t )( iNCoils > 8 ? 8 : iNCoils ),
                                *pucRegBuffer++ );
                iNCoils -= 8;
                usBitOffset += 8;
            }
            break;
        }
        printf("Coils\r\n");
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    //����״̬
    eMBErrorCode    eStatus = MB_ENOERR;
    //�����Ĵ�������
    int16_t         iNDiscrete = ( int16_t )usNDiscrete;
    //ƫ����
    uint16_t        usBitOffset;

    //�жϼĴ���ʱ�����ƶ���Χ��
    if( ( (int16_t)usAddress >= REG_DISCRETE_START ) &&
            ( usAddress + usNDiscrete <= REG_DISCRETE_START + REG_DISCRETE_SIZE ) )
    {
        //���ƫ����
        usBitOffset = ( uint16_t )( usAddress - REG_DISCRETE_START );

        while( iNDiscrete > 0 )
        {
            *pucRegBuffer++ = xMBUtilGetBits( ucRegDiscreteBuf, usBitOffset,
                                              ( uint8_t)( iNDiscrete > 8 ? 8 : iNDiscrete ) );
            iNDiscrete -= 8;
            usBitOffset += 8;
        }
        printf("Discrete\r\n");
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

/**
  * @brief  LED��ʼ��
  * @param  None
  * @retval None
  */
void LED_Config(void)
{
	
}

/**
  * @brief  LEDѭ�����
  * @param  None
  * @retval None
  */
void LED_Poll(void)
{
    uint32_t nIndex, nBit = 0x1;
    uint8_t  LED_Status = 0;
    uint16_t Coil_Status;
    Coil_Status   = ucRegCoilsBuf[1];		//MSB
    Coil_Status <<= 8;
    Coil_Status  |= ucRegCoilsBuf[0];		//LSB

    for (nIndex = 0; nIndex < 16; nIndex ++)
    {
        if(Coil_Status & nBit)
        {
            LED_Status = nIndex;
            break;
        }
        nBit <<= 1;
    }
//	LED1 = (BOOL)((LED_Status & 0x01) ? true : false);
//	LED2 = (BOOL)((LED_Status & 0x02) ? true : false);
//	LED3 = (BOOL)((LED_Status & 0x04) ? true : false);
//	LED4 = (BOOL)((LED_Status & 0x08) ? true : false);
	(void) LED_Status;
}

/**
  * @brief  ���뿪�س�ʼ��
  * @param  None
  * @retval None
  */
void Button_Config(void)
{
	
}

/**
  * @brief  ���뿪�ؼ��ѭ��
  * @param  None
  * @retval None
  */
void Button_Poll(void)
{
    uint8_t Button_Status = 0x00;

//	KEY1 ? (Button_Status |= 0x01) : (Button_Status &= ~ 0x01);
//	KEY2 ? (Button_Status |= 0x02) : (Button_Status &= ~ 0x02);
//	KEY3 ? (Button_Status |= 0x04) : (Button_Status &= ~ 0x04);
//	KEY4 ? (Button_Status |= 0x08) : (Button_Status &= ~ 0x08);

    ucRegDiscreteBuf[0] = Button_Status;
}


void Relay_Config(void)
{

}


void Relay_Poll(void)
{

    uint8_t Relay_Status = ucRegCoilsBuf[0];

    if(Relay_Status & 0x01)
    {
        Relay1_ON();
    }
    else
    {
        Relay1_OFF();
    }
    if(Relay_Status & 0x02)
    {
        Relay2_ON();
    }
    else
    {
        Relay2_OFF();
    }
    if(Relay_Status & 0x04)
    {
        Relay3_ON();
    }
    else
    {
        Relay3_OFF();
    }
    if(Relay_Status & 0x08)
    {
        Relay4_ON();
    }
    else
    {
        Relay4_OFF();
    }
}

#ifdef __cplusplus
PR_END_EXTERN_C
#endif

#ifdef __cplusplus
//}
#endif

#endif /* FREEMODBUS_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

