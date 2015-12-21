/**
  ******************************************************************************
  * @file              : bsp_powerdeal.c
  * @author            : 
  * @version           : 
  * @date              : 2015-10-xx
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
#ifndef __BSPPOWER_C
#define __BSPPOWER_C

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

/*******************************************************************************
  * @brief  Description 电池电压检测数据处理
						数据采集时打开AD采集
						电池电压低于2.85V时，低电压
  * @param  无
  * @retval 无
  *****************************************************************************/
static void BatteryVoltage_ADC1config(void)
{
    unsigned char i=0,j=0;
    unsigned int temp=0;
    unsigned int sum=0,adc_temp[vtest_cnt];
    float temp2=0;
    
    BATTEST_POWER(ON);  //开启电池电压检测电源
    Delay_ms(4);    //延时4ms,等待电压稳定,adc采集很准确,电池电压测试很准确

    for(i=0;i<vtest_cnt;i++)
    { 
		DMA_ClearFlag(DMA1_FLAG_TC1);
        /* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
        ADC_SoftwareStartConv(ADC1);
		while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//等待DMA 传输完adc采集的数据
        //while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//End of conversion flag
		//Delay_ms(2);    //延时2ms
		
		adc_temp[i] = ADC_ConvertedValue[2];//电池电压
    }
    
    BATTEST_POWER(OFF);  //关闭电池电压检测电源
    
	
    for(i=1;i<=vtest_cnt;i++)
    for(j=0;j<vtest_cnt-i;j++)
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

    temp2=sum/(vtest_cnt-2);
    temp2=temp2/4095*3.3*2;
    temp =(uint16_t)(temp2*100);
	PManage.BatVoltage = temp; //电池电压最新值
    //voltage_zhengshu=temp/100;
    //voltage_xiaoshu=temp-voltage_zhengshu*100;
    //voltage_xiaoshu=((voltage_xiaoshu/10)<<4)+(voltage_xiaoshu%10);
    
    if(Flag.ExPwOn==0)  //没外接电，电池电压检测
    {
        while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
		
        if(temp>=405)
			{showBATT;}
        else if((temp>=350)&&(temp<405))//380
            {showBATT2;}
        else if((temp>=300)&&(temp<350))//350 380
            {showBATT1;}
        else if(temp<300)//350
            {showBATT0;}
			
        LCD_UpdateDisplayRequest();
			
        if(temp<=285)  /*电压低于2.85V，置低电压标志*/
            Flag.Low_Voltage = 1;
        else 
            Flag.Low_Voltage = 0;
    }
}
/*******************************************************************************
  * @brief  Description 电池电压检测 
						在外接电断掉之后开启电池电压检测
  * @param  无
  * @retval 无
  *****************************************************************************/
void BatteryVoltageDetection(void)
{
    #ifdef VOLTAGE
	if(Flag.ExPwOn == 0)
	{
		PManage.BatVoltage_TestTime--;
		if(PManage.BatVoltage_TestTime==0)
		{
			BatteryVoltage_ADC1config();
			if(Flag.Low_Voltage==0)
				PManage.BatVoltage_TestTime=NormalVoltageTestTimeNum;/*电池未低电压 10分钟检测一次*/
			else 	//=1 低电压标志
				PManage.BatVoltage_TestTime=LowVoltageTestTimeNum;	/*电池低电压 5分钟检测一次*/
		}
		if(Flag.Low_Voltage == 1)//低压标志
		{
			//电池0格闪烁
			while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
			
			if(Flag.BatLowShan == 1)
			{
				Flag.BatLowShan = 0;
				showBATT0;
			}else
			{
				Flag.BatLowShan = 1;
				clearBATT;
			}
			
			LCD_UpdateDisplayRequest();
		}
	}
    #endif
}
/*******************************************************************************
  * @brief  Description 系统上电检测是否接外接电
						接外接电充电中：电池符号一格一格往前进
						接外接电并充满：电池符号满格闪烁
						不接外接电：显示电池符号，不闪烁
  * @param  无
  * @retval 无
  *****************************************************************************/
void FirstCheckExternPower(void)
{
	if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_ACtest_PIN) == 0)
    {
        Flag.ExPwOn = 1;	/*有外接电*/
		PManage.BatVoltage_TestTime = 1; 
    }
    else
    {
        Flag.ExPwOn = 0;
        PManage.BatVoltage_TestTime = 1;         
    }
}
/*******************************************************************************
  * @brief  Description 检测是否接外接电
						接外接电并接电池而电未充满：电池符号一格一格往前跳
						接外接电并接电池而电已充满：电池符号闪烁
						不接外接电接电池：显示电池实际电量，低电量电池符号空格闪烁						
  * @param  无
  * @retval 无
  *****************************************************************************/
void ExternalPowerDetection(void)
{
	/*****************************************************************/
    /*程序运行中 检测外接电接入 */
	if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_ACtest_PIN) == 0)         
    {
		/*
        PManage.HaveExternalPower++;
        if(PManage.HaveExternalPower >= ExternalPowerchecktime)
        {
            PManage.HaveExternalPower=0;
        }
		*/
		
		Flag.ExPwOn=1;
		Flag.ExPwFirstDown = 0;
		
    }else
	{
        Flag.ExPwOn=0;
		if(Flag.ExPwFirstDown == 0)
		{
			PManage.BatVoltage_TestTime=1;/*没有外接电立即开启电池电压检测*/
			Flag.ExPwFirstDown = 1;
		}
		
        //PManage.HaveExternalPower=0;
    }
	/*****************************************************************
	 *有外接电的情况
	 *(1)接锂电池，检测其充满电
	 *(2)未接锂电池
	 *****************************************************************/
	if(Flag.ExPwOn == 1)	
	{
		if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_CHGtest_PIN) == 1)         
		{
			/*
			PManage.BatChargeFullCount++;
			if(PManage.BatChargeFullCount >= ExternalPowerchecktime)//检测 3 s
			{
				PManage.BatChargeFullCount=0;
			}	
			*/
			Flag.BatChargeFull=1;/*接外接电，电池充满标志*/
			Flag.BatCharging = 0;/*接外接电未接电池，电池未充电*/
			
		}else
		{
			Flag.BatChargeFull=0;
			Flag.BatCharging = 1;/*接外接电，电池正在充电中*/
		}
		
	}else
	{
		Flag.BatChargeFull=0;
		Flag.BatCharging = 0;/*外接电未接*/
	}
    
    // ----------------------------------------------------
    if(Flag.ExPwOn==1)
    {
        
		while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
		if(Flag.BatChargeFull == 1)/*电池充满电*/
		{
			if(Flag.ExPwShan==1)
			{
				Flag.ExPwShan=0;
				showBATT;
			}
			else
			{
				Flag.ExPwShan=1;
				clearBATT;
			}
		}
		else
		{
			if(Flag.BatCharging == 1)
			{
				PManage.JinDuCounts++;
				if(PManage.JinDuCounts==1)
				{showBATT0;}
				else if(PManage.JinDuCounts==2)
				{showBATT1;}
				else if(PManage.JinDuCounts==3)
				{showBATT2;}
				else if(PManage.JinDuCounts>=4)
				{
					clearBATT;
					showBATT;
					PManage.JinDuCounts=0;
				}
			}
		}
		LCD_UpdateDisplayRequest(); 
		PManage.BatVoltage = 412;//电池充电的时候电压赋值412,电压为4.12V
        
    }
}
#endif /* __BSPPOWER_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
