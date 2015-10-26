/**
  ******************************************************************************
  * @file    bsp_xxx.c
  * @author  
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   adc1 应用bsp / DMA 模式
  ******************************************************************************
  * @attention
  *
  * 实验平台:32bits Discovery 开发板 
  *
  ******************************************************************************
  */ 
  
#include "main.h"

#define ADC1_DR_Address    ((uint32_t)0x40012400+0x58)
/**
  * @brief  配置外接电池 ADC1检测 GPIO
  * @param  无
  * @retval 无
  */

/**
  * @brief  使能ADC1和DMA1的时钟，初始化PC.0
  * @param  无
  * @retval 无
  */
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable GPIOE clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
		
	/* Configure PE.9 10 as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);				// 输入时不用设置速率

	/* Enable GPIOF clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
	
	/*PF6 接 AN0 检测电池电压*/
	/* Configure PF 6|7|8|9|10  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}

/**
  * @brief  配置ADC1的工作模式为MDA模式
  * @param  无
  * @retval 无
  */
static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/* Enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 			    //ADC地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue[0];	//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 6+1;										//6个传感器通道，1电压采集
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	        //外设地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  				    //内存地址自增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/* note !!!ADC1 configuration */	
	ADC_StructInit(&ADC_InitStructure);
    
	/* Enable ADC1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	
//	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;			                //独立ADC模式
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 				            //扫描模式，扫描模式用于多通道采集
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;			                //开启连续转换模式，即不停地进行ADC转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	//不使用外部触发转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	                    //采集数据右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 6+1;	 							//要转换的通道数目
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*配置ADC时钟，为PCLK2的8分频，即9MHz*/
//	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 

	ADC_RegularChannelConfig(ADC1, ADC_Channel_24, 1, ADC_SampleTime_4Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_25, 2, ADC_SampleTime_4Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_27, 3, ADC_SampleTime_4Cycles);	//电池电压采集
	ADC_RegularChannelConfig(ADC1, ADC_Channel_28, 4, ADC_SampleTime_4Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_29, 5, ADC_SampleTime_4Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_30, 6, ADC_SampleTime_4Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_31, 7, ADC_SampleTime_4Cycles);
	/* Enable the request after last transfer for DMA Circular mode */
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/* Wait until the ADC1 is ready */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
	{
	}
	
	/* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
//	ADC_SoftwareStartConv(ADC1);
	
}

/**
  * @brief  ADC1初始化
  * @param  无
  * @retval 无
  */
void ADC1_Init(void)
{
	/* Enable the HSI oscillator */
	RCC_HSICmd(ENABLE);	//ADC使用
	/* Check that HSI oscillator is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
   
}
/**
  * @brief  Description 数据采样,滤数据
  * @param  无
  * @retval 无
  */
void  Dealing_Gather(unsigned char all_channel_code)
{
    unsigned char channel_cp;
    unsigned char m,i,GatherMaxCt;
//    unsigned int  add_adc;
    if(!all_channel_code)
    {
        for(m=0;m<32;m++)adc[m] = 0;
        return;
    }
    else
    {
        GatherMaxCt = 14;
        if(GatherMaxCt==0||GatherMaxCt>15) //
            return;
        else
        for(i=0;i<GatherMaxCt;i++)//GatherMaxCt=15
        {
            channel_cp = all_channel_code;
            for(m = 0;m < 32;m++)//
            {
                if(channel_cp & 0x01)
                {
                    /* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
                    ADC_SoftwareStartConv(ADC1);
                    Delay_ms(5);    //延时5ms
                    adcCopy[m] = adcCopy[m] + ADC_ConvertedValue[m];
                }                		
                else
                {
                    adc[m] = 0; 
                }
                channel_cp>>=1;
            }
        }
        
        for(m = 0;m < 32;m++)//GatherMaxCt
        {
            adc[m] = adcCopy[m]/GatherMaxCt;
            adcCopy[m] = 0;
        }
    }
//    ChannelDataFloat[0] = 23.5;//(((float)ADC_ConvertedValue[1]/4095)-0.1515)/0.00636;
//	ChannelDataFloat[1] = (((float)adc[1]/4095)-0.1515)/0.00636;
}

/*********************************************END OF FILE**********************/
