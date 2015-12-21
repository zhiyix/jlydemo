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
static void FirstScanSysData(void);

/*******************************************************************************
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  ******************************************************************************/
static void SysClock_Config(void)
{  
	RCC_ClocksTypeDef RCC_ClockFreq;
	
	/*STOPģʽ�£�����ʹ��*/
//	DBGMCU_Config(DBGMCU_STOP,ENABLE);
//	DBGMCU_Config(DBGMCU_STOP,DISABLE);
	
	
	//��ȡϵͳʱ�����ͣ�0x00: MSI used as system clock ��0x04: HSI used as system clock ��0x08: HSE used as system clock ��0x0C: PLL used as system clock ��
	SYS_CLK = RCC_GetSYSCLKSource();
	//��ȡϵͳ��ʱ��Ƶ��ֵ
	RCC_GetClocksFreq(&RCC_ClockFreq);	

	/* Reset Backup Domain */
//	RCC_RTCResetCmd(ENABLE);
//	RCC_RTCResetCmd(DISABLE);	
//	PWR_BackupAccessCmd(ENABLE);//�����޸�RTC�ͺ󱸼Ĵ���
//	BKP_TamperPinCmd(DISABLE);//�ر����⹦�ܣ�Ҳ����PC13��Ҳ���Ե���ͨIOʹ��
//	PWR_BackupAccessCmd(DISABLE);//��ֹ�޸ĺ󱸼Ĵ���
//BKP_ITConfig(DISABLE);  
	
	
	/*�رյ����ⲿʱ���źŹ��ܺ�,PC13 PC14 PC15�ſ��Ե���ͨIO��*/
    RCC_LSEConfig(RCC_LSE_OFF); 		
	
	/* Allow access to the RTC */
	PWR_RTCAccessCmd(ENABLE);
	/*���������ڲ�ʱ�ӹ�LCD ʹ��*/
	RCC_LSICmd(ENABLE);
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	RCC_RTCCLKCmd(ENABLE); 
	/* Wait for RTC APB registers synchronisation */
	RTC_WaitForSynchro();
	
}

/*******************************************************************************
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  ******************************************************************************/
void SysClock_ReConfig(void)
{  
	RCC_ClocksTypeDef RCC_ClockFreq;
	ErrorStatus HSEStartUpStatus;
	//ʹ�� HSE
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus == SUCCESS)
	{
		//ʹ�� PLL
		RCC_PLLCmd(ENABLE);
		//�ȴ� PLL׼������
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}
		//ѡ�� PLL��Ϊϵͳʱ��Դ
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		//�ȴ� PLL��ѡ��Ϊϵͳʱ��Դ
		while(RCC_GetSYSCLKSource() != 0x0C){}
	}
	
	/* ʹ���ڲ� HSI ע��ʱ��*/
	RCC_HSICmd(ENABLE);	//ADCʹ��
	/* Check that HSI oscillator is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	
	/*STOPģʽ�£�����ʹ��*/
	DBGMCU_Config(DBGMCU_STOP,ENABLE);
//	DBGMCU_Config(DBGMCU_STOP,DISABLE);
	
	//��ȡϵͳʱ�����ͣ�0x00: MSI used as system clock ��0x04: HSI used as system clock ��0x08: HSE used as system clock ��0x0C: PLL used as system clock ��
	SYS_CLK = RCC_GetSYSCLKSource();
	//��ȡϵͳ��ʱ��Ƶ��ֵ
	RCC_GetClocksFreq(&RCC_ClockFreq);

	/* Reset Backup Domain */
//	RCC_RTCResetCmd(ENABLE);
//	RCC_RTCResetCmd(DISABLE);
//	PWR_BackupAccessCmd(ENABLE);//�����޸�RTC�ͺ󱸼Ĵ���
//	BKP_TamperPinCmd(DISABLE);//�ر����⹦�ܣ�Ҳ����PC13��Ҳ���Ե���ͨIOʹ��
//	PWR_BackupAccessCmd(DISABLE);//��ֹ�޸ĺ󱸼Ĵ���
//BKP_ITConfig(DISABLE);  
	
	
	/*�رյ����ⲿʱ���źŹ��ܺ�,PC13 PC14 PC15�ſ��Ե���ͨIO��*/
    RCC_LSEConfig(RCC_LSE_OFF); 
	
	/* Allow access to the RTC */
	PWR_RTCAccessCmd(ENABLE);
	/*���������ڲ�ʱ�ӹ�LCD ʹ��*/
	RCC_LSICmd(ENABLE);
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	RCC_RTCCLKCmd(ENABLE); 
	/* Wait for RTC APB registers synchronisation */
	//RTC_WaitForSynchro();
	
	//���¿�ADC
	ADC_Cmd(ADC1, ENABLE);
}

/*****************************************************************************
 * ��������GPIO_ReConfig
 * ����  �����е�GPIO���³�ʼ��Ϊģ�����룬���͹���
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 *****************************************************************************/
static void GPIO_ReConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOH, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOG, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;	//ģ������
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;//Very Low Speed
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_Init(GPIOH, &GPIO_InitStructure);
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, DISABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, DISABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, DISABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, DISABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, DISABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOH, DISABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, DISABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOG, DISABLE);
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
 * ��������WakeUp_GPIO_Config
 * ����  ������PA0 ��������,�ⲿ�ж�
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 *****************************************************************************/
static void WakeUp_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;	
	
	/*���� PA0*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
  
	// Enable SYSCFG clock 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    /**************************************************************/
	// Connect EXTI0 Line to PA0 pin 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	// Configure EXTI0 line 
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;

	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&EXTI_InitStructure);
	/**************************************************************/
	
	// Enable and set EXTI0_IRQn Interrupt to the lowest priority
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0E;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0E;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
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
	
	GPIO_SetBits(LcdVccCtrl_PORT,LcdVccCtrl_PIN); 
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
	
	//HAC��Դ
	RCC_AHBPeriphClockCmd(HacVccCtrl_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Pin = HacVccCtrl_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(HacVccCtrl_PORT,&GPIO_InitStructure);
	
	GPIO_SetBits(HacVccCtrl_PORT,HacVccCtrl_PIN);//�رմ���������Դ 
}

/******************************************************************************
  * @brief  Description �����ڴ��м�¼�ǲ���
  * @param  None
  * @retval None
  *****************************************************************************/
static void SetJlyParamData(void)
{
	uint8_t i;
	//���� ��ʶ�룬��һ����Ϊ���ò�һ��
	Queue.IDCode = 0xFF;
	//����ʹ�� ��Ҫ����
	Flag.KeyEnableOrDisable = Conf.Jly.KeyEnableOrDisable;
	//���ݴ洢���ζ��� ��Ҫ����
	Queue.FlashRecOverFlow = Conf.Basic.RecordFlashOverFlow ; //����flash�����־
	Queue.FlashSectorPointer = Conf.Basic.FlashSectorPointer;
	Queue.WriteFlashDataPointer = Conf.Basic.WriteFlashDataPointer;
	Queue.FlashReadDataBeginPointer = Conf.Basic.FlashReadDataBeginPointer;
	Queue.ReadFlashDataPointer = Conf.Basic.ReadFlashDataPointer;
	Queue.FlashNoReadingDataNum = Conf.Basic.FlashNoReadingDataNum;
	//ͨ����
	JlyParam.ChannelNumOld = Conf.Jly.ChannelNum;	//�״��ϵ籸��ͨ������
	JlyParam.ChannelNumActual = JlyParam.ChannelNumOld;	
	/*------------------------------------------------------*/
	for(i=0; i<Conf.Jly.ChannelNum; i++)
	{
		JlyParam.SensorTypeOld[i] = Conf.Sensor[i].SensorType; //���ݸ���ͨ������
		
		JlyParam.ChannelSwitchOld[i] = Conf.Sensor[i].ChannelSwitch;//����֮ǰ��ͨ��ʹ��λ
	}
	//�ϵ��жϣ����������λ�����ж�
	for(i=0;i < JlyParam.ChannelNumOld;i++)//����ͨ��ѭ��һ��
	{
		if(Conf.Sensor[i].ChannelSwitch == 0x01)
		{
			Flag.ChannelSwitchIsOn =1;//---------�����־��Ϊ������---------
			JlyParam.ChannelNumActual = JlyParam.ChannelNumActual - 1;//��ͨ�����ر���ʵ��ͨ��������ȥ�رյ�ͨ������ ���¸�ֵ
			if(JlyParam.ChannelNumActual <=0)
			{
				JlyParam.ChannelNumActual =0;//ͨ��������
			}
		}
	}
	JlyParam.ChannelNumActualOld = JlyParam.ChannelNumActual;//���ڵ�ǰλ�ñ��ֺ�û�и�λǰ��һ��
	/*------------------------------------------------------*/
	Queue.HIS_ONE_BYTES = (uint16_t)(JlyParam.ChannelNumActual*2+8*Gps_choose+5+Clock_choose); //һ֡���ݴ�С
	Conf.Basic.HisOneBytes = Queue.HIS_ONE_BYTES;
	Fram_Write(&Conf.Basic.HisOneBytes,HisOneBytesAddr,1);//һ���ֽ� �������û��޸�ͨ��������һ֡���ݻᱻ�޸���Ҫд��fram

	Queue.FlashRecActualStorage = FLASH_RecMaxSize/Queue.HIS_ONE_BYTES*Queue.HIS_ONE_BYTES; //flash��ʵ�ʴ洢�ֽ���
	
	Conf.Basic.FlashRecMaxSize = FLASH_RecMaxSize;//flash���洢���� 
	WriteU32Pointer(FLASH_RecMaxSizeAddr,Conf.Basic.FlashRecMaxSize);//flash���洢���� 4�ֽڱ���
	/*------------------------------------------------------*/
	JlyParam.delay_start_time = ReadDelayStartTime;			//��ȡ��ʱ����ʱ��
	JlyParam.NormalRecInterval = ReadNormalRecIntervalTime;	//��ȡ������¼��� ��λ��s
	JlyParam.NormalRecIntervalMin = JlyParam.NormalRecInterval;//������¼��� ��λ��min
	
	//�ɼ�ʱ���� ��һ�汾������ms����Э������Ƶ� uint16_t ���65535 s,
	//JlyParam.SampleInterval = Conf.Jly.SampleInterval/1000 ;
	JlyParam.SampleInterval = Conf.Jly.SampleInterval;
	JlyParam.SampleTimeCount = JlyParam.SampleInterval;  		//�ɼ�ʱ�����
	
	//������������
	JlyParam.ContinueExcessiveTimes = 3; //����������� 0-10������ Ĭ��3
	JlyParam.SoundLightAlarmTimeDelay =30;  //���ⱨ����ʱ ��λs 1s��18Сʱ������ Ĭ��30s
	
}
/******************************************************************************
  * @brief  Description ��ȡ��¼�ǲ���
  * @param  None
  * @retval None
  *****************************************************************************/
static void FirstScanSysData(void)
{
	if(JlyParam.FramErrorCode != 1)//Fram is ok
	{
		if(ReadSetFramFlag() == 0x5050)	//���ù�Fram
		{
			Fram_Read(Conf.Buf,FRAM_BasicConfAddr,FRAM_ConfSize);	//ϵͳ�ϵ��ȡ������Ϣ��
			SetJlyParamData();
		}else{
			
			JlyParam.ChannelNumActual = 0;//Ĭ������0��ͨ��
			JlyParam.ChannelNumOld = 0;
		}
	}else{//fram����
		Conf.Jly.WorkStatueIsOrNotStop = 0;//ֹͣ����
		JlyParam.ChannelNumActual = 0;
		JlyParam.ChannelNumOld = 0;
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
  * @brief  Description ���Fram�Ƿ�������,�������¼��ֹͣ������ʾ1Er
  * @param  None
  * @retval None
  *****************************************************************************/
static void TestFramIsOrNotOk(void)
{
	uint8_t Fram_Buf_Write[256];
	uint8_t Fram_Buf_Read[256];
	uint16_t i;
	
	//û�����ù�Fram������¼��Fram
	if(ReadSetFramFlag() != 0x5050)
	{
		for ( i=0; i<=255; i++ ) //��仺��
		{   
			Fram_Buf_Write[i] = i;
			
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
				JlyParam.FramErrorCode = 1;
				return;
			}
			//printf("0x%02X ", Fram_Buf_Read[i]);
			//if(i%16 == 15)    
			//	printf("\r\n");
		}
	}
}

/******************************************************************************
  * @brief  Description �ص������Դ
						1.�ش�������Դ
						2.�ص�ص�ѹ����Դ
						3.�ط�����
						4.��LED��
						5.LCD�����Դ
						6.model
						7.HAC
						8.��������
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
	//MODEL_PWRCTRL(OFF);
	//TOUCHKEY_POWER(OFF);
	HAC_POWER(OFF);
}
/******************************************************************************
  * @brief  Description �����ʼ��
  * @param  None
  * @retval None
  *****************************************************************************/
void PeripheralInit(void)
{
	SysClock_Config();

	//GPIO_ReConfig();//Ӱ��ADC��������,��д����ʱ����reset��λ

	SysTick_Init();
	
	WakeUp_GPIO_Config();
	
	KEY_GPIO_Config();
	EXTI15_10_Config();

	General_GPIO_Config();	
	I2C_GPIO_Config();

	LCD_GLASS_Init();

	USART1_Config(Usart1_DefaultBaudRate);

	LCD_GLASS_Clear();

	ADC1_Init();

	SPI_FLASH_Init();
	
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
    
	//���Fram
	TestFramIsOrNotOk();
	
	FirstScanSysData();
	
	//ϵͳ�ϵ�����ӵ�
	FirstCheckExternPower();
	//�ϵ���ʾ
    FisrtPowerOnDisplay();
	
	//�ϵ���ж�ͨ������,����Ϊ0��ʾNUL
	if(JlyParam.ChannelNumActual >0)
	{
		JudgingChannelNumberDisplay(JlyParam.ChannelNumOld);
	}else{
		JudgingChannelNumberDisplay(JlyParam.ChannelNumActual);
	}
	
	RTC8025_Reset(true);
	
    TIM2_Configuration();	//������ʱ��
	
	
	
    Flag.MucReset = 1;
	
	MODEL_PWRCTRL(ON);	  //������ӿڵ�Դ
	TOUCHKEY_POWER(ON);	  //������������Դ
	
	BellNn(1); //���￪��ϵͳ�δ�ʱ�ӶԽ��͹�����Ӱ��
		
}

