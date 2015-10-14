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
//输入寄存器内容
uint16_t usRegInputBuf[REG_INPUT_NREGS];

//寄存器起始地址
uint16_t usRegInputStart = REG_INPUT_START;

//保持寄存器内容
uint8_t  ucRegHoldingBuf[REG_HOLDING_NREGS];

//保持寄存器起始地址
uint16_t usRegHoldingStart = REG_HOLDING_START;

//线圈状态
uint8_t ucRegCoilsBuf [REG_COILS_SIZE / 8] = {0x01, 0x00, };
uint8_t ucRegCoilsRerv[REG_COILS_SIZE / 8] = {0x5A, 0x5F, };

//开关输入状态
uint8_t ucRegDiscreteBuf [REG_DISCRETE_SIZE / 8] = {0x00, 0x01, };
uint8_t ucRegDiscreteRerv[REG_DISCRETE_SIZE / 8] = {0xA5, 0xAF, };

/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
/**
  * @brief  FreeModBus 初始化
  * @param  None
  * @retval None
  */
int
btbus_init(void)
{
    //Relay_Config();
    // 初始化模式：RTU模式
    // 从机地址：为'1'
    // 端口：COM1
    // 参数：9600 无校验
    eBTInit(BT_RTU, BT_DefaultSlaveAddress, 1, BT_DefaultDeviceBaudRate, BT_PAR_NONE);
    //启动FreeModbus
    eBTEnable();
    return 0;
}

/**
  * @brief  FreeModBus 主循环
  * @param  None
  * @retval None
  */
int
btbus_main(void)
{
    do
    {
        //不断循环
        eBTPoll();
    }
    while (0);
    return 0;
}

/**
  * @brief  保持寄存器处理函数，保持寄存器可读，可读可写
  * @param  pucRegBuffer  读操作时--返回数据指针，写操作时--输入数据指针
  *         usAddress     寄存器起始地址
  *         usNRegs       寄存器长度
  *         eMode         操作方式，读或者写
  * @retval eStatus       寄存器状态
  */
eBTErrorCode
eBTRegReportingCB( UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                   eBTRegisterMode eMode )
{
    //错误状态
    eBTErrorCode    eStatus = BT_ENOERR;
    //偏移量
    int16_t         iRegIndex = 0;
    //数据长度
    int16_t         iRegLength = usNRegs;

    //判断寄存器是不是在范围内
    if( ( usAddress >= REG_HOLDING_START ) && \
        ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        //计算偏移量
        iRegIndex = ( int16_t )( usAddress + usRegHoldingStart );

        switch ( eMode )
        {
            //读处理函数
        case BT_REG_READ:
            while( iRegLength > 0 )
            {
                *pucRegBuffer++ = ( uint8_t )( ucRegHoldingBuf[iRegIndex] & 0xFF );
                iRegIndex++;
                iRegLength--;
            }
            break;

            //写处理函数
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
        //返回错误状态
        eStatus = BT_ENOREG;
    }

    return eStatus;
}

#ifdef __cplusplus
//}
#endif

#endif /* BTBUS_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

