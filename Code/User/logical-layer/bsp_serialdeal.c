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
  * @brief  Description 修改记录仪配置数据地址表 2 ,设置内存中记录仪参数,判断通
道数量是否修改,如果修改了就清除历史数据

通过记录仪配置数据表2修改通道数,记录间隔/采集间隔
修改通道数的权限只有出厂前
测试：
	1.只改通道数
	2.通道数不变改通道类型
	3.改变通道数和通道类型
  * @param  None
  * @retval None
  *****************************************************************************/
static void JlyConfDataUpData(void)
{
	//参数复位
	ChannelForDisplay =0;//显示
	Flag.StopRecording = 0;	//停止记录标志清0  这样的话在别的启动方式未生效前任可修改到别的记录方式,立即刷新显示
	
	//按键使能 重要参数,在这里客户通过app进行设置
	Flag.KeyEnableOrDisable = Conf.Jly.KeyEnableOrDisable;
	
	//通道数,上位机软件配置修改通道数量
	JlyParam.ChannelNumActual = Conf.Jly.ChannelNum;//这里会修改通道数量
	JlyParam.ChannelNumActualOld = JlyParam.ChannelNumActual;
	/*------------------------------------------------------*/
	/*清历史数据
	 *(1)未出厂前 通道数改变清除历史数据
	 */
	if(JlyParam.ChannelNumOld != Conf.Jly.ChannelNum)
	{
		JlyParam.ChannelNumOld = Conf.Jly.ChannelNum;	//通道数量修改了重新备份通道数量
		
		Queue.FlashSectorPointer = 0;
		Queue.FlashNoReadingDataNum = 0;
		Queue.FlashReadDataBeginPointer =0;
		Queue.WriteFlashDataPointer =0;
		Queue.ReadFlashDataPointer = 0;

		WriteU16Pointer(FLASH_SectorWriteAddr_Lchar,0);
		WriteU32Pointer(FLASH_NoReadingDataNumAddr_Lchar,0);
		WriteU32Pointer(FLASH_ReadDataBeginAddr_Lchar,0);
		WriteU32Pointer(FLASH_WriteDataAddr_Lchar,0);
		WriteU32Pointer(FLASH_ReadDataAddr_Lchar,0);

		SetFlashOverFlow(0);//清除flash溢出标志
	}
	/*------------------------------------------------------*/
	Queue.HIS_ONE_BYTES = (uint16_t)(JlyParam.ChannelNumOld*2+8*Gps_choose+5+Clock_choose); //一帧数据大小
	Conf.Basic.HisOneBytes = Queue.HIS_ONE_BYTES;
	Fram_Write(&Conf.Basic.HisOneBytes,HisOneBytesAddr,1);//这里配置会修改通道数，即一帧数据会被修改需要写入fram
	//Queue.FRAM_MAX_NUM = FRAM_RecMaxSize/Queue.HIS_ONE_BYTES;	//fram中存储数据的最大包数 4096/
	Queue.FLASH_SECTOR_PER_NUM = FLASH_SectorPerSize/Queue.HIS_ONE_BYTES; //flash中一个扇区存储数据的最大包数
	Queue.FlashRecActualStorage = FLASH_RecMaxSize/Queue.HIS_ONE_BYTES*Queue.HIS_ONE_BYTES; //flash中实际存储字节数
	/*------------------------------------------------------*/
	JlyParam.delay_start_time = ReadDelayStartTime;			//读取延时启动时间
	JlyParam.NormalRecInterval = ReadNormalRecIntervalTime;	//读取正常记录间隔 单位：s
	JlyParam.NormalRecIntervalMin = JlyParam.NormalRecInterval/60;//正常记录间隔 单位：min
	
	//采集时间间隔 单位:ms转s,按协议中设计的 uint16_t 最大65535ms 65s,
	JlyParam.SampleInterval = Conf.Jly.SampleInterval/1000 ;
	JlyParam.SampleTime = JlyParam.SampleInterval;  		//采集时间 单位:s	
}

/******************************************************************************
  * @brief  Description  报警配置数据表 数据更新
  * @param  无
  * @retval 无
  *****************************************************************************/
static void AlarmConfDataUpdate(void)
{
	JlyParam.ContinueExcessiveTimes = Conf.Alarm.ContinueExcessiveTimes; //连续超标次数 0-10可设置
	JlyParam.SoundLightAlarmTimeDelay =Conf.Alarm.SoundLightAlarmDelay;  //声光报警延时 单位s 1s到18小时可设置
}

/******************************************************************************
  * @brief  Description 设置内存中记录仪参数,判断通道类型/通道使能是否修改
						如果修改了就清除历史数据
通过传感器配置数据表5 修改通道类型/通道使能
修改通道类型的权限只有出厂前,客户有通道使能权限
测试：
	1.通道数不变改通道类型
	2.通道数不变，打开关闭通道
  * @param  None
  * @retval None
  *****************************************************************************/
static void SetJlyParamJudgeSensorTypeChannelSwitch(void)
{
	uint8_t i;
	//参数复位
	ChannelForDisplay =0;//显示
	
	//判断实际的通道数
	for(i=0;i < Conf.Jly.ChannelNum;i++)//所有通道循环一遍
	{
		if(JlyParam.ChannelSwitchOld[i] != Conf.Sensor[i].ChannelSwitch)//通道使能位被修改
		{
			if(Conf.Sensor[i].ChannelSwitch == 0x01)
			{
				Flag.ChannelSwitchIsOn = 1;//---------这个标志作为测试用---------
				JlyParam.ChannelNumActual = JlyParam.ChannelNumActual - 1;//有通道被关闭则实际通道数量减去关闭的通道数量 重新赋值
				if(JlyParam.ChannelNumActual <=0)
				{
					JlyParam.ChannelNumActual =0;//通道数保护
				}
			}else{
				JlyParam.ChannelNumActual = JlyParam.ChannelNumActual + 1;//有通道被关闭则实际通道数量减去关闭的通道数量 重新赋值
				if(JlyParam.ChannelNumActual > Conf.Jly.ChannelNum)
				{
					JlyParam.ChannelNumActual =Conf.Jly.ChannelNum;//通道数保护
				}
			}
			JlyParam.ChannelSwitchOld[i] = Conf.Sensor[i].ChannelSwitch;//通道使能位修改过，备份
		}
	}
	
	/*------------------------------------------------------*/
	/*清历史数据
	 *在客户手里Conf.Jly.ChannelNum不变，某个通道被关闭询问是否下载历史数据否则清除历史数据
	 */
	if(JlyParam.ChannelNumActualOld != JlyParam.ChannelNumActual)
	{
		JlyParam.ChannelNumActualOld = JlyParam.ChannelNumActual;//Conf.Jly.ChannelNum不变，有通道被关闭
		
		Queue.FlashSectorPointer = 0;
		Queue.FlashNoReadingDataNum = 0;
		Queue.FlashReadDataBeginPointer =0;
		Queue.WriteFlashDataPointer =0;
		Queue.ReadFlashDataPointer = 0;

		WriteU16Pointer(FLASH_SectorWriteAddr_Lchar,0);
		WriteU32Pointer(FLASH_NoReadingDataNumAddr_Lchar,0);
		WriteU32Pointer(FLASH_ReadDataBeginAddr_Lchar,0);
		WriteU32Pointer(FLASH_WriteDataAddr_Lchar,0);
		WriteU32Pointer(FLASH_ReadDataAddr_Lchar,0);

		SetFlashOverFlow(0);//清除flash溢出标志
	}
	/*清历史数据
	 *未出厂前 通道数不变，通道类型改变清除历史数据
	 */
	if(JlyParam.ChannelNumOld == Conf.Jly.ChannelNum)
	{
		for(i=0; i< JlyParam.ChannelNumActual; i++)
		{
			if(JlyParam.SensorTypeOld[i] != Conf.Sensor[i].SensorType)
			{
				//通道数循环完，并且备份新的通道类型，如果有改变的则清除历史数据
				JlyParam.SensorTypeOld[i] = Conf.Sensor[i].SensorType;
				Flag.SensorTypeIsChange = 1;
			}
		}
		//循环完所有通道再执行
		if(Flag.SensorTypeIsChange == 1)
		{
			Flag.SensorTypeIsChange = 0; //标志清除
			
			Queue.FlashSectorPointer = 0;
			Queue.FlashNoReadingDataNum = 0;
			Queue.FlashReadDataBeginPointer =0;
			Queue.WriteFlashDataPointer =0;
			Queue.ReadFlashDataPointer = 0;

			WriteU16Pointer(FLASH_SectorWriteAddr_Lchar,0);
			WriteU32Pointer(FLASH_NoReadingDataNumAddr_Lchar,0);
			WriteU32Pointer(FLASH_ReadDataBeginAddr_Lchar,0);
			WriteU32Pointer(FLASH_WriteDataAddr_Lchar,0);
			WriteU32Pointer(FLASH_ReadDataAddr_Lchar,0);

			SetFlashOverFlow(0);//清除flash溢出标志
		}
	}
	/*------------------------------------------------------*/
	Queue.HIS_ONE_BYTES = (uint16_t)(JlyParam.ChannelNumActual*2+8*Gps_choose+5+Clock_choose); //一帧数据大小
	Conf.Basic.HisOneBytes = Queue.HIS_ONE_BYTES;
	Fram_Write(&Conf.Basic.HisOneBytes,HisOneBytesAddr,1);//这里配置会修改通道数，即一帧数据会被修改需要写入fram
	//Queue.FRAM_MAX_NUM = FRAM_RecMaxSize/Queue.HIS_ONE_BYTES;	//fram中存储数据的最大包数 4096/
	Queue.FLASH_SECTOR_PER_NUM = FLASH_SectorPerSize/Queue.HIS_ONE_BYTES; //flash中一个扇区存储数据的最大包数
	Queue.FlashRecActualStorage = FLASH_RecMaxSize/Queue.HIS_ONE_BYTES*Queue.HIS_ONE_BYTES; //flash中实际存储字节数
	
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
		size = 8*2;//基本配置表只有前8个字节需要配置到fram中
		Fram_Write(pucBuffer,ConfMap_Address[0][1] + offset * 2,size);
		Display_SN();//如果修改SN号,则刷新显示
		WriteSetFramFlag();//设置过fram标志
		/*更新配置表数据 */
		Fram_Read(Conf.Buf,ConfMap_Address[0][1] + offset * 2,size);
	} else if (usAddress < VirJlyTimeConfAddr)
	{
		/* 记录仪配置数据地址表 */
		offset = usAddress - VirtJlyConfAddr;
		Fram_Write(pucBuffer, ConfMap_Address[1][1] + offset * 2, size);
		Fram_Read(&Conf.Buf[FRAM_JlyConfAddr],ConfMap_Address[1][1] + offset * 2,size);
		/*通道数量 变化时做相应处理*/
		JlyConfDataUpData();
		JudgingChannelNumberDisplay(JlyParam.ChannelNumOld);
		
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
		AlarmConfDataUpdate();
	} else if (usAddress < VirtTempHumiAdjustConfAddr)
	{
		/* 传感器通道配置数据地址表  */
		// 通道0配置
		offsetnum = (usAddress - VirtSensorChanelConfAddr) / VirtOffset;	/* 偏移多少个虚拟地址 */
		offset = offsetnum * FRAM_SensorChanelOffset;						/* Fram中的偏移量 */
		Fram_Write(pucBuffer, ConfMap_Address[4][1] + offset * 2, size);
		Fram_Read(&Conf.Buf[FRAM_SensorChanelConfAddr + offset * 2],ConfMap_Address[4][1] + offset * 2,size);
		/*通道类型变化时和通道使能(打开关闭时)做相应处理*/
		SetJlyParamJudgeSensorTypeChannelSwitch();
		if(JlyParam.ChannelNumActual >0)
		{
			JudgingChannelNumberDisplay(JlyParam.ChannelNumOld);
		}else{
			JudgingChannelNumberDisplay(JlyParam.ChannelNumActual);
		}
		
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
		ReadBatVoltage(PManage.BatVoltage);//更新Fram中电池电量
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
  * @brief  Description 读取实时数据
  * @param  pucBuffer   存放读出数据的指针
  * @param  usAddress	读出数据的地址 0
  * @param  usNRegs		读的数量
  * @retval 无
  *****************************************************************************/
bool REALDATA_DATA_READ(uint8_t *pucBuffer, USHORT usAddress, USHORT usNRegs)
{
	usAddress *= 2;
	usNRegs *= 2;
	if((usAddress == 0) &&(usNRegs % Queue.HIS_ONE_BYTES) ==0)
	{
		//Queue.WriteFlashDataPointer = ReadU32Pointer(FLASH_WriteDataAddr_Lchar); 
		SPI_FLASH_BufferRead(pucBuffer,(Queue.WriteFlashDataPointer-Queue.HIS_ONE_BYTES),usNRegs);
		
		return true;
	}else{
		
		return false;
	}
}
/******************************************************************************
  * @brief  Description 读取未读数据
usNRegs 不是 Queue.HIS_ONE_BYTES整数倍时处理
						(1)读数据测试ok
						(2)读最新数据时测试ok，
						(3)当读取数据大于Queue.FlashNoReadingDataNum,反复测试
测试
1.未溢出 r<w 测试最大边界
2.溢出 r>w 测试最大边界
3.溢出 r<w 测试r追上w
  * @param  pucBuffer   存放读出数据的指针
  * @param  usAddress	读出数据的地址 0,在本函数中没有实际意义
  * @param  usNRegs   读取数据的字节数,为一帧数据的整数倍
  * @retval 无
  *****************************************************************************/
bool STORAGE_DATA_READ(uint8_t *pucBuffer, USHORT usAddress, USHORT usNRegs)
{
	uint8_t LessThanBytes=0,i=0;
	uint32_t NoReadingDataNumTemp;
	//usAddress *= 2;
	usNRegs *= 2;
	
	if((usAddress == 0) && (usNRegs % Queue.HIS_ONE_BYTES ==0))
	{
		//优化这两个变量的读取，节省时间，复位测试读取数据ok
		//Queue.FlashNoReadingDataNum = ReadU32Pointer(FLASH_NoReadingDataNumAddr_Lchar);
		//Queue.ReadFlashDataPointer = ReadU32Pointer(FLASH_ReadDataAddr_Lchar);
		
		NoReadingDataNumTemp = usNRegs/Queue.HIS_ONE_BYTES;

				
			if(Queue.FlashNoReadingDataNum >= NoReadingDataNumTemp)
			{
				if((Queue.ReadFlashDataPointer + usNRegs) <= Queue.FlashRecActualStorage)
				{
					SPI_FLASH_BufferRead(pucBuffer,Queue.ReadFlashDataPointer,usNRegs);//读取usNRegs 字节的数据
				}else{
					SPI_FLASH_BufferRead(pucBuffer,Queue.ReadFlashDataPointer,Queue.FlashRecActualStorage - Queue.ReadFlashDataPointer);
					LessThanBytes = Queue.FlashRecActualStorage - Queue.ReadFlashDataPointer;
					usNRegs = usNRegs - (Queue.FlashRecActualStorage - Queue.ReadFlashDataPointer);
					Queue.ReadFlashDataPointer = 0;
					SPI_FLASH_BufferRead(&pucBuffer[LessThanBytes],Queue.ReadFlashDataPointer,usNRegs);//注意pucBuffer的起始地址
				}
			}else{
				LessThanBytes = Queue.FlashNoReadingDataNum * Queue.HIS_ONE_BYTES;//只有这么多未读字节数
				SPI_FLASH_BufferRead(pucBuffer,Queue.ReadFlashDataPointer,LessThanBytes);
				for(i=0;i<(usNRegs-LessThanBytes);i++)
				{
					pucBuffer[LessThanBytes+i] = 0xFF;
				}
				usNRegs = Queue.FlashNoReadingDataNum * Queue.HIS_ONE_BYTES;
				NoReadingDataNumTemp = Queue.FlashNoReadingDataNum;
			}
			Queue.ReadFlashDataPointer += usNRegs;
			if(Queue.ReadFlashDataPointer >= Queue.FlashRecActualStorage)
			{
				Queue.ReadFlashDataPointer = 0;
			}
			WriteU32Pointer(FLASH_ReadDataAddr_Lchar,Queue.ReadFlashDataPointer);
			
			
			Queue.FlashNoReadingDataNum = Queue.FlashNoReadingDataNum - NoReadingDataNumTemp; //未读条数减
			if(Queue.FlashNoReadingDataNum <= 0)
			{
				Queue.FlashNoReadingDataNum = 0;
			}
			WriteU32Pointer(FLASH_NoReadingDataNumAddr_Lchar,Queue.FlashNoReadingDataNum);
		
		
		//rtc_deel();
		
		return true;
	}else{
		
		return false;
	}
}

/******************************************************************************
  * @brief  Description 下载历史数据
usAddress不是 Queue.HIS_ONE_BYTES整数倍时处理
usNRegs 不是 Queue.HIS_ONE_BYTES整数倍时处理
						(1)读数据测试ok
  * @param  pucBuffer   存放读出数据的指针
  * @param  usAddress	读数据起始地址,为一帧数据的整数倍
  * @param  usNRegs		读取数据的字节数
  * @retval 无
  *****************************************************************************/
bool HISTORY_DATA_READ(uint8_t *pucBuffer, USHORT usAddress, USHORT usNRegs)
{
	uint8_t LessThanBytes=0,i=0;
	uint32_t NoReadingDataNumTemp;
	//usAddress *= 2;
	usNRegs *= 2;
	
	if((usAddress % Queue.HIS_ONE_BYTES ==0) && (usNRegs % Queue.HIS_ONE_BYTES ==0))
	{
		//Queue.FlashReadDataBeginPointer = ReadU32Pointer(FLASH_ReadDataBeginAddr_Lchar);
		//Queue.FlashNoReadingDataNum = ReadU32Pointer(FLASH_NoReadingDataNumAddr_Lchar);
		
		NoReadingDataNumTemp = usNRegs/Queue.HIS_ONE_BYTES;
		if(usAddress < Queue.FlashReadDataBeginPointer)//小于最小
		{
			usAddress = Queue.FlashReadDataBeginPointer;
		}
		
			if(Queue.FlashNoReadingDataNum >= NoReadingDataNumTemp)
			{
				if((usAddress + usNRegs) <= Queue.FlashRecActualStorage)
				{
					SPI_FLASH_BufferRead(pucBuffer,usAddress,usNRegs);//读取usNRegs 字节的数据
				}else{
					SPI_FLASH_BufferRead(pucBuffer,usAddress,Queue.FlashRecActualStorage - usAddress);
					LessThanBytes = Queue.FlashRecActualStorage - usAddress;
					usNRegs = usNRegs - (Queue.FlashRecActualStorage - usAddress);
					usAddress = 0;
					SPI_FLASH_BufferRead(&pucBuffer[LessThanBytes],usAddress,usNRegs);//注意pucBuffer的起始地址
				}
			}else{
				LessThanBytes = Queue.FlashNoReadingDataNum * Queue.HIS_ONE_BYTES;//只有这么多未读字节数
				SPI_FLASH_BufferRead(pucBuffer,usAddress,LessThanBytes);
				for(i=0;i<(usNRegs-LessThanBytes);i++)
				{
					pucBuffer[LessThanBytes+i] = 0xFF;
				}
				
			}
		
		//测试
		
		return true;
	}else{
		return false;
	}
}

#endif /* __BSPSERIAL_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
