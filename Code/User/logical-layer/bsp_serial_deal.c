/**
  ******************************************************************************
  * @file              : bsp_serial_deal.c
  * @author            : 
  * @version           : 
  * @date              : 
  * @brief             : c file
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
#ifndef __BSPSERIAL_C
#define __BSPSERIAL_C

/*============================ INCLUDES ======================================*/
/* Includes ------------------------------------------------------------------*/

#include "main.h"

//	<<< end of configuration section >>>

/*============================ MACRO =========================================*/

/*============================ TYPES =========================================*/

/*============================ LOCAL VARIABLES ===============================*/

/*============================ GLOBAL VARIABLES ==============================*/

/*============================ EXTERN FUNCTIONS ==============================*/
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Description 
  * @param  无
  * @retval 无
  */
bool PARAM_DATA_WRITE(uint8_t *pucBuffer, USHORT usAddress, USHORT usNRegs)
{
	uint16_t size = usNRegs * 2;
	uint16_t offset = 0;
	
	if (usAddress < VirtJlyConfAddr)	//0x0100
	{
		Fram_Write(pucBuffer,ConfMap_Address[0][1],size);
		
	} else if (usAddress < 0x0200)
	{
		offset = usAddress - 0x0100;
//		Fram_Read(pucBuffer, map[0][1] + offset * 2, size);
	} else if (usAddress < 0x1000)
	{
		
	} else if (usAddress < 0x1080)
	{
		// 通道0配置
	} else if (usAddress < 0x2000)
	{
	
	}
	return true;
}
/**
  * @brief  Description 
  * @param  无
  * @retval 无
  */
void JlySerialDeal(void)
{
	
}
#endif /* __BSPSERIAL_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
