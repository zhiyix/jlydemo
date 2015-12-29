/**
  ******************************************************************************
  * @file    bsp_storagedata.c
  * @author  
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   存储历史数据
  ******************************************************************************
  * @attention
  *
  * 实验平台:32 BITS Discovery
  *
  ******************************************************************************
  */ 

#include "main.h"


const char temp_unit[2]={0xA1,0xE6};//"温度符号"
const char shidu_unit[3]={0x25,0x52,0x48};//"%RH"


/******************************************************************************
  * @brief  Description 温湿度数据和时间组成一帧,温湿度数据由float转uint16
  * @param  channelnum  通道数		
  * @param  clockchoose		
  * @param  Gpschoose	选择GPS
  * @retval 无		
  *****************************************************************************/
static void ChannelDataDeal(uint8_t channelnum,uint8_t clockchoose,uint8_t Gpschoose)
{
    uint16_t i=0,j=0;
    uint16_t UtTmp;
    
    //---------------
    
    for(j=0;j<channelnum;j++)
    {
		if(Conf.Sensor[j].ChannelSwitch ==0)//通道开 保存数据
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
					DataBuf[i++]=0XFF&(UtTmp);	//数据低位
					DataBuf[i++]=0XFF&((UtTmp)>>8); //数据高位
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
	
	read_time();
    //---------------
    if(clockchoose==0)
    {
        DataBuf[i++]=Rtc.Year;//(5)
        DataBuf[i++]=Rtc.Month;
        DataBuf[i++]=Rtc.Day;
        DataBuf[i++]=Rtc.Hour;
        DataBuf[i++]=Rtc.Minute;
    }else
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

}

/******************************************************************************
  * @brief  Description 
  * @param  PointerAddr	读指针地址  		 	
  * @retval 无		
  *****************************************************************************/
uint16_t ReadU16Pointer(const uint16_t PointerAddr)
{
	union MyU16Data myu16;
	Fram_Read(myu16.Byte,PointerAddr,2); //读出 u16 2字节
	return myu16.Variable; //更新
}
/******************************************************************************
  * @brief  Description 
  * @param  PointerAddr	写指针地址  	
  * @param  Pointer		指针
  * @retval 无		
  *****************************************************************************/
void WriteU16Pointer(const uint16_t PointerAddr,uint16_t Pointer)
{
	union MyU16Data myu16;
	myu16.Variable = Pointer;
	Fram_Write(myu16.Byte,PointerAddr,2); //保存 u16 2字节
}


/******************************************************************************
  * @brief  Description 
  * @param  PointerAddr	读指针地址 		 	
  * @retval 无		
  *****************************************************************************/
uint32_t ReadU32Pointer(const uint32_t PointerAddr)
{
    union MyU32Data myu32; 
    Fram_Read(myu32.Byte,PointerAddr,4);    //读出 u32 4字节
    return myu32.Variable;	//更新
}
/******************************************************************************
  * @brief  Description 保存
  * @param  PointerAddr	写指针地址
  * @param  Pointer     指针
  * @retval 无		
  *****************************************************************************/
void WriteU32Pointer(const uint32_t PointerAddr,uint32_t Pointer)
{
    union MyU32Data myu32; 
    myu32.Variable = Pointer;
    Fram_Write(myu32.Byte, PointerAddr, 4); //保存 u32 4字节
}

/******************************************************************************
  * @brief  Description  设置flash溢出、未溢出标志
  * @param  flowvalue    0/1 1表示溢出
  * @retval 无		
  *****************************************************************************/
void SetFlashOverFlow(uint8_t flowvalue)
{
	uint8_t OverFlowBuf[1];
	
	Queue.FlashRecOverFlow = flowvalue;	
	OverFlowBuf[0] = flowvalue;
	Fram_Write(OverFlowBuf,FLASH_FlashRecOverFlowAddr,1);//存储flash有未溢出标志
}
/******************************************************************************
  * @brief  Description 保存数据到flash,先用2k 模拟实现
						未读条数减少，记录最大地址不变
写满一个扇区时擦除下一个扇区
flash写到 (Queue.FlashSectorPointer * FLASH_SectorPerSize/Queue.HIS_ONE_BYTES)*Queue.HIS_ONE_BYTES)时，
擦除下一个扇区
flash中每个扇区的字节都利用起来
测试：
	(1)做实验反复测试存储，8通道测试ok
	(2)反复测试未读条数的正确性，8通道测试ok
	(3)测试复位溢出标志
  * @param  无  		 	
  * @retval 无		
  *****************************************************************************/
static void SaveHisDataToFlash(void)
{
	//优化这三个变量的读取，节省时间，复位测试存储数据ok
	//Queue.ReadFlashDataPointer = ReadU32Pointer(FLASH_ReadDataAddr_Lchar);
	//Queue.FlashNoReadingDataNum = ReadU32Pointer(FLASH_NoReadingDataNumAddr_Lchar); //读未读条数
	//Queue.WriteFlashDataPointer = ReadU32Pointer(FLASH_WriteDataAddr_Lchar); //读取Flash写数据指针
	
	
	//flash扇区写满，擦除下一扇区
	//第一次擦除 Queue.RecFlashWritePointer =0
	//最后一次擦除 
	if((Queue.WriteFlashDataPointer < Queue.FlashRecActualStorage)&&(Queue.WriteFlashDataPointer % ((Queue.FlashSectorPointer * FLASH_SectorPerSize/Queue.HIS_ONE_BYTES)*Queue.HIS_ONE_BYTES) ==0))
	{
		Queue.FlashReadDataBeginPointer = ReadU32Pointer(FLASH_ReadDataBeginAddr_Lchar); //读 读数据起始指针
		Queue.FlashSectorPointer = ReadU16Pointer(FLASH_SectorWriteAddr_Lchar); //读取falsh sector存储指针
		
		SPI_FLASH_SectorErase(FLASH_SectorFirstAddr + (Queue.FlashSectorPointer * FLASH_SectorPerSize));//擦除扇区,考虑边界问题
		
		Queue.FlashSectorPointer++;	//Flash Sector指针加 1
		if(Queue.FlashSectorPointer >= FLASH_SectorNum)
		{
			Queue.FlashSectorPointer = 0;
		}
		WriteU16Pointer(FLASH_SectorWriteAddr_Lchar,Queue.FlashSectorPointer);	//保存Flash Sector存储指针 
		
		if(Queue.FlashRecOverFlow ==1)	//数据溢出，擦除当前扇区，读指针偏移 
		{
			//擦倒数第二个扇区
			if(Queue.WriteFlashDataPointer < (Queue.FlashRecActualStorage - FLASH_SectorPerSize - Queue.HIS_ONE_BYTES))//数据记录指针小于8192-4096-10
			{
				Queue.SectorHeadBytes = Queue.HIS_ONE_BYTES - (Queue.FlashSectorPointer*FLASH_SectorPerSize) % Queue.HIS_ONE_BYTES;
				Queue.FlashReadDataBeginPointer = Queue.FlashSectorPointer * FLASH_SectorPerSize + Queue.SectorHeadBytes;//Queue.FlashSectorPointer++之后 
			}else
			{//写最后一个扇区时
				Queue.FlashReadDataBeginPointer = 0;
			}
			//当读和写都为0时,包括两种情况 情况考虑清楚，没问题
			//情况1：一条数据也未读，Queue.ReadFlashDataPointer = Queue.FlashReadDataBeginPointer;
			//情况2：当数据存满溢出读和写都为0，Queue.ReadFlashDataPointer = Queue.FlashReadDataBeginPointer;
			if(((Queue.ReadFlashDataPointer - Queue.WriteFlashDataPointer) >= 0) &&((Queue.ReadFlashDataPointer - Queue.WriteFlashDataPointer) < FLASH_SectorPerSize))
			{
				Queue.ReadFlashDataPointer = Queue.FlashReadDataBeginPointer;
				WriteU32Pointer(FLASH_ReadDataAddr_Lchar,Queue.ReadFlashDataPointer);//保存读指针
			}
			
		}else
		{
			Queue.FlashReadDataBeginPointer = 0;
		}
		WriteU32Pointer(FLASH_ReadDataBeginAddr_Lchar,Queue.FlashReadDataBeginPointer);//保存读数据起始指针
	}
	//--------------------------历史数据写入flash，每次写入Queue.HIS_ONE_BYTES 大小
	SPI_FLASH_BufferWrite(DataBuf, Queue.WriteFlashDataPointer, Queue.HIS_ONE_BYTES);
	Queue.WriteFlashDataPointer += Queue.HIS_ONE_BYTES;	//Flash写数据指针加 一帧Byte
	if(Queue.WriteFlashDataPointer >= Queue.FlashRecActualStorage)
	{
		RecorderStopModeHandle();//记录仪记满 判断记录方式
		
		Queue.WriteFlashDataPointer = 0;//判断完记录方式 清除写指针
		if(Queue.FlashRecOverFlow ==0)//上电已读取
		{
			SetFlashOverFlow(1);//设置flash溢出标志
		}
	}
	WriteU32Pointer(FLASH_WriteDataAddr_Lchar,Queue.WriteFlashDataPointer); //保存Flash写数据指针
		
	//--------------------------未读数据条数计算
	if(Queue.ReadFlashDataPointer >= Queue.WriteFlashDataPointer)	//溢出后的处理
	{
		Queue.FlashNoReadingDataNum = (Queue.WriteFlashDataPointer + Queue.FlashRecActualStorage - Queue.ReadFlashDataPointer)/Queue.HIS_ONE_BYTES;
		//Queue.FlashNoReadingDataNum = Queue.WriteFlashDataPointer/Queue.HIS_ONE_BYTES + (Queue.FlashRecActualStorage - Queue.ReadFlashDataPointer)/Queue.HIS_ONE_BYTES;
	}else	//未溢出的处理
	{
		Queue.FlashNoReadingDataNum = (Queue.WriteFlashDataPointer - Queue.ReadFlashDataPointer)/Queue.HIS_ONE_BYTES;
	}	
	WriteU32Pointer(FLASH_NoReadingDataNumAddr_Lchar,Queue.FlashNoReadingDataNum);//保存未读数据指针
	
	//--------------------------历史数据条数计算
	if(Queue.FlashRecOverFlow < 1)
	{
		Queue.FlashHistoryDataNum = Queue.WriteFlashDataPointer/Queue.HIS_ONE_BYTES;//未溢出
	}else
	{
		Queue.FlashHistoryDataNum = (Queue.WriteFlashDataPointer + Queue.FlashRecActualStorage - Queue.FlashReadDataBeginPointer)/Queue.HIS_ONE_BYTES;
	}
	WriteU32Pointer(FLASH_HistoryDataNumAddr,Queue.FlashHistoryDataNum);//保存历史数据条数
}

/******************************************************************************
  * @brief  Description 读铁电数据到内存中
  * @param  无   			
  * @retval 无		
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
  * @brief  Description 读flash中的数据
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
    
    for(down_hisdata_count=RecPointerBeginAddr;down_hisdata_count < RecPointerEndAddr;)//- Queue.HIS_ONE_BYTES
    {
        SPI_FLASH_BufferRead(BufTemp,(FLASH_RecFirstAddr+down_hisdata_count),Queue.HIS_ONE_BYTES);
        down_hisdata_count += Queue.HIS_ONE_BYTES;
		//test---------------------
		JlyParam.DataNumInFlash ++;
		if(Flag.ChannelSwitchIsOn ==0)
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
  * @brief  Description 下载Flash中的数据
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
			printf("JlyParam.DataNumInFlash: %d\r\n",JlyParam.DataNumInFlash);
			JlyParam.DataNumInFlash = 0;
			Flag.TouchKey1DuanAn = 0;
		}
		if(Queue.FlashRecOverFlow >=1)
		{
			if(Queue.WriteFlashDataPointer < (Queue.FlashRecActualStorage - FLASH_SectorPerSize - Queue.HIS_ONE_BYTES))//如果正在写数据到最后一个扇区，如数据起始地址为0
			{
				ReadFlashHisData(Queue.FlashReadDataBeginPointer,Queue.FlashRecActualStorage); 
			}
			printf("Flow......: \r\n");
			ReadFlashHisData(0,RecorderPoint_temp);
			printf("JlyParam.DataNumInFlash: %d\r\n",JlyParam.DataNumInFlash);
			JlyParam.DataNumInFlash = 0;
			Flag.TouchKey1DuanAn = 0;            
		}
	}
}

/******************************************************************************
  * @brief  Description 到记录间隔保存数据到flash
  * @param  无  		
  * @retval 无		
  *****************************************************************************/
/*
void SaveDataOnTimeDeal(void)
{
	read_time();
	
	RtcBcdToD10(&Rtc);
	Rtc.TMPS=RTC_Date_Time_To_Second(&Rtc);
	
	RTC_Second_To_Date_Time(Rtc.TMPS,&Rtc);
	RtcD10ToBcd(&Rtc);
	
	if(Rtc.TMPS % JlyParam.NormalRecIntervalSec ==0)
	{   
		
		ChannelDataDeal(JlyParam.ChannelNumOld,Clock_choose,Gps_choose);
		SaveHisDataToFlash();
	}
}
*/
void SaveDataOnTimeDeal(void)
{
	
	ChannelDataDeal(JlyParam.ChannelNumOld,Clock_choose,Gps_choose);
	SaveHisDataToFlash();

}
/******************************************************************************
  * @brief  Description 存储历史数据
  * @param    		
  * @retval 		
  *****************************************************************************/
void StorageHistoryData(void)
{
	if((Conf.Jly.WorkStatueIsOrNotStop >= 1)&&(JlyParam.ChannelNumActual >0))
	{

		if(Flag.StorageData ==1)
		{
			Flag.StorageData =0;
			SaveDataOnTimeDeal();
		}
		
	}
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
