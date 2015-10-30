/**
  ******************************************************************************
  * @file              : bsp_powerdeal.h
  * @author            : 
  * @version           : 
  * @date              : 2015-10-xx
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
#ifndef __BSPPOWER_H
#define __BSPPOWER_H

//#ifdef __cplusplus
// extern "C" {
//#endif

/*============================ INCLUDES ======================================*/
//! @name include
//! @{
#include "stm32l1xx.h"
//! @}

/*============================ MACROS ========================================*/
//! @{
//! \brief 

#define Power_Deal_PORT 	GPIOF
/*指示 有效输入电源 ACOK*/
#define Power_ACtest_PIN 	GPIO_Pin_4
/*指示充电完成 CHGOK*/
#define Power_CHGtest_PIN 	GPIO_Pin_5
//! @}

/*============================ TYPES =========================================*/
//!

/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/

/*============================ EXTERN FUNCTIONS ==============================*/
void VoltageTest(void);
void FirstCheckExternPower(void);
void OutpowerShan(void);
//#ifdef __cplusplus
//}
//#endif

#endif /* bsp_powerdeal.h */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

