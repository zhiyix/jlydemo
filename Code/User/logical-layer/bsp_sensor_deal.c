/**
  ******************************************************************************
  * @file              : bsp_sensor_deal.c
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
#ifndef __BSPSENSORDEAL_C
#define __BSPSENSORDEAL_C

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

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Description  计算开启的通道
  * @param  chanel_num   总的通道数量		
  * @retval start_chanel 开启的通道		
  */
uint32_t GetStartChanel(uint8_t chanel_num)
{
    uint8_t  i;
    uint32_t start_chanel;
   
    for(i=0;i<chanel_num;i++)
    {
        start_chanel |= (0x01<<i);
    }
    return start_chanel;
}
/**
  * @brief  Description 通道数据校准
  * @param  ChannelItem  		
  * @retval bool		
  */
static float Chang_to_Shishu(unsigned char ChannelItem)
{
    float low_data;//
    float top_data;
    float temp;//
    float ft00;
    unsigned int adjust_fa;
    unsigned int t;
    unsigned char x;
    unsigned char y;
    unsigned char j;
    unsigned char fuhao;
    unsigned char div_member;

    low_data = 1.0;
    x = RESET_CHANNEL_SETUP_TABLE[0x08*3+2*ChannelItem]; 
    y = RESET_CHANNEL_SETUP_TABLE[0x08*3+2*ChannelItem+1];
    if(x>=0x80)
    {
        fuhao = 1;
        x = x - 0x80;
    }
    else
        fuhao = 0;

    for(j=0;j<y;j++)
        low_data = low_data *10;
    low_data = low_data*x;
    if(fuhao==1)
    {
        fuhao=0;
        low_data = (-1.0)*low_data;
    }
    
    top_data = 1.0;
    x = RESET_CHANNEL_SETUP_TABLE[0x08*5+2*ChannelItem]; 
    y = RESET_CHANNEL_SETUP_TABLE[0x08*5+2*ChannelItem+1];
    if(x>=0x80)
    {
        fuhao = 1;
        x = x - 0x80;
    }
    else
        fuhao = 0;
    for(j=0;j<y;j++)
        top_data = top_data *10;
    top_data = top_data*x;
    if(fuhao==1)
    {
        fuhao=0;
        top_data = (-1.0)*top_data;
    }
    
    adjust_fa = RESET_CHANNEL_SETUP_TABLE[0x08*7+ChannelItem]-1;
    div_member = AdjustCurveFirAddress[adjust_fa][0];

    x=0;
//    WDT_START;
    while( adc[ChannelItem] > Char_to_Int(AdjustCurveFirAddress[adjust_fa][ADJUST_TABLE_HEAD_LENGTH+2*x],AdjustCurveFirAddress[adjust_fa][ADJUST_TABLE_HEAD_LENGTH+2*x+1]) && x < div_member )
      x++;
    //----------------------------------------//
    if(x==0)temp = low_data;
    else if(x>=div_member)temp = top_data;
    else
    {
          float SegValue0;
          float SegOffset,SegOffset1;
          SegValue0 = 0;
          SegOffset = 0;
          t = ADJUST_TABLE_HEAD_LENGTH + div_member*2 + x;
          y = AdjustCurveFirAddress[adjust_fa][t];
          for(j=0;j<x-1;j++)
          {
              SegValue0 = SegValue0 + AdjustCurveFirAddress[adjust_fa][ADJUST_TABLE_HEAD_LENGTH+ div_member*2 + j];
          }
//          WDT_START;
          SegOffset = AdjustCurveFirAddress[adjust_fa][ADJUST_TABLE_HEAD_LENGTH+ div_member*2 + j];
          SegValue0 = low_data + SegValue0;
          
          t = ADJUST_TABLE_HEAD_LENGTH + (x-1)*2;//??ad????????
          ft00 = adc[ChannelItem] - Char_to_Int(AdjustCurveFirAddress[adjust_fa][t],AdjustCurveFirAddress[adjust_fa][t+1]);//????????????
          SegOffset1 =  Char_to_Int(AdjustCurveFirAddress[adjust_fa][t],AdjustCurveFirAddress[adjust_fa][t+1]);
          SegOffset1 =  Char_to_Int(AdjustCurveFirAddress[adjust_fa][t+2],AdjustCurveFirAddress[adjust_fa][t+3])-SegOffset1;
          temp = SegValue0 +  (ft00/SegOffset1)*SegOffset ;//??????
     }
     if(temp>top_data) temp = top_data;
     return temp;
}
/**
  * @brief  Description 传感器类型判断，并对数据处理
  * @param  sensortype  		
  * @retval bool		
  */
static void Sensor_Deal(uint8_t sensortype,uint8_t i)
{
    float temp;
    
    switch(sensortype)
    {
        case NULL:
            
            break;
        //温度处理
        case SENSOR_TEMP:
                ChannelDataFloat[i] = Chang_to_Shishu(i);
                FlagSeniorErr[i]=0;//v2.0
                if((adc[i]<ADC_ERR_L)|(adc[i]>ADC_ERR_H))
                {
                    FlagSeniorErr[i]=1;
                }
            break;
        //湿度处理
        case SENSOR_HUMI:
                if(adc[i]<ADC_ERR_L)
                {
                    FlagSeniorErr[i]=1;
                }
                else
                {   //HIH5030
                    //0.00636y+0.1515=V输出/V3.3=adc/4096  (ADC基准采用外部3.3v)//HIH5030湿度校准
                    //y=(adc/4096-0.1515)/0.00636
                    
                    FlagSeniorErr[i]=0;
                    temp=adc[i];
                    temp=((temp/4096)-0.1515)/0.00636;
                    
                    if(FlagSeniorErr[0]!=1)
                    temp=temp/(1.0546-0.00216*ChannelDataFloat[0]);//??????????????????????,ChannelDataFloat[0]存在疑问
                    
                    temp=temp*100.0;
                    adc[i]=(unsigned int)temp;//采样的实际值
                    if(adc[i]%10>4)//四舍五入
                    adc[i]=adc[i]+10;
                    adc[i]=adc[i]/10;  //为采样值的10倍
                    ChannelDataFloat[i]=Chang_to_Shishu(i);//校正后的值
                }
            break;
        
        default:
            break;
    }
}

/**
  * @brief  Description 把通道对应数据转换成实际对应的物理量
  * @param  ChannelCode 
  * @retval 无		
  */
void DoGatherChannelDataFloat(unsigned char ChannelCode)
{
    uint8_t i;
    uint8_t ChiFang2 = 0x01;
    for(i=1;i<33;i++)
    {
        if(ChannelCode & ChiFang2)
        {
            switch(i)
            {
                case 1:
                    Sensor_Deal(Sensor1.sensor1_str.sensor_type,1-1);
                    break;
                
                case 2:
                    Sensor_Deal(2,2-1);//通道2配置信息未定义?????????
                    break;
                default:
                    break;
            }

        }
        ChiFang2 = ChiFang2*2;
    }
}


#endif /* __BSPSENSORDEAL_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
