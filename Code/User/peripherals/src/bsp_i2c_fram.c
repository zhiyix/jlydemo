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
                DataBuf[i++]=0XFF&(UtTmp);
                DataBuf[i++]=0XFF&((UtTmp)>>8);
            }
            
        }
    }
    //---------------
    if(Gpschoose==1)
    {
		
    }
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
        DataBuf[i++]=Rtc.Year;//(6)
        DataBuf[i++]=Rtc.Month;
        DataBuf[i++]=Rtc.Day;
        DataBuf[i++]=Rtc.Hour;
        DataBuf[i++]=Rtc.Minute;
        DataBuf[i++]=Rtc.Second;
    }
    //---------------
    
    zhuangtai_temp=0;
    DataBuf[i++]=zhuangtai_temp;
}
/******************************************************************************
  * @brief  Description 读Fram存储指针
  * @param  无  		 	
  * @retval 无		
  *****************************************************************************/
static uint16_t ReadFramRecPointer(void)
{
	union MyU16Data myu16;
	Fram_Read(myu16.Byte,FRAM_RecWriteAddr_Lchar,2); //读出Fram存储指针 u16 2字节
	return myu16.Variable; //更新Fram存储指针
}
/******************************************************************************
  * @brief  Description 保存Fram存储指针
  * @param  无  		 	
  * @retval 无		
  *****************************************************************************/
void WriteFramRecPointer(uint16_t Pointer)
{
	union MyU16Data myu16;
	myu16.Variable = Pointer;
	Fram_Write(myu16.Byte,FRAM_RecWriteAddr_Lchar,2); //保存Fram存储指针 u16 2字节
}


/******************************************************************************
  * @brief  Description 读Flash Sector存储指针
  * @param  无  		 	
  * @retval 无		
  *****************************************************************************/
static uint16_t ReadFlashSectorPointer(void)
{
	union MyU16Data myu16;
	Fram_Read(myu16.Byte,FLASH_SectorWriteAddr_Lchar,2); //读出Flash Sector存储指针 u16 2字节
	return myu16.Variable; //更新Flash Sector存储指针
}
/******************************************************************************
  * @brief  Description 保存Flash Sector存储指针
  * @param  无  		 	
  * @retval 无		
  *****************************************************************************/
void WriteFlashSectorPointer(uint16_t Pointer)
{
	union MyU16Data myu16;
	myu16.Variable = Pointer;
	Fram_Write(myu16.Byte,FLASH_SectorWriteAddr_Lchar,2); //保存Flash Sector存储指针 u16 2字节
}

/******************************************************************************
  * @brief  Description 读数据指针
  * @param  无  		 	
  * @retval 无		
  *****************************************************************************/
uint32_t ReadFlashDataPointer(void)
{
    union MyU32Data myu32; 
    Fram_Read(myu32.Byte,FLASH_ReadDataAddr_Lchar,4);    //读出 读数据指针 u32 4字节
    return myu32.Variable;	//更新读指针
}
/******************************************************************************
  * @brief  Description 保存读数据指针
  * @param  无  		 	
  * @retval 无		
  *****************************************************************************/
void WriteFlashDtatPointer(uint32_t Pointer)
{
    union MyU32Data myu32; 
    myu32.Variable = Pointer;
    Fram_Write(myu32.Byte, FLASH_ReadDataAddr_Lchar, 4); //保存读数据指针 u32 4字节
}

/******************************************************************************
  * @brief  Description 读Flash存储指针
  * @param  无  		 	
  * @retval 无		
  *****************************************************************************/
static uint32_t ReadFlashRecPointer(void)
{
    union MyU32Data myu32; 
    Fram_Read(myu32.Byte,FLASH_RecWriteAddr_Lchar,4);    //读出Flash存储指针 u32 4字节
    return myu32.Variable;	//更新Flash存储指针
}
/******************************************************************************
  * @brief  Description 保存Flash存储指针
  * @param  无  		 	
  * @retval 无		
  *****************************************************************************/
void WriteFlashRecPointer(uint32_t Pointer)
{
    union MyU32Data myu32; 
    myu32.Variable = Pointer;
    Fram_Write(myu32.Byte, FLASH_RecWriteAddr_Lchar, 4); //保存Flash存储指针 u32 4字节
}
/******************************************************************************
  * @brief  Description 保存数据到flash
  * @param  无  		 	
  * @retval 无		
  *****************************************************************************/
void SaveHisDataToFlash(void)
{
    uint32_t FlashOffset;
    
	Queue.FlashReadDataPointer = ReadFlashDataPointer(); //读 读数据指针
	Queue.FlashSectorPointer = ReadFlashSectorPointer(); //读取falsh sector存储指针
	Queue.RecFlashWritePointer = ReadFlashRecPointer(); //读取flash存储指针
	FlashOffset = Queue.RecFlashWritePointer * Queue.HIS_ONE_BYTES; //偏移量
	
	//flash扇区写满，擦除下一扇区
	//第一次擦除 Queue.RecFlashWritePointer =0
	//最后一次擦除 
	if((Queue.RecFlashWritePointer < (Queue.FLASH_MAX_NUM - 1))&&(Queue.RecFlashWritePointer % Queue.FLASH_SECTOR_PER_NUM ==0))
	{
		SPI_FLASH_SectorErase(FLASH_SectorFirstAddr + (Queue.FlashSectorPointer * FLASH_SectorPerSize));//考虑边界问题
		
		Queue.FlashSectorPointer++;	//Flash Sector指针加 1
		if(Queue.FlashSectorPointer >= FLASH_SectorNum)
		{
			Queue.FlashSectorPointer = 0;
		}
		WriteFlashSectorPointer(Queue.FlashSectorPointer);	//保存Flash Sector存储指针 
		
		if(Flag.RecordFlashOverFlow ==1)	//数据溢出，擦除当前扇区，读指针偏移 Queue.FLASH_SECTOR_PER_NUM
		{
			//倒数第二个扇区
			if(Queue.FlashReadDataPointer < (Queue.FLASH_MAX_NUM - Queue.FLASH_SECTOR_PER_NUM))
			{
				Queue.FlashReadDataPointer = Queue.FlashReadDataPointer + Queue.FLASH_SECTOR_PER_NUM;
			}
		}
	}
	//历史数据写入flash，每次写入Queue.HIS_ONE_BYTES 大小
	SPI_FLASH_BufferWrite(&DataBuf[3], (FLASH_RecFirstAddr + FlashOffset), Queue.HIS_ONE_BYTES);
	Queue.RecFlashWritePointer++;	//flash存储指针加 1
	if(Queue.RecFlashWritePointer >= Queue.FLASH_MAX_NUM)
	{
		Queue.RecFlashWritePointer = 0;
		Flag.RecordFlashOverFlow = 1;	//flash存满 溢出
	}
	WriteFlashRecPointer(Queue.RecFlashWritePointer); //保存Flash存储指针 
	
	if(Queue.FlashReadDataPointer < Queue.FLASH_MAX_NUM)
	{
		Queue.FlashReadDataPointer++;
	}else{
		Queue.FlashReadDataPointer = 0;
	}	
	WriteFlashDtatPointer(Queue.FlashReadDataPointer);
	
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

	Queue.RecFramWritePointer = ReadFramRecPointer();	//读取Fram指针
    //ReadFlashRecAddr();
    
    eeOffset = Queue.RecFramWritePointer * Queue.HIS_ONE_BYTES; 
	//Queue.RecFramWritePointer = 408,写完这一条 此时Fram已写满
	Fram_Write(&DataBuf[3], (FRAM_RecFirstAddr+eeOffset), Queue.HIS_ONE_BYTES); //写历史数据到Fram中
	
    Queue.RecFramWritePointer++;	//数据在Fram中存完一条指针加1
    Queue.RecFlashWritePointer++;
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
	
	if(Queue.RecFlashWritePointer >= (Queue.FLASH_MAX_NUM + 1))
	{
		Queue.RecFlashWritePointer = 0;
		Flag.RecordFlashOverFlow =1;
		
	}
    WriteFramRecPointer(Queue.RecFramWritePointer); //保存完数据，保存当前Fram中的记录指针
    WriteFlashRecPointer(Queue.RecFlashWritePointer); //保存Flash写指针
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
    
    AI2C_Read(&Recorderpoint_L,FRAM_RecWriteAddr_Lchar,1);
	AI2C_Read(&Recorderpoint_H,FRAM_RecWriteAddr_Hchar,1);
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
    
    Queue.RecFramWritePointer = ReadFramRecPointer(); //
	
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
void ReadFlashHisData(uint32_t RecorderPoint_Begin,uint32_t RecorderPoint)
{
	uint8_t Buf[HIS_ONE_MAX_BYTES+Headend_BYTES+ID_BYTES];
    uint8_t BufTemp[HIS_ONE_MAX_BYTES+Headend_BYTES+ID_BYTES];
    uint8_t i=0;
	uint16_t Temp;
    uint32_t down_hisdata_count,down_eeOffset;
    
    for(down_hisdata_count=RecorderPoint_Begin;down_hisdata_count<RecorderPoint;down_hisdata_count++)
    {
        down_eeOffset = down_hisdata_count*Queue.HIS_ONE_BYTES;
        
        SPI_FLASH_BufferRead(BufTemp,(FLASH_RecFirstAddr+down_eeOffset),Queue.HIS_ONE_BYTES);
        
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
  * @brief  Description 下载Flash中的数据
  * @param    		
  * @retval 		
  *****************************************************************************/
void DownFlash_HisData(void)
{
    uint32_t  RecorderPoint_temp;
    
    Queue.RecFlashWritePointer = ReadFlashRecPointer();
	
    RecorderPoint_temp = Queue.RecFlashWritePointer;
    
    
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
        ReadFlashHisData(RecorderPoint_temp,Queue.FLASH_MAX_NUM); 
        
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
