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
static void Voltage_ADC1config(void)
{
    unsigned char i=0,j=0;
    unsigned int temp=0;
    unsigned int sum=0,adc_temp[vtest_cnt];
    float temp2=0;
    //unsigned char        voltage_zhengshu=0;    //电压整数位
    //unsigned char        voltage_xiaoshu=0;     //电压小数位
    
    BATTEST_POWER(ON);  //开启电池电压检测电源
    
    for(i=0;i<vtest_cnt;i++)//
    { 
        /* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
        ADC_SoftwareStartConv(ADC1);
        Delay_ms(5);    //延时5ms
		PManage.BatADC_ConvertedValue = ADC_ConvertedValue[2];
        adc_temp[i] = PManage.BatADC_ConvertedValue;
    }
    
    BATTEST_POWER(OFF);  //关闭电池电压检测电源
    
	//Bat.BatDataFloat = (float)ADC_ConvertedValue[2]/4095*3.3*2;
	
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

    //temp2=5.0/4095;///4095*3.3*2
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
        else if((temp>=380)&&(temp<405))
            {showBATT2;}
        else if((temp>=350)&&(temp<380))
            {showBATT1;}
        else if(temp<350)
            {showBATT0;}
			
        LCD_UpdateDisplayRequest();
			
        if(temp<=285)  /*电压低于2.85V，置低电压标志*/
            Flag.Low_Voltage = 1;
        else 
            Flag.Low_Voltage = 0;
    }
	/*
    else
    {
        if(temp>=412)
        {
			if(Flag.BatChargeFull == 1)
			{
            //在外接电下,电压>4.1v,认为充满电,这里置满电标志位!
				Flag.BatteryFull=1;
			}
			else
			{
				Flag.BatteryFull=0;
			}
        }
        else
        {
            Flag.BatteryFull=0;
        }
    }
	*/
}
/*******************************************************************************
  * @brief  Description 电池电压检测 
						在外接电断掉之后开启电池电压检测
  * @param  无
  * @retval 无
  *****************************************************************************/
void VoltageTest(void)
{
    #ifdef VOLTAGE
	if(Flag.ExPwOn == 0)
	{
		PManage.BatVoltage_TestTime--;
		if(PManage.BatVoltage_TestTime==0)
		{
			Voltage_ADC1config();
			if(Flag.Low_Voltage==0)
				PManage.BatVoltage_TestTime=voltagetesttimenum1;/*电池未低电压 10分钟检测一次*/
			else 	//=1 低电压标志
				PManage.BatVoltage_TestTime=voltagetesttimenum;	/*电池低电压 5分钟检测一次*/
		}
		if(Flag.Low_Voltage == 1)//低压标志
		{
			//电池0格闪烁
			while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
			
			if(Flag.BatLowShan == 1)
			{
				Flag.BatLowShan = 0;
				showBATT0;
			}else{
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
						不接外接电接电池：显示电池实际电量，电池符号不闪烁						
  * @param  无
  * @retval 无
  *****************************************************************************/
void OutpowerShan(void)
{
    // --------------------------------------------------
    if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_ACtest_PIN) == 0)/*有外接电*/
    {
        if(Flag.Powerdowncountflag==1)
        {
            if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_ACtest_PIN) == 0)
            {
                //BellNn(1);
                Flag.ExPwOn=1;
                Flag.Powerdowncountflag=0;
                
                PManage.HaveExternalPower=0;
				Flag.ExPwFirstDown = 0;
            }
        }
    }
    else
    {
        Flag.ExPwOn=0;
		if(Flag.ExPwFirstDown == 0)
		{
			PManage.BatVoltage_TestTime=1;/*没有外接电立即开启电池电压检测*/
			Flag.ExPwFirstDown = 1;
		}
        PManage.HaveExternalPower=0;
        Flag.Powerdowncountflag=0;
    }
    // ----------------------------------------------------
    if(Flag.ExPwOn==1)
    {
        if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_ACtest_PIN) == 0)/*有外接电*/
        {
            Flag.ExPwOn=1;
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
        else
        {
            Flag.ExPwOn=0;
            PManage.BatVoltage_TestTime=1;
        }
    }
}
#endif /* __BSPPOWER_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
