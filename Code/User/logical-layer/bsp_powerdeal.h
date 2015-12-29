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
//! @}

/*============================ TYPES =========================================*/
//!

/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/

/*============================ EXTERN FUNCTIONS ==============================*/
void BatteryVoltageDeal(void);
void FirstCheckExternPower(void);
void ExternalPowerDetection(void);
//#ifdef __cplusplus
//}
//#endif

#endif /* bsp_powerdeal.h */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

