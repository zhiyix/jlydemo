/**
  ******************************************************************************
  * @file              : bsp_bootstop.h
  * @author            : 
  * @version           : 
  * @date              : 
  * @brief             : 记录仪启动方式,停止方式处理 h file
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
#ifndef __BSPBOOTSTOP_H
#define __BSPBOOTSTOP_H

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

void RecorderBootModeHandle(void);
void RecorderStopModeHandle(void);

//#ifdef __cplusplus
//}
//#endif

#endif /* bsp_bootstop.h */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

