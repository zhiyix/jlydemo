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
  * @brief  Description ��ص�ѹ������ݴ���
  * @param  ��
  * @retval ��
  *****************************************************************************/
static void Voltage_ADC1config(void)
{
    unsigned char i=0,j=0;
    unsigned int temp=0;
    unsigned int sum=0,adc_temp[vtest_cnt];
    float temp2=0;
    unsigned char        voltage_zhengshu=0;    //��ѹ����λ
    unsigned char        voltage_xiaoshu=0;     //��ѹС��λ
    
    BATTEST_POWER(ON);  //������ص�ѹ����Դ
    
    for(i=0;i<vtest_cnt;i++)//
    { 
        /* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
        ADC_SoftwareStartConv(ADC1);
        Delay_ms(5);    //��ʱ5ms
		PManage.BatADC_ConvertedValue = ADC_ConvertedValue[2];
        adc_temp[i] = PManage.BatADC_ConvertedValue;
    }
    
    BATTEST_POWER(OFF);  //�رյ�ص�ѹ����Դ
    
	//Bat.BatDataFloat = (float)ADC_ConvertedValue[2]/4095*3.3*2;
	
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

    //temp2=5.0/4095;///4095*3.3*2
    temp2=sum/(vtest_cnt-2);
    temp2=temp2/4095*3.3*2;
    temp =(uint16_t)(temp2*100);

    voltage_zhengshu=temp/100;
    voltage_xiaoshu=temp-voltage_zhengshu*100;
    voltage_xiaoshu=((voltage_xiaoshu/10)<<4)+(voltage_xiaoshu%10);
    
    if(Flag.ExPwOn==0)  //û��ӵ磬��ص�ѹ���
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
			
        if(temp<=340)  
            Flag.Low_Voltage = 1;
        else 
            Flag.Low_Voltage = 0;
    }
    else
    {
        if(temp>=412)
        {
			if(Flag.BatChargeFull == 1)
			{
            //����ӵ���,��ѹ>4.1v,��Ϊ������,�����������־λ!
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
}
/*******************************************************************************
  * @brief  Description ��ص�ѹ���
  * @param  ��
  * @retval ��
  *****************************************************************************/
void VoltageTest(void)
{
    #ifdef VOLTAGE
    PManage.BatVoltage_TestTime--;
    if(PManage.BatVoltage_TestTime==0)
    {
        Voltage_ADC1config();
        if(Flag.Low_Voltage==0)//=1 �͵�ѹ��־
            PManage.BatVoltage_TestTime=voltagetesttimenum;
        else 
            PManage.BatVoltage_TestTime=voltagetesttimenum1;
    }
    #endif
}
/*******************************************************************************
  * @brief  Description ϵͳ�ϵ����Ƿ����ӵ�
						����ӵ����У���ط���һ��һ����ǰ��
						����ӵ粢��������ط���������˸
						������ӵ磺��ʾ��ط��ţ�����˸
  * @param  ��
  * @retval ��
  *****************************************************************************/
void FirstCheckExternPower(void)
{
	if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_Deal_ACtest) == 0)
    {
        Flag.ExPwOn=1;	/*����ӵ�*/
    }
    else
    {
        Flag.ExPwOn=0;
        PManage.BatVoltage_TestTime=1;         
    }
}
/*******************************************************************************
  * @brief  Description ����Ƿ����ӵ�
						����ӵ磺��ط���������˸
						������ӵ磺��ʾ��ط��ţ�����˸
  * @param  ��
  * @retval ��
  *****************************************************************************/
void OutpowerShan(void)
{
    // --------------------------------------------------
    if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_Deal_ACtest) == 0)
    {
        if(Flag.Powerdowncountflag==1)
        {
            if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_Deal_ACtest) == 0)
            {
                //BellNn(1);
                Flag.ExPwOn=1;
                Flag.Powerdowncountflag=0;
                
                PManage.HaveExternalPower=0;
            }
        }
    }
    else
    {
        Flag.ExPwOn=0;
		PManage.BatVoltage_TestTime=1;/*û����ӵ�����������ص�ѹ���*/
        PManage.HaveExternalPower=0;
        Flag.Powerdowncountflag=0;
    }
    // ----------------------------------------------------
    if(Flag.ExPwOn==1)
    {
        if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_Deal_ACtest) == 0)
        {
            Flag.ExPwOn=1;
			while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
            if(Flag.BatteryFull==1)
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
                PManage.JinDuCounts++;
                if(PManage.JinDuCounts==1)
                {clearBATT;showBATT0;}
                else if(PManage.JinDuCounts==2)
                {clearBATT;showBATT1;}
                else if(PManage.JinDuCounts==3)
                {clearBATT;showBATT2;}
                else if(PManage.JinDuCounts>=4)
                {
					clearBATT;
                    showBATT;
                    PManage.JinDuCounts=0;
                }
            }
            LCD_UpdateDisplayRequest(); 
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
