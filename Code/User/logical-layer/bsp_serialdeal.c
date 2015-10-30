/**
  ******************************************************************************
  * @file              : bsp_serialdeal.c
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
static void ReadBatVoltage(uint16_t value);

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
  * @brief  Description ��ȡ�����������ݵ�ַ��ʱ����Fram�е�ص�ѹ
  * @param  value       ��ص�ѹ
  * @retval ��
  *****************************************************************************/
static void ReadBatVoltage(uint16_t value)
{
	Conf.Basic.BatVoltage.hwd = value;//�����ڴ�
	Fram_Write(Conf.Basic.BatVoltage.byte,FRAM_BatVoltageAddr,2);//��ص���д��Fram��
}
/******************************************************************************
  * @brief  Description д�������ݵ�Fram�У�ͬʱ�����ڴ��е�����
  * @param  pucBuffer   Ҫд������ָ��
  * @param  usAddress	Ҫд�����ݵ�ַ
  * @param  usNRegs		д����������
  * @retval ��
  *****************************************************************************/
bool PARAM_DATA_WRITE(uint8_t *pucBuffer, USHORT usAddress, USHORT usNRegs)
{
	uint8_t  offsetnum=0;	//ƫ�Ƽ��������ַ
	uint16_t size = usNRegs * 2;
	uint16_t offset = 0;
	
	if (usAddress < VirtJlyConfAddr)	//0x0100
	{
		/* �����������ݵ�ַ�� */
		offset = usAddress - VirtBasicConfAddr;
		/*д���ñ����ݵ�Fram��ÿ����ַ�������ֽ����ݣ�offset * 2*/
		Fram_Write(pucBuffer,ConfMap_Address[0][1] + offset * 2,size);
//		Fram_Read(Buf,ConfMap_Address[0][1] + offset * 2,size);
		/*�������ñ����� */
		Fram_Read(Conf.Buf,ConfMap_Address[0][1] + offset * 2,size);
	} else if (usAddress < VirJlyTimeConfAddr)
	{
		/* ��¼���������ݵ�ַ�� */
		offset = usAddress - VirtJlyConfAddr;
		Fram_Write(pucBuffer, ConfMap_Address[1][1] + offset * 2, size);
		Fram_Read(&Conf.Buf[FRAM_JlyConfAddr],ConfMap_Address[1][1] + offset * 2,size);
		/*��¼����仯ʱ����Ӧ����*/
		//if(Conf.)
		
	}else if (usAddress < VirtAlarmConfAddr)
	{
		/*��¼��ʵʱʱ���ַ��*/
		offset = usAddress - VirJlyTimeConfAddr;
		Fram_Write(pucBuffer, ConfMap_Address[2][1] + offset * 2, size);
		Fram_Read(&Conf.Buf[FRAM_JlyTimeConfAddr],ConfMap_Address[2][1] + offset * 2,size);
		set_time();	/*��������ʱ��*/
		displayTIME(Conf.Time.Time_Hour,Conf.Time.Time_Min);
	}
	else if (usAddress < VirtSensorChanelConfAddr)
	{
		/* �����������ݵ�ַ�� */ 
		offset = usAddress - VirtAlarmConfAddr;
		Fram_Write(pucBuffer, ConfMap_Address[3][1] + offset * 2, size);
		Fram_Read(&Conf.Buf[FRAM_AlarmConfAddr],ConfMap_Address[3][1] + offset * 2,size);
		
	} else if (usAddress < VirtTempHumiAdjustConfAddr)
	{
		/* ������ͨ���������ݵ�ַ��  */
		// ͨ��0����
		offsetnum = (usAddress - VirtSensorChanelConfAddr) / VirtOffset;	/* ƫ�ƶ��ٸ������ַ */
		offset = offsetnum * FRAM_SensorChanelOffset;						/* Fram�е�ƫ���� */
		Fram_Write(pucBuffer, ConfMap_Address[4][1] + offset * 2, size);
		Fram_Read(&Conf.Buf[FRAM_SensorChanelConfAddr + offset * 2],ConfMap_Address[4][1] + offset * 2,size);
		
	} else if (usAddress < VirtMax)
	{
		/* ��ʪ�ȴ�����У׼�������ݵ�ַ�� */
		offsetnum = (usAddress - VirtTempHumiAdjustConfAddr) / VirtOffset;
		offset = offsetnum * FRAM_TempHumiAdjustOffset;
		Fram_Write(pucBuffer, ConfMap_Address[5][1] + offset * 2, size);
		Fram_Read(&Conf.Buf[FRAM_TempHumiAdjustConfAddr + offset * 2],ConfMap_Address[5][1] + offset * 2,size);
	}
	return true;
}
/******************************************************************************
  * @brief  Description ��ȡ����������Ϣ
  * @param  pucBuffer   ��Ŷ������ݵ�ָ��
  * @param  usAddress	�������ݵĵ�ַ
  * @param  usNRegs		��������
  * @retval ��
  *****************************************************************************/
bool PARAM_DATA_READ(uint8_t *pucBuffer, USHORT usAddress, USHORT usNRegs)
{
	uint8_t  offsetnum=0;	//ƫ�Ƽ��������ַ
	//uint8_t	 frambuf[128];
	uint16_t size = usNRegs * 2;
	uint16_t offset = 0;
	
	if (usAddress < VirtJlyConfAddr)	//0x0100
	{
		/* �����������ݵ�ַ�� */
		offset = usAddress - VirtBasicConfAddr;
		ReadBatVoltage(PManage.BatVoltage);//����Fram�е�ص���
		Fram_Read(pucBuffer,ConfMap_Address[0][1] + offset * 2,size);
//		Fram_Read(Buf,ConfMap_Address[0][1] + offset * 2,size);
	} else if (usAddress < VirJlyTimeConfAddr)
	{
		
		/* ��¼���������ݵ�ַ�� */
		offset = usAddress - VirtJlyConfAddr;
		Fram_Read(pucBuffer, ConfMap_Address[1][1] + offset * 2, size);
		
	} else if (usAddress < VirtAlarmConfAddr)
	{
		/*��¼��ʵʱʱ���ַ��*/
		offset = usAddress - VirJlyTimeConfAddr;
		serialread_time();
		Fram_Read(pucBuffer, ConfMap_Address[2][1] + offset * 2, size);
	}
	else if (usAddress < VirtSensorChanelConfAddr)
	{
		/* �����������ݵ�ַ�� */ 
		offset = usAddress - VirtAlarmConfAddr;
		Fram_Read(pucBuffer, ConfMap_Address[3][1] + offset * 2, size);
		//Fram_Read(frambuf, ConfMap_Address[3][1] + offset * 2, size);
		
	} else if (usAddress < VirtTempHumiAdjustConfAddr)
	{
		/* ������ͨ���������ݵ�ַ��  */
		// ͨ��0����
		offsetnum = (usAddress - VirtSensorChanelConfAddr) / VirtOffset;	/* ƫ�ƶ��ٸ������ַ */
		offset = offsetnum * FRAM_SensorChanelOffset;						/* Fram�е�ƫ���� */
		Fram_Read(pucBuffer, ConfMap_Address[4][1] + offset * 2, size);
		
	} else if (usAddress < VirtMax)
	{
		/* ��ʪ�ȴ�����У׼�������ݵ�ַ�� */
		offsetnum = (usAddress - VirtTempHumiAdjustConfAddr) / VirtOffset;
		offset = offsetnum * FRAM_TempHumiAdjustOffset;
		Fram_Read(pucBuffer, ConfMap_Address[5][1] + offset * 2, size);
	}
	return true;
}
/******************************************************************************
  * @brief  Description 
  * @param  ��
  * @retval ��
  *****************************************************************************/
void JlySerialDeal(void)
{
	
}
#endif /* __BSPSERIAL_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
