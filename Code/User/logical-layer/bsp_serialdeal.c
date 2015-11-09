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
  * @brief  Description 读取基本配置数据地址表时更新Fram中电池电压
  * @param  value       电池电压
  * @retval 无
  *****************************************************************************/
static void ReadBatVoltage(uint16_t value)
{
	Conf.Basic.BatVoltage.hwd = value;//更新内存
	Fram_Write(Conf.Basic.BatVoltage.byte,FRAM_BatVoltageAddr,2);//电池电量写入Fram中
}
/******************************************************************************
  * @brief  Description 写配置数据到Fram中，同时更新内存中的数据
  * @param  pucBuffer   要写入数据指针
  * @param  usAddress	要写入数据地址
  * @param  usNRegs		写入数据数量
  * @retval 无
  *****************************************************************************/
bool PARAM_DATA_WRITE(uint8_t *pucBuffer, USHORT usAddress, USHORT usNRegs)
{
	uint8_t  offsetnum=0;	//偏移几个虚拟地址
	uint16_t size = usNRegs * 2;
	uint16_t offset = 0;
	
	if (usAddress < VirtJlyConfAddr)	//0x0100
	{
		/* 基本配置数据地址表 */
		offset = usAddress - VirtBasicConfAddr;
		/*写配置表数据到Fram，每个地址有两个字节数据，offset * 2*/
		Fram_Write(pucBuffer,ConfMap_Address[0][1] + offset * 2,size);
		WriteSetFramFlag();//设置过fram标志
		/*更新配置表数据 */
		Fram_Read(Conf.Buf,ConfMap_Address[0][1] + offset * 2,size);
	} else if (usAddress < VirJlyTimeConfAddr)
	{
		/* 记录仪配置数据地址表 */
		offset = usAddress - VirtJlyConfAddr;
		Fram_Write(pucBuffer, ConfMap_Address[1][1] + offset * 2, size);
		Fram_Read(&Conf.Buf[FRAM_JlyConfAddr],ConfMap_Address[1][1] + offset * 2,size);
		/*通道数量、通道类型变化时做相应处理*/
		SetJlyParamJudgeChannelNumSensorType();
		JudgingChannelNumberDisplay(Conf.Jly.ChannelNum);//修改通道数量
		//if(Conf.)
		
	}else if (usAddress < VirtAlarmConfAddr)
	{
		/*记录仪实时时间地址表*/
		offset = usAddress - VirJlyTimeConfAddr;
		Fram_Write(pucBuffer, ConfMap_Address[2][1] + offset * 2, size);
		Fram_Read(&Conf.Buf[FRAM_JlyTimeConfAddr],ConfMap_Address[2][1] + offset * 2,size);
		set_time();	/*设置仪器时间*/
		displayTIME(Conf.Time.Time_Hour,Conf.Time.Time_Min);
	}
	else if (usAddress < VirtSensorChanelConfAddr)
	{
		/* 报警配置数据地址表 */ 
		offset = usAddress - VirtAlarmConfAddr;
		Fram_Write(pucBuffer, ConfMap_Address[3][1] + offset * 2, size);
		Fram_Read(&Conf.Buf[FRAM_AlarmConfAddr],ConfMap_Address[3][1] + offset * 2,size);
		
	} else if (usAddress < VirtTempHumiAdjustConfAddr)
	{
		/* 传感器通道配置数据地址表  */
		// 通道0配置
		offsetnum = (usAddress - VirtSensorChanelConfAddr) / VirtOffset;	/* 偏移多少个虚拟地址 */
		offset = offsetnum * FRAM_SensorChanelOffset;						/* Fram中的偏移量 */
		Fram_Write(pucBuffer, ConfMap_Address[4][1] + offset * 2, size);
		Fram_Read(&Conf.Buf[FRAM_SensorChanelConfAddr + offset * 2],ConfMap_Address[4][1] + offset * 2,size);
		
	} else if (usAddress < VirtMax)
	{
		/* 温湿度传感器校准配置数据地址表 */
		offsetnum = (usAddress - VirtTempHumiAdjustConfAddr) / VirtOffset;
		offset = offsetnum * FRAM_TempHumiAdjustOffset;
		Fram_Write(pucBuffer, ConfMap_Address[5][1] + offset * 2, size);
		Fram_Read(&Conf.Buf[FRAM_TempHumiAdjustConfAddr + offset * 2],ConfMap_Address[5][1] + offset * 2,size);
	}
	return true;
}
/******************************************************************************
  * @brief  Description 读取配置数据信息
  * @param  pucBuffer   存放读出数据的指针
  * @param  usAddress	读出数据的地址
  * @param  usNRegs		读的数量
  * @retval 无
  *****************************************************************************/
bool PARAM_DATA_READ(uint8_t *pucBuffer, USHORT usAddress, USHORT usNRegs)
{
	uint8_t  offsetnum=0;	//偏移几个虚拟地址
	//uint8_t	 frambuf[128];
	uint16_t size = usNRegs * 2;
	uint16_t offset = 0;
	
	if (usAddress < VirtJlyConfAddr)	//0x0100
	{
		/* 基本配置数据地址表 */
		offset = usAddress - VirtBasicConfAddr;
		ReadBatVoltage(PManage.BatVoltage);//跟新Fram中电池电量
		Fram_Read(pucBuffer,ConfMap_Address[0][1] + offset * 2,size);
//		Fram_Read(Buf,ConfMap_Address[0][1] + offset * 2,size);
	} else if (usAddress < VirJlyTimeConfAddr)
	{
		
		/* 记录仪配置数据地址表 */
		offset = usAddress - VirtJlyConfAddr;
		Fram_Read(pucBuffer, ConfMap_Address[1][1] + offset * 2, size);
		
	} else if (usAddress < VirtAlarmConfAddr)
	{
		/*记录仪实时时间地址表*/
		offset = usAddress - VirJlyTimeConfAddr;
		serialread_time();
		Fram_Read(pucBuffer, ConfMap_Address[2][1] + offset * 2, size);
	}
	else if (usAddress < VirtSensorChanelConfAddr)
	{
		/* 报警配置数据地址表 */ 
		offset = usAddress - VirtAlarmConfAddr;
		Fram_Read(pucBuffer, ConfMap_Address[3][1] + offset * 2, size);
		//Fram_Read(frambuf, ConfMap_Address[3][1] + offset * 2, size);
		
	} else if (usAddress < VirtTempHumiAdjustConfAddr)
	{
		/* 传感器通道配置数据地址表  */
		// 通道0配置
		offsetnum = (usAddress - VirtSensorChanelConfAddr) / VirtOffset;	/* 偏移多少个虚拟地址 */
		offset = offsetnum * FRAM_SensorChanelOffset;						/* Fram中的偏移量 */
		Fram_Read(pucBuffer, ConfMap_Address[4][1] + offset * 2, size);
		
	} else if (usAddress < VirtMax)
	{
		/* 温湿度传感器校准配置数据地址表 */
		offsetnum = (usAddress - VirtTempHumiAdjustConfAddr) / VirtOffset;
		offset = offsetnum * FRAM_TempHumiAdjustOffset;
		Fram_Read(pucBuffer, ConfMap_Address[5][1] + offset * 2, size);
	}
	return true;
}

/******************************************************************************
  * @brief  Description 读取历史数据
						(1)读数据测试ok
						(2)读最新数据时测试ok，
						(3)当读取数据大于Queue.FlashNoReadingDataNum,反复测试
  * @param  pucBuffer   存放读出数据的指针
  * @param  usAddress	读出数据的地址
  * @param  usNRegs		读的数量
  * @retval 无
  *****************************************************************************/
bool STORAGE_DATA_READ(uint8_t *pucBuffer, USHORT usAddress, USHORT usNRegs)
{
	uint32_t NoReadingDataNumTemp;
	//usAddress *= 2;
	usNRegs *= 2;
	Queue.FlashNoReadingDataNum = ReadU32Pointer(FLASH_NoReadingDataNumAddr_Lchar);
	Queue.ReadFlashDataPointer = ReadU32Pointer(FLASH_ReadDataAddr_Lchar);
	
	NoReadingDataNumTemp = usNRegs/Queue.HIS_ONE_BYTES;
	if(NoReadingDataNumTemp > Queue.FlashNoReadingDataNum)//要读的数据包数大于flash中未读数据包数
	{
		usNRegs = Queue.FlashNoReadingDataNum * Queue.HIS_ONE_BYTES;
		NoReadingDataNumTemp = Queue.FlashNoReadingDataNum;
	}
	if(Queue.FlashNoReadingDataNum >=2) //读最新的两条数据
	{
		SPI_FLASH_BufferRead(pucBuffer,Queue.ReadFlashDataPointer,usNRegs);
		Queue.ReadFlashDataPointer += usNRegs;
		if(Queue.ReadFlashDataPointer >= FLASH_RecMaxSize)
		{
			Queue.ReadFlashDataPointer = 0;
		}
		WriteU32Pointer(FLASH_ReadDataAddr_Lchar,Queue.ReadFlashDataPointer);
		
		
		Queue.FlashNoReadingDataNum -= NoReadingDataNumTemp; //未读条数减
		if(Queue.FlashNoReadingDataNum <= 0)
		{
			Queue.FlashNoReadingDataNum = 0;
		}
		WriteU32Pointer(FLASH_NoReadingDataNumAddr_Lchar,Queue.FlashNoReadingDataNum);
		
		//测试
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
  * @brief  Description 读取历史数据
  * @param  pucBuffer   存放读出数据的指针
  * @param  usAddress	读出数据的地址
  * @param  usNRegs		读的数量
  * @retval 无
  *****************************************************************************/
//bool STORAGE_DATA_READ(uint8_t *pucBuffer, USHORT usAddress, USHORT usNRegs)
//{
//	uint8_t serialbuf[10];
//	uint32_t RecorderPoint_temp,RecorderNum_temp,read_eeOffset_temp;
//	uint32_t NoReadingDataNumTemp;
//	//usAddress *= 2;
//	//usNRegs *= 2;
//	NoReadingDataNumTemp = usNRegs/Queue.HIS_ONE_BYTES;
//	if(NoReadingDataNumTemp > Queue.FlashNoReadingDataNum)//要读的数据包数大于flash中未读数据包数
//	{
//		usNRegs = Queue.FlashNoReadingDataNum * Queue.HIS_ONE_BYTES;
//		NoReadingDataNumTemp = Queue.FlashNoReadingDataNum;
//	}
//	if(Queue.FlashNoReadingDataNum >=1)
//	{
//		//-----当前记录条数 Queue.FlashNoReadingDataNum, 即未读取条数
//		Queue.FlashNoReadingDataNum = ReadFlashNoReadingDataNum();
//		RecorderPoint_temp = Queue.WriteFlashDataPointer;	//记录数据指针
//		RecorderNum_temp = Queue.FlashNoReadingDataNum;	//未读条数
//		
//		//指针还原:存储指针保存的是下一条数据的指针
//		if(RecorderPoint_temp>0)
//		{
//			RecorderPoint_temp--;
//		}
//		else
//		{	
//			RecorderPoint_temp = Queue.FLASH_MAX_NUM - Queue.FLASH_SECTOR_PER_NUM - 1;
//		}
//		//根据 存储数量 记录指针 记录容量 计算读取指针
//		if(RecorderNum_temp <= (RecorderPoint_temp+1))
//		{
//			RecorderPoint_temp = RecorderPoint_temp-(RecorderNum_temp - 1);
//		}
//		else
//		{
//			RecorderPoint_temp = RecorderPoint_temp+(Queue.FLASH_MAX_NUM - Queue.FLASH_SECTOR_PER_NUM - RecorderNum_temp + 1);
//		}
//		
//		read_eeOffset_temp = RecorderPoint_temp * Queue.HIS_ONE_BYTES;//偏移量计算
//		
//		SPI_FLASH_BufferRead(pucBuffer,(Queue.FlashReadDataBeginPointer + read_eeOffset_temp),usNRegs);
//		//Queue.FlashReadDataBeginPointer
//		
//		Queue.FlashNoReadingDataNum = Queue.FlashNoReadingDataNum - NoReadingDataNumTemp; //未读条数减
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
  * @param  无
  * @retval 无
  *****************************************************************************/
void JlySerialDeal(void)
{
	
}
#endif /* __BSPSERIAL_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
