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
//LED控制
#define LED1_OFF()          LED1.Switch(true)
#define LED1_ON()           LED1.Switch(true)

#define LED2_OFF()          LED2.Switch(true)
#define LED2_ON()           LED2.Switch(true)

#define LED3_OFF()          LED3.Switch(true)
#define LED3_ON()           LED3.Switch(true)

#define LED4_OFF()          LED4.Switch(true)
#define LED4_ON()           LED4.Switch(true)

//拨码开关控制
#define BUTTON1_READ()      (BOOL)KEY1
#define BUTTON2_READ()      (BOOL)KEY2
#define BUTTON3_READ()      (BOOL)KEY3
#define BUTTON4_READ()      (BOOL)KEY4
#define BUTTON5_READ()      false
#define BUTTON6_READ()      false
#define BUTTON7_READ()      false
#define BUTTON8_READ()      false

//继电器控制
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
//输入寄存器内容
uint16_t usRegInputBuf[REG_INPUT_NREGS];

//寄存器起始地址
uint16_t usRegInputStart = REG_INPUT_START;

//保持寄存器内容
uint16_t usRegHoldingBuf[REG_HOLDING_NREGS];

//保持寄存器起始地址
uint16_t usRegHoldingStart = REG_HOLDING_START;

//线圈状态
uint8_t ucRegCoilsBuf [REG_COILS_SIZE / 8] = {0x01, 0x00, };
uint8_t ucRegCoilsRerv[REG_COILS_SIZE / 8] = {0x5A, 0x5F, };

//开关输入状态
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
  * @brief  FreeModBus 初始化
  * @param  None
  * @retval None
  */
int
freemodbus_init(void)
{
    LED_Config();
    Button_Config();
    //Relay_Config();
    // 初始化模式：RTU模式
    // 从机地址：为'1'
    // 端口：COM1
    // 参数：9600 无校验
    eMBInit(MB_RTU, MB_DefaultSlaveAddress, 1, MB_DefaultDeviceBaudRate, MB_PAR_NONE);
    //启动FreeModbus
    eMBEnable();
    //
	EEPROM_DATA_READ((uint8_t *)&usRegHoldingBuf[0],
			usRegHoldingStart*2, REG_HOLDING_NREGS*2);
    return 0;
}
/**
  * @brief  FreeModBus 主循环
  * @param  None
  * @retval None
  */
int
freemodbus_main(void)
{
    do
    {
        //不断循环
        eMBPoll();
        //LED 控制循环
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
  * @brief  输入寄存器处理函数，输入寄存器可读，但不可写。
  * @param  pucRegBuffer  返回数据指针
  *         usAddress     寄存器起始地址
  *         usNRegs       寄存器长度
  * @retval eStatus       寄存器状态
  */
eMBErrorCode
eMBRegInputCB( UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int16_t         iRegIndex;

    //查询是否在寄存器范围内
    //为了避免警告，修改为有符号整数
    if( ( (int16_t)usAddress >= REG_INPUT_START ) \
            && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        //获得操作偏移量，本次操作起始地址-输入寄存器的初始地址
        iRegIndex = ( int16_t )( usAddress - usRegInputStart );
		MB_STATUS_IND(MB_STA_IND_RESV1, TRUE);
		FRAM_DATA_READ((uint8_t *)&usRegInputBuf[iRegIndex], usAddress*2, usNRegs*2);
		MB_STATUS_IND(MB_STA_IND_RESV1, FALSE);
        //逐个赋值
        while( usNRegs > 0 )
        {
            //赋值高字节
            *pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] >> 8 );
            //赋值低字节
            *pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] & 0xFF );
            //偏移量增加
            iRegIndex++;
            //被操作寄存器数量递减
            usNRegs--;
        }
        printf("Input\r\n");
    }
    else
    {
        //返回错误状态，无寄存器
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

/**
  * @brief  保持寄存器处理函数，保持寄存器可读，可读可写
  * @param  pucRegBuffer  读操作时--返回数据指针，写操作时--输入数据指针
  *         usAddress     寄存器起始地址
  *         usNRegs       寄存器长度
  *         eMode         操作方式，读或者写
  * @retval eStatus       寄存器状态
  */
eMBErrorCode
eMBRegHoldingCB( UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
    //错误状态
    eMBErrorCode    eStatus = MB_ENOERR;
    //偏移量
    int16_t         iRegIndex;
    //数据长度
    int16_t         iRegLength = usNRegs;

    //判断寄存器是不是在范围内
    if( ( (int16_t)usAddress >= REG_HOLDING_START ) \
            && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        //计算偏移量
        iRegIndex = ( int16_t )( usAddress - usRegHoldingStart );

        switch ( eMode )
        {
            //读处理函数
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

            //写处理函数
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
        //返回错误状态
        eStatus = MB_ENOREG;
    }

    return eStatus;
}


/**
  * @brief  线圈寄存器处理函数，线圈寄存器可读，可读可写
  * @param  pucRegBuffer  读操作---返回数据指针，写操作--返回数据指针
  *         usAddress     寄存器起始地址
  *         usNRegs       寄存器长度
  *         eMode         操作方式，读或者写
  * @retval eStatus       寄存器状态
  */
eMBErrorCode
eMBRegCoilsCB( UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
    //错误状态
    eMBErrorCode    eStatus = MB_ENOERR;
    //寄存器个数
    int16_t         iNCoils = ( int16_t )usNCoils;
    //寄存器偏移量
    int16_t         usBitOffset;

    //检查寄存器是否在指定范围内
    if( ( (int16_t)usAddress >= REG_COILS_START ) &&
            ( usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE ) )
    {
        //计算寄存器偏移量
        usBitOffset = ( int16_t )( usAddress - REG_COILS_START );
        switch ( eMode )
        {
            //读操作
        case MB_REG_READ:
            while( iNCoils > 0 )
            {
                *pucRegBuffer++ = xMBUtilGetBits( ucRegCoilsBuf, usBitOffset,
                                                  ( uint8_t )( iNCoils > 8 ? 8 : iNCoils ) );
                iNCoils -= 8;
                usBitOffset += 8;
            }
            break;

            //写操作
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
    //错误状态
    eMBErrorCode    eStatus = MB_ENOERR;
    //操作寄存器个数
    int16_t         iNDiscrete = ( int16_t )usNDiscrete;
    //偏移量
    uint16_t        usBitOffset;

    //判断寄存器时候再制定范围内
    if( ( (int16_t)usAddress >= REG_DISCRETE_START ) &&
            ( usAddress + usNDiscrete <= REG_DISCRETE_START + REG_DISCRETE_SIZE ) )
    {
        //获得偏移量
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
  * @brief  LED初始化
  * @param  None
  * @retval None
  */
void LED_Config(void)
{
	
}

/**
  * @brief  LED循环检测
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
  * @brief  拨码开关初始化
  * @param  None
  * @retval None
  */
void Button_Config(void)
{
	
}

/**
  * @brief  拨码开关检测循环
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

