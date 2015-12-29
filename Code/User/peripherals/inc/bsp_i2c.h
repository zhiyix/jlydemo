/**
  ******************************************************************************
  * @file              : bsp_i2c.h
  * @author            : 
  * @version           : 
  * @date              : 
  * @brief             : h file
  * @description       : Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 201x STMicroelectronics
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSPI2C_H
#define __BSPI2C_H

//#ifdef __cplusplus
// extern "C" {
//#endif

/*============================ INCLUDES ======================================*/
//! @name include
//! @{
#include "stm32l1xx.h"

#include "stdbool.h"
//#include "stm32glibc_type.h"

//#include "stm32glibc_rcc.h"
//#include "stm32glibc_nvic.h"
//#include "stm32glibc_systick.h"
//#include "stm32glibc_tim.h"
//#include "stm32glibc_afio.h"
//#include "stm32glibc_gpio.h"
//#include "stm32glibc_usart.h"
//#include "stm32glibc_trace.h"
//#include "interface.h"
//! @}

/*============================ MACROS ========================================*/
//! @{
//! \brief ai2c_address_define 
//!
#define AI2C_FRAM_ADDRESS		0xA0
#define AI2C_PAGE_SIZE			128
#define AI2C_G_SDA			    0
#define AI2C_G_SCL			    0
#define AI2C_G_SDA_STR			"G_SDA"
#define AI2C_G_SCL_STR			"G_SCL"
//! \brief rx8025_address_define 
#define AI2C_RX8025_ADDRESS		0x64
//!
#define RCC_I2C_PORT			RCC_AHBPeriph_GPIOD
#define BSP_I2C_PORT			GPIOD
#define BSP_I2C_SCL				GPIO_Pin_3
#define BSP_I2C_SDA				GPIO_Pin_4
//!
//#define EM_SCL_H()         		I2CIO.SetBits(BSP_I2C_SCL)
//#define EM_SCL_L()         		I2CIO.ResetBits(BSP_I2C_SCL)
#define EM_SCL_H()         		GPIO_SetBits(BSP_I2C_PORT,BSP_I2C_SCL)
#define EM_SCL_L()         		GPIO_ResetBits(BSP_I2C_PORT,BSP_I2C_SCL)
//!
//#define EM_SDA_H()         		I2CIO.SetBits(BSP_I2C_SDA)
//#define EM_SDA_L()         		I2CIO.ResetBits(BSP_I2C_SDA)
#define EM_SDA_H()         		GPIO_SetBits(BSP_I2C_PORT,BSP_I2C_SDA)
#define EM_SDA_L()         		GPIO_ResetBits(BSP_I2C_PORT,BSP_I2C_SDA)
//! @}

/*============================ TYPES =========================================*/
//!
//typedef enum
//{
//  false = 0, true  = !false
//}
//bool;
/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/
extern uint8_t 	   uAddress ;

/*============================ EXTERN FUNCTIONS ==============================*/

bool Start(void);
bool Stop(void);
bool WaitAck(void);
void Ack(bool ack);
void SendByte(uint8_t chData);
void RecvByte(uint8_t *pData);

bool AI2C_Write(uint8_t *pData, uint16_t addr, uint16_t count);
bool AI2C_Read(uint8_t *pData, uint16_t addr, uint16_t count);

bool RTC8025_Write(uint8_t *pData, uint16_t addr, uint16_t count);
bool RTC8025_Read(uint8_t *pData, uint16_t addr, uint16_t count);
bool RTC8025_Reset(bool need_reset);
bool RTC8025_ClearDALE(bool need_reset);

bool Fram_Write(uint8_t *pData, uint16_t addr, uint16_t count);
bool Fram_Read(uint8_t *pData, uint16_t addr, uint16_t count);
//#ifdef __cplusplus
//}
//#endif

#endif /* DEV_I2C_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

