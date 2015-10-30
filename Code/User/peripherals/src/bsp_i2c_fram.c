/**
  ******************************************************************************
  * @file    bsp_i2c_fram.c
  * @author  
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   i2c FRAM (FM24CL64B)Ӧ�ú���bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:32 BITS Discovery
  *
  ******************************************************************************
  */ 

#include "main.h"
//#include "dev_i2c.h"

const char temp_unit[2]={0xA1,0xE6};//"�¶ȷ���"
const char shidu_unit[3]={0x25,0x52,0x48};//"%RH"

/******************************************************************************
  * @brief  Description ����I2C���ߵ�GPIO������ģ��IO�ķ�ʽʵ��
  * @param  ��			
  * @retval ��		
  *****************************************************************************/
void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_I2C_PORT, ENABLE);	/* ��GPIOʱ�� */

	GPIO_InitStructure.GPIO_Pin = BSP_I2C_SCL | BSP_I2C_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;/* ��©��� */
    
	GPIO_Init(BSP_I2C_PORT, &GPIO_InitStructure);

	/* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
//	i2c_Stop();
}
/******************************************************************************
  * @brief  Description ͨ�����ݴ���
  * @param  channelnum  ͨ����		
  * @param  clockchoose		
  * @param  Gpschoose	ѡ��GPS
  * @retval ��		
  *****************************************************************************/
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
/******************************************************************************
  * @brief  Description ��Fram�洢ָ��
  * @param  ��  		 	
  * @retval ��		
  *****************************************************************************/
static void ReadFramRecPointer(void)
{
	union MyU16Data myu16;
	Fram_Read(myu16.Byte,FRAM_RecAddr_Lchar,2); //����Fram�洢ָ�� u16 2�ֽ�
	Queue.RecorderFramPointer = myu16.Variable; //����Fram�洢ָ��
}
/******************************************************************************
  * @brief  Description ��Fram�洢ָ��
  * @param  ��  		 	
  * @retval ��		
  *****************************************************************************/
static void WriteFramRecPointer(void)
{
	union MyU16Data myu16;
	myu16.Variable = Queue.RecorderFramPointer;
	Fram_Write(myu16.Byte,FRAM_RecAddr_Lchar,2); //����Fram�洢ָ�� u16 2�ֽ�
}
/******************************************************************************
  * @brief  Description ��Flash�洢ָ��
  * @param  ��  		 	
  * @retval ��		
  *****************************************************************************/
static void ReadFlashRecPointer(void)
{
    union MyU32Data myu32; 
    Fram_Read(myu32.Byte,FLASH_RecAddr_Lchar,4);    //����Flash�洢ָ�� u32 4�ֽ�
    Queue.RecorderFlashPointer = myu32.Variable;	//����Flash�洢ָ��
}
/******************************************************************************
  * @brief  Description ��Flash�洢ָ��
  * @param  ��  		 	
  * @retval ��		
  *****************************************************************************/
static void WriteFlashRecPointer(void)
{
    union MyU32Data myu32; 
    myu32.Variable = Queue.RecorderFlashPointer;
    Fram_Write(myu32.Byte, FLASH_RecAddr_Lchar, 4); //����Flash�洢ָ�� u32 4�ֽ�
}
/******************************************************************************
  * @brief  Description �������ݵ�flash
  * @param  ��  		 	
  * @retval ��		
  *****************************************************************************/
void SaveHisDataToFlash(void)
{
    
    
}
/******************************************************************************
  * @brief  Description �������ݵ�����
  * @param  ��  		 	
  * @retval ��		
  *****************************************************************************/
void SaveHisDataToFram(void)
{
    //uint8_t TempBuf[512];
    uint16_t eeOffset;

	ReadFramRecPointer();	//��ȡFramָ��
    //ReadFlashRecAddr();
    
    eeOffset = Queue.RecorderFramPointer * HIS_ONE_BYTES; 
	Fram_Write(&DataBuf[3], (FRAM_RecFirstAddr+eeOffset), HIS_ONE_BYTES); //д��ʷ���ݵ�Fram��

    Queue.RecorderFramPointer++;	//������Fram�д���һ��ָ���1
    Queue.RecorderFlashPointer++;
    if(Queue.RecorderFramPointer >= HIS_MAX_NUM)
    {
        Queue.RecorderFramPointer = 0;
        Flag.RecordFramOverFlow = 1;
        
        //AI2C_Read(TempBuf,FRAM_RecFirstAddr,HIS_MAX_NUM*HIS_ONE_BYTES);
		
//		if(Queue.FlashSectorPoint%8 ==0)
//		{
//			if(Queue.FlashSectorPoint >= 8*2048)//flash��2048 sector
//			{
//				Queue.FlashSectorPoint = 0;
//			}
//			SPI_FLASH_SectorErase(FLASH_SectorFirstAddr+(Queue.FlashSectorPoint/8)*FLASH_SectorPerSize);//���Ǳ߽�����
//		}
		Queue.FlashSectorPointer++;
		//SPI_FLASH_SectorErase();
		//fram��һ�δ洢������flash��0��ַ��ʼ�洢,
//        SPI_FLASH_BufferWrite(TempBuf, (FLASH_RecFirstAddr+(Queue.RecorderFlashPoint-HIS_MAX_NUM)*HIS_ONE_BYTES), HIS_MAX_NUM*HIS_ONE_BYTES);
    
        if(Queue.RecorderFlashPointer >= Flash_MAX_NUM)
        {
            Queue.RecorderFlashPointer = 0;
            Flag.RecordFlashOverFlow =1;
            
        }
    }
	
    WriteFramRecPointer(); //���������ݣ����浱ǰFram�еļ�¼ָ��
	
    //WriteFlashRecPointer();
}
/******************************************************************************
  * @brief  Description ���������ݵ��ڴ���
  * @param  ��   			
  * @retval ��		
  *****************************************************************************/
void ReadFramHisDataToRam(void)
{
    uint8_t i=0,j=0,zhuangtai_temp=0;
    uint8_t BufA[HIS_ONE_BYTES+2+ID_BYTES];
    uint8_t Recorderpoint_L,Recorderpoint_H;
    uint16_t read_eeOffset_temp;    
    uint16_t RecorderPoint_temp;  
    
    AI2C_Read(&Recorderpoint_L,FRAM_RecAddr_Lchar,1);
	AI2C_Read(&Recorderpoint_H,FRAM_RecAddr_Hchar,1);
    Queue.RecorderFramPointer=Recorderpoint_L+(Recorderpoint_H<<8);
    RecorderPoint_temp = Queue.RecorderFramPointer;
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
/******************************************************************************
  * @brief  Description �������ݴ���
  * @param    		
  * @retval 		
  *****************************************************************************/
static void HisData_Tidy(uint16_t RecorderPoint_Begin,uint16_t RecorderPoint)
{   
    uint8_t Buf[HIS_ONE_BYTES+Headend_BYTES+ID_BYTES];
    uint8_t BufTemp[HIS_ONE_BYTES+Headend_BYTES+ID_BYTES];
    uint8_t i=0;
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
/******************************************************************************
  * @brief  Description ����Fram�е�����
  * @param    		
  * @retval 		
  *****************************************************************************/
void Down_HisData(void)
{
    uint16_t  RecorderPoint_temp;
    
    ReadFramRecPointer(); //
	
    RecorderPoint_temp = Queue.RecorderFramPointer;
    
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
        HisData_Tidy(RecorderPoint_temp,HIS_MAX_NUM); 
        
        HisData_Tidy(0,RecorderPoint_temp);
        Flag.TouchKey1DuanAn = 0;            
    }
}
/******************************************************************************
  * @brief  Description ��flash�е�����
  * @param    		
  * @retval 		
  *****************************************************************************/
void ReadFlashHisData(uint32_t RecorderPoint_Begin,uint32_t RecorderPoint)
{
	uint8_t Buf[HIS_ONE_BYTES+Headend_BYTES+ID_BYTES];
    uint8_t BufTemp[HIS_ONE_BYTES+Headend_BYTES+ID_BYTES];
    uint8_t i=0;
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
/******************************************************************************
  * @brief  Description ����Flash�е�����
  * @param    		
  * @retval 		
  *****************************************************************************/
void DownFlash_HisData(void)
{
    uint32_t  RecorderPoint_temp;
    
    ReadFlashRecPointer();
	
    RecorderPoint_temp = Queue.RecorderFlashPointer;
    
    
    if(RecorderPoint_temp==0)
    {
        Flag.Key1DuanAn = 0;
    }
    if(Flag.RecordFlashOverFlow ==0)
    {
        ReadFlashHisData(0,RecorderPoint_temp);
        Flag.Key1DuanAn = 0;
    }
    if(Flag.RecordFlashOverFlow >=1)
    {
        ReadFlashHisData(RecorderPoint_temp,Flash_MAX_NUM); 
        
        ReadFlashHisData(0,RecorderPoint_temp);
        Flag.Key1DuanAn = 0;            
    }
}
/******************************************************************************
  * @brief  Description ����
  * @param    		
  * @retval 		
  *****************************************************************************/
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
