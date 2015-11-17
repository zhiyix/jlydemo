/**
  ******************************************************************************
  * @file    init.c
  * @author  
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   初始化函数接口
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
	
	/*STOP模式下，调试使能*/
//	DBGMCU_Config(DBGMCU_STOP,ENABLE);
//	DBGMCU_Config(DBGMCU_STOP,DISABLE);
	
	
	//获取系统时钟类型（0x00: MSI used as system clock ；0x04: HSI used as system clock ；0x08: HSE used as system clock ；0x0C: PLL used as system clock ）
	SYS_CLK = RCC_GetSYSCLKSource();
	//获取系统各时钟频率值
	RCC_GetClocksFreq(&RCC_ClockFreq);	

	/* Reset Backup Domain */
//	RCC_RTCResetCmd(ENABLE);
//	RCC_RTCResetCmd(DISABLE);	
//	PWR_BackupAccessCmd(ENABLE);//允许修改RTC和后备寄存器
//	BKP_TamperPinCmd(DISABLE);//关闭入检测功能，也就是PC13，也可以当普通IO使用
//	PWR_BackupAccessCmd(DISABLE);//禁止修改后备寄存器
//BKP_ITConfig(DISABLE);  
	
	
	/*关闭低速外部时钟信号功能后,PC13 PC14 PC15才可以当普通IO用*/
    RCC_LSEConfig(RCC_LSE_OFF); 		
	
	/* Allow access to the RTC */
	PWR_RTCAccessCmd(ENABLE);
	/*开启低速内部时钟供LCD 使用*/
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
	//使能 HSE
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus == SUCCESS)
	{
		//使能 PLL
		RCC_PLLCmd(ENABLE);
		//等待 PLL准备就绪
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}
		//选择 PLL作为系统时钟源
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		//等待 PLL被选择为系统时钟源
		while(RCC_GetSYSCLKSource() != 0x0C){}
	}
	
	/* 使能内部 HSI 注意时钟*/
	RCC_HSICmd(ENABLE);	//ADC使用
	/* Check that HSI oscillator is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	
	/*STOP模式下，调试使能*/
	DBGMCU_Config(DBGMCU_STOP,ENABLE);
//	DBGMCU_Config(DBGMCU_STOP,DISABLE);
	
	//获取系统时钟类型（0x00: MSI used as system clock ；0x04: HSI used as system clock ；0x08: HSE used as system clock ；0x0C: PLL used as system clock ）
	SYS_CLK = RCC_GetSYSCLKSource();
	//获取系统各时钟频率值
	RCC_GetClocksFreq(&RCC_ClockFreq);

	/* Reset Backup Domain */
//	RCC_RTCResetCmd(ENABLE);
//	RCC_RTCResetCmd(DISABLE);
//	PWR_BackupAccessCmd(ENABLE);//允许修改RTC和后备寄存器
//	BKP_TamperPinCmd(DISABLE);//关闭入检测功能，也就是PC13，也可以当普通IO使用
//	PWR_BackupAccessCmd(DISABLE);//禁止修改后备寄存器
//BKP_ITConfig(DISABLE);  
	
	
	/*关闭低速外部时钟信号功能后,PC13 PC14 PC15才可以当普通IO用*/
    RCC_LSEConfig(RCC_LSE_OFF); 
	
	/* Allow access to the RTC */
	PWR_RTCAccessCmd(ENABLE);
	/*开启低速内部时钟供LCD 使用*/
	RCC_LSICmd(ENABLE);
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	RCC_RTCCLKCmd(ENABLE); 
	/* Wait for RTC APB registers synchronisation */
	//RTC_WaitForSynchro();
	
}

/*****************************************************************************
 * 函数名：GPIO_ReConfig
 * 描述  ：所有的GPIO重新初始化为模拟输入，降低功耗
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
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
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;	//模拟输入
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
  * @brief  Description 配置I2C总线的GPIO，采用模拟IO的方式实现
						fram、rx8025用到I2C
  * @param  无			
  * @retval 无		
  *****************************************************************************/
static void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_I2C_PORT, ENABLE);	/* 打开GPIO时钟 */

	GPIO_InitStructure.GPIO_Pin = BSP_I2C_SCL | BSP_I2C_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;/* 开漏输出 */
    
	GPIO_Init(BSP_I2C_PORT, &GPIO_InitStructure);

	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
//	i2c_Stop();
}
/*****************************************************************************
 * 函数名：WakeUp_GPIO_Config
 * 描述  ：配置PA0 唤醒引脚,外部中断
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 *****************************************************************************/
static void WakeUp_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;	
	
	/*唤醒 PA0*/
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
}
/*****************************************************************************
 * 函数名：General_GPIO_Config
 * 描述  ：配置用到的I/O口
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 *****************************************************************************/
static void General_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/************************GPIOF***************************/
	/*外接电接入，充电指示完成*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF,ENABLE);
	GPIO_InitStructure.GPIO_Pin = Power_ACtest_PIN | Power_CHGtest_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(Power_Deal_PORT,&GPIO_InitStructure);
	/*液晶背光 PF2*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF,ENABLE);
	GPIO_InitStructure.GPIO_Pin = LcdVccCtrl_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LcdVccCtrl_PORT,&GPIO_InitStructure);
	
	GPIO_SetBits(LcdVccCtrl_PORT,LcdVccCtrl_PIN); 
	/*触摸按键控制电源 PF3*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF,ENABLE);
	GPIO_InitStructure.GPIO_Pin = TouchVccCtrl_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(TouchVccCtrl_PORT,&GPIO_InitStructure);
	
	GPIO_SetBits(TouchVccCtrl_PORT,TouchVccCtrl_PIN);//关闭触摸按键电源 
	//LED和 蜂鸣器GPIO初始化 ，蜂鸣器GPIO_Pin_15
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
	GPIO_ResetBits(GPIOF,GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15);//关闭
	
	/************************GPIOE***************************/
	//ADC电源开关
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOE,GPIO_Pin_5);
//	GPIO_ResetBits(GPIOE,GPIO_Pin_5);
    
    //外接电池电压检测开关
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOE,GPIO_Pin_4);   //关闭电池电源检测
	
	/************************GPIOD***************************/
	//model 对外接口电源
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOD,GPIO_Pin_0);   //关闭model电源
	
	//HAC电源
	RCC_AHBPeriphClockCmd(HacVccCtrl_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Pin = HacVccCtrl_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(HacVccCtrl_PORT,&GPIO_InitStructure);
	
	GPIO_SetBits(HacVccCtrl_PORT,HacVccCtrl_PIN);//关闭触摸按键电源 
}

/******************************************************************************
  * @brief  Description 设置内存中记录仪参数
  * @param  None
  * @retval None
  *****************************************************************************/
static void SetJlyParamData(void)
{
	uint8_t i;
	//重要参数
	Queue.FlashRecOverFlow = Conf.Basic.RecordFlashOverFlow ; //读出flash溢出标志
	Queue.FlashSectorPointer = Conf.Basic.FlashSectorPointer;
	Queue.WriteFlashDataPointer = Conf.Basic.WriteFlashDataPointer;
	Queue.FlashReadDataBeginPointer = Conf.Basic.FlashReadDataBeginPointer;
	Queue.ReadFlashDataPointer = Conf.Basic.ReadFlashDataPointer;
	Queue.FlashNoReadingDataNum = Conf.Basic.FlashNoReadingDataNum;
	/*------------------------------------------------------*/
	Queue.HIS_ONE_BYTES = (uint16_t)(Conf.Jly.ChannelNum*2+8*Gps_choose+5+Clock_choose); //一帧数据大小
	//Queue.FRAM_MAX_NUM = FRAM_RecMaxSize/Queue.HIS_ONE_BYTES;	//fram中存储数据的最大包数 4096/
	Queue.FLASH_SECTOR_PER_NUM = FLASH_SectorPerSize/Queue.HIS_ONE_BYTES; //flash中一个扇区存储数据的最大包数
	Queue.FLASH_MAX_NUM = FLASH_RecMaxSize/Queue.HIS_ONE_BYTES; //flash中存储数据的最大包数 8388608/
	/*------------------------------------------------------*/
	JlyParam.delay_start_time = ReadDelayStartTime;			//读取延时启动时间
	JlyParam.NormalRecInterval = ReadNormalRecIntervalTime;	//读取正常记录间隔 单位：s
	JlyParam.NormalRecIntervalMin = JlyParam.NormalRecInterval/60;//正常记录间隔 单位：min
	
	//采集时间间隔 单位:ms转s,按协议中设计的 uint16_t 最大65535ms 65s,
	JlyParam.SampleInterval = Conf.Jly.SampleInterval/1000 ;
	JlyParam.SampleTime = JlyParam.SampleInterval;  		//采集时间 单位:s
	/*------------------------------------------------------*/
	JlyParam.ChannelNumOld = Conf.Jly.ChannelNum;	//备份通道数量
	for(i=0; i<Conf.Jly.ChannelNum; i++)
	{
		JlyParam.SensorTypeOld[i] = Conf.Sensor[i].SensorType; //备份各个通道类型
	}
}
/******************************************************************************
  * @brief  Description 读取记录仪参数
  * @param  None
  * @retval None
  *****************************************************************************/
static void FirstScanSysData(void)
{
	if(JlyParam.LastErrorCode != 1)//Fram is ok
	{
		if(ReadSetFramFlag() == 0x5050)
		{
			Fram_Read(Conf.Buf,FRAM_BasicConfAddr,FRAM_ConfSize);	//系统上电读取配置信息表
		}else{
			
			Conf.Jly.ChannelNum = 0;//默认设置0个通道
		}
		SetJlyParamData();
	}else{//fram出错
		Conf.Jly.WorkStatueIsStop = 0;//停止工作
		Conf.Jly.ChannelNum = 0;
	}
}

/******************************************************************************
  * @brief  Description 设置 配置Fram标志位 0x5050
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
  * @brief  Description 读 配置Fram标志位
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
  * @brief  Description 设置内存中记录仪参数,判断通道数量、通道类型是否修改
						如果修改了就清除历史数据
  * @param  None
  * @retval None
  *****************************************************************************/
void SetJlyParamJudgeChannelNumSensorType(void)
{
	uint8_t i;
	//重要参数
	Queue.FlashRecOverFlow = Conf.Basic.RecordFlashOverFlow; //读出flash溢出标志
	Queue.FlashSectorPointer = Conf.Basic.FlashSectorPointer;
	Queue.ReadFlashDataPointer = Conf.Basic.ReadFlashDataPointer;
	/*------------------------------------------------------*/
	Queue.HIS_ONE_BYTES = (uint16_t)(Conf.Jly.ChannelNum*2+8*Gps_choose+5+Clock_choose); //一帧数据大小
	//Queue.FRAM_MAX_NUM = FRAM_RecMaxSize/Queue.HIS_ONE_BYTES;	//fram中存储数据的最大包数 4096/
	Queue.FLASH_SECTOR_PER_NUM = FLASH_SectorPerSize/Queue.HIS_ONE_BYTES; //flash中一个扇区存储数据的最大包数
	Queue.FLASH_MAX_NUM = FLASH_RecMaxSize/Queue.HIS_ONE_BYTES; //flash中存储数据的最大包数 8388608/
	/*------------------------------------------------------*/
	JlyParam.delay_start_time = ReadDelayStartTime;			//读取延时启动时间
	JlyParam.NormalRecInterval = ReadNormalRecIntervalTime;	//读取正常记录间隔 单位：s
	JlyParam.NormalRecIntervalMin = JlyParam.NormalRecInterval/60;//正常记录间隔 单位：min
	
	//采集时间间隔 单位:ms转s,按协议中设计的 uint16_t 最大65535ms 65s,
	JlyParam.SampleInterval = Conf.Jly.SampleInterval/1000 ;
	JlyParam.SampleTime = JlyParam.SampleInterval;  		//采集时间 单位:s
	/*------------------------------------------------------*/
	if(JlyParam.ChannelNumOld != Conf.Jly.ChannelNum)
	{
		JlyParam.ChannelNumOld = Conf.Jly.ChannelNum;	//备份通道数量
		
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

		SetFlashOverFlow(0);//清除flash溢出标志
	}else{
		for(i=0; i<Conf.Jly.ChannelNum; i++)
		{
			if(JlyParam.SensorTypeOld[i] != Conf.Sensor[i].SensorType)
			{
				//通道数循环完，并且备份新的通道类型，如果有改变的则清除历史数据
				JlyParam.SensorTypeOld[i] = Conf.Sensor[i].SensorType;
				Flag.SensorTypeIsChange = 1;
			}
		}
		//循环完所有通道再执行
		if(Flag.SensorTypeIsChange == 1)
		{
			Flag.SensorTypeIsChange = 0; //标志清除
			
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

			SetFlashOverFlow(0);//清除flash溢出标志
		}
	}
}

/******************************************************************************
  * @brief  Description 检测Fram是否有问题,有问题记录仪停止工作显示1Er
  * @param  None
  * @retval None
  *****************************************************************************/
static void TestFramIsOrNotOk(void)
{
	uint8_t Fram_Buf_Write[256];
	uint8_t Fram_Buf_Read[256];
	uint16_t i;
 
	for ( i=0; i<=255; i++ ) //填充缓冲
	{   
		Fram_Buf_Write[i] = i;
		//printf("0x%02X ", Fram_Buf_Write[i]);
		//if(i%16 == 15)    
		//printf("\r\n");    
	}

	//将Fram_Buf_Write中顺序递增的数据写入FRAM中 
	Fram_Write( Fram_Buf_Write, FRAM_TestIsOkAddr, 256);
	
	//将FRAM读出数据顺序存储到Fram_Buf_Read
	Fram_Read(Fram_Buf_Read, FRAM_TestIsOkAddr, 256); 

	//将I2c_Buf_Read中的数据通过串口打印
	for (i=0; i<256; i++)
	{	
		if(Fram_Buf_Read[i] != Fram_Buf_Write[i])
		{
			//printf("0x%02X ", Fram_Buf_Read[i]);
			//printf("错误:I2C EEPROM写入与读出的数据不一致\r\n");
			JlyParam.LastErrorCode = 1;
			return;
		}
		//printf("0x%02X ", Fram_Buf_Read[i]);
		//if(i%16 == 15)    
		//	printf("\r\n");
	}
}

/******************************************************************************
  * @brief  Description 关掉外设电源
						1.关传感器电源
						2.关电池电压检测电源
						3.关蜂鸣器
						4.关LED灯
						5.LCD背光电源
						6.model
						7.HAC
						8.触摸按键
  * @param  None
  * @retval None
  *****************************************************************************/
void OffPowerSupply(void)
{
	//AVCC1_POWER(OFF);     //关传感器电源
    BATTEST_POWER(OFF);   //关电池电压检测电源
	BEEP(OFF);
	LED1(OFF);LED2(OFF);
	LcdBackLight(OFF);
	//MODEL_PWRCTRL(OFF);
	TOUCHKEY_POWER(OFF);
	HAC_POWER(OFF);
}
/******************************************************************************
  * @brief  Description 系统初始化
  * @param  None
  * @retval None
  *****************************************************************************/
void SysInit(void)
{
    //关外设电源
	OffPowerSupply();
    //系统上电检测外接电
	FirstCheckExternPower();
	//检测Fram
	TestFramIsOrNotOk();
	
	FirstScanSysData();
	
	//上电显示
    FisrtPowerOnDisplay();
	
	//上电后判断通道数量,数量为0显示NUL
	JudgingChannelNumberDisplay(Conf.Jly.ChannelNum);
    
	
    Flag.MucReset = 1;
	Flag.FirstNotEnterStopMode = 1;
	
	MODEL_PWRCTRL(ON);	  //开对外接口电源
	TOUCHKEY_POWER(ON);	  //开触摸按键电源
	
	BellNn(1); //这里开启系统滴答时钟对进低功耗有影响
	
	/*****************************************/
	//测试
	//Reset_Time();
	/*****************************************/
}
/******************************************************************************
  * @brief  Description 外设初始化
  * @param  None
  * @retval None
  *****************************************************************************/
void PeripheralInit(void)
{
	SysClock_Config();

	//GPIO_ReConfig();//影响ADC正常工作,烧写程序时必须reset复位

	SysTick_Init();

	TIM2_Configuration();	//开启定时器
	
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

	//RX8025_RTC_Init();
	RTC8025_Reset(true);
	
}
