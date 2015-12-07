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
  * @brief  Description �޸ļ�¼���������ݵ�ַ�� 2 ,�����ڴ��м�¼�ǲ���,�ж�ͨ
�������Ƿ��޸�,����޸��˾������ʷ����

ͨ����¼���������ݱ�2�޸�ͨ����,��¼���/�ɼ����
�޸�ͨ������Ȩ��ֻ�г���ǰ
���ԣ�
	1.ֻ��ͨ����
	2.ͨ���������ͨ������
	3.�ı�ͨ������ͨ������
  * @param  None
  * @retval None
  *****************************************************************************/
static void JlyConfDataUpData(void)
{
	//������λ
	ChannelForDisplay =0;//��ʾ
	Flag.StopRecording = 0;	//ֹͣ��¼��־��0  �����Ļ��ڱ��������ʽδ��Чǰ�ο��޸ĵ���ļ�¼��ʽ,����ˢ����ʾ
	
	//����ʹ�� ��Ҫ����,������ͻ�ͨ��app��������
	Flag.KeyEnableOrDisable = Conf.Jly.KeyEnableOrDisable;
	
	//ͨ����,��λ����������޸�ͨ������
	JlyParam.ChannelNumActual = Conf.Jly.ChannelNum;//������޸�ͨ������
	JlyParam.ChannelNumActualOld = JlyParam.ChannelNumActual;
	/*------------------------------------------------------*/
	/*����ʷ����
	 *(1)δ����ǰ ͨ�����ı������ʷ����
	 */
	if(JlyParam.ChannelNumOld != Conf.Jly.ChannelNum)
	{
		JlyParam.ChannelNumOld = Conf.Jly.ChannelNum;	//ͨ�������޸������±���ͨ������
		
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

		SetFlashOverFlow(0);//���flash�����־
	}
	/*------------------------------------------------------*/
	Queue.HIS_ONE_BYTES = (uint16_t)(JlyParam.ChannelNumOld*2+8*Gps_choose+5+Clock_choose); //һ֡���ݴ�С
	Conf.Basic.HisOneBytes = Queue.HIS_ONE_BYTES;
	Fram_Write(&Conf.Basic.HisOneBytes,HisOneBytesAddr,1);//�������û��޸�ͨ��������һ֡���ݻᱻ�޸���Ҫд��fram
	//Queue.FRAM_MAX_NUM = FRAM_RecMaxSize/Queue.HIS_ONE_BYTES;	//fram�д洢���ݵ������� 4096/
	Queue.FLASH_SECTOR_PER_NUM = FLASH_SectorPerSize/Queue.HIS_ONE_BYTES; //flash��һ�������洢���ݵ�������
	Queue.FlashRecActualStorage = FLASH_RecMaxSize/Queue.HIS_ONE_BYTES*Queue.HIS_ONE_BYTES; //flash��ʵ�ʴ洢�ֽ���
	/*------------------------------------------------------*/
	JlyParam.delay_start_time = ReadDelayStartTime;			//��ȡ��ʱ����ʱ��
	JlyParam.NormalRecInterval = ReadNormalRecIntervalTime;	//��ȡ������¼��� ��λ��s
	JlyParam.NormalRecIntervalMin = JlyParam.NormalRecInterval/60;//������¼��� ��λ��min
	
	//�ɼ�ʱ���� ��λ:msתs,��Э������Ƶ� uint16_t ���65535ms 65s,
	JlyParam.SampleInterval = Conf.Jly.SampleInterval/1000 ;
	JlyParam.SampleTime = JlyParam.SampleInterval;  		//�ɼ�ʱ�� ��λ:s	
}

/******************************************************************************
  * @brief  Description  �����������ݱ� ���ݸ���
  * @param  ��
  * @retval ��
  *****************************************************************************/
static void AlarmConfDataUpdate(void)
{
	JlyParam.ContinueExcessiveTimes = Conf.Alarm.ContinueExcessiveTimes; //����������� 0-10������
	JlyParam.SoundLightAlarmTimeDelay =Conf.Alarm.SoundLightAlarmDelay;  //���ⱨ����ʱ ��λs 1s��18Сʱ������
}

/******************************************************************************
  * @brief  Description �����ڴ��м�¼�ǲ���,�ж�ͨ������/ͨ��ʹ���Ƿ��޸�
						����޸��˾������ʷ����
ͨ���������������ݱ�5 �޸�ͨ������/ͨ��ʹ��
�޸�ͨ�����͵�Ȩ��ֻ�г���ǰ,�ͻ���ͨ��ʹ��Ȩ��
���ԣ�
	1.ͨ���������ͨ������
	2.ͨ�������䣬�򿪹ر�ͨ��
  * @param  None
  * @retval None
  *****************************************************************************/
static void SetJlyParamJudgeSensorTypeChannelSwitch(void)
{
	uint8_t i;
	//������λ
	ChannelForDisplay =0;//��ʾ
	
	//�ж�ʵ�ʵ�ͨ����
	for(i=0;i < Conf.Jly.ChannelNum;i++)//����ͨ��ѭ��һ��
	{
		if(JlyParam.ChannelSwitchOld[i] != Conf.Sensor[i].ChannelSwitch)//ͨ��ʹ��λ���޸�
		{
			if(Conf.Sensor[i].ChannelSwitch == 0x01)
			{
				Flag.ChannelSwitchIsOn = 1;//---------�����־��Ϊ������---------
				JlyParam.ChannelNumActual = JlyParam.ChannelNumActual - 1;//��ͨ�����ر���ʵ��ͨ��������ȥ�رյ�ͨ������ ���¸�ֵ
				if(JlyParam.ChannelNumActual <=0)
				{
					JlyParam.ChannelNumActual =0;//ͨ��������
				}
			}else{
				JlyParam.ChannelNumActual = JlyParam.ChannelNumActual + 1;//��ͨ�����ر���ʵ��ͨ��������ȥ�رյ�ͨ������ ���¸�ֵ
				if(JlyParam.ChannelNumActual > Conf.Jly.ChannelNum)
				{
					JlyParam.ChannelNumActual =Conf.Jly.ChannelNum;//ͨ��������
				}
			}
			JlyParam.ChannelSwitchOld[i] = Conf.Sensor[i].ChannelSwitch;//ͨ��ʹ��λ�޸Ĺ�������
		}
	}
	
	/*------------------------------------------------------*/
	/*����ʷ����
	 *�ڿͻ�����Conf.Jly.ChannelNum���䣬ĳ��ͨ�����ر�ѯ���Ƿ�������ʷ���ݷ��������ʷ����
	 */
	if(JlyParam.ChannelNumActualOld != JlyParam.ChannelNumActual)
	{
		JlyParam.ChannelNumActualOld = JlyParam.ChannelNumActual;//Conf.Jly.ChannelNum���䣬��ͨ�����ر�
		
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

		SetFlashOverFlow(0);//���flash�����־
	}
	/*����ʷ����
	 *δ����ǰ ͨ�������䣬ͨ�����͸ı������ʷ����
	 */
	if(JlyParam.ChannelNumOld == Conf.Jly.ChannelNum)
	{
		for(i=0; i< JlyParam.ChannelNumActual; i++)
		{
			if(JlyParam.SensorTypeOld[i] != Conf.Sensor[i].SensorType)
			{
				//ͨ����ѭ���꣬���ұ����µ�ͨ�����ͣ�����иı���������ʷ����
				JlyParam.SensorTypeOld[i] = Conf.Sensor[i].SensorType;
				Flag.SensorTypeIsChange = 1;
			}
		}
		//ѭ��������ͨ����ִ��
		if(Flag.SensorTypeIsChange == 1)
		{
			Flag.SensorTypeIsChange = 0; //��־���
			
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

			SetFlashOverFlow(0);//���flash�����־
		}
	}
	/*------------------------------------------------------*/
	Queue.HIS_ONE_BYTES = (uint16_t)(JlyParam.ChannelNumActual*2+8*Gps_choose+5+Clock_choose); //һ֡���ݴ�С
	Conf.Basic.HisOneBytes = Queue.HIS_ONE_BYTES;
	Fram_Write(&Conf.Basic.HisOneBytes,HisOneBytesAddr,1);//�������û��޸�ͨ��������һ֡���ݻᱻ�޸���Ҫд��fram
	//Queue.FRAM_MAX_NUM = FRAM_RecMaxSize/Queue.HIS_ONE_BYTES;	//fram�д洢���ݵ������� 4096/
	Queue.FLASH_SECTOR_PER_NUM = FLASH_SectorPerSize/Queue.HIS_ONE_BYTES; //flash��һ�������洢���ݵ�������
	Queue.FlashRecActualStorage = FLASH_RecMaxSize/Queue.HIS_ONE_BYTES*Queue.HIS_ONE_BYTES; //flash��ʵ�ʴ洢�ֽ���
	
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
		size = 8*2;//�������ñ�ֻ��ǰ8���ֽ���Ҫ���õ�fram��
		Fram_Write(pucBuffer,ConfMap_Address[0][1] + offset * 2,size);
		Display_SN();//����޸�SN��,��ˢ����ʾ
		WriteSetFramFlag();//���ù�fram��־
		/*�������ñ����� */
		Fram_Read(Conf.Buf,ConfMap_Address[0][1] + offset * 2,size);
	} else if (usAddress < VirJlyTimeConfAddr)
	{
		/* ��¼���������ݵ�ַ�� */
		offset = usAddress - VirtJlyConfAddr;
		Fram_Write(pucBuffer, ConfMap_Address[1][1] + offset * 2, size);
		Fram_Read(&Conf.Buf[FRAM_JlyConfAddr],ConfMap_Address[1][1] + offset * 2,size);
		/*ͨ������ �仯ʱ����Ӧ����*/
		JlyConfDataUpData();
		JudgingChannelNumberDisplay(JlyParam.ChannelNumOld);
		
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
		AlarmConfDataUpdate();
	} else if (usAddress < VirtTempHumiAdjustConfAddr)
	{
		/* ������ͨ���������ݵ�ַ��  */
		// ͨ��0����
		offsetnum = (usAddress - VirtSensorChanelConfAddr) / VirtOffset;	/* ƫ�ƶ��ٸ������ַ */
		offset = offsetnum * FRAM_SensorChanelOffset;						/* Fram�е�ƫ���� */
		Fram_Write(pucBuffer, ConfMap_Address[4][1] + offset * 2, size);
		Fram_Read(&Conf.Buf[FRAM_SensorChanelConfAddr + offset * 2],ConfMap_Address[4][1] + offset * 2,size);
		/*ͨ�����ͱ仯ʱ��ͨ��ʹ��(�򿪹ر�ʱ)����Ӧ����*/
		SetJlyParamJudgeSensorTypeChannelSwitch();
		if(JlyParam.ChannelNumActual >0)
		{
			JudgingChannelNumberDisplay(JlyParam.ChannelNumOld);
		}else{
			JudgingChannelNumberDisplay(JlyParam.ChannelNumActual);
		}
		
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
  * @brief  Description ��ȡʵʱ����
  * @param  pucBuffer   ��Ŷ������ݵ�ָ��
  * @param  usAddress	�������ݵĵ�ַ 0
  * @param  usNRegs		��������
  * @retval ��
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
  * @brief  Description ��ȡδ������
usNRegs ���� Queue.HIS_ONE_BYTES������ʱ����
						(1)�����ݲ���ok
						(2)����������ʱ����ok��
						(3)����ȡ���ݴ���Queue.FlashNoReadingDataNum,��������
����
1.δ��� r<w �������߽�
2.��� r>w �������߽�
3.��� r<w ����r׷��w
  * @param  pucBuffer   ��Ŷ������ݵ�ָ��
  * @param  usAddress	�������ݵĵ�ַ 0,�ڱ�������û��ʵ������
  * @param  usNRegs   ��ȡ���ݵ��ֽ���,Ϊһ֡���ݵ�������
  * @retval ��
  *****************************************************************************/
bool STORAGE_DATA_READ(uint8_t *pucBuffer, USHORT usAddress, USHORT usNRegs)
{
	uint8_t LessThanBytes=0,i=0;
	uint32_t NoReadingDataNumTemp;
	//usAddress *= 2;
	usNRegs *= 2;
	
	if((usAddress == 0) && (usNRegs % Queue.HIS_ONE_BYTES ==0))
	{
		//�Ż������������Ķ�ȡ����ʡʱ�䣬��λ���Զ�ȡ����ok
		//Queue.FlashNoReadingDataNum = ReadU32Pointer(FLASH_NoReadingDataNumAddr_Lchar);
		//Queue.ReadFlashDataPointer = ReadU32Pointer(FLASH_ReadDataAddr_Lchar);
		
		NoReadingDataNumTemp = usNRegs/Queue.HIS_ONE_BYTES;

				
			if(Queue.FlashNoReadingDataNum >= NoReadingDataNumTemp)
			{
				if((Queue.ReadFlashDataPointer + usNRegs) <= Queue.FlashRecActualStorage)
				{
					SPI_FLASH_BufferRead(pucBuffer,Queue.ReadFlashDataPointer,usNRegs);//��ȡusNRegs �ֽڵ�����
				}else{
					SPI_FLASH_BufferRead(pucBuffer,Queue.ReadFlashDataPointer,Queue.FlashRecActualStorage - Queue.ReadFlashDataPointer);
					LessThanBytes = Queue.FlashRecActualStorage - Queue.ReadFlashDataPointer;
					usNRegs = usNRegs - (Queue.FlashRecActualStorage - Queue.ReadFlashDataPointer);
					Queue.ReadFlashDataPointer = 0;
					SPI_FLASH_BufferRead(&pucBuffer[LessThanBytes],Queue.ReadFlashDataPointer,usNRegs);//ע��pucBuffer����ʼ��ַ
				}
			}else{
				LessThanBytes = Queue.FlashNoReadingDataNum * Queue.HIS_ONE_BYTES;//ֻ����ô��δ���ֽ���
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
			
			
			Queue.FlashNoReadingDataNum = Queue.FlashNoReadingDataNum - NoReadingDataNumTemp; //δ��������
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
  * @brief  Description ������ʷ����
usAddress���� Queue.HIS_ONE_BYTES������ʱ����
usNRegs ���� Queue.HIS_ONE_BYTES������ʱ����
						(1)�����ݲ���ok
  * @param  pucBuffer   ��Ŷ������ݵ�ָ��
  * @param  usAddress	��������ʼ��ַ,Ϊһ֡���ݵ�������
  * @param  usNRegs		��ȡ���ݵ��ֽ���
  * @retval ��
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
		if(usAddress < Queue.FlashReadDataBeginPointer)//С����С
		{
			usAddress = Queue.FlashReadDataBeginPointer;
		}
		
			if(Queue.FlashNoReadingDataNum >= NoReadingDataNumTemp)
			{
				if((usAddress + usNRegs) <= Queue.FlashRecActualStorage)
				{
					SPI_FLASH_BufferRead(pucBuffer,usAddress,usNRegs);//��ȡusNRegs �ֽڵ�����
				}else{
					SPI_FLASH_BufferRead(pucBuffer,usAddress,Queue.FlashRecActualStorage - usAddress);
					LessThanBytes = Queue.FlashRecActualStorage - usAddress;
					usNRegs = usNRegs - (Queue.FlashRecActualStorage - usAddress);
					usAddress = 0;
					SPI_FLASH_BufferRead(&pucBuffer[LessThanBytes],usAddress,usNRegs);//ע��pucBuffer����ʼ��ַ
				}
			}else{
				LessThanBytes = Queue.FlashNoReadingDataNum * Queue.HIS_ONE_BYTES;//ֻ����ô��δ���ֽ���
				SPI_FLASH_BufferRead(pucBuffer,usAddress,LessThanBytes);
				for(i=0;i<(usNRegs-LessThanBytes);i++)
				{
					pucBuffer[LessThanBytes+i] = 0xFF;
				}
				
			}
		
		//����
		
		return true;
	}else{
		return false;
	}
}

#endif /* __BSPSERIAL_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
