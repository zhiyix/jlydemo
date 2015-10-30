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
	
//	PWR_BackupAccessCmd(ENABLE);//允许修改RTC和后备寄存器
//	BKP_TamperPinCmd(DISABLE);//关闭入检测功能，也就是PC13，也可以当普通IO使用
//	PWR_BackupAccessCmd(DISABLE);//禁止修改后备寄存器

//BKP_ITConfig(DISABLE);  
	
	/*关闭低速外部时钟信号功能后,PC13 PC14 PC15才可以当普通IO用*/
    RCC_LSEConfig(RCC_LSE_OFF); 
	///*!< Wait till LSE is ready */
    //while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
		
	/*开启低速内部时钟供LCD 使用*/
	RCC_LSICmd(ENABLE);
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	RCC_RTCCLKCmd(ENABLE); 

	/* Wait for RTC APB registers synchronisation */
	RTC_WaitForSynchro();
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
	
	GPIO_ResetBits(LcdVccCtrl_PORT,LcdVccCtrl_PIN); 
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
	
}

/******************************************************************************
  * @brief  Description 读取记录仪参数
  * @param  None
  * @retval None
  *****************************************************************************/
static void FirstScanSysData(void)
{
	
	Fram_Read(Conf.Buf,FRAM_BasicConfAddr,FRAM_ConfSize);	//系统上电读取配置信息表
	
	/*重要参数*/
	JlyParam.delay_start_time = ReadDelayStartTime;			//读取延时启动时间
	JlyParam.NormalRecInterval = ReadNormalRecIntervalTime;	//读取正常记录间隔 单位：s
	JlyParam.NormalRecIntervalOld = JlyParam.NormalRecInterval;	  //保存上一次的记录间隔
	JlyParam.NormalRecIntervalMin = JlyParam.NormalRecInterval/60;//正常记录间隔 单位：min
	
	//采集时间间隔 单位:ms转s,按协议中设计的 uint16_t 最大65535ms 65s,
	JlyParam.SampleInterval = Conf.Jly.SampleInterval/1000 ;
	JlyParam.SampleTime = JlyParam.SampleInterval;  		//采集时间 单位:s
	
}
/******************************************************************************
  * @brief  Description 设置内存中记录仪参数,判断记录间隔是否已改变，改变了则
						询问是否清楚历史数据
  * @param  None
  * @retval None
  *****************************************************************************/
void SetJlyParamData(void)
{
	/*重要参数*/
	JlyParam.delay_start_time = ReadDelayStartTime;			//读取延时启动时间
	JlyParam.NormalRecInterval = ReadNormalRecIntervalTime;	//读取正常记录间隔 单位：s
	JlyParam.NormalRecIntervalMin = JlyParam.NormalRecInterval/60;//正常记录间隔 单位：min
	if(JlyParam.NormalRecInterval != JlyParam.NormalRecIntervalOld)//记录间隔已改变
	{
		JlyParam.NormalRecIntervalOld = JlyParam.NormalRecInterval;	  //保存上一次的记录间隔
		Queue.RecorderFramPointer = 0; //清除Fram中的历史数据
	}
	
	//采集时间间隔 单位:ms转s,按协议中设计的 uint16_t 最大65535ms 65s,
	JlyParam.SampleInterval = Conf.Jly.SampleInterval/1000 ;
	JlyParam.SampleTime = JlyParam.SampleInterval;  		//采集时间 单位:s
	
}
/******************************************************************************
  * @brief  Description 关掉外设电源
						1.关传感器电源
						2.关电池电压检测电源
						3.关蜂鸣器
						4.关LED灯
						5.LCD背光电源
  * @param  None
  * @retval None
  *****************************************************************************/
void OffPowerSupply(void)
{
	AVCC1_POWER(OFF);     //关传感器电源
    BATTEST_POWER(OFF);   //关电池电压检测电源
	BEEP(OFF);
	LED1(OFF);LED2(OFF);
	LcdBackLight(OFF);
}
/******************************************************************************
  * @brief  Description 系统初始化
  * @param  None
  * @retval None
  *****************************************************************************/
void SysInit(void)
{
    
	OffPowerSupply();
    //系统上电检测外接电
	FirstCheckExternPower();
	//上电显示
    FisrtPowerOnDisplay();
	
	FirstScanSysData();
	
	//上电后判断通道数量,数量为0显示NUL
	JudgingChannelNumberDisplay(Conf.Jly.ChannelNum);
    
	
    Flag.MucReset = 1;
    Flag.IsDisplayRightNow = 1;    
	
	Queue.FlashSectorPointer = 0;
	
	
	MODEL_PWRCTRL(ON);	  //开对外接口电源
	TOUCHKEY_POWER(ON);	  //开触摸按键电源
	
	BellNn(1);
	
	/*****************************************/
	//测试
	
	/*****************************************/
}
/******************************************************************************
  * @brief  Description 外设初始化
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
	
	USART1_Config(115200);
    
//	Delay_ms(10);   //开启滴答定时
	LCD_GLASS_Clear();
	
	ADC1_Init();
    
    /* 8M串行flash W25Q64初始化 */
	SPI_FLASH_Init();
	
    //RX8025AC 初始化
	RX8025_RTC_Init();
}
