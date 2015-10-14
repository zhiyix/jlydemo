/**
  ******************************************************************************
  * @file              : 
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
#ifndef __BSPTIMER_H
#define __BSPTIMER_H

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
//! \brief 1s÷–∂œ“ª¥Œ
#define TIM2_Prescaler 999
#define TIM2_Delay     32000 
//! @}

/*============================ TYPES =========================================*/
//!


/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/
// MCU:
/*============================ EXTERN FUNCTIONS ==============================*/

void TIM2_Configuration(void);
//#ifdef __cplusplus
//}
//#endif

#endif /* __BSPTIMER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
