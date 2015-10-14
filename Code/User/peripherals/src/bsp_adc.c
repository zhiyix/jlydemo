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
static void ADC1_BATTEST_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOF clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
		
	/* Configure PF6 as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);				// 输入时不用设置速率
}

/**
  * @brief  配置外接电池 ADC1检测的工作模式为DMA模式
  * @param  无
  * @retval 无
  */
static void ADC1_BATTEST_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/* Enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 			        //ADC地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&Bat.ADC_BatConvertedValue;	//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	        //外设地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  				    //内存地址固定
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
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 	 				            //关闭扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;			                //关连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	//不使用外部触发转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	                    //采集数据右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;	 							    //要转换的通道数目
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*配置ADC时钟，为PCLK2的8分频，即9MHz*/
//	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 

	ADC_RegularChannelConfig(ADC1, ADC_Channel_27, 1, ADC_SampleTime_4Cycles);
	
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
	DMA_InitStructure.DMA_BufferSize = 2;
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
	ADC_InitStructure.ADC_NbrOfConversion = 2;	 							    //要转换的通道数目
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*配置ADC时钟，为PCLK2的8分频，即9MHz*/
//	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 

	ADC_RegularChannelConfig(ADC1, ADC_Channel_24, 1, ADC_SampleTime_4Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_25, 2, ADC_SampleTime_4Cycles);
	
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
	
    ADC1_BATTEST_GPIO_Config();
    ADC1_BATTEST_Mode_Config();
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
   
}
/**
  * @brief  Description 电池电压检测数据处理
  * @param  无
  * @retval 无
  */
static void Voltage_ADC1config(void)
{
    unsigned char i=0,j=0;
    unsigned int temp=0;
    unsigned int sum=0,adc_temp[vtest_cnt];
    float temp2=0;
    unsigned char        voltage_zhengshu=0;    //电压整数位
    unsigned char        voltage_xiaoshu=0;     //电压小数位
    
    BATTEST_POWER(ON);  //开启电池电压检测电源
    
    for(i=0;i<vtest_cnt;i++)//????
    { 
        /* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
        ADC_SoftwareStartConv(ADC1);
        Delay_ms(5);    //延时5ms
        adc_temp[i] = Bat.ADC_BatConvertedValue;
    }
    
    BATTEST_POWER(OFF);  //关闭电池电压检测电源
    
    for(i=1;i<=vtest_cnt;i++)//
    for(j=0;j<=vtest_cnt-i;j++)
    if(adc_temp[j]>=adc_temp[j+1])
    {
        temp=adc_temp[j];
        adc_temp[j]=adc_temp[j+1];
        adc_temp[j+1]=temp;
    }

    for(i=0;i<(vtest_cnt-2);i++)//舍弃最大、最小值，求累加和
    { 
        sum=sum+adc_temp[1+i];
    }

    temp2=5.0/4095;
    temp2=temp2*sum;
    temp2=temp2/(vtest_cnt-2);
    temp =(uint16_t)(temp2*100);

    voltage_zhengshu=temp/100;
    voltage_xiaoshu=temp-voltage_zhengshu*100;
    voltage_xiaoshu=((voltage_xiaoshu/10)<<4)+(voltage_xiaoshu%10);
    
    if(Flag.ExPwOn==0)  //没外接电，电池电压检测
    {
        
        if(temp>=400)
            {while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){} showBATT;LCD_UpdateDisplayRequest();}
        else if((temp>=380)&&(temp<400))
            {while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){} showBATT2;LCD_UpdateDisplayRequest();}
        else if((temp>=350)&&(temp<380))
            {while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){} showBATT1;LCD_UpdateDisplayRequest();}
        else if(temp<350)
            {while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){} showBATT0;LCD_UpdateDisplayRequest();}
            
        if(temp<=340)  
            Flag.Low_Voltage = 1;
        else 
            Flag.Low_Voltage = 0;
    }
    else
    {
        if(temp>=390)
        {
            //在外接电下,电压>4.1v,认为充满电,这里置充满电标志位!
            Flag.BatteryFull=1;
        }
        else
        {
            Flag.BatteryFull=0;
        }
    }
}
/**
  * @brief  Description 电池电压检测
  * @param  无
  * @retval 无
  */
void  voltage_test(void)
{
    #ifdef VOLTAGE
    Bat.Voltage_TestTime--;
    if(Bat.Voltage_TestTime==0)
    {
        Voltage_ADC1config();
        if(Flag.Low_Voltage==0)//=1 低电压标志
            Bat.Voltage_TestTime=voltagetesttimenum;
        else 
            Bat.Voltage_TestTime=voltagetesttimenum1;
    }
    #endif
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
        GatherMaxCt = 15;
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
