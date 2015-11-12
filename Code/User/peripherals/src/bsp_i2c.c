/**
  ******************************************************************************
  * @file              : bsp_i2c.c
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
#ifndef __BSPI2C_C
#define __BSPI2C_C

/*============================ INCLUDES ======================================*/
/* Includes ------------------------------------------------------------------*/
#include <stdarg.h>

#include "main.h"

//	<<< Use Configuration Wizard in Context Menu >>>
//	<e>使用printf文本输出
//	<i>通过系统函数printf输出打印调试信息
#define	DEBUG_OUTPUT_EN				0
//		<e>USART1映射printf
//		<i>使用USART1文本输出打印调试信息
//		<i>或者使用ITM Trace输出打印调试信息
#define DEBUG_OUTPUT_USART1_EN		0
//		</e>
//	</e>
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
static void Delay(uint32_t time)
{
// 这里可以优化速度，经超频与降频测试，保持为空函数时还可以写入
	uint32_t i=time;
	while(i)
	{
		i--;
	}
}
/*
********************************************************************************
			 起动总线函数               
函数原型: void  Start_I2c();  
功能:     启动I2C总线,即发送I2C起始条件.  
********************************************************************************
*/
bool Start(void)
{
//开始信号 SCL在高电平期间，SDA一个下降沿则表示启动信号，本函数也用来产生重复起始条件
//当前只有一个IIC设备，不会有其它设备占用的情况，所以不会有总线忙的情况出现，所以可以没有返回值
	EM_SDA_H();			///* 发送起始条件的数据信号*/释放SDA总线
	EM_SCL_H();
	Delay(200);			/* 起始条件建立时间大于4.7us,延时*/
//			if(!I2CIO.ReadInputDataBit(BSP_I2C_SDA))
	if(!GPIO_ReadInputDataBit(BSP_I2C_PORT,BSP_I2C_SDA))
		return false;	//SDA线为低电平则总线忙,退出
	EM_SDA_L();			/* 发送起始信号*/
	Delay(200);			/* 起始条件锁定时间大于4μs*/
//			if(I2CIO.ReadInputDataBit(BSP_I2C_SDA))
	if(GPIO_ReadInputDataBit(BSP_I2C_PORT,BSP_I2C_SDA))
		return false;	//SDA线为高电平则总线忙,退出
	EM_SCL_L();			/* 钳住I2C总线，准备发送或接收数据 */
	Delay(200);
	return true;
}
/*
********************************************************************************
			  结束总线函数               
函数原型	: void  Stop_I2c();  
功能		:     结束I2C总线,即发送I2C结束条件.  
********************************************************************************
*/
bool Stop(void)
{
	EM_SCL_L();
	Delay(200);	
//停止 SCL在高电平期间，SDA一个上升沿则表示停止信号
	EM_SDA_L();			/*发送结束条件的时钟/数据信号*/
	Delay(200);	
	EM_SCL_H();
	Delay(200);	/*结束条件建立时间大于4μs*/
	EM_SDA_H();			/*发送I2C总线结束信号*/
	Delay(200);
	return true;
}
/*
********************************************************************************
函数原型	: bit I2C_Get_Ack_From_Slave(void)
功能		: Get_Ack_From_Slave
返回值	: Get the Ack or lose
********************************************************************************
*/
bool WaitAck(void)
{
	int i = 0;			//WaitAck
	EM_SCL_L();
	Delay(200);
	EM_SDA_H();   		/*8位发送完后释放数据线，准备接收应答位*/
	Delay(200);
//应答 SCL在高电平期间，SDA被从设备拉为低电平表示应答
	EM_SCL_H();  		
	Delay(200);
//			while((I2CIO.ReadInputDataBit(BSP_I2C_SDA)==1)&&(i<250))i++;  
	while((GPIO_ReadInputDataBit(BSP_I2C_PORT,BSP_I2C_SDA)==1)&&(i<250))i++;  
//			if((I2CIO.ReadInputDataBit(BSP_I2C_SDA)==1))
	if((GPIO_ReadInputDataBit(BSP_I2C_PORT,BSP_I2C_SDA)==1))
	{		
		EM_SCL_L();  
		Delay(200);
		Delay(200);

		return false;
	} else				/*判断是否接收到应答信号 (ack == 0) <ACK>*/
	{
		Delay(200);
		EM_SCL_L();  
		Delay(200);

		return true;
	}
}

/******************************************************************************
			 应答子函数
函数原型:  void Ack_I2c(bit a);
功能:      主控器进行应答信号(可以是应答或非应答信号，由位参数a决定)
******************************************************************************/
/******************************************************************************
函数：I2C_PutAck()
功能：主机产生应答位（应答或非应答），用于通知从机：主机是否成功接收从机数据
参数：Ack = 0：主机应答
Ack = 1：主机非应答
说明：主机在收到每一个从机字节后都要求从机产生应答，在收到最后一个字节时，应当产生非应答，通知从机不要再应答
******************************************************************************/
void Ack(bool ack)
{
	EM_SCL_L();
	Delay(200);	
	/*(ack == 0) : ACK : An acknowledge(ACK) by pulling down the SDA line 
						 During the 9th clock cycle*/
	if(ack)				
	{
		EM_SDA_H();		//I2C_NoAck()
	}
	else
	{
		EM_SDA_L();		/*在此发出应答或非应答信号 *///I2C_Ack()
	}
	Delay(200);
	EM_SCL_H();
	Delay(200);			/*时钟低电平周期大于4μs*/
	EM_SCL_L();			/*清时钟线，钳住I2C总线以便继续接收*/
	Delay(200);
}
/*
********************************************************************************
		 字节数据发送函数               
函数原型: u8 I2C_LLV_SendByte(u8 SendByte);
功能:     将数据c发送出去,可以是地址,也可以是数据,发完后等待应答,并对
  此状态位进行操作.(不应答或非应答都使ack=0)     
  发送数据正常，ack=1; ack=0表示被控器无应答或损坏。
********************************************************************************
*/
void SendByte(uint8_t chData)
{
	uint8_t i = 0;
	for(i=0; i < 8; i++)
	{
		///拉低SCL，因为只有在时钟信号为低电平期间，数据线上的高低电平状态
		///才允许变化;并在此时和上一个循环的SDA_H一起形成一个上升沿
		EM_SCL_L();
		Delay(200);
		if(chData&0x80)	/*判断发送位*/
		{
			EM_SDA_H(); 
		}       
		else 
		{
			EM_SDA_L();
		}    
		chData<<=1;
		Delay(200);
		EM_SCL_H();		/*置时钟线为高，通知被控器开始接收数据位*/
		Delay(200);		/*保证时钟高电平周期大于4μs*/
	}
	EM_SCL_L();
}
/*
********************************************************************************
		 字节数据接收函数               
函数原型: u8 I2C_LLV_ReceiveByte(void);
功能:     用来接收从器件传来的数据,并判断总线错误(不发应答信号)，
  发完后请用应答函数应答从机。  
********************************************************************************
*/
void RecvByte(uint8_t *pData)
{
	uint8_t i;
	*pData = 0;
	//数据从高位到低位
	//EM_SCL_L();  
	//Delay();
	EM_SDA_H();  				/*置数据线为输入方式*/ 
	Delay(200);
	for(i=0; i < 8; i++)
	{
		(*pData)<<=1;
		EM_SCL_L();  
		Delay(200);
		EM_SCL_H();				//上升沿时，IIC设备将数据放在sda线上，
								//并在高电平期间数据已经稳定，可以接收啦  
								/*置时钟线为高使数据线上数据有效*/		
		Delay(200);
//				if (I2CIO.ReadInputDataBit(BSP_I2C_SDA))
		if (GPIO_ReadInputDataBit(BSP_I2C_PORT,BSP_I2C_SDA))
		{
			(*pData)|=0x01;
								//读数据位,接收的数据位放入pdata中
		}
		//EM_SCL_L();			//拉低SCL，使发送端可以把数据放在SDA上   
								/*置时钟线为低，准备接收数据位*/
		//Delay(200);			/*时钟低电平周期大于4.7μs*/
	}
	EM_SCL_L();
}

/**
  * @brief  Description "模拟I2C写函数"
  * @param  pdata  		写入的数据缓冲区指针
  * @param  addr		写入的器件内存地址
  * @param  count		写入的器件数据大小
  * @retval bool		I2C写函数是否成功
  */
bool AI2C_Write(uint8_t *pData, uint16_t addr, uint16_t count)
{
    
	uint8_t		device_addr = 0;
	uint8_t 	addr_msb = 0, addr_lsb = 0;
    uint16_t 	i;
	uAddress = AI2C_FRAM_ADDRESS;
//    CAI2C		theAI2C(0xA0);
	/*            从机ID   + 器件选择 + 读写选择  */
	device_addr = uAddress + 0x0		+ 0;
    addr_msb = ((addr / 256)&0X3F);
    addr_lsb = (addr % 256);

    if (count == 0)
    {
        return true;
    }

    if (!Start())
        return false;
    SendByte(device_addr);	// I2C_WRITE
    if (!WaitAck())
    {
        Stop();
        return false;
    }
    SendByte(addr_msb);
    WaitAck();
    SendByte(addr_lsb);
    WaitAck();
    for (i = 0; i < count; i ++)
    {
        SendByte(*(pData + i));
        WaitAck();
    }
    Stop();
    return true;
}

/**
  * @brief  Description "模拟I2C读函数"
  * @param  pdata  		读取的数据缓冲区指针
  * @param  addr		读取的器件内存地址
  * @param  count		读取的器件数据大小
  * @retval bool		I2C读函数是否成功
  */
bool AI2C_Read(uint8_t *pData, uint16_t addr, uint16_t count)
{
	uint8_t		device_addr = 0;
	uint8_t 	addr_msb = 0, addr_lsb = 0;
    uint16_t 	i;
	uAddress = AI2C_FRAM_ADDRESS;
//    CAI2C		theAI2C(0xA0);
	/*            从机ID   + 器件选择 + 读写选择  */
	device_addr = uAddress + 0x0		+ 0;
    addr_msb = ((addr / 256)&0X3F);
    addr_lsb = (addr % 256);
    if (count == 0)
    {
        return true;
    }
    if (!Start())
        return false;
    SendByte(device_addr);
    if (!WaitAck())
    {
        Stop();
        return false;
    }
    SendByte(addr_msb);
    WaitAck();
    SendByte(addr_lsb);
    WaitAck();
    Start();
    SendByte(device_addr|0X01);
    WaitAck();
    for (i = 0; i < count - 1; i ++)
    {
        RecvByte((pData + i));
        Ack(false);
    }
    RecvByte(pData + count - 1);
    Ack(true);//接收结束,发送非应答信号
    Stop();
	return true;
}

/**
  * @brief  Description "模拟I2C写RTC8025函数"
  * @param  pdata  		写入的数据缓冲区指针
  * @param  addr		写入的器件内存地址
  * @param  count		写入的器件数据大小
  * @retval bool		RTC8025写函数是否成功
  */
bool RTC8025_Write(uint8_t *pData, uint16_t addr, uint16_t count)
{
    uint8_t 	addr_h = 0, addr_l = 0;
    uint16_t 	i;
    uAddress = AI2C_RX8025_ADDRESS;

    addr_l = ((addr % 16) << 4);

    addr_h = uAddress + addr_h;
    if (count == 0)
    {
        return true;
    }
	//! START condition sent by Master
    if (!Start())
        return false;
	//! Slave address + write specification
    SendByte(addr_h);	// I2C_WRITE
	//! Confirmation response from Master
    if (!WaitAck())
    {
        Stop();
        return false;
    }
    //theAI2C.SendByte(addr_h);
    //theAI2C.WaitAck();
    SendByte(addr_l);
    WaitAck();
    for (i = 0; i < count; i ++)
    {
        SendByte(*(pData + i));
        WaitAck();
    }
    Stop();
    return true;
}

/**
  * @brief  Description "模拟I2C读RTC8025函数"
  * @param  pdata  		读取的数据缓冲区指针
  * @param  addr		读取的器件内存地址
  * @param  count		读取的器件数据大小
  * @retval bool		RTC8025读函数是否成功
  */
bool RTC8025_Read(uint8_t *pData, uint16_t addr, uint16_t count)
{
    uint8_t 	addr_h = 0, addr_l = 0;
    uint16_t 	i;
    uAddress = AI2C_RX8025_ADDRESS;

    addr_l = ((addr % 16) << 4);

    addr_h = uAddress + addr_h;
    if (count == 0)
    {
        return true;
    }
	//! START condition sent by Master
    if (!Start())
        return false;
	//! Slave address + write specification
    SendByte(addr_h);
	//! Confirmation response from Master
    if (!WaitAck())
    {
        Stop();
        return false;
    }
//	theAI2C.SendByte(addr_h);
//	theAI2C.WaitAck();
    SendByte(addr_l);
    WaitAck();
	//! RESTART condition sent by Master
    Start();
	//! Indicates next byte will be read
    SendByte((addr_h) | 0x01);
    WaitAck();
	//! Data is read from the specified start address
    for (i = 0; i < count - 1; i ++)
    {
        RecvByte((pData + i));
        Ack(false);
    }
    RecvByte(pData + count - 1);
	//! Master does not respond
    Ack(true);//接收结束,发送非应答信号
	//! STOP condition sent by Master
    Stop();
	return true;
}

/**
  * @brief  Description "模拟I2C写函数"
  * @param  pdata  		写入的数据缓冲区指针
  * @param  addr		写入的器件内存地址
  * @param  count		写入的器件数据大小
  * @retval bool		I2C写函数是否成功
  */
bool Fram_Write(uint8_t *pData, uint16_t addr, uint16_t count)
{
    
	uint8_t		device_addr = 0;
	uint8_t 	addr_msb = 0, addr_lsb = 0;
    uint16_t 	i;
	uAddress = AI2C_FRAM_ADDRESS;
//    CAI2C		theAI2C(0xA0);
	/*            从机ID   + 器件选择 + 读写选择  */
	device_addr = uAddress + 0x0		+ 0;
    addr_msb = ((addr / 256)&0X3F);
    addr_lsb = (addr % 256);

    if (count == 0)
    {
        return true;
    }

    if (!Start())
        return false;
    SendByte(device_addr);	// I2C_WRITE
    if (!WaitAck())
    {
        Stop();
        return false;
    }
    SendByte(addr_msb);
    WaitAck();
    SendByte(addr_lsb);
    WaitAck();
    for (i = 0; i < count; i ++)
    {
        SendByte(*(pData + i));
        WaitAck();
    }
    Stop();
    return true;
}

/**
  * @brief  Description "模拟I2C读函数"
  * @param  pdata  		读取的数据缓冲区指针
  * @param  addr		读取的器件内存地址
  * @param  count		读取的器件数据大小
  * @retval bool		I2C读函数是否成功
  */
bool Fram_Read(uint8_t *pData, uint16_t addr, uint16_t count)
{
	uint8_t		device_addr = 0;
	uint8_t 	addr_msb = 0, addr_lsb = 0;
    uint16_t 	i;
	uAddress = AI2C_FRAM_ADDRESS;
//    CAI2C		theAI2C(0xA0);
	/*            从机ID   + 器件选择 + 读写选择  */
	device_addr = uAddress + 0x0		+ 0;
    addr_msb = ((addr / 256)&0X3F);
    addr_lsb = (addr % 256);
    if (count == 0)
    {
        return true;
    }
    if (!Start())
        return false;
    SendByte(device_addr);
    if (!WaitAck())
    {
        Stop();
        return false;
    }
    SendByte(addr_msb);
    WaitAck();
    SendByte(addr_lsb);
    WaitAck();
    Start();
    SendByte(device_addr|0X01);
    WaitAck();
    for (i = 0; i < count - 1; i ++)
    {
        RecvByte((pData + i));
        Ack(false);
    }
    RecvByte(pData + count - 1);
    Ack(true);//接收结束,发送非应答信号
    Stop();
	return true;
}
/**
  * @brief  Description "RTC8025状态位复位函数"
  * @param  None
  * @retval bool		RTC8025读函数是否成功
  */
bool RTC8025_Reset(bool need_reset)
{
	bool need_clear = 0;
	
	RTC8025_Ctrl_Typedef ctrl;
	RTC8025_Read((char*)&ctrl.Control, 0x0E, 2);
	if (ctrl.STREG.PON)
	{
		//DebugOutPrintf(DEBUG_INF, "power-on reset was detected, ");
		//DebugOutPrintf(DEBUG_INF, "you may have to readjust the clock.\r\n");
		need_reset = true;
	}
	if (ctrl.STREG.VDET)
	{
		//DebugOutPrintf(DEBUG_INF, "a power voltage drop was detected, ");
		//DebugOutPrintf(DEBUG_INF, "you may have to readjust the clock.\r\n");
		need_reset = true;
	}
	if (!ctrl.STREG.XST)
	{
		//DebugOutPrintf(DEBUG_INF, "Oscillation stop was detected,");
		//DebugOutPrintf(DEBUG_INF, "you may have to readjust the clock.\r\n");
		need_reset = true;
	}
	if (ctrl.STREG.DAFG | ctrl.STREG.WAFG)
	{
		//DebugOutPrintf(DEBUG_INF, "Alarm was detected.\r\n");
		need_clear = true;
	}
	if (ctrl.STREG.CTFG)
	{
		//DebugOutPrintf(DEBUG_INF, "Periodic interrupt output OFF status.\r\n");
		need_clear = true;
	}
	if (need_reset || need_clear)
	{
		/* 设置 RTC8025 固定周期中断功能 */
		ctrl.STREG.CT = 3;		
		ctrl.STREG.PON = 0;
		ctrl.STREG.VDET = 0;
		ctrl.STREG.XST = 1;
		ctrl.STREG.DAFG = 0;
		ctrl.STREG.WAFG = 0;
		ctrl.STREG.CTFG = 0;
		ctrl.STREG.H12_24 = 1;
		RTC8025_Write((char*)&ctrl.Control, 0x0E, 2);
		RTC8025_Read((char*)&ctrl.Control, 0x0E, 2);
	}
	if (need_reset)
	{
		return false;
	}
	return true;
}


#endif /* __BSPI2C_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
