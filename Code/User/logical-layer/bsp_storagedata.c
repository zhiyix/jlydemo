/**
  ******************************************************************************
  * @file    bsp_storagedata.c
  * @author  
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   �洢��ʷ����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:32 BITS Discovery
  *
  ******************************************************************************
  */ 

#include "main.h"


const char temp_unit[2]={0xA1,0xE6};//"�¶ȷ���"
const char shidu_unit[3]={0x25,0x52,0x48};//"%RH"


/******************************************************************************
  * @brief  Description ͨ�����ݴ���
  * @param  channelnum  ͨ����		
  * @param  clockchoose		
  * @param  Gpschoose	ѡ��GPS
  * @retval ��		
  *****************************************************************************/
static void ChannelDataDeal(uint8_t channelnum,uint8_t clockchoose,uint8_t Gpschoose)
{
    //-------------
	uint8_t zhuangtai_temp;
    uint16_t i=0,j=0;
    uint16_t UtTmp;
    
    DataBuf[i++]=0xaa; //����ͷ
	DataBuf[i++]= 0;   //SN��
	DataBuf[i++]= 1;
    //---------------
    
    for(j=0;j<channelnum;j++)
    {
		if(Conf.Sensor[j].ChannelSwitch ==0)//ͨ���� ��������
		{
			if(1==FlagSeniorErr[j])
			{
				DataBuf[i++]=0X01;//DataBuf[i++]=0XFF;
				DataBuf[i++]=0X80;//DataBuf[i++]=0XFF;
			}
			else
			{
				
				if((ChannelDataFloat[j]>200)|(ChannelDataFloat[j]<-300))
				{
					DataBuf[i++]=0X01;
					DataBuf[i++]=0X80;
				}
				else
				{
					UtTmp=ValueFtToU16(ChannelDataFloat[j]);
					DataBuf[i++]=0XFF&(UtTmp);	//���ݵ�λ
					DataBuf[i++]=0XFF&((UtTmp)>>8); //���ݸ�λ
				}
				
			}
		}
    }
    //---------------
//    if(Gpschoose==1)
//    {
//		
//    }
    //---------------
    //
//    if((Rtc.Day>0X60)||(Rtc.Minute>0X60)||(Rtc.Hour>0X24)||(Rtc.Day>0X31))
//    {
//        read_time();
//    }
    //---------------
    if(clockchoose==0)
    {
        DataBuf[i++]=Rtc.Year;//(5)
        DataBuf[i++]=Rtc.Month;
        DataBuf[i++]=Rtc.Day;
        DataBuf[i++]=Rtc.Hour;
        DataBuf[i++]=Rtc.Minute;
    }
    else
    {
//        DataBuf[i++]=Rtc.Year;//(6)
//        DataBuf[i++]=Rtc.Month;
//        DataBuf[i++]=Rtc.Day;
//        DataBuf[i++]=Rtc.Hour;
//        DataBuf[i++]=Rtc.Minute;
//        DataBuf[i++]=Rtc.Second;
		DataBuf[i++]=Rtc.Month;
		DataBuf[i++]=Rtc.Year;//(6)
        DataBuf[i++]=Rtc.Hour;
        DataBuf[i++]=Rtc.Day;
        DataBuf[i++]=Rtc.Second;
        DataBuf[i++]=Rtc.Minute;
    }
    //---------------
    
    zhuangtai_temp=0;
    DataBuf[i++]=zhuangtai_temp;
}

/******************************************************************************
  * @brief  Description 
  * @param  PointerAddr	��ָ���ַ  		 	
  * @retval ��		
  *****************************************************************************/
uint16_t ReadU16Pointer(const uint16_t PointerAddr)
{
	union MyU16Data myu16;
	Fram_Read(myu16.Byte,PointerAddr,2); //���� u16 2�ֽ�
	return myu16.Variable; //����
}
/******************************************************************************
  * @brief  Description 
  * @param  PointerAddr	дָ���ַ  	
  * @param  Pointer		ָ��
  * @retval ��		
  *****************************************************************************/
void WriteU16Pointer(const uint16_t PointerAddr,uint16_t Pointer)
{
	union MyU16Data myu16;
	myu16.Variable = Pointer;
	Fram_Write(myu16.Byte,PointerAddr,2); //���� u16 2�ֽ�
}


/******************************************************************************
  * @brief  Description 
  * @param  PointerAddr	��ָ���ַ 		 	
  * @retval ��		
  *****************************************************************************/
uint32_t ReadU32Pointer(const uint32_t PointerAddr)
{
    union MyU32Data myu32; 
    Fram_Read(myu32.Byte,PointerAddr,4);    //���� u32 4�ֽ�
    return myu32.Variable;	//����
}
/******************************************************************************
  * @brief  Description ����
  * @param  PointerAddr	дָ���ַ
  * @param  Pointer     ָ��
  * @retval ��		
  *****************************************************************************/
void WriteU32Pointer(const uint32_t PointerAddr,uint32_t Pointer)
{
    union MyU32Data myu32; 
    myu32.Variable = Pointer;
    Fram_Write(myu32.Byte, PointerAddr, 4); //���� u32 4�ֽ�
}

/******************************************************************************
  * @brief  Description  ����flash�����δ�����־
  * @param  flowvalue    0/1 1��ʾ���
  * @retval ��		
  *****************************************************************************/
void SetFlashOverFlow(uint8_t flowvalue)
{
	uint8_t OverFlowBuf[1];
	
	Queue.FlashRecOverFlow = flowvalue;	
	OverFlowBuf[0] = flowvalue;
	Fram_Write(OverFlowBuf,FLASH_FlashRecOverFlowAddr,1);//�洢flash��δ�����־
}
/******************************************************************************
  * @brief  Description �������ݵ�flash,����2k ģ��ʵ��
						δ���������٣���¼����ַ����
д��һ������ʱ������һ������
flashд�� Queue.FLASH_SECTOR_PER_NUM - 1ʱ��Queue.WriteFlashDataPointer++��
��ʱQueue.WriteFlashDataPointer % Queue.FLASH_SECTOR_PER_NUM��������һ������
flash��ÿ���������ֽڶ���������
���ԣ�
	(1)��ʵ�鷴�����Դ洢��8ͨ������ok
	(2)��������δ����������ȷ�ԣ�8ͨ������ok
	(3)���Ը�λ�����־
  * @param  ��  		 	
  * @retval ��		
  *****************************************************************************/
static void SaveHisDataToFlash(void)
{
	
	Queue.ReadFlashDataPointer = ReadU32Pointer(FLASH_ReadDataAddr_Lchar);
	Queue.FlashNoReadingDataNum = ReadU32Pointer(FLASH_NoReadingDataNumAddr_Lchar); //��δ������
	
	Queue.WriteFlashDataPointer = ReadU32Pointer(FLASH_WriteDataAddr_Lchar); //��ȡFlashд����ָ��
	//FlashOffset = Queue.WriteFlashDataPointer * Queue.HIS_ONE_BYTES; //ƫ����
	
	//flash����д����������һ����
	//��һ�β��� Queue.RecFlashWritePointer =0
	//���һ�β��� 
	if((Queue.WriteFlashDataPointer < (FLASH_RecMaxSize - Queue.HIS_ONE_BYTES))&&(Queue.WriteFlashDataPointer % ((Queue.FlashSectorPointer * FLASH_SectorPerSize/Queue.HIS_ONE_BYTES)*Queue.HIS_ONE_BYTES) ==0))
	{
		Queue.FlashReadDataBeginPointer = ReadU32Pointer(FLASH_ReadDataBeginAddr_Lchar); //�� ��������ʼָ��
		Queue.FlashSectorPointer = ReadU16Pointer(FLASH_SectorWriteAddr_Lchar); //��ȡfalsh sector�洢ָ��
		
		SPI_FLASH_SectorErase(FLASH_SectorFirstAddr + (Queue.FlashSectorPointer * FLASH_SectorPerSize));//��������,���Ǳ߽�����
		
		Queue.FlashSectorPointer++;	//Flash Sectorָ��� 1
		if(Queue.FlashSectorPointer >= FLASH_SectorNum)
		{
			Queue.FlashSectorPointer = 0;
		}
		WriteU16Pointer(FLASH_SectorWriteAddr_Lchar,Queue.FlashSectorPointer);	//����Flash Sector�洢ָ�� 
		
		if(Queue.FlashRecOverFlow ==1)	//���������������ǰ��������ָ��ƫ�� Queue.FLASH_SECTOR_PER_NUM
		{
			//�������ڶ�������
			if(Queue.WriteFlashDataPointer < (FLASH_RecMaxSize - FLASH_SectorPerSize - Queue.HIS_ONE_BYTES))//���ݼ�¼ָ��С��8192-4096-10
			{
				Queue.SectorHeadBytes = Queue.HIS_ONE_BYTES - (Queue.FlashSectorPointer * FLASH_SectorPerSize )%Queue.HIS_ONE_BYTES;
				Queue.FlashReadDataBeginPointer = Queue.FlashSectorPointer * FLASH_SectorPerSize + Queue.SectorHeadBytes;//Queue.FlashSectorPointer++֮�� 
			}else{//д���һ������ʱ
				Queue.FlashReadDataBeginPointer = 0;
			}
			//������д��Ϊ0ʱ,�����������
			//���1��һ������Ҳδ����Queue.ReadFlashDataPointer = Queue.FlashReadDataBeginPointer;
			//���2�������ݴ����������д��Ϊ0��Queue.ReadFlashDataPointer = Queue.FlashReadDataBeginPointer;
			if(((Queue.ReadFlashDataPointer - Queue.WriteFlashDataPointer) >= 0) &&((Queue.ReadFlashDataPointer - Queue.WriteFlashDataPointer) < FLASH_SectorPerSize))
			{
				Queue.ReadFlashDataPointer = Queue.FlashReadDataBeginPointer;
				WriteU32Pointer(FLASH_ReadDataAddr_Lchar,Queue.ReadFlashDataPointer);//�����ָ��
			}
		}else{
			Queue.FlashReadDataBeginPointer = 0;
		}
		WriteU32Pointer(FLASH_ReadDataBeginAddr_Lchar,Queue.FlashReadDataBeginPointer);//�����������ʼָ��
	}
	//��ʷ����д��flash��ÿ��д��Queue.HIS_ONE_BYTES ��С
	SPI_FLASH_BufferWrite(&DataBuf[3], Queue.WriteFlashDataPointer, Queue.HIS_ONE_BYTES);
	Queue.WriteFlashDataPointer += Queue.HIS_ONE_BYTES;	//Flashд����ָ��� һ֡Byte
	if(Queue.WriteFlashDataPointer >= FLASH_RecMaxSize)
	{
		Queue.WriteFlashDataPointer = 0;
		if(Queue.FlashRecOverFlow ==0)//�ϵ��Ѷ�ȡ
		{
			SetFlashOverFlow(1);//����flash�����־
		}
	}
	WriteU32Pointer(FLASH_WriteDataAddr_Lchar,Queue.WriteFlashDataPointer); //����Flashд����ָ��
		
	if(Queue.FlashRecOverFlow == 0)
	{
		if(Queue.FlashNoReadingDataNum < Queue.FLASH_MAX_NUM)
		{
			Queue.FlashNoReadingDataNum++;
		}
	}else{
		/*
		if(Queue.FlashNoReadingDataNum < Queue.FLASH_MAX_NUM ) //����ok
		{
			Queue.FlashNoReadingDataNum++;
		}else{//819-409
			Queue.FlashNoReadingDataNum = Queue.FLASH_MAX_NUM - Queue.FLASH_SECTOR_PER_NUM;//��ʱ�պ�д��һ������
		}*/
		//Queue.FlashNoReadingDataNum = Queue.FLASH_MAX_NUM - Queue.FLASH_SECTOR_PER_NUM + (Queue.WriteFlashDataPointer%FLASH_SectorPerSize)/Queue.HIS_ONE_BYTES;//��ʱ�պ�д��һ������
		if(Queue.FlashReadDataBeginPointer !=0 )
		{
			Queue.FlashNoReadingDataNum = Queue.WriteFlashDataPointer/Queue.HIS_ONE_BYTES + (FLASH_RecMaxSize - Queue.FlashReadDataBeginPointer)/Queue.HIS_ONE_BYTES;//��ʱ�պ�д��һ������
		}else{
			Queue.FlashNoReadingDataNum = Queue.WriteFlashDataPointer/Queue.HIS_ONE_BYTES;
		}
	}
		
	WriteU32Pointer(FLASH_NoReadingDataNumAddr_Lchar,Queue.FlashNoReadingDataNum);//����δ������ָ��
}

/******************************************************************************
  * @brief  Description ���������ݵ��ڴ���
  * @param  ��   			
  * @retval ��		
  *****************************************************************************/
void ReadFramHisDataToRam(void)
{
    uint8_t i=0,j=0,zhuangtai_temp=0;
    uint8_t BufA[HIS_ONE_MAX_BYTES+2+ID_BYTES];
    uint8_t Recorderpoint_L,Recorderpoint_H;
    uint16_t read_eeOffset_temp;    
    uint16_t RecorderPoint_temp;  
    
//    AI2C_Read(&Recorderpoint_L,FRAM_RecWriteAddr_Lchar,1);
//	AI2C_Read(&Recorderpoint_H,FRAM_RecWriteAddr_Hchar,1);
    Queue.RecFramWritePointer=Recorderpoint_L+(Recorderpoint_H<<8);
    RecorderPoint_temp = Queue.RecFramWritePointer;
    read_eeOffset_temp = RecorderPoint_temp*Queue.HIS_ONE_BYTES;
    
    BufA[i++]=0xaa;//
    BufA[i++]=0;//ID(2) 
    BufA[i++]=1;
    
    //BufA[i++]=(u8)0xff&(*((int*)BSQ_XX_ADDR));
    //BufA[i++]=(u8)0xff&(*((int*)BSQ_XX_ADDR)>>8);
    AI2C_Read(&BufA[i],(FRAM_RecFirstAddr+read_eeOffset_temp),Queue.HIS_ONE_BYTES);
    
    zhuangtai_temp=0x66;
    BufA[i+Queue.HIS_ONE_BYTES]=zhuangtai_temp;
    
    printf("\r\n");
    for(j=0;j<(Queue.HIS_ONE_BYTES+Headend_BYTES+ID_BYTES);j++)
    {
        printf("%02x ",BufA[j]);
    }  
    printf("%d ",RecorderPoint_temp);
    printf("\r\n");
    for(j=0;j<(Queue.HIS_ONE_BYTES+Headend_BYTES+ID_BYTES);j++)
    {
        BufA[i] = 0;
        
    }
}

/******************************************************************************
  * @brief  Description ��flash�е�����
  * @param    		
  * @retval 		
  *****************************************************************************/
void ReadFlashHisData(uint32_t RecPointerBeginAddr,uint32_t RecPointerEndAddr)
{
	uint8_t Buf[HIS_ONE_MAX_BYTES+Headend_BYTES+ID_BYTES];
    uint8_t BufTemp[HIS_ONE_MAX_BYTES+Headend_BYTES+ID_BYTES];
    uint8_t i=0;
	uint16_t Temp;
    uint32_t down_hisdata_count;
    
    for(down_hisdata_count=RecPointerBeginAddr;down_hisdata_count < (RecPointerEndAddr - Queue.HIS_ONE_BYTES);)
    {
        SPI_FLASH_BufferRead(BufTemp,(FLASH_RecFirstAddr+down_hisdata_count),Queue.HIS_ONE_BYTES);
        down_hisdata_count += Queue.HIS_ONE_BYTES;
		if(JlyParam.ChannelNumOld == JlyParam.ChannelNumActual)
		{	
			if(JlyParam.ChannelNumOld == 1)
			{
				i=0;
				Buf[i++]=BufTemp[2]; 
				Buf[i++]=BufTemp[3];
				Buf[i++]=BufTemp[4];
				Buf[i++]=BufTemp[5];
				Buf[i++]=BufTemp[6];
				Buf[i++]=BufTemp[7];
				
				//Buf[i++]=BufTemp[0];
				//Buf[i++]=BufTemp[1];
				Temp=U16ToValue10(BufTemp[1],BufTemp[0]);
				Buf[i++]=Temp/100;
				Buf[i++]=(Temp%100)/10;
				Buf[i++]=Temp%10;
				
				//Buf[i++]=BufTemp[2];
				//Buf[i++]=BufTemp[3];
	//			Temp=U16ToValue10(BufTemp[3],BufTemp[2]);
	//			Buf[i++]=Temp/100;
	//			Buf[i++]=(Temp%100)/10;
	//			Buf[i++]=Temp%10;
				
				printf("%02x-%02x-%02x %02x:%02x:%02x ",Buf[1],Buf[0],Buf[3],Buf[2],Buf[5],Buf[4]);
				printf("%d%d.%d \r\n",Buf[6],Buf[7],Buf[8]);
			}else if(JlyParam.ChannelNumOld == 2)
			{
				i=0;
				Buf[i++]=BufTemp[4]; 
				Buf[i++]=BufTemp[5];
				Buf[i++]=BufTemp[6];
				Buf[i++]=BufTemp[7];
				Buf[i++]=BufTemp[8];
				Buf[i++]=BufTemp[9];
				
				//Buf[i++]=BufTemp[0];
				//Buf[i++]=BufTemp[1];
				Temp=U16ToValue10(BufTemp[1],BufTemp[0]);
				Buf[i++]=Temp/100;
				Buf[i++]=(Temp%100)/10;
				Buf[i++]=Temp%10;
				
				//Buf[i++]=BufTemp[2];
				//Buf[i++]=BufTemp[3];
				Temp=U16ToValue10(BufTemp[3],BufTemp[2]);
				Buf[i++]=Temp/100;
				Buf[i++]=(Temp%100)/10;
				Buf[i++]=Temp%10;
				
				printf("%02x-%02x-%02x %02x:%02x:%02x ",Buf[1],Buf[0],Buf[3],Buf[2],Buf[5],Buf[4]);
				printf("%d%d.%d %d%d.%d\r\n",Buf[6],Buf[7],Buf[8],Buf[9],Buf[10],Buf[11]);
			}else if(JlyParam.ChannelNumOld == 8){
				i=0;
				Buf[i++]=BufTemp[16];
				Buf[i++]=BufTemp[17];
				Buf[i++]=BufTemp[18]; 
				Buf[i++]=BufTemp[19];
				Buf[i++]=BufTemp[20];
				Buf[i++]=BufTemp[21];
				
				//Buf[i++]=BufTemp[0];
				//Buf[i++]=BufTemp[1];
				Temp=U16ToValue10(BufTemp[1],BufTemp[0]);
				Buf[i++]=Temp/100;
				Buf[i++]=(Temp%100)/10;
				Buf[i++]=Temp%10;
				
				//Buf[i++]=BufTemp[2];
				//Buf[i++]=BufTemp[3];
				Temp=U16ToValue10(BufTemp[3],BufTemp[2]);
				Buf[i++]=Temp/100;
				Buf[i++]=(Temp%100)/10;
				Buf[i++]=Temp%10;
				
				printf("%02x-%02x-%02x %02x:%02x:%02x ",Buf[1],Buf[0],Buf[3],Buf[2],Buf[5],Buf[4]);
				printf("%d%d.%d %d%d.%d %d%d.%d %d%d.%d %d%d.%d %d%d.%d %d%d.%d %d%d.%d\r\n",Buf[6],Buf[7],Buf[8],Buf[9],Buf[10],Buf[11],0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
			}
		}else{
			if(JlyParam.ChannelNumActual == 1)
			{
				i=0;
				Buf[i++]=BufTemp[2]; 
				Buf[i++]=BufTemp[3];
				Buf[i++]=BufTemp[4];
				Buf[i++]=BufTemp[5];
				Buf[i++]=BufTemp[6];
				Buf[i++]=BufTemp[7];
				
				//Buf[i++]=BufTemp[0];
				//Buf[i++]=BufTemp[1];
				Temp=U16ToValue10(BufTemp[1],BufTemp[0]);
				Buf[i++]=Temp/100;
				Buf[i++]=(Temp%100)/10;
				Buf[i++]=Temp%10;
				
				//Buf[i++]=BufTemp[2];
				//Buf[i++]=BufTemp[3];
	//			Temp=U16ToValue10(BufTemp[3],BufTemp[2]);
	//			Buf[i++]=Temp/100;
	//			Buf[i++]=(Temp%100)/10;
	//			Buf[i++]=Temp%10;
				
				printf("%02x-%02x-%02x %02x:%02x:%02x ",Buf[1],Buf[0],Buf[3],Buf[2],Buf[5],Buf[4]);
				printf("%d%d.%d \r\n",Buf[6],Buf[7],Buf[8]);
			}else if(JlyParam.ChannelNumActual == 2)
			{
				i=0;
				Buf[i++]=BufTemp[4]; 
				Buf[i++]=BufTemp[5];
				Buf[i++]=BufTemp[6];
				Buf[i++]=BufTemp[7];
				Buf[i++]=BufTemp[8];
				Buf[i++]=BufTemp[9];
				
				//Buf[i++]=BufTemp[0];
				//Buf[i++]=BufTemp[1];
				Temp=U16ToValue10(BufTemp[1],BufTemp[0]);
				Buf[i++]=Temp/100;
				Buf[i++]=(Temp%100)/10;
				Buf[i++]=Temp%10;
				
				//Buf[i++]=BufTemp[2];
				//Buf[i++]=BufTemp[3];
				Temp=U16ToValue10(BufTemp[3],BufTemp[2]);
				Buf[i++]=Temp/100;
				Buf[i++]=(Temp%100)/10;
				Buf[i++]=Temp%10;
				
				printf("%02x-%02x-%02x %02x:%02x:%02x ",Buf[1],Buf[0],Buf[3],Buf[2],Buf[5],Buf[4]);
				printf("%d%d.%d %d%d.%d\r\n",Buf[6],Buf[7],Buf[8],Buf[9],Buf[10],Buf[11]);
			}else if(JlyParam.ChannelNumActual == 8){
				i=0;
				Buf[i++]=BufTemp[16];
				Buf[i++]=BufTemp[17];
				Buf[i++]=BufTemp[18]; 
				Buf[i++]=BufTemp[19];
				Buf[i++]=BufTemp[20];
				Buf[i++]=BufTemp[21];
				
				//Buf[i++]=BufTemp[0];
				//Buf[i++]=BufTemp[1];
				Temp=U16ToValue10(BufTemp[1],BufTemp[0]);
				Buf[i++]=Temp/100;
				Buf[i++]=(Temp%100)/10;
				Buf[i++]=Temp%10;
				
				//Buf[i++]=BufTemp[2];
				//Buf[i++]=BufTemp[3];
				Temp=U16ToValue10(BufTemp[3],BufTemp[2]);
				Buf[i++]=Temp/100;
				Buf[i++]=(Temp%100)/10;
				Buf[i++]=Temp%10;
				
				printf("%02x-%02x-%02x %02x:%02x:%02x ",Buf[1],Buf[0],Buf[3],Buf[2],Buf[5],Buf[4]);
				printf("%d%d.%d %d%d.%d %d%d.%d %d%d.%d %d%d.%d %d%d.%d %d%d.%d %d%d.%d\r\n",Buf[6],Buf[7],Buf[8],Buf[9],Buf[10],Buf[11],0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
			}
		}
        
    }
}
/******************************************************************************
  * @brief  Description ����Flash�е�����
  * @param    		
  * @retval 		
  *****************************************************************************/
void DownFlash_HisData(void)
{
    uint32_t  RecorderPoint_temp;
    
    Queue.WriteFlashDataPointer = ReadU32Pointer(FLASH_WriteDataAddr_Lchar);
	Queue.FlashReadDataBeginPointer = ReadU32Pointer(FLASH_ReadDataBeginAddr_Lchar);
    RecorderPoint_temp = Queue.WriteFlashDataPointer;
    
    
    if(RecorderPoint_temp==0)
    {
        Flag.TouchKey1DuanAn = 0;
    }else{
		if(Queue.FlashRecOverFlow ==0)
		{
			ReadFlashHisData(0,RecorderPoint_temp);
			Flag.TouchKey1DuanAn = 0;
		}
		if(Queue.FlashRecOverFlow >=1)
		{
			if(Queue.WriteFlashDataPointer < (FLASH_RecMaxSize - FLASH_SectorPerSize - Queue.HIS_ONE_BYTES))//�������д���ݵ����һ����������������ʼ��ַΪ0
			{
				ReadFlashHisData(Queue.FlashReadDataBeginPointer,FLASH_RecMaxSize); 
			}
			ReadFlashHisData(0,RecorderPoint_temp);
			Flag.TouchKey1DuanAn = 0;            
		}
	}
}

/******************************************************************************
  * @brief  Description ����¼����������ݵ�flash
  * @param  ��  		
  * @retval ��		
  *****************************************************************************/
void SaveDataOnTimeDeal(void)
{
    
	if((JlyParam.NormalRecInterval > 0) && (JlyParam.NormalRecInterval < 60))//1s-60s�����ݼ�¼
	{
		Rtc.SCount++;
		if(Rtc.SCount >= 60)
		{
			Rtc.SCount = 0;
		}
		if(Rtc.SCount % JlyParam.NormalRecInterval ==0)
		{
			
			read_time();
			ChannelDataDeal(JlyParam.ChannelNumOld,Clock_choose,Gps_choose);
//			SaveHisDataToFram();
			SaveHisDataToFlash();
		}
	}
	else
	{
		read_time();
    
		RtcBcdToD10(&Rtc);
		
		Rtc.TMPS=DateToSeconds(&Rtc);
		
		RtcD10ToBcd(&Rtc);
		
		Rtc.TMPS = Rtc.TMPS/60;	//����
		if(Rtc.TMPS % JlyParam.NormalRecIntervalMin ==0)//1����-
		{   
			if(Rtc.TMPS!=Rtc.TCPS)
			{
				Rtc.TCPS=Rtc.TMPS;
				
				read_time();
				ChannelDataDeal(JlyParam.ChannelNumOld,Clock_choose,Gps_choose);
//				SaveHisDataToFram();
				SaveHisDataToFlash();
			}
		}
	}
	
}
/******************************************************************************
  * @brief  Description �洢��ʷ����
  * @param    		
  * @retval 		
  *****************************************************************************/
//void StorageHistoryData(void)
//{
//	if(Conf.Jly.WorkStatueIsStop >= 1)
//	{
//		SaveDataOnTimeDeal();
//	}
//}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
