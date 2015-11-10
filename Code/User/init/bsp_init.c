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

static void RCC_Config(void);
static void General_GPIO_Config(void);
static void FirstScanSysData(void);

/*******************************************************************************
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  ******************************************************************************/
static void RCC_Config(void)
{  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Allow access to the RTC */
	PWR_RTCAccessCmd(ENABLE);

	/* Reset Backup Domain */
//	RCC_RTCResetCmd(ENABLE);
//	RCC_RTCResetCmd(DISABLE);
	
//  /*!< Wait till LSE is ready */
//  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
//  {}
//  /*!< LCD Clock Source Selection */
//  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
//	PWR_BackupAccessCmd(ENABLE);//�����޸�RTC�ͺ󱸼Ĵ���
//	BKP_TamperPinCmd(DISABLE);//�ر����⹦�ܣ�Ҳ����PC13��Ҳ���Ե���ͨIOʹ��
//	PWR_BackupAccessCmd(DISABLE);//��ֹ�޸ĺ󱸼Ĵ���

//BKP_ITConfig(DISABLE);  
	
	/*�رյ����ⲿʱ���źŹ��ܺ�,PC13 PC14 PC15�ſ��Ե���ͨIO��*/
    RCC_LSEConfig(RCC_LSE_OFF); 
	///*!< Wait till LSE is ready */
    //while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
		
	/*���������ڲ�ʱ�ӹ�LCD ʹ��*/
	RCC_LSICmd(ENABLE);
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	RCC_RTCCLKCmd(ENABLE); 

	/* Wait for RTC APB registers synchronisation */
	RTC_WaitForSynchro();
}

/******************************************************************************
  * @brief  Description ����I2C���ߵ�GPIO������ģ��IO�ķ�ʽʵ��
						fram��rx8025�õ�I2C
  * @param  ��			
  * @retval ��		
  *****************************************************************************/
static void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_I2C_PORT, ENABLE);	/* ��GPIOʱ�� */

	GPIO_InitStructure.GPIO_Pin = BSP_I2C_SCL | BSP_I2C_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;/* ��©��� */
    
	GPIO_Init(BSP_I2C_PORT, &GPIO_InitStructure);

	/* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
//	i2c_Stop();
}
/*****************************************************************************
 * ��������General_GPIO_Config
 * ����  �������õ���I/O��
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 *****************************************************************************/
static void General_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/************************GPIOF***************************/
	/*��ӵ���룬���ָʾ���*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF,ENABLE);
	GPIO_InitStructure.GPIO_Pin = Power_ACtest_PIN | Power_CHGtest_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(Power_Deal_PORT,&GPIO_InitStructure);
	/*Һ������ PF2*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF,ENABLE);
	GPIO_InitStructure.GPIO_Pin = LcdVccCtrl_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LcdVccCtrl_PORT,&GPIO_InitStructure);
	
	GPIO_ResetBits(LcdVccCtrl_PORT,LcdVccCtrl_PIN); 
	/*�����������Ƶ�Դ PF3*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF,ENABLE);
	GPIO_InitStructure.GPIO_Pin = TouchVccCtrl_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(TouchVccCtrl_PORT,&GPIO_InitStructure);
	
	GPIO_SetBits(TouchVccCtrl_PORT,TouchVccCtrl_PIN);//�رմ���������Դ 
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
	
	/************************GPIOE***************************/
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
	
	/************************GPIOD***************************/
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

/******************************************************************************
  * @brief  Description �����ڴ��м�¼�ǲ���
  * @param  None
  * @retval None
  *****************************************************************************/
static void SetJlyParamData(void)
{
	uint8_t i;
	//��Ҫ����
	Queue.FlashRecOverFlow = Conf.Basic.RecordFlashOverFlow ; //����flash�����־
	Queue.FlashSectorPointer = Conf.Basic.FlashSectorPointer;
	Queue.WriteFlashDataPointer = Conf.Basic.WriteFlashDataPointer;
	Queue.FlashReadDataBeginPointer = Conf.Basic.FlashReadDataBeginPointer;
	Queue.ReadFlashDataPointer = Conf.Basic.ReadFlashDataPointer;
	Queue.FlashNoReadingDataNum = Conf.Basic.FlashNoReadingDataNum;
	/*------------------------------------------------------*/
	Queue.HIS_ONE_BYTES = (uint16_t)(Conf.Jly.ChannelNum*2+8*Gps_choose+5+Clock_choose); //һ֡���ݴ�С
	//Queue.FRAM_MAX_NUM = FRAM_RecMaxSize/Queue.HIS_ONE_BYTES;	//fram�д洢���ݵ������� 4096/
	Queue.FLASH_SECTOR_PER_NUM = FLASH_SectorPerSize/Queue.HIS_ONE_BYTES; //flash��һ�������洢���ݵ�������
	Queue.FLASH_MAX_NUM = FLASH_RecMaxSize/Queue.HIS_ONE_BYTES; //flash�д洢���ݵ������� 8388608/
	/*------------------------------------------------------*/
	JlyParam.delay_start_time = ReadDelayStartTime;			//��ȡ��ʱ����ʱ��
	JlyParam.NormalRecInterval = ReadNormalRecIntervalTime;	//��ȡ������¼��� ��λ��s
	JlyParam.NormalRecIntervalMin = JlyParam.NormalRecInterval/60;//������¼��� ��λ��min
	
	//�ɼ�ʱ���� ��λ:msתs,��Э������Ƶ� uint16_t ���65535ms 65s,
	JlyParam.SampleInterval = Conf.Jly.SampleInterval/1000 ;
	JlyParam.SampleTime = JlyParam.SampleInterval;  		//�ɼ�ʱ�� ��λ:s
	/*------------------------------------------------------*/
	JlyParam.ChannelNumOld = Conf.Jly.ChannelNum;	//����ͨ������
	for(i=0; i<Conf.Jly.ChannelNum; i++)
	{
		JlyParam.SensorTypeOld[i] = Conf.Sensor[i].SensorType; //���ݸ���ͨ������
	}
}
/******************************************************************************
  * @brief  Description ��ȡ��¼�ǲ���
  * @param  None
  * @retval None
  *****************************************************************************/
static void FirstScanSysData(void)
{
	if(JlyParam.LastErrorCode != 1)//Fram is ok
	{
		if(ReadSetFramFlag() == 0x5050)
		{
			Fram_Read(Conf.Buf,FRAM_BasicConfAddr,FRAM_ConfSize);	//ϵͳ�ϵ��ȡ������Ϣ��
		}else{
			
			Conf.Jly.ChannelNum = 0;//Ĭ������0��ͨ��
		}
		SetJlyParamData();
	}else{//fram����
		Conf.Jly.WorkStatueIsStop = 0;//ֹͣ����
		Conf.Jly.ChannelNum = 0;
	}
}

/******************************************************************************
  * @brief  Description ���� ����Fram��־λ 0x5050
  * @param  None
  * @retval None
  *****************************************************************************/
void WriteSetFramFlag(void)
{
	uint8_t TempBuf[2];
	
	TempBuf[0] = 0x50;
	TempBuf[1] = 0x50;
	
	Fram_Write(TempBuf,FRAM_AlreadySetFlagAddr,2);//2 Byte
}
/******************************************************************************
  * @brief  Description �� ����Fram��־λ
  * @param  None
  * @retval None
  *****************************************************************************/
int16_t ReadSetFramFlag(void)
{
	uint8_t  TempBuf[2];
	uint16_t Flag;
	
	Fram_Read(TempBuf,FRAM_AlreadySetFlagAddr,2);//2 Byte
	Flag = (TempBuf[0]<<8) + TempBuf[1];
	return Flag;
}

/******************************************************************************
  * @brief  Description �����ڴ��м�¼�ǲ���,�ж�ͨ��������ͨ�������Ƿ��޸�
						����޸��˾������ʷ����
  * @param  None
  * @retval None
  *****************************************************************************/
void SetJlyParamJudgeChannelNumSensorType(void)
{
	uint8_t i;
	//��Ҫ����
	Queue.FlashRecOverFlow = Conf.Basic.RecordFlashOverFlow; //����flash�����־
	Queue.FlashSectorPointer = Conf.Basic.FlashSectorPointer;
	Queue.ReadFlashDataPointer = Conf.Basic.ReadFlashDataPointer;
	/*------------------------------------------------------*/
	Queue.HIS_ONE_BYTES = (uint16_t)(Conf.Jly.ChannelNum*2+8*Gps_choose+5+Clock_choose); //һ֡���ݴ�С
	//Queue.FRAM_MAX_NUM = FRAM_RecMaxSize/Queue.HIS_ONE_BYTES;	//fram�д洢���ݵ������� 4096/
	Queue.FLASH_SECTOR_PER_NUM = FLASH_SectorPerSize/Queue.HIS_ONE_BYTES; //flash��һ�������洢���ݵ�������
	Queue.FLASH_MAX_NUM = FLASH_RecMaxSize/Queue.HIS_ONE_BYTES; //flash�д洢���ݵ������� 8388608/
	/*------------------------------------------------------*/
	JlyParam.delay_start_time = ReadDelayStartTime;			//��ȡ��ʱ����ʱ��
	JlyParam.NormalRecInterval = ReadNormalRecIntervalTime;	//��ȡ������¼��� ��λ��s
	JlyParam.NormalRecIntervalMin = JlyParam.NormalRecInterval/60;//������¼��� ��λ��min
	
	//�ɼ�ʱ���� ��λ:msתs,��Э������Ƶ� uint16_t ���65535ms 65s,
	JlyParam.SampleInterval = Conf.Jly.SampleInterval/1000 ;
	JlyParam.SampleTime = JlyParam.SampleInterval;  		//�ɼ�ʱ�� ��λ:s
	/*------------------------------------------------------*/
	if(JlyParam.ChannelNumOld != Conf.Jly.ChannelNum)
	{
		JlyParam.ChannelNumOld = Conf.Jly.ChannelNum;	//����ͨ������
		
		Queue.FlashSectorPointer = 0;
		Queue.FlashNoReadingDataNum = 0;
		Queue.FlashReadDataBeginPointer =0;
		Queue.WriteFlashDataPointer =0;
		Queue.ReadFlashDataPointer = 0;

		WriteU16Pointer(FLASH_SectorWriteAddr_Lchar,0);
		WriteU32Pointer(FLASH_NoReadingDataNumAddr_Lchar,0);
		WriteU32Pointer(FLASH_ReadDataBeginAddr_Lchar,0);
		WriteU32Pointer(FLASH_WriteDataAddr_Lchar,0);
		WriteU32Pointer(FLASH_ReadDataAddr_Lchar,0);

		SetFlashOverFlow(0);//���flash�����־
	}else{
		for(i=0; i<Conf.Jly.ChannelNum; i++)
		{
			if(JlyParam.SensorTypeOld[i] != Conf.Sensor[i].SensorType)
			{
				//ͨ����ѭ���꣬���ұ����µ�ͨ�����ͣ�����иı���������ʷ����
				JlyParam.SensorTypeOld[i] = Conf.Sensor[i].SensorType;
				Flag.SensorTypeIsChange = 1;
			}
		}
		//ѭ��������ͨ����ִ��
		if(Flag.SensorTypeIsChange == 1)
		{
			Flag.SensorTypeIsChange = 0; //��־���
			
			Queue.FlashSectorPointer = 0;
			Queue.FlashNoReadingDataNum = 0;
			Queue.FlashReadDataBeginPointer =0;
			Queue.WriteFlashDataPointer =0;
			Queue.ReadFlashDataPointer = 0;

			WriteU16Pointer(FLASH_SectorWriteAddr_Lchar,0);
			WriteU32Pointer(FLASH_NoReadingDataNumAddr_Lchar,0);
			WriteU32Pointer(FLASH_ReadDataBeginAddr_Lchar,0);
			WriteU32Pointer(FLASH_WriteDataAddr_Lchar,0);
			WriteU32Pointer(FLASH_ReadDataAddr_Lchar,0);

			SetFlashOverFlow(0);//���flash�����־
		}
	}
}

/******************************************************************************
  * @brief  Description ���Fram�Ƿ�������,�������¼��ֹͣ������ʾ1Er
  * @param  None
  * @retval None
  *****************************************************************************/
static void TestFramIsOrNotOk(void)
{
	uint8_t Fram_Buf_Write[256];
	uint8_t Fram_Buf_Read[256];
	uint16_t i;
 
	for ( i=0; i<=255; i++ ) //��仺��
	{   
		Fram_Buf_Write[i] = i;
		//printf("0x%02X ", Fram_Buf_Write[i]);
		//if(i%16 == 15)    
		//printf("\r\n");    
	}

	//��Fram_Buf_Write��˳�����������д��FRAM�� 
	Fram_Write( Fram_Buf_Write, FRAM_TestIsOkAddr, 256);
	
	//��FRAM��������˳��洢��Fram_Buf_Read
	Fram_Read(Fram_Buf_Read, FRAM_TestIsOkAddr, 256); 

	//��I2c_Buf_Read�е�����ͨ�����ڴ�ӡ
	for (i=0; i<256; i++)
	{	
		if(Fram_Buf_Read[i] != Fram_Buf_Write[i])
		{
			//printf("0x%02X ", Fram_Buf_Read[i]);
			//printf("����:I2C EEPROMд������������ݲ�һ��\r\n");
			JlyParam.LastErrorCode = 1;
			return;
		}
		//printf("0x%02X ", Fram_Buf_Read[i]);
		//if(i%16 == 15)    
		//	printf("\r\n");
	}
}

/******************************************************************************
  * @brief  Description �ص������Դ
						1.�ش�������Դ
						2.�ص�ص�ѹ����Դ
						3.�ط�����
						4.��LED��
						5.LCD�����Դ
  * @param  None
  * @retval None
  *****************************************************************************/
void OffPowerSupply(void)
{
	AVCC1_POWER(OFF);     //�ش�������Դ
    BATTEST_POWER(OFF);   //�ص�ص�ѹ����Դ
	BEEP(OFF);
	LED1(OFF);LED2(OFF);
	LcdBackLight(OFF);
}
/******************************************************************************
  * @brief  Description ϵͳ��ʼ��
  * @param  None
  * @retval None
  *****************************************************************************/
void SysInit(void)
{
    //�������Դ
	OffPowerSupply();
    //ϵͳ�ϵ�����ӵ�
	FirstCheckExternPower();
	//���Fram
	TestFramIsOrNotOk();
	
	FirstScanSysData();
	
	//�ϵ���ʾ
    FisrtPowerOnDisplay();
	
	//�ϵ���ж�ͨ������,����Ϊ0��ʾNUL
	JudgingChannelNumberDisplay(Conf.Jly.ChannelNum);
    
	
    Flag.MucReset = 1;
	
	
	MODEL_PWRCTRL(ON);	  //������ӿڵ�Դ
	TOUCHKEY_POWER(ON);	  //������������Դ
	
	BellNn(1);
	
	/*****************************************/
	//����
	
	/*****************************************/
}
/******************************************************************************
  * @brief  Description �����ʼ��
  * @param  None
  * @retval None
  *****************************************************************************/
void PeripheralInit(void)
{
	RCC_Config();
	
	SysTick_Init();
	
    TIM2_Configuration();
    
	KEY_GPIO_Config();
	EXTI15_10_Config();
	
	General_GPIO_Config();	
	I2C_GPIO_Config();
	
	LCD_GLASS_Init();
	
	USART1_Config(Usart1_DefaultBaudRate);
    
//	Delay_ms(10);   //�����δ�ʱ
	LCD_GLASS_Clear();
	
	ADC1_Init();
    
    // 8M����flash W25Q64��ʼ�� 
	SPI_FLASH_Init();
	
    //RX8025AC ��ʼ��
	RX8025_RTC_Init();
}
