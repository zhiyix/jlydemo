/**
  ******************************************************************************
  * @file              : bsp_alarmdeal.h
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
#ifndef __BSPALARM_H
#define __BSPALARM_H

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
void BellNn(uint8_t n);
void BellNn_longer(uint8_t n);
void AlarmDeal(uint8_t channel);
//#ifdef __cplusplus
//}
//#endif

#endif /* bsp_alarmdeal.h */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

