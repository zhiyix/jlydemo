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
		WriteSetFramFlag();//���ù�fram��־
		/*�������ñ����� */
		Fram_Read(Conf.Buf,ConfMap_Address[0][1] + offset * 2,size);
	} else if (usAddress < VirJlyTimeConfAddr)
	{
		/* ��¼���������ݵ�ַ�� */
		offset = usAddress - VirtJlyConfAddr;
		Fram_Write(pucBuffer, ConfMap_Address[1][1] + offset * 2, size);
		Fram_Read(&Conf.Buf[FRAM_JlyConfAddr],ConfMap_Address[1][1] + offset * 2,size);
		/*ͨ��������ͨ�����ͱ仯ʱ����Ӧ����*/
		SetJlyParamJudgeChannelNumSensorType();
		JudgingChannelNumberDisplay(Conf.Jly.ChannelNum);//�޸�ͨ������
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
  * @brief  Description ��ȡ��ʷ����
						(1)�����ݲ���ok
						(2)����������ʱ����ok��
						(3)����ȡ���ݴ���Queue.FlashNoReadingDataNum,��������
  * @param  pucBuffer   ��Ŷ������ݵ�ָ��
  * @param  usAddress	�������ݵĵ�ַ
  * @param  usNRegs		��������
  * @retval ��
  *****************************************************************************/
bool STORAGE_DATA_READ(uint8_t *pucBuffer, USHORT usAddress, USHORT usNRegs)
{
	uint32_t NoReadingDataNumTemp;
	//usAddress *= 2;
	usNRegs *= 2;
	Queue.FlashNoReadingDataNum = ReadU32Pointer(FLASH_NoReadingDataNumAddr_Lchar);
	Queue.ReadFlashDataPointer = ReadU32Pointer(FLASH_ReadDataAddr_Lchar);
	
	NoReadingDataNumTemp = usNRegs/Queue.HIS_ONE_BYTES;
	if(NoReadingDataNumTemp > Queue.FlashNoReadingDataNum)//Ҫ�������ݰ�������flash��δ�����ݰ���
	{
		usNRegs = Queue.FlashNoReadingDataNum * Queue.HIS_ONE_BYTES;
		NoReadingDataNumTemp = Queue.FlashNoReadingDataNum;
	}
	if(Queue.FlashNoReadingDataNum >=2) //�����µ���������
	{
		SPI_FLASH_BufferRead(pucBuffer,Queue.ReadFlashDataPointer,usNRegs);
		Queue.ReadFlashDataPointer += usNRegs;
		if(Queue.ReadFlashDataPointer >= FLASH_RecMaxSize)
		{
			Queue.ReadFlashDataPointer = 0;
		}
		WriteU32Pointer(FLASH_ReadDataAddr_Lchar,Queue.ReadFlashDataPointer);
		
		
		Queue.FlashNoReadingDataNum -= NoReadingDataNumTemp; //δ��������
		if(Queue.FlashNoReadingDataNum <= 0)
		{
			Queue.FlashNoReadingDataNum = 0;
		}
		WriteU32Pointer(FLASH_NoReadingDataNumAddr_Lchar,Queue.FlashNoReadingDataNum);
		
		//����
		printf("Flag.RecordFlashOverFlow %d\r\n",Queue.FlashRecOverFlow);
		printf("Queue.FlashNoReadingDataNum %d\r\n",Queue.FlashNoReadingDataNum);
		printf("Queue.FlashSectorPointer %d\r\n",Queue.FlashSectorPointer);
		printf("Queue.WriteFlashDataPointer %d\r\n",Queue.WriteFlashDataPointer);
		printf("Queue.FlashReadDataBeginPointer %d\r\n",Queue.FlashReadDataBeginPointer);
		printf("Queue.ReadFlashDataPointer %d\r\n",Queue.ReadFlashDataPointer);
	}
	
	rtc_deel();
	
	return true;
}
/******************************************************************************
  * @brief  Description ��ȡ��ʷ����
  * @param  pucBuffer   ��Ŷ������ݵ�ָ��
  * @param  usAddress	�������ݵĵ�ַ
  * @param  usNRegs		��������
  * @retval ��
  *****************************************************************************/
//bool STORAGE_DATA_READ(uint8_t *pucBuffer, USHORT usAddress, USHORT usNRegs)
//{
//	uint8_t serialbuf[10];
//	uint32_t RecorderPoint_temp,RecorderNum_temp,read_eeOffset_temp;
//	uint32_t NoReadingDataNumTemp;
//	//usAddress *= 2;
//	//usNRegs *= 2;
//	NoReadingDataNumTemp = usNRegs/Queue.HIS_ONE_BYTES;
//	if(NoReadingDataNumTemp > Queue.FlashNoReadingDataNum)//Ҫ�������ݰ�������flash��δ�����ݰ���
//	{
//		usNRegs = Queue.FlashNoReadingDataNum * Queue.HIS_ONE_BYTES;
//		NoReadingDataNumTemp = Queue.FlashNoReadingDataNum;
//	}
//	if(Queue.FlashNoReadingDataNum >=1)
//	{
//		//-----��ǰ��¼���� Queue.FlashNoReadingDataNum, ��δ��ȡ����
//		Queue.FlashNoReadingDataNum = ReadFlashNoReadingDataNum();
//		RecorderPoint_temp = Queue.WriteFlashDataPointer;	//��¼����ָ��
//		RecorderNum_temp = Queue.FlashNoReadingDataNum;	//δ������
//		
//		//ָ�뻹ԭ:�洢ָ�뱣�������һ�����ݵ�ָ��
//		if(RecorderPoint_temp>0)
//		{
//			RecorderPoint_temp--;
//		}
//		else
//		{	
//			RecorderPoint_temp = Queue.FLASH_MAX_NUM - Queue.FLASH_SECTOR_PER_NUM - 1;
//		}
//		//���� �洢���� ��¼ָ�� ��¼���� �����ȡָ��
//		if(RecorderNum_temp <= (RecorderPoint_temp+1))
//		{
//			RecorderPoint_temp = RecorderPoint_temp-(RecorderNum_temp - 1);
//		}
//		else
//		{
//			RecorderPoint_temp = RecorderPoint_temp+(Queue.FLASH_MAX_NUM - Queue.FLASH_SECTOR_PER_NUM - RecorderNum_temp + 1);
//		}
//		
//		read_eeOffset_temp = RecorderPoint_temp * Queue.HIS_ONE_BYTES;//ƫ��������
//		
//		SPI_FLASH_BufferRead(pucBuffer,(Queue.FlashReadDataBeginPointer + read_eeOffset_temp),usNRegs);
//		//Queue.FlashReadDataBeginPointer
//		
//		Queue.FlashNoReadingDataNum = Queue.FlashNoReadingDataNum - NoReadingDataNumTemp; //δ��������
//		if(Queue.FlashNoReadingDataNum <= 0)
//		{
//			Queue.FlashNoReadingDataNum = 1;
//		}
//		WriteFlashNoReadingDataNum(Queue.FlashNoReadingDataNum);
//	}
//	
//	rtc_deel();
//	
//	return true;
//}

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
