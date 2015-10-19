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
  * @param  ��
  * @retval ��
  */
bool PARAM_DATA_WRITE(uint8_t *pucBuffer, USHORT usAddress, USHORT usNRegs)
{
	uint8_t  offsetnum=0;	//ƫ�Ƽ��������ַ
	uint16_t size = usNRegs * 2;
	uint16_t offset = 0;
	
	if (usAddress < VirtJlyConfAddr)	//0x0100
	{
		/*!< �����������ݵ�ַ�� */
		offset = usAddress - VirtBasicConfAddr;
		Fram_Write(pucBuffer,ConfMap_Address[0][1] + offset * 2,size);
//		Fram_Read(Buf,ConfMap_Address[0][1] + offset * 2,size);
	} else if (usAddress < VirtAlarmConfAddr)
	{
		/*!< ��¼���������ݵ�ַ�� */
		offset = usAddress - VirtJlyConfAddr;
		Fram_Write(pucBuffer, ConfMap_Address[1][1] + offset * 2, size);
	} else if (usAddress < VirtSensorChanelConfAddr)
	{
		/*!< �����������ݵ�ַ�� */ 
		offset = usAddress - VirtAlarmConfAddr;
		Fram_Write(pucBuffer, ConfMap_Address[2][1] + offset * 2, size);
		
	} else if (usAddress < VirtTempHumiAdjustConfAddr)
	{
		/*!< ������ͨ���������ݵ�ַ��  */
		// ͨ��0����
		offsetnum = (usAddress - VirtSensorChanelConfAddr) / VirtOffset;	/*!< ƫ�ƶ��ٸ������ַ */
		offset = offsetnum * FRAM_SensorChanelOffset;						/*!< Fram�е�ƫ���� */
		Fram_Write(pucBuffer, ConfMap_Address[3][1] + offset * 2, size);
		
	} else if (usAddress < VirtMax)
	{
		/*!< ��ʪ�ȴ�����У׼�������ݵ�ַ�� */
		offsetnum = (usAddress - VirtTempHumiAdjustConfAddr) / VirtOffset;
		offset = offsetnum * FRAM_TempHumiAdjustOffset;
		Fram_Write(pucBuffer, ConfMap_Address[4][1] + offset * 2, size);
	}
	return true;
}
/**
  * @brief  Description 
  * @param  ��
  * @retval ��
  */
bool PARAM_DATA_READ(uint8_t *pucBuffer, USHORT usAddress, USHORT usNRegs)
{
	uint8_t  offsetnum=0;	//ƫ�Ƽ��������ַ
	uint8_t	 buf[128];
	uint16_t size = usNRegs * 2;
	uint16_t offset = 0;
	
	if (usAddress < VirtJlyConfAddr)	//0x0100
	{
		/*!< �����������ݵ�ַ�� */
		offset = usAddress - VirtBasicConfAddr;
		Fram_Read(pucBuffer,ConfMap_Address[0][1] + offset * 2,size);
//		Fram_Read(Buf,ConfMap_Address[0][1] + offset * 2,size);
	} else if (usAddress < VirtAlarmConfAddr)
	{
		/*!< ��¼���������ݵ�ַ�� */
		offset = usAddress - VirtJlyConfAddr;
		Fram_Read(pucBuffer, ConfMap_Address[1][1] + offset * 2, size);
	} else if (usAddress < VirtSensorChanelConfAddr)
	{
		/*!< �����������ݵ�ַ�� */ 
		offset = usAddress - VirtAlarmConfAddr;
		Fram_Read(pucBuffer, ConfMap_Address[2][1] + offset * 2, size);
		Fram_Read(buf, ConfMap_Address[2][1] + offset * 2, size);
		
	} else if (usAddress < VirtTempHumiAdjustConfAddr)
	{
		/*!< ������ͨ���������ݵ�ַ��  */
		// ͨ��0����
		offsetnum = (usAddress - VirtSensorChanelConfAddr) / VirtOffset;	/*!< ƫ�ƶ��ٸ������ַ */
		offset = offsetnum * FRAM_SensorChanelOffset;						/*!< Fram�е�ƫ���� */
		Fram_Write(pucBuffer, ConfMap_Address[3][1] + offset * 2, size);
		
	} else if (usAddress < VirtMax)
	{
		/*!< ��ʪ�ȴ�����У׼�������ݵ�ַ�� */
		offsetnum = (usAddress - VirtTempHumiAdjustConfAddr) / VirtOffset;
		offset = offsetnum * FRAM_TempHumiAdjustOffset;
		Fram_Write(pucBuffer, ConfMap_Address[4][1] + offset * 2, size);
	}
	return true;
}
/**
  * @brief  Description 
  * @param  ��
  * @retval ��
  */
void JlySerialDeal(void)
{
	
}
#endif /* __BSPSERIAL_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/