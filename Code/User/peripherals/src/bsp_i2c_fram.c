/**
  ******************************************************************************
  * @file    bsp_i2c_fram.c
  * @author  
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   i2c FRAM (FM24CL16B)应用函数bsp
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

/**
  * @brief  Description 配置I2C总线的GPIO，采用模拟IO的方式实现
  * @param  无			
  * @retval 无		
  */
void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_I2C_PORT, ENABLE);	/* 打开GPIO时钟 */

	GPIO_InitStructure.GPIO_Pin = BSP_I2C_SCL | BSP_I2C_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;/* 开漏输出 */
    
	GPIO_Init(BSP_I2C_PORT, &GPIO_InitStructure);

	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
//	i2c_Stop();
}
/**
  * @brief  Description 通道数据处理
  * @param  channelnum  通道数		
  * @param  clockchoose		
  * @param  Gpschoose	选择GPS
  * @retval 无		
  */
void ChannelDataDeal(uint8_t channelnum,uint8_t clockchoose,uint8_t Gpschoose)
{
    //-------------
	uint8_t zhuangtai_temp;
    uint16_t i=0,j=0;
    uint16_t UtTmp;
    
    DataBuf[i++]=0xaa;
	DataBuf[i++]= 0;
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
/**
  * @brief  Description 保存数据处理
  * @param  无  		 	
  * @retval 无		
  */
void SaveData(void)
{
    if(JlyParam.SaveDataTimeOnlyRead>=7)//至少7s v1.7
    {
        if(JlyParam.Save_Time == JlyParam.SaveDataTimeOnlyRead-5)//保存数据
        {
//            if(IsReadingI2c)
//            {
//                ++IsReadingI2cCt;
//                if(IsReadingI2cCt>200){IsReadingI2cCt=0;IsReadingI2c=0;}
//                save_time = SaveDataTimeOnlyRead-6;
//            }
//            else
//            {
//                if(FileContinuouslyReadTimeoutCt!=0)
//                {
//                    WriteGatherDataToRam(started_channel);
//                    Flag.IsWritenToRam = 1;
//                }
//                else if(Flag.IsWritenToRam)
//                {
//                    WriteE2promFromRam();
//                    Flag.IsWritenToRam = 0;
//                }
//                else
//                {
                      read_time();
                      ChannelDataDeal(Channel_count,Clock_choose,Gps_choose);
                      SaveHisDataToFram();
//                    Save_Channel_Data(started_channel);	//??????????
                    //Flag.recflag=1;//????,REC???
                    //showREC;
                    //display_mem();
//                }
//            }
        }
        if((--JlyParam.Save_Time)<=0)
        {
            JlyParam.Save_Time = JlyParam.SaveDataTimeOnlyRead;
        }
    }
    else
    {
        if((--JlyParam.Save_Time)<=0)
        {
//            if(IsReadingI2c)
//            {
//                ++IsReadingI2cCt;
//                if(IsReadingI2cCt>200){IsReadingI2cCt=0;IsReadingI2c=0;}
//                save_time = SaveDataTimeOnlyRead-1;
//            }
//            else
//            {
                JlyParam.Save_Time = JlyParam.SaveDataTimeOnlyRead;
//                
//                if(FileContinuouslyReadTimeoutCt!=0)
//                {
//                    WriteGatherDataToRam(started_channel);
//                    Flag.IsWritenToRam = 1;
//                }
//                else if(Flag.IsWritenToRam)
//                {
//                    WriteE2promFromRam();
//                    Flag.IsWritenToRam = 0;
//                }
//                else
//                {
                    read_time();
                    ChannelDataDeal(Channel_count,Clock_choose,Gps_choose);
                    SaveHisDataToFram();	//??????????
                    
//                }
//            }
        }
    }
    
//    if((--JlyParam.Save_Time)<=0)
//    {
//        JlyParam.Save_Time = JlyParam.SaveDataTimeOnlyRead;
//    }

    Flag.IsDisplayRightNow=1;
}
/**
  * @brief  Description 读Flash存储指针
  * @param  无  		 	
  * @retval 无		
  */
static void ReadFlashRecAddr(void)
{
    union MYU32 myu32; 
    AI2C_Read(myu32.Byte,FLASH_RecAddr_Lchar,4);    //读出Flash存储指针
    Queue.RecorderFlashPoint = myu32.Variable;
}
/**
  * @brief  Description 存Flash存储指针
  * @param  无  		 	
  * @retval 无		
  */
static void WriteFlashRecAddr(void)
{
    union MYU32 myu32; 
    myu32.Variable = Queue.RecorderFlashPoint;
    AI2C_Write(myu32.Byte, FLASH_RecAddr_Lchar, 4); //保存Flash存储指针
}
/**
  * @brief  Description 保存数据到flash
  * @param  无  		 	
  * @retval 无		
  */
void SaveHisDataToFlash(void)
{
    
    
}
/**
  * @brief  Description 保存数据到铁电
  * @param  无  		 	
  * @retval 无		
  */
void SaveHisDataToFram(void)
{
	uint8_t Recorderpoint_L,Recorderpoint_H;
    uint8_t TempBuf[512];
    uint16_t eeOffset;

	AI2C_Read(&Recorderpoint_L,FRAM_RecAddr_Lchar,1);
	AI2C_Read(&Recorderpoint_H,FRAM_RecAddr_Hchar,1);
    Queue.RecorderPoint=Recorderpoint_L+(Recorderpoint_H<<8);
    ReadFlashRecAddr();
    
    eeOffset = Queue.RecorderPoint*HIS_ONE_BYTES; 
	AI2C_Write(&DataBuf[3], (FRAM_RecFirstAddr+eeOffset), HIS_ONE_BYTES);

    Queue.RecorderPoint++;
    Queue.RecorderFlashPoint ++;
    if(Queue.RecorderPoint >= HIS_MAX_NUM)
    {
        Queue.RecorderPoint=0;
        Flag.RecordFramOverFlow=1;
        
        AI2C_Read(TempBuf,FRAM_RecFirstAddr,HIS_MAX_NUM*HIS_ONE_BYTES);
		
		if(Queue.FlashSectorPoint%8 ==0)
		{
			if(Queue.FlashSectorPoint >= 8*2048)//flash有2048 sector
			{
				Queue.FlashSectorPoint = 0;
			}
			SPI_FLASH_SectorErase(FLASH_SectorFirstAddr+(Queue.FlashSectorPoint/8)*FLASH_SectorPerSize);//考虑边界问题
		}
		Queue.FlashSectorPoint++;
		//SPI_FLASH_SectorErase();
		//fram第一次存储满，从flash中0地址开始存储,
        SPI_FLASH_BufferWrite(TempBuf, (FLASH_RecFirstAddr+(Queue.RecorderFlashPoint-HIS_MAX_NUM)*HIS_ONE_BYTES), HIS_MAX_NUM*HIS_ONE_BYTES);
    
        if(Queue.RecorderFlashPoint >=Flash_MAX_NUM)
        {
            Queue.RecorderFlashPoint = 0;
            Flag.RecordFlashOverFlow =1;
            
        }
    }
	
    Recorderpoint_L=(uint8_t)Queue.RecorderPoint&0X00FF;
    Recorderpoint_H=(uint8_t)Queue.RecorderPoint>>8;
    
	AI2C_Write(&Recorderpoint_L, FRAM_RecAddr_Lchar, 1);
	AI2C_Write(&Recorderpoint_H, FRAM_RecAddr_Hchar, 1);
    WriteFlashRecAddr();
}
/**
  * @brief  Description 读铁电数据到内存中
  * @param  无   			
  * @retval 无		
  */
void ReadFramHisDataToRam(void)
{
    uint8_t i=0,j=0,zhuangtai_temp=0;
    uint8_t BufA[HIS_ONE_BYTES+2+ID_BYTES];
    uint8_t Recorderpoint_L,Recorderpoint_H;
    uint16_t read_eeOffset_temp;    
    uint16_t RecorderPoint_temp;  
    
    AI2C_Read(&Recorderpoint_L,FRAM_RecAddr_Lchar,1);
	AI2C_Read(&Recorderpoint_H,FRAM_RecAddr_Hchar,1);
    Queue.RecorderPoint=Recorderpoint_L+(Recorderpoint_H<<8);
    RecorderPoint_temp = Queue.RecorderPoint;
    read_eeOffset_temp = RecorderPoint_temp*HIS_ONE_BYTES;
    
    BufA[i++]=0xaa;//
    BufA[i++]=0;//ID(2) 
    BufA[i++]=1;
    
    //BufA[i++]=(u8)0xff&(*((int*)BSQ_XX_ADDR));
    //BufA[i++]=(u8)0xff&(*((int*)BSQ_XX_ADDR)>>8);
    AI2C_Read(&BufA[i],(FRAM_RecFirstAddr+read_eeOffset_temp),HIS_ONE_BYTES);
    
    zhuangtai_temp=0x66;
    BufA[i+HIS_ONE_BYTES]=zhuangtai_temp;
    
    printf("\r\n");
    for(j=0;j<(HIS_ONE_BYTES+Headend_BYTES+ID_BYTES);j++)
    {
        printf("%02x ",BufA[j]);
    }  
    printf("%d ",RecorderPoint_temp);
    printf("\r\n");
    for(j=0;j<(HIS_ONE_BYTES+Headend_BYTES+ID_BYTES);j++)
    {
        BufA[i] = 0;
        
    }
}
/**
  * @brief  Description 下载数据处理
  * @param    		
  * @retval 		
  */
static void HisData_Tidy(uint16_t RecorderPoint_Begin,uint16_t RecorderPoint)
{   
    uint8_t Buf[HIS_ONE_BYTES+Headend_BYTES+ID_BYTES];
    uint8_t BufTemp[HIS_ONE_BYTES+Headend_BYTES+ID_BYTES];
    uint8_t i=0,j=0,PagLen;
    uint16_t  down_hisdata_count,down_eeOffset,Temp;
    
    for(down_hisdata_count=RecorderPoint_Begin;down_hisdata_count<RecorderPoint;down_hisdata_count++)
    {
        down_eeOffset = down_hisdata_count*HIS_ONE_BYTES;
        
        AI2C_Read(BufTemp,(FRAM_RecFirstAddr+down_eeOffset),HIS_ONE_BYTES);
        
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
/**
  * @brief  Description 下载Fram中的数据
  * @param    		
  * @retval 		
  */
void Down_HisData(void)
{
    uint8_t Recorderpoint_L,Recorderpoint_H;
    uint16_t  RecorderPoint_temp;
    
    AI2C_Read(&Recorderpoint_L,FRAM_RecAddr_Lchar,1);
	AI2C_Read(&Recorderpoint_H,FRAM_RecAddr_Hchar,1);
    Queue.RecorderPoint=Recorderpoint_L+(Recorderpoint_H<<8);
    RecorderPoint_temp = Queue.RecorderPoint;
    
    //RecorderPoint_temp=3;
    //Flag.Record_BelowMax =0;
    if(RecorderPoint_temp==0)
    {
        Flag.KeyDuanAn = 0;
    }
    if(Flag.RecordFramOverFlow ==0)
    {
        HisData_Tidy(0,RecorderPoint_temp);
        Flag.KeyDuanAn = 0;
    }
    if(Flag.RecordFramOverFlow >=1)
    {
        HisData_Tidy(RecorderPoint_temp,HIS_MAX_NUM); 
        
        HisData_Tidy(0,RecorderPoint_temp);
        Flag.KeyDuanAn = 0;            
    }
}
/**
  * @brief  Description 读flash中的数据
  * @param    		
  * @retval 		
  */
void ReadFlashHisData(uint32_t RecorderPoint_Begin,uint32_t RecorderPoint)
{
	uint8_t Buf[HIS_ONE_BYTES+Headend_BYTES+ID_BYTES];
    uint8_t BufTemp[HIS_ONE_BYTES+Headend_BYTES+ID_BYTES];
    uint8_t i=0,j=0,PagLen;
	uint16_t Temp;
    uint32_t down_hisdata_count,down_eeOffset;
    
    for(down_hisdata_count=RecorderPoint_Begin;down_hisdata_count<RecorderPoint;down_hisdata_count++)
    {
        down_eeOffset = down_hisdata_count*HIS_ONE_BYTES;
        
        SPI_FLASH_BufferRead(BufTemp,(FLASH_RecFirstAddr+down_eeOffset),HIS_ONE_BYTES);
        
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
/**
  * @brief  Description 下载Flash中的数据
  * @param    		
  * @retval 		
  */
void DownFlash_HisData(void)
{
    uint32_t  RecorderPoint_temp;
    
    ReadFlashRecAddr();
    RecorderPoint_temp = Queue.RecorderFlashPoint;
    
    
    if(RecorderPoint_temp==0)
    {
        Flag.KeyDuanAn = 0;
    }
    if(Flag.RecordFlashOverFlow ==0)
    {
        ReadFlashHisData(0,RecorderPoint_temp);
        Flag.KeyDuanAn = 0;
    }
    if(Flag.RecordFlashOverFlow >=1)
    {
        ReadFlashHisData(RecorderPoint_temp,Flash_MAX_NUM); 
        
        ReadFlashHisData(0,RecorderPoint_temp);
        Flag.KeyDuanAn = 0;            
    }
}
/**
  * @brief  Description 测试
  * @param    		
  * @retval 		
  */
void Fram_Test(void)
{
	char i=0;
	uint8_t Buf[15]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    uint8_t value[20]={0};
	AI2C_Write(&Buf[0], FRAM_RecAddr_Hchar+2,10);
	
	AI2C_Read(&value[0],FRAM_RecAddr_Hchar+2,10);
    printf("\r\n");
    for(i=0;i<10;i++)
        printf("%d ",value[i]);
    
    printf("\r\n");
}
