/**
  ******************************************************************************
  * @file    bsp_i2c_fram.c
  * @author  
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   i2c FRAM (FM24CL64B)应用函数bsp
  ******************************************************************************
  * @attention
  *
  * 实验平台:32 BITS Discovery
  *
  ******************************************************************************
  */ 

#include "main.h"
//#include "dev_i2c.h"

const char temp_unit[2]={0xA1,0xE6};//"温度符号"
const char shidu_unit[3]={0x25,0x52,0x48};//"%RH"


/******************************************************************************
  * @brief  Description 通道数据处理
  * @param  channelnum  通道数		
  * @param  clockchoose		
  * @param  Gpschoose	选择GPS
  * @retval 无		
  *****************************************************************************/
void ChannelDataDeal(uint8_t channelnum,uint8_t clockchoose,uint8_t Gpschoose)
{
    //-------------
	uint8_t zhuangtai_temp;
    uint16_t i=0,j=0;
    uint16_t UtTmp;
    
    DataBuf[i++]=0xaa; //数据头
	DataBuf[i++]= 0;   //SN号
	DataBuf[i++]= 1;
    //---------------
    
    for(j=0;j<channelnum;j++)
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
  * @brief  Description 保存数据到flash,先用2k 模拟实现
						未读条数减少，记录最大地址不变
写满一个扇区时擦除下一个扇区
flash写到 Queue.FLASH_SECTOR_PER_NUM - 1时，Queue.WriteFlashDataPointer++，
此时Queue.WriteFlashDataPointer % Queue.FLASH_SECTOR_PER_NUM，擦除下一个扇区
flash中每个扇区的字节都利用起来
  * @param  无  		 	
  * @retval 无		
  *****************************************************************************/
void SaveHisDataToFlash(void)
{
	uint8_t OverFlowBuf[1];
    //uint32_t FlashOffset;
    
	Queue.ReadFlashDataPointer = ReadU32Pointer(FLASH_ReadDataAddr_Lchar);
	Queue.FlashNoReadingDataNum = ReadU32Pointer(FLASH_NoReadingDataNumAddr_Lchar); //读未读条数
	
	Queue.WriteFlashDataPointer = ReadU32Pointer(FLASH_WriteDataAddr_Lchar); //读取Flash写数据指针
	//FlashOffset = Queue.WriteFlashDataPointer * Queue.HIS_ONE_BYTES; //偏移量
	
	//flash扇区写满，擦除下一扇区
	//第一次擦除 Queue.RecFlashWritePointer =0
	//最后一次擦除 
	if((Queue.WriteFlashDataPointer < (FLASH_RecMaxSize - Queue.HIS_ONE_BYTES))&&(Queue.WriteFlashDataPointer % ((Queue.FlashSectorPointer * FLASH_SectorPerSize/Queue.HIS_ONE_BYTES)*Queue.HIS_ONE_BYTES) ==0))
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
		
		if(Flag.RecordFlashOverFlow ==1)	//数据溢出，擦除当前扇区，读指针偏移 Queue.FLASH_SECTOR_PER_NUM
		{
			//倒数第二个扇区
			if(Queue.WriteFlashDataPointer < (FLASH_RecMaxSize - FLASH_SectorPerSize - Queue.HIS_ONE_BYTES))//数据记录指针小于8192-4096-10
			{
				Queue.SectorHeadBytes = Queue.HIS_ONE_BYTES - (Queue.FlashSectorPointer * FLASH_SectorPerSize )%Queue.HIS_ONE_BYTES;
				Queue.FlashReadDataBeginPointer = Queue.FlashSectorPointer * FLASH_SectorPerSize + Queue.SectorHeadBytes;//Queue.FlashSectorPointer++之后 
			}else{
				Queue.FlashReadDataBeginPointer = 0;
				Queue.ReadFlashDataPointer = 0;
				WriteU32Pointer(FLASH_ReadDataAddr_Lchar,Queue.ReadFlashDataPointer);//保存读指针
			}
		}else{
			Queue.FlashReadDataBeginPointer = 0;
			Queue.ReadFlashDataPointer = 0;
			WriteU32Pointer(FLASH_ReadDataAddr_Lchar,Queue.ReadFlashDataPointer);//保存读指针
		}
		WriteU32Pointer(FLASH_ReadDataBeginAddr_Lchar,Queue.FlashReadDataBeginPointer);//保存读数据起始指针
	}
	//历史数据写入flash，每次写入Queue.HIS_ONE_BYTES 大小
	SPI_FLASH_BufferWrite(&DataBuf[3], Queue.WriteFlashDataPointer, Queue.HIS_ONE_BYTES);
	Queue.WriteFlashDataPointer += Queue.HIS_ONE_BYTES;	//Flash写数据指针加 一帧Byte
	if(Queue.WriteFlashDataPointer >= FLASH_RecMaxSize)
	{
		Queue.WriteFlashDataPointer = 0;
		if(Flag.RecordFlashOverFlow ==0)
		{
			Flag.RecordFlashOverFlow = 1;	//flash存满 溢出
			OverFlowBuf[0] = 1;
			Fram_Write(OverFlowBuf,FLASH_RecordFlashOverFlow,1);//存储flash溢出标志
		}
	}
	WriteU32Pointer(FLASH_WriteDataAddr_Lchar,Queue.WriteFlashDataPointer); //保存Flash写数据指针
		
	if(Flag.RecordFlashOverFlow == 0)
	{
		if(Queue.FlashNoReadingDataNum < Queue.FLASH_MAX_NUM)
		{
			Queue.FlashNoReadingDataNum++;
		}
	}else{		
		if(Queue.FlashNoReadingDataNum < ((Queue.FLASH_MAX_NUM - Queue.FLASH_SECTOR_PER_NUM) + Queue.WriteFlashDataPointer%FLASH_SectorPerSize)) //已经擦除了一个扇区
		{
			Queue.FlashNoReadingDataNum++;
		}else{//819-409+x/409=410+x/409
			Queue.FlashNoReadingDataNum = (Queue.FLASH_MAX_NUM - Queue.FLASH_SECTOR_PER_NUM) + Queue.WriteFlashDataPointer%FLASH_SectorPerSize;
		}
		
		if(((Queue.ReadFlashDataPointer - Queue.WriteFlashDataPointer)>0) &&((Queue.ReadFlashDataPointer - Queue.WriteFlashDataPointer) < FLASH_SectorPerSize))
		{
			Queue.ReadFlashDataPointer = Queue.FlashReadDataBeginPointer;
			WriteU32Pointer(FLASH_ReadDataAddr_Lchar,Queue.ReadFlashDataPointer);//保存读指针
		}
	}
		
	WriteU32Pointer(FLASH_NoReadingDataNumAddr_Lchar,Queue.FlashNoReadingDataNum);//保存未读数据指针
}
/******************************************************************************
  * @brief  Description 保存数据到铁电,这种方法在Fram中最后有几个字节没有使用
  * @param  无  		 	
  * @retval 无		
  *****************************************************************************/
void SaveHisDataToFram(void)
{
    uint8_t TempBuf[FLASH_SectorPerSize];
    uint16_t eeOffset;

	Queue.RecFramWritePointer = ReadU16Pointer(FRAM_RecWriteAddr_Lchar);	//读取Fram指针
    //ReadFlashRecAddr();
    
    eeOffset = Queue.RecFramWritePointer * Queue.HIS_ONE_BYTES; 
	//Queue.RecFramWritePointer = 408,写完这一条 此时Fram已写满
	Fram_Write(&DataBuf[3], (FRAM_RecFirstAddr+eeOffset), Queue.HIS_ONE_BYTES); //写历史数据到Fram中
	
    Queue.RecFramWritePointer++;	//数据在Fram中存完一条指针加1
    Queue.WriteFlashDataPointer++;
	//Queue.FRAM_MAX_NUM = 409 ,这时清除Fram写指针
    if(Queue.RecFramWritePointer >= Queue.FRAM_MAX_NUM )
    {
        Queue.RecFramWritePointer = 0;
        Flag.RecordFramOverFlow = 1;
        
        Fram_Read(TempBuf,FRAM_RecFirstAddr,Queue.FRAM_MAX_NUM*Queue.HIS_ONE_BYTES);
		
		SPI_FLASH_SectorErase(FLASH_SectorFirstAddr + (Queue.FlashSectorPointer * FLASH_SectorPerSize));//考虑边界问题
		
		Queue.FlashSectorPointer++;
		
		//fram第一次存储满，从flash中0地址开始存储,
        SPI_FLASH_BufferWrite(TempBuf, (FLASH_RecFirstAddr + Queue.FlashSectorPointer * FLASH_SectorPerSize), Queue.FRAM_MAX_NUM*Queue.HIS_ONE_BYTES);
    
    }
	if(Queue.FlashSectorPointer >= FLASH_SectorNum)
	{
		Queue.FlashSectorPointer = 0;	//Flash扇区指针清零
	}
	
	if(Queue.WriteFlashDataPointer >= (Queue.FLASH_MAX_NUM + 1))
	{
		Queue.WriteFlashDataPointer = 0;
		Flag.RecordFlashOverFlow =1;
		
	}
    WriteU16Pointer(FRAM_RecWriteAddr_Lchar,Queue.RecFramWritePointer); //保存完数据，保存当前Fram中的记录指针
    WriteU32Pointer(FLASH_WriteDataAddr_Lchar,Queue.WriteFlashDataPointer); //保存Flash写指针
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
  * @brief  Description 下载数据处理
  * @param    		
  * @retval 		
  *****************************************************************************/
static void HisData_Tidy(uint16_t RecorderPoint_Begin,uint16_t RecorderPoint)
{   
    uint8_t Buf[HIS_ONE_MAX_BYTES+Headend_BYTES+ID_BYTES];
    uint8_t BufTemp[HIS_ONE_MAX_BYTES+Headend_BYTES+ID_BYTES];
    uint8_t i=0;
    uint16_t  down_hisdata_count,down_eeOffset,Temp;
    
    for(down_hisdata_count=RecorderPoint_Begin;down_hisdata_count<RecorderPoint;down_hisdata_count++)
    {
        down_eeOffset = down_hisdata_count*Queue.HIS_ONE_BYTES;
        
        AI2C_Read(BufTemp,(FRAM_RecFirstAddr+down_eeOffset),Queue.HIS_ONE_BYTES);
        
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
        
        printf("%02x-%02x-%02x %02x:%02x:%02x ",Buf[0],Buf[1],Buf[2],Buf[3],Buf[4],Buf[5]);
        printf("%d%d.%d %d%d.%d\r\n",Buf[6],Buf[7],Buf[8],Buf[9],Buf[10],Buf[11]);
        
    }
}
/******************************************************************************
  * @brief  Description 下载Fram中的数据
  * @param    		
  * @retval 		
  *****************************************************************************/
void Down_HisData(void)
{
    uint16_t  RecorderPoint_temp;
    
    Queue.RecFramWritePointer = ReadU16Pointer(FRAM_RecWriteAddr_Lchar); //
	
    RecorderPoint_temp = Queue.RecFramWritePointer;
    
    //RecorderPoint_temp=3;
    //Flag.Record_BelowMax =0;
    if(RecorderPoint_temp==0)
    {
        Flag.TouchKey1DuanAn = 0;
    }
    if(Flag.RecordFramOverFlow ==0)
    {
        HisData_Tidy(0,RecorderPoint_temp);
        Flag.TouchKey1DuanAn = 0;
    }
    if(Flag.RecordFramOverFlow >=1)
    {
        HisData_Tidy(RecorderPoint_temp,Queue.FRAM_MAX_NUM); 
        
        HisData_Tidy(0,RecorderPoint_temp);
        Flag.TouchKey1DuanAn = 0;            
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
    
    for(down_hisdata_count=RecPointerBeginAddr;down_hisdata_count < (RecPointerEndAddr - Queue.HIS_ONE_BYTES);)
    {
		down_hisdata_count += Queue.HIS_ONE_BYTES;
        SPI_FLASH_BufferRead(BufTemp,(FLASH_RecFirstAddr+down_hisdata_count),Queue.HIS_ONE_BYTES);
        
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
	
    RecorderPoint_temp = Queue.WriteFlashDataPointer;
    
    
    if(RecorderPoint_temp==0)
    {
        Flag.TouchKey1DuanAn = 0;
    }
    if(Flag.RecordFlashOverFlow ==0)
    {
        ReadFlashHisData(0,RecorderPoint_temp);
        Flag.TouchKey1DuanAn = 0;
    }
    if(Flag.RecordFlashOverFlow >=1)
    {
		if(Queue.WriteFlashDataPointer < (FLASH_RecMaxSize - FLASH_SectorPerSize - Queue.HIS_ONE_BYTES))//如果正在写数据到最后一个扇区，如数据起始地址为0
		{
			ReadFlashHisData(Queue.FlashReadDataBeginPointer,FLASH_RecMaxSize); 
		}
        ReadFlashHisData(0,RecorderPoint_temp);
        Flag.TouchKey1DuanAn = 0;            
    }
}
/******************************************************************************
  * @brief  Description 测试
  * @param    		
  * @retval 		
  *****************************************************************************/
void Fram_Test(void)
{
	char i=0;
	uint8_t Buf[15]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    uint8_t value[20]={0};
	AI2C_Write(&Buf[0], FRAM_RecWriteAddr_Hchar+2,10);
	
	AI2C_Read(&value[0],FRAM_RecWriteAddr_Hchar+2,10);
    printf("\r\n");
    for(i=0;i<10;i++)
        printf("%d ",value[i]);
    
    printf("\r\n");
}
