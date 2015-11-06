/**
  ******************************************************************************
  * @file              : bsp_sensordeal.c
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

/******************************************************************************
  * @brief  Description  ���㿪����ͨ��
  * @param  chanel_num   �ܵ�ͨ������		
  * @retval start_chanel ������ͨ��		
  *****************************************************************************/
static uint32_t GetStartChanel(uint8_t chanel_num)
{
    uint8_t  i;
    uint32_t start_chanel;
   
    for(i=0;i<chanel_num;i++)
    {
        start_chanel |= (0x01<<i);
    }
    return start_chanel;
}

/******************************************************************************
  * @brief  Description ����ͨ��ԭʼ��ʪ������
  * @param  ChannelItem ������ͨ�� 		
  * @retval 		
  *****************************************************************************/
/*
static float Chang_to_Shishu(uint8_t ChannelItem)
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
    //WDT_START;
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
          //WDT_START;
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
*/
/******************************************************************************
  * @brief  Description ����ͨ��ԭʼ�¶�����(��ADֵת��Ϊʵ���¶�����)
  * @param  adcvalue    ��Ӧͨ��ADֵ		
  * @retval temp        ��Ӧʵ���¶�ֵ		
  *****************************************************************************/
static float TempChang_to_Shishu(uint16_t adcvalue)
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
    x = RESET_CHANNEL_SETUP_TABLE[0]; 
    y = RESET_CHANNEL_SETUP_TABLE[1];
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
    x = RESET_CHANNEL_SETUP_TABLE[0x08+2]; 
    y = RESET_CHANNEL_SETUP_TABLE[0x08+2+1];
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
    
    adjust_fa = 0;
    div_member = 0x0F;

    x=0;
    //WDT_START;
    while( adcvalue > Char_to_Int(AdjustCurveFirAddress[adjust_fa][2*x],AdjustCurveFirAddress[adjust_fa][2*x+1]) && x < div_member )
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
          //t = ADJUST_TABLE_HEAD_LENGTH + div_member*2 + x;
          //y = AdjustCurveFirAddress[adjust_fa][t];
          for(j=0;j<x-1;j++)
          {
              SegValue0 = SegValue0 + TempHumi_Offset;
          }
          //WDT_START;
          SegOffset = TempHumi_Offset;
          SegValue0 = low_data + SegValue0;
          
          t = (x-1)*2;//??ad????????
          ft00 = adcvalue - Char_to_Int(AdjustCurveFirAddress[adjust_fa][t],AdjustCurveFirAddress[adjust_fa][t+1]);//????????????
          SegOffset1 =  Char_to_Int(AdjustCurveFirAddress[adjust_fa][t],AdjustCurveFirAddress[adjust_fa][t+1]);
          SegOffset1 =  Char_to_Int(AdjustCurveFirAddress[adjust_fa][t+2],AdjustCurveFirAddress[adjust_fa][t+3])-SegOffset1;
          temp = SegValue0 +  (ft00/SegOffset1)*SegOffset ;//??????
     }
     if(temp>top_data) temp = top_data;
     return temp;
}
/******************************************************************************
  * @brief  Description ����ͨ��ԭʼʪ������(��ADֵת��Ϊʵ���¶�����)
  * @param  adcvalue    ��Ӧͨ��ADֵ		
  * @retval temp        ��Ӧʵ���¶�ֵ		
  *****************************************************************************/
static float HumiChang_to_Shishu(uint16_t adcvalue)
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
    x = RESET_CHANNEL_SETUP_TABLE[2]; 
    y = RESET_CHANNEL_SETUP_TABLE[2+1];
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
    x = RESET_CHANNEL_SETUP_TABLE[0x08+2]; 
    y = RESET_CHANNEL_SETUP_TABLE[0x08+2+1];
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
    
    adjust_fa = 1;
    div_member = 0x0F;

    x=0;
    //WDT_START;
    while( adcvalue > Char_to_Int(AdjustCurveFirAddress[adjust_fa][2*x],AdjustCurveFirAddress[adjust_fa][2*x+1]) && x < div_member )
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
          //t = ADJUST_TABLE_HEAD_LENGTH + div_member*2 + x;
          //y = AdjustCurveFirAddress[adjust_fa][t];
          for(j=0;j<x-1;j++)
          {
              SegValue0 = SegValue0 + TempHumi_Offset;
          }
          //WDT_START;
          SegOffset = TempHumi_Offset;
          SegValue0 = low_data + SegValue0;
          
          t = (x-1)*2;//??ad????????
          ft00 = adcvalue - Char_to_Int(AdjustCurveFirAddress[adjust_fa][t],AdjustCurveFirAddress[adjust_fa][t+1]);//????????????
          SegOffset1 =  Char_to_Int(AdjustCurveFirAddress[adjust_fa][t],AdjustCurveFirAddress[adjust_fa][t+1]);
          SegOffset1 =  Char_to_Int(AdjustCurveFirAddress[adjust_fa][t+2],AdjustCurveFirAddress[adjust_fa][t+3])-SegOffset1;
          temp = SegValue0 +  (ft00/SegOffset1)*SegOffset ;//??????
     }
     if(temp>top_data) temp = top_data;
     return temp;
}
/******************************************************************************
  * @brief  Description �¶�У׼
  * @param  pchAry 	    У׼�� 
  * @param  uCount      У׼������ 
  * @param  dat_x       �����¶���ֵ�����У׼֮�������Ҳ�Ǹ�ֵ 
  * @retval bool		
  *****************************************************************************/
bool Temp_Adjust(int16_t *pchAry, uint8_t uCount, float *dat_x)
{
    int idx = 0;
	// У׼Դ��
    const int16_t __KEY_PTR[15] =
    {
        -400,-300,-200, -100, 0, 100, 200, 300, 400, 500, 600,700,800,900,1000,
    };
    float k, x0, y0, x1, y1;
/*
    //! ����������
    if ((pchAry == NULL) || (uCount != 10))
    {
        return false;
    }
    if ((pchAry[0] >> 8) != 0x5A)
    {
        return false;
    }
    //! ������Ч�Լ��
    for (idx = 0; idx < countof(__KEY_PTR); idx++)
    {
        if (abs((float)(pchAry[idx + 1] - __KEY_PTR[idx])) > 99.0)
            pchAry[idx + 1] = __KEY_PTR[idx];
    }
	*/
    //! ��������
	//! С����Сֵ
    if ((*dat_x - (-80.0)) <= 0)
    {
        *dat_x = -80.0;
        goto APP_TEMP_CALIB_END;
    }
	//! �������ֵ
    if ((*dat_x - (150.0)) >= 0)
    {
        *dat_x = 150.0;
        goto APP_TEMP_CALIB_END;
    }
    //!
    *dat_x = *dat_x * 10;
	//! ��Сֵ����
    if ((*dat_x - (pchAry[0 + 1])) <= 0)
    {
        y0 = x0 = -800;
        x1 = pchAry[0 + 1];
        y1 = __KEY_PTR[0];
        k = (y1 - y0) / (x1 - x0);
        *dat_x = (y0 + k * (*dat_x - x0)) / 10;
        goto APP_TEMP_CALIB_END;
    }
	//! ���ֵ����
    if ((*dat_x - (pchAry[13 + 1])) >= 0)
    {
        y1 = x1 = 1500;
        x0 = pchAry[13 + 1];
        y0 = __KEY_PTR[13];
        k = (y1 - y0) / (x1 - x0);
        *dat_x = (y0 + k * (*dat_x - x0)) / 10;
        goto APP_TEMP_CALIB_END;
    }
	//! У׼����
    for (idx = 0; idx < countof(__KEY_PTR) - 1; idx++)
    {
        if (((*dat_x - (pchAry[idx + 1])) >= 0) &&
                ((*dat_x - (pchAry[idx + 2])) < 0))
        {
            x0 = pchAry[idx + 1];
            x1 = pchAry[idx + 2];
            y0 = __KEY_PTR[idx+1];
            y1 = __KEY_PTR[idx + 2];
            k = (y1 - y0) / (x1 - x0);
            *dat_x = (y0 + k * (*dat_x - x0)) / 10;
            break;
        }
    }
APP_TEMP_CALIB_END:
    return true;
}
/******************************************************************************
  * @brief  Description ʪ��У׼
  * @param  pchAry 	    У׼�� 
  * @param  uCount      У׼������ 
  * @param  dat_x       �����¶���ֵ�����У׼֮�������Ҳ�Ǹ�ֵ 
  * @retval bool		
  *****************************************************************************/
bool Humi_Adjust(int16_t *pchAry, uint8_t uCount, float *dat_x)
{
    int idx = 0;
	// У׼Դ��
    const int16_t __KEY_PTR[] =
    {
        0, 100, 200, 300, 400, 500, 600,700,800,900,1000,
    };
    float k, x0, y0, x1, y1;
/*
    //! ����������
    if ((pchAry == NULL) || (uCount != 10))
    {
        return false;
    }
    if ((pchAry[0] >> 8) != 0x5A)
    {
        return false;
    }
    //! ������Ч�Լ��
    for (idx = 0; idx < countof(__KEY_PTR); idx++)
    {
        if (abs((float)(pchAry[idx + 1] - __KEY_PTR[idx])) > 99.0)
            pchAry[idx + 1] = __KEY_PTR[idx];
    }
	*/
    //! ��������
	//! С����Сֵ
    if ((*dat_x - (0.0)) <= 0)
    {
        *dat_x = 0.0;
        goto APP_TEMP_CALIB_END;
    }
	//! �������ֵ
    if ((*dat_x - (100.0)) >= 0)
    {
        *dat_x = 100.0;
        goto APP_TEMP_CALIB_END;
    }
    //!
    *dat_x = *dat_x * 10;
	//! У׼����
    for (idx = 0; idx < countof(__KEY_PTR) - 1; idx++)
    {
        if (((*dat_x - (pchAry[idx + 1])) >= 0) &&
                ((*dat_x - (pchAry[idx + 2])) < 0))
        {
            x0 = pchAry[idx + 1];
            x1 = pchAry[idx + 2];
            y0 = __KEY_PTR[idx+1];
            y1 = __KEY_PTR[idx + 2];
            k = (y1 - y0) / (x1 - x0);
            *dat_x = (y0 + k * (*dat_x - x0)) / 10;
            break;
        }
    }
APP_TEMP_CALIB_END:
    return true;
}
/******************************************************************************
  * @brief  Description �����������жϣ��������ݴ���
  * @param  sensortype  ����������
  * @param  i			������ͨ��
  * @retval bool		
  *****************************************************************************/
static void Sensor_Deal(uint8_t sensortype,uint8_t i)
{
    float temp;
    
    switch(sensortype)
    {
        case NULL:
				
            break;
        //�¶ȴ���
        case SENSOR_TEMP:
				/*����ԭʼ�¶�����*/
                ChannelDataFloat[i] = TempChang_to_Shishu(adc[i]);//
				/*У׼������У׼����*/
				if(Conf.Sensor[i].AdjustSwitch == 0x01)
				{
					Temp_Adjust(Conf.Adjust[i].adbuf,1,&ChannelDataFloat[i]);
				}
                FlagSeniorErr[i]=0;//v2.0
                if((adc[i]<ADC_ERR_L)|(adc[i]>ADC_ERR_H))
                {
                    FlagSeniorErr[i]=1;
                }
            break;
        //ʪ�ȴ���
        case SENSOR_HUMI:
                if(adc[i]<ADC_ERR_L)
                {
                    FlagSeniorErr[i]=1;
                }
                else
                {   //HIH5030
                    //0.00636y+0.1515=V���/V3.3=adc/4096  (ADC��׼�����ⲿ3.3v)//HIH5030ʪ��У׼
                    //y=(adc/4096-0.1515)/0.00636
                    
                    FlagSeniorErr[i]=0;
                    temp=adc[i];
                    temp=((temp/4096)-0.1515)/0.00636;
                    
                    if(FlagSeniorErr[0]!=1)
						temp=temp/(1.0546-0.00216*ChannelDataFloat[0]);//������ô���㣬ʪ�Ⱥ��¶���һ�������
                    
                    temp=temp*100.0;
                    adc[i]=(unsigned int)temp;//������ʵ��ֵ
                    if(adc[i]%10>4)//��������
						adc[i]=adc[i]+10;
					
                    adc[i]=adc[i]/10;  //Ϊ����ֵ��10��
					/*����ԭʼʪ������*/
                    ChannelDataFloat[i] = HumiChang_to_Shishu(adc[i]);
					/*У׼������У׼����*/
					if(Conf.Sensor[i].AdjustSwitch == 0x01)
					{
						Humi_Adjust(Conf.Adjust[i].adbuf,1,&ChannelDataFloat[i]);
					}
                }
            break;
        
        default:
            break;
    }
}

/******************************************************************************
  * @brief  Description ��ͨ����Ӧ����ת����ʵ�ʶ�Ӧ��������
  * @param  ChannelCode ������ͨ��
  * @retval ��		
  *****************************************************************************/
void DoGatherChannelDataFloat(uint8_t ChannelCode)
{
    uint8_t i;
    uint8_t ChiFang2 = 0x01;
    for(i=0;i<Conf.Jly.ChannelNum;i++)
    {
        if(ChannelCode & ChiFang2)
        {
			Sensor_Deal(Conf.Sensor[i].SensorType ,i);	/*���ݴ��������ͽ��д���*/
        }else{
			ChannelDataFloat[i] = 0.0;/*û�����óɴ�ͨ�� ��������*/
		}
        ChiFang2 = ChiFang2*2;
    }
}

/******************************************************************************
  * @brief  Description  �ж�ͨ����������ʾ
  * @param  chanel_num   
  * @retval start_chanel 
  *****************************************************************************/
void JudgingChannelNumberDisplay(uint8_t ChannelNum)
{
	if(ChannelNum <= 0)
	{
		Display_NUL();//���õ�ͨ����Ϊ0,��ʾNUL
	}else{
		Started_Channel = GetStartChanel(ChannelNum); //ͨ����ת��Ϊ ������ͨ��
		StartedChannelForDisplay = Started_Channel;
	}
}
#endif /* __BSPSENSORDEAL_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
