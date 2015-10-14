/**
  ******************************************************************************
  * @file    bsp_xxx.c
  * @author  
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   adc1 Ӧ��bsp / DMA ģʽ
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:32bits Discovery ������ 
  *
  ******************************************************************************
  */ 
  
#include "main.h"

#define ADC1_DR_Address    ((uint32_t)0x40012400+0x58)
/**
  * @brief  ������ӵ�� ADC1��� GPIO
  * @param  ��
  * @retval ��
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
	GPIO_Init(GPIOF, &GPIO_InitStructure);				// ����ʱ������������
}

/**
  * @brief  ������ӵ�� ADC1���Ĺ���ģʽΪDMAģʽ
  * @param  ��
  * @retval ��
  */
static void ADC1_BATTEST_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/* Enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 			        //ADC��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&Bat.ADC_BatConvertedValue;	//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	        //�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  				    //�ڴ��ַ�̶�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/* note !!!ADC1 configuration */	
	ADC_StructInit(&ADC_InitStructure);
    
	/* Enable ADC1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	
//	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;			                //����ADCģʽ
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 	 				            //�ر�ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;			                //������ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	//��ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	                    //�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion = 1;	 							    //Ҫת����ͨ����Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9MHz*/
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
	
	/* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
//	ADC_SoftwareStartConv(ADC1);
	
}
/**
  * @brief  ʹ��ADC1��DMA1��ʱ�ӣ���ʼ��PC.0
  * @param  ��
  * @retval ��
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
	GPIO_Init(GPIOE, &GPIO_InitStructure);				// ����ʱ������������

}

/**
  * @brief  ����ADC1�Ĺ���ģʽΪMDAģʽ
  * @param  ��
  * @retval ��
  */
static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/* Enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 			    //ADC��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue[0];	//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	        //�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  				    //�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/* note !!!ADC1 configuration */	
	ADC_StructInit(&ADC_InitStructure);
    
	/* Enable ADC1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	
//	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;			                //����ADCģʽ
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 				            //ɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;			                //��������ת��ģʽ������ͣ�ؽ���ADCת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	//��ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	                    //�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion = 2;	 							    //Ҫת����ͨ����Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9MHz*/
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
	
	/* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
//	ADC_SoftwareStartConv(ADC1);
	
}

/**
  * @brief  ADC1��ʼ��
  * @param  ��
  * @retval ��
  */
void ADC1_Init(void)
{
	/* Enable the HSI oscillator */
	RCC_HSICmd(ENABLE);	//ADCʹ��
	/* Check that HSI oscillator is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	
    ADC1_BATTEST_GPIO_Config();
    ADC1_BATTEST_Mode_Config();
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
   
}
/**
  * @brief  Description ��ص�ѹ������ݴ���
  * @param  ��
  * @retval ��
  */
static void Voltage_ADC1config(void)
{
    unsigned char i=0,j=0;
    unsigned int temp=0;
    unsigned int sum=0,adc_temp[vtest_cnt];
    float temp2=0;
    unsigned char        voltage_zhengshu=0;    //��ѹ����λ
    unsigned char        voltage_xiaoshu=0;     //��ѹС��λ
    
    BATTEST_POWER(ON);  //������ص�ѹ����Դ
    
    for(i=0;i<vtest_cnt;i++)//????
    { 
        /* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
        ADC_SoftwareStartConv(ADC1);
        Delay_ms(5);    //��ʱ5ms
        adc_temp[i] = Bat.ADC_BatConvertedValue;
    }
    
    BATTEST_POWER(OFF);  //�رյ�ص�ѹ����Դ
    
    for(i=1;i<=vtest_cnt;i++)//
    for(j=0;j<=vtest_cnt-i;j++)
    if(adc_temp[j]>=adc_temp[j+1])
    {
        temp=adc_temp[j];
        adc_temp[j]=adc_temp[j+1];
        adc_temp[j+1]=temp;
    }

    for(i=0;i<(vtest_cnt-2);i++)//���������Сֵ�����ۼӺ�
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
    
    if(Flag.ExPwOn==0)  //û��ӵ磬��ص�ѹ���
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
            //����ӵ���,��ѹ>4.1v,��Ϊ������,�����ó������־λ!
            Flag.BatteryFull=1;
        }
        else
        {
            Flag.BatteryFull=0;
        }
    }
}
/**
  * @brief  Description ��ص�ѹ���
  * @param  ��
  * @retval ��
  */
void  voltage_test(void)
{
    #ifdef VOLTAGE
    Bat.Voltage_TestTime--;
    if(Bat.Voltage_TestTime==0)
    {
        Voltage_ADC1config();
        if(Flag.Low_Voltage==0)//=1 �͵�ѹ��־
            Bat.Voltage_TestTime=voltagetesttimenum;
        else 
            Bat.Voltage_TestTime=voltagetesttimenum1;
    }
    #endif
}
/**
  * @brief  Description ���ݲ���,������
  * @param  ��
  * @retval ��
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
                    /* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
                    ADC_SoftwareStartConv(ADC1);
                    Delay_ms(5);    //��ʱ5ms
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
