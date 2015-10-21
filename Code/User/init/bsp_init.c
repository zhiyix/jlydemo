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

/*
 * 函数名：General_GPIO_Config
 * 描述  ：配置用到的I/O口
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void General_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
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

/**
  * @brief  Description 读取记录仪参数
  * @param  None
  * @retval None
  */
static void FirstScanSysData(void)
{
	uint8_t num=0;
	uint8_t buf[20];
	buf[0]=8;
    JlyParam.SaveDataTimeOnlyRead = 10; //采样时间
    JlyParam.Save_Time = JlyParam.SaveDataTimeOnlyRead;
    
	//test
//	Fram_Write(buf,FRAM_WorkStatueIsStopAddr,1);
//	num = Fram_Read(buf,FRAM_WorkStatueIsStopAddr,1);
	
	Fram_Read(Conf.Buf,FRAM_BasicConfAddr,FRAM_ConfSize);	/*!< 系统上电读取配置信息表*/
	
	JlyParam.delay_start_time = ReadDelayStartTime ;
	num = Conf.Jly.WorkStatueIsStop;
	JlyParam.SaveHisDataTime = 2;  //保存数据间隔
	
	
}
/**
  * @brief  Description 系统初始化
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
    
	Started_Channel = GetStartChanel(Conf.Jly.ChannelNum); //通道数转换为 启动的通道
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
  * @brief  Description 外设初始化
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
    
//	Delay_ms(10);   //开启滴答定时
	LCD_GLASS_Clear();
	
	AVCC1_POWER(OFF);    //关传感器电源
    BATTEST_POWER(OFF);  //关电池电压检测电源
	MODEL_PWRCTRL(ON);	//开对外接口电源
	ADC1_Init();
    
    /* 8M串行flash W25Q64初始化 */
	SPI_FLASH_Init();
	
	I2C_GPIO_Config();
    
    //rtc  初始化
    rtc_init();
    
    
}

