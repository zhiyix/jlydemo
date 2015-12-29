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
						���ݲɼ�ʱ��AD�ɼ�
						��ص�ѹ����2.85Vʱ���͵�ѹ
  * @param  ��
  * @retval ��
  *****************************************************************************/
  /*
static void BatteryVoltage_ADC1config(void)
{
    uint8_t i=0,j=0;
    uint16_t temp=0;
    uint16_t sum=0,adc_temp[vtest_cnt];
    float temp2=0;
    
    BATTEST_POWER(ON);  //������ص�ѹ����Դ
    Delay_ms(4);    //��ʱ4ms,�ȴ���ѹ�ȶ�,adc�ɼ���׼ȷ,��ص�ѹ���Ժ�׼ȷ

    for(i=0;i<vtest_cnt;i++)
    { 
        // ����û�в����ⲿ����������ʹ���������ADCת�� 
        ADC_SoftwareStartConv(ADC1);
		while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//�ȴ�DMA ������adc�ɼ�������
        //while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//End of conversion flag
		//Delay_ms(2);    //��ʱ2ms
		
		adc_temp[i] = ADC_ConvertedValue[2];//��ص�ѹ
    }
    
    BATTEST_POWER(OFF);  //�رյ�ص�ѹ����Դ
    
	
    for(i=1;i<=vtest_cnt;i++)
    for(j=0;j<vtest_cnt-i;j++)
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

    temp2=sum/(vtest_cnt-2);
    temp2=temp2/4095*3.3*2;
    temp =(uint16_t)(temp2*100);
	PManage.BatVoltage = temp; //��ص�ѹ����ֵ
    
    
    if(Flag.ExPwOn==0)  //û��ӵ磬��ص�ѹ���
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
			
        if(temp<=285)  //��ѹ����2.85V���õ͵�ѹ��־
            Flag.Low_Voltage = 1;
        else 
            Flag.Low_Voltage = 0;
    }
}
*/
/*******************************************************************************
  * @brief  Description ��ص�ѹ�������ݴ���
						���ݲɼ�ʱ��AD�ɼ�
						��ص�ѹ����2.85Vʱ���͵�ѹ
  * @param  ��
  * @retval ��
  *****************************************************************************/
static void BatteryVoltageSampleDataDeal(uint16_t BatteryVoltageAdcValue)
{
	uint16_t BatteryVoltageU16;
	float BatteryVoltageFt=0;
	
    BatteryVoltageFt = (float)BatteryVoltageAdcValue/4095*3.3*2;
    BatteryVoltageU16 = (uint16_t)(BatteryVoltageFt*100);
	PManage.BatVoltage = BatteryVoltageU16; //��ص�ѹ����ֵ
    
    
    if(Flag.ExPwOn==0)  //û��ӵ磬��ص�ѹ���
    {
        while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
		
        if(BatteryVoltageU16>=405)
			{showBATT;}
        else if((BatteryVoltageU16>=350)&&(BatteryVoltageU16<405))//380
            {showBATT2;}
        else if((BatteryVoltageU16>=300)&&(BatteryVoltageU16<350))//350 380
            {showBATT1;}
        else if(BatteryVoltageU16<300)//350
            {showBATT0;}
			
        LCD_UpdateDisplayRequest();
			
        if(BatteryVoltageU16 <= 285)  //��ѹ����2.85V���õ͵�ѹ��־
            Flag.Low_Voltage = 1;
        else 
            Flag.Low_Voltage = 0;
    }
}
/*******************************************************************************
  * @brief  Description ��ص�ѹ����
						����ӵ�ϵ�֮������ص�ѹ���
  * @param  ��
  * @retval ��
  *****************************************************************************/
void BatteryVoltageDeal(void)
{
    #ifdef VOLTAGE
	if(Flag.ExPwOn == 0)
	{
		//PManage.BatVoltage_TestTime--;
		if(PManage.BatVoltage_TestTime==0)	//��ĵط�������
		{
			BatteryVoltageSampleDataDeal(PManage.BatADCValue);
			/*
			BatteryVoltage_ADC1config();
			if(Flag.Low_Voltage==0)
				PManage.BatVoltage_TestTime=NormalVoltageTestTimeNum;//���δ�͵�ѹ 10���Ӽ��һ��
			else 	//=1 �͵�ѹ��־
				PManage.BatVoltage_TestTime=LowVoltageTestTimeNum;	//��ص͵�ѹ 5���Ӽ��һ��
			*/
		}
		if(Flag.Low_Voltage == 1)//��ѹ��־
		{
			//���0����˸
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
  * @brief  Description ϵͳ�ϵ����Ƿ����ӵ�
						����ӵ����У���ط���һ��һ����ǰ��
						����ӵ粢��������ط���������˸
						������ӵ磺��ʾ��ط��ţ�����˸
  * @param  ��
  * @retval ��
  *****************************************************************************/
void FirstCheckExternPower(void)
{
	if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_ACtest_PIN) == 0)
    {
        Flag.ExPwOn = 1;	//����ӵ�
		PManage.BatVoltage_TestTime = 0; 
    }
    else
    {
        Flag.ExPwOn = 0;
        PManage.BatVoltage_TestTime = 0;         
    }
}
/*******************************************************************************
  * @brief  Description ����Ƿ����ӵ�
						����ӵ粢�ӵ�ض���δ��������ط���һ��һ����ǰ��
						����ӵ粢�ӵ�ض����ѳ�������ط�����˸
						������ӵ�ӵ�أ���ʾ���ʵ�ʵ������͵�����ط��ſո���˸						
  * @param  ��
  * @retval ��
  *****************************************************************************/
void ExternalPowerDetection(void)
{
	/*****************************************************************/
    //���������� �����ӵ���� 
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
			PManage.BatVoltage_TestTime=0;	//û����ӵ�����������ص�ѹ���
			Flag.ExPwFirstDown = 1;
		}
		
        //PManage.HaveExternalPower=0;
    }
	/*****************************************************************
	 *����ӵ�����
	 *(1)��﮵�أ�����������
	 *(2)δ��﮵��
	 *****************************************************************/
	if(Flag.ExPwOn == 1)	
	{
		if(GPIO_ReadInputDataBit(Power_Deal_PORT,Power_CHGtest_PIN) == 1)         
		{
			/*
			PManage.BatChargeFullCount++;
			if(PManage.BatChargeFullCount >= ExternalPowerchecktime)//��� 3 s
			{
				PManage.BatChargeFullCount=0;
			}	
			*/
			Flag.BatChargeFull=1;	//����ӵ磬��س�����־
			Flag.BatCharging = 0;	//����ӵ�δ�ӵ�أ����δ���
			
		}else
		{
			Flag.BatChargeFull=0;
			Flag.BatCharging = 1;	//����ӵ磬������ڳ����
		}
		
	}else
	{
		Flag.BatChargeFull=0;
		Flag.BatCharging = 0;	//��ӵ�δ��
	}
    
    // ----------------------------------------------------
    if(Flag.ExPwOn==1)
    {
        
		while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
		if(Flag.BatChargeFull == 1)	//��س�����
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
		PManage.BatVoltage = 412;//��س���ʱ���ѹ��ֵ412,��ѹΪ4.12V
        
    }
}
#endif /* __BSPPOWER_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
