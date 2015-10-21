/**
  ******************************************************************************
  * @file    init.c
  * @author  
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ��ʼ�������ӿ�
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */

#include "main.h"

static void General_GPIO_Config(void);

/*
 * ��������General_GPIO_Config
 * ����  �������õ���I/O��
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void General_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//LED�� ������GPIO��ʼ�� ��������GPIO_Pin_15
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_Init(GPIOC,&GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//	GPIO_Init(GPIOC,&GPIO_InitStructure);

//	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	GPIO_ResetBits(GPIOF,GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15);//�ر�
	//ADC��Դ����
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOE,GPIO_Pin_5);
//	GPIO_ResetBits(GPIOE,GPIO_Pin_5);
    
    //��ӵ�ص�ѹ��⿪��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOE,GPIO_Pin_4);   //�رյ�ص�Դ���
	
	//model ����ӿڵ�Դ
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOD,GPIO_Pin_0);   //�ر�model��Դ
	
}

/**
  * @brief  Description ��ȡ��¼�ǲ���
  * @param  None
  * @retval None
  */
static void FirstScanSysData(void)
{
	uint8_t num=0;
	uint8_t buf[20];
	buf[0]=8;
    JlyParam.SaveDataTimeOnlyRead = 10; //����ʱ��
    JlyParam.Save_Time = JlyParam.SaveDataTimeOnlyRead;
    
	//test
//	Fram_Write(buf,FRAM_WorkStatueIsStopAddr,1);
//	num = Fram_Read(buf,FRAM_WorkStatueIsStopAddr,1);
	
	Fram_Read(Conf.Buf,FRAM_BasicConfAddr,FRAM_ConfSize);	/*!< ϵͳ�ϵ��ȡ������Ϣ��*/
	
	JlyParam.delay_start_time = ReadDelayStartTime ;
	num = Conf.Jly.WorkStatueIsStop;
	JlyParam.SaveHisDataTime = 2;  //�������ݼ��
	
	
}
/**
  * @brief  Description ϵͳ��ʼ��
  * @param  None
  * @retval None
  */
void SysInit(void)
{
    FirstScanSysData();
    
//	set_time();
    read_time();
    
    while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET); 
	displayTIME(Rtc.Hour,Rtc.Minute);
    showTIME; 
	LCD_UpdateDisplayRequest();
    
	Started_Channel = GetStartChanel(Conf.Jly.ChannelNum); //ͨ����ת��Ϊ ������ͨ��
	StartedChannelForDisplay = Started_Channel;
    
    Sensor1.sensor_infor[0] = 4;
    Sensor1.sensor_infor[1] = 2;
    
    Bat.Voltage_TestTime=11;
    Sensor1.sensor1_str.sensor_type = 1;
    
    Flag.MucReset = 1;
    Flag.IsDisplayRightNow = 1;    
    
    JlyParam.WorkStatueIsStop = 1;
	
	Queue.FlashSectorPoint = 0;
	
	BellNn(1);

}
/**
  * @brief  Description �����ʼ��
  * @param  None
  * @retval None
  */
void PeripheralInit(void)
{
	RCC_Config();
	
	SysTick_Init();
	
    TIM2_Configuration();
    
	KEY_GPIO_Config();
	EXTI15_10_Config();
	
	General_GPIO_Config();
	
	LCD_GLASS_Init();
	
	USART1_Config(115200);
    
//	Delay_ms(10);   //�����δ�ʱ
	LCD_GLASS_Clear();
	
	AVCC1_POWER(OFF);    //�ش�������Դ
    BATTEST_POWER(OFF);  //�ص�ص�ѹ����Դ
	MODEL_PWRCTRL(ON);	//������ӿڵ�Դ
	ADC1_Init();
    
    /* 8M����flash W25Q64��ʼ�� */
	SPI_FLASH_Init();
	
	I2C_GPIO_Config();
    
    //rtc  ��ʼ��
    rtc_init();
    
    
}

