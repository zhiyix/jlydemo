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
#ifndef __BSPCLASS_H
#define __BSPCLASS_H

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
// MCU:
/*============================ EXTERN FUNCTIONS ==============================*/

uint16_t ValueFtToU16(float FtTmp);
uint16_t U16ToValue10(uint8_t high,uint8_t low);
unsigned int  Char_to_Int(unsigned int high,unsigned int low);
int hex_2_ascii(uint8_t *data, uint8_t *buffer, uint16_t len);
//#ifdef __cplusplus
//}
//#endif

#endif /* __BSPCLASS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
