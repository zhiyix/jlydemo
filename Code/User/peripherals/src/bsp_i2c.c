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
//	<e>ʹ��printf�ı����
//	<i>ͨ��ϵͳ����printf�����ӡ������Ϣ
#define	DEBUG_OUTPUT_EN				0
//		<e>USART1ӳ��printf
//		<i>ʹ��USART1�ı������ӡ������Ϣ
//		<i>����ʹ��ITM Trace�����ӡ������Ϣ
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
// ��������Ż��ٶȣ�����Ƶ�뽵Ƶ���ԣ�����Ϊ�պ���ʱ������д��
	uint32_t i=time;
	while(i)
	{
		i--;
	}
}
/*
********************************************************************************
			 �����ߺ���               
����ԭ��: void  Start_I2c();  
����:     ����I2C����,������I2C��ʼ����.  
********************************************************************************
*/
bool Start(void)
{
//��ʼ�ź� SCL�ڸߵ�ƽ�ڼ䣬SDAһ���½������ʾ�����źţ�������Ҳ���������ظ���ʼ����
//��ǰֻ��һ��IIC�豸�������������豸ռ�õ���������Բ���������æ��������֣����Կ���û�з���ֵ
	EM_SDA_H();			///* ������ʼ�����������ź�*/�ͷ�SDA����
	EM_SCL_H();
	Delay(200);			/* ��ʼ��������ʱ�����4.7us,��ʱ*/
//			if(!I2CIO.ReadInputDataBit(BSP_I2C_SDA))
	if(!GPIO_ReadInputDataBit(BSP_I2C_PORT,BSP_I2C_SDA))
		return false;	//SDA��Ϊ�͵�ƽ������æ,�˳�
	EM_SDA_L();			/* ������ʼ�ź�*/
	Delay(200);			/* ��ʼ��������ʱ�����4��s*/
//			if(I2CIO.ReadInputDataBit(BSP_I2C_SDA))
	if(GPIO_ReadInputDataBit(BSP_I2C_PORT,BSP_I2C_SDA))
		return false;	//SDA��Ϊ�ߵ�ƽ������æ,�˳�
	EM_SCL_L();			/* ǯסI2C���ߣ�׼�����ͻ�������� */
	Delay(200);
	return true;
}
/*
********************************************************************************
			  �������ߺ���               
����ԭ��	: void  Stop_I2c();  
����		:     ����I2C����,������I2C��������.  
********************************************************************************
*/
bool Stop(void)
{
	EM_SCL_L();
	Delay(200);	
//ֹͣ SCL�ڸߵ�ƽ�ڼ䣬SDAһ�����������ʾֹͣ�ź�
	EM_SDA_L();			/*���ͽ���������ʱ��/�����ź�*/
	Delay(200);	
	EM_SCL_H();
	Delay(200);	/*������������ʱ�����4��s*/
	EM_SDA_H();			/*����I2C���߽����ź�*/
	Delay(200);
	return true;
}
/*
********************************************************************************
����ԭ��	: bit I2C_Get_Ack_From_Slave(void)
����		: Get_Ack_From_Slave
����ֵ	: Get the Ack or lose
********************************************************************************
*/
bool WaitAck(void)
{
	int i = 0;			//WaitAck
	EM_SCL_L();
	Delay(200);
	EM_SDA_H();   		/*8λ��������ͷ������ߣ�׼������Ӧ��λ*/
	Delay(200);
//Ӧ�� SCL�ڸߵ�ƽ�ڼ䣬SDA�����豸��Ϊ�͵�ƽ��ʾӦ��
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
	} else				/*�ж��Ƿ���յ�Ӧ���ź� (ack == 0) <ACK>*/
	{
		Delay(200);
		EM_SCL_L();  
		Delay(200);

		return true;
	}
}

/******************************************************************************
			 Ӧ���Ӻ���
����ԭ��:  void Ack_I2c(bit a);
����:      ����������Ӧ���ź�(������Ӧ����Ӧ���źţ���λ����a����)
******************************************************************************/
/******************************************************************************
������I2C_PutAck()
���ܣ���������Ӧ��λ��Ӧ����Ӧ�𣩣�����֪ͨ�ӻ��������Ƿ�ɹ����մӻ�����
������Ack = 0������Ӧ��
Ack = 1��������Ӧ��
˵�����������յ�ÿһ���ӻ��ֽں�Ҫ��ӻ�����Ӧ�����յ����һ���ֽ�ʱ��Ӧ��������Ӧ��֪ͨ�ӻ���Ҫ��Ӧ��
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
		EM_SDA_L();		/*�ڴ˷���Ӧ����Ӧ���ź� *///I2C_Ack()
	}
	Delay(200);
	EM_SCL_H();
	Delay(200);			/*ʱ�ӵ͵�ƽ���ڴ���4��s*/
	EM_SCL_L();			/*��ʱ���ߣ�ǯסI2C�����Ա��������*/
	Delay(200);
}
/*
********************************************************************************
		 �ֽ����ݷ��ͺ���               
����ԭ��: u8 I2C_LLV_SendByte(u8 SendByte);
����:     ������c���ͳ�ȥ,�����ǵ�ַ,Ҳ����������,�����ȴ�Ӧ��,����
  ��״̬λ���в���.(��Ӧ����Ӧ��ʹack=0)     
  ��������������ack=1; ack=0��ʾ��������Ӧ����𻵡�
********************************************************************************
*/
void SendByte(uint8_t chData)
{
	uint8_t i = 0;
	for(i=0; i < 8; i++)
	{
		///����SCL����Ϊֻ����ʱ���ź�Ϊ�͵�ƽ�ڼ䣬�������ϵĸߵ͵�ƽ״̬
		///������仯;���ڴ�ʱ����һ��ѭ����SDA_Hһ���γ�һ��������
		EM_SCL_L();
		Delay(200);
		if(chData&0x80)	/*�жϷ���λ*/
		{
			EM_SDA_H(); 
		}       
		else 
		{
			EM_SDA_L();
		}    
		chData<<=1;
		Delay(200);
		EM_SCL_H();		/*��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ*/
		Delay(200);		/*��֤ʱ�Ӹߵ�ƽ���ڴ���4��s*/
	}
	EM_SCL_L();
}
/*
********************************************************************************
		 �ֽ����ݽ��պ���               
����ԭ��: u8 I2C_LLV_ReceiveByte(void);
����:     �������մ���������������,���ж����ߴ���(����Ӧ���ź�)��
  ���������Ӧ����Ӧ��ӻ���  
********************************************************************************
*/
void RecvByte(uint8_t *pData)
{
	uint8_t i;
	*pData = 0;
	//���ݴӸ�λ����λ
	//EM_SCL_L();  
	//Delay();
	EM_SDA_H();  				/*��������Ϊ���뷽ʽ*/ 
	Delay(200);
	for(i=0; i < 8; i++)
	{
		(*pData)<<=1;
		EM_SCL_L();  
		Delay(200);
		EM_SCL_H();				//������ʱ��IIC�豸�����ݷ���sda���ϣ�
								//���ڸߵ�ƽ�ڼ������Ѿ��ȶ������Խ�����  
								/*��ʱ����Ϊ��ʹ��������������Ч*/		
		Delay(200);
//				if (I2CIO.ReadInputDataBit(BSP_I2C_SDA))
		if (GPIO_ReadInputDataBit(BSP_I2C_PORT,BSP_I2C_SDA))
		{
			(*pData)|=0x01;
								//������λ,���յ�����λ����pdata��
		}
		//EM_SCL_L();			//����SCL��ʹ���Ͷ˿��԰����ݷ���SDA��   
								/*��ʱ����Ϊ�ͣ�׼����������λ*/
		//Delay(200);			/*ʱ�ӵ͵�ƽ���ڴ���4.7��s*/
	}
	EM_SCL_L();
}

/**
  * @brief  Description "ģ��I2Cд����"
  * @param  pdata  		д������ݻ�����ָ��
  * @param  addr		д��������ڴ��ַ
  * @param  count		д����������ݴ�С
  * @retval bool		I2Cд�����Ƿ�ɹ�
  */
bool AI2C_Write(uint8_t *pData, uint16_t addr, uint16_t count)
{
    
	uint8_t		device_addr = 0;
	uint8_t 	addr_msb = 0, addr_lsb = 0;
    uint16_t 	i;
	uAddress = AI2C_FRAM_ADDRESS;
//    CAI2C		theAI2C(0xA0);
	/*            �ӻ�ID   + ����ѡ�� + ��дѡ��  */
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
  * @brief  Description "ģ��I2C������"
  * @param  pdata  		��ȡ�����ݻ�����ָ��
  * @param  addr		��ȡ�������ڴ��ַ
  * @param  count		��ȡ���������ݴ�С
  * @retval bool		I2C�������Ƿ�ɹ�
  */
bool AI2C_Read(uint8_t *pData, uint16_t addr, uint16_t count)
{
	uint8_t		device_addr = 0;
	uint8_t 	addr_msb = 0, addr_lsb = 0;
    uint16_t 	i;
	uAddress = AI2C_FRAM_ADDRESS;
//    CAI2C		theAI2C(0xA0);
	/*            �ӻ�ID   + ����ѡ�� + ��дѡ��  */
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
    Ack(true);//���ս���,���ͷ�Ӧ���ź�
    Stop();
	return true;
}

/**
  * @brief  Description "ģ��I2CдRTC8025����"
  * @param  pdata  		д������ݻ�����ָ��
  * @param  addr		д��������ڴ��ַ
  * @param  count		д����������ݴ�С
  * @retval bool		RTC8025д�����Ƿ�ɹ�
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
  * @brief  Description "ģ��I2C��RTC8025����"
  * @param  pdata  		��ȡ�����ݻ�����ָ��
  * @param  addr		��ȡ�������ڴ��ַ
  * @param  count		��ȡ���������ݴ�С
  * @retval bool		RTC8025�������Ƿ�ɹ�
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
    Ack(true);//���ս���,���ͷ�Ӧ���ź�
	//! STOP condition sent by Master
    Stop();
	return true;
}

/**
  * @brief  Description "ģ��I2Cд����"
  * @param  pdata  		д������ݻ�����ָ��
  * @param  addr		д��������ڴ��ַ
  * @param  count		д����������ݴ�С
  * @retval bool		I2Cд�����Ƿ�ɹ�
  */
bool Fram_Write(uint8_t *pData, uint16_t addr, uint16_t count)
{
    
	uint8_t		device_addr = 0;
	uint8_t 	addr_msb = 0, addr_lsb = 0;
    uint16_t 	i;
	uAddress = AI2C_FRAM_ADDRESS;
//    CAI2C		theAI2C(0xA0);
	/*            �ӻ�ID   + ����ѡ�� + ��дѡ��  */
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
  * @brief  Description "ģ��I2C������"
  * @param  pdata  		��ȡ�����ݻ�����ָ��
  * @param  addr		��ȡ�������ڴ��ַ
  * @param  count		��ȡ���������ݴ�С
  * @retval bool		I2C�������Ƿ�ɹ�
  */
bool Fram_Read(uint8_t *pData, uint16_t addr, uint16_t count)
{
	uint8_t		device_addr = 0;
	uint8_t 	addr_msb = 0, addr_lsb = 0;
    uint16_t 	i;
	uAddress = AI2C_FRAM_ADDRESS;
//    CAI2C		theAI2C(0xA0);
	/*            �ӻ�ID   + ����ѡ�� + ��дѡ��  */
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
    Ack(true);//���ս���,���ͷ�Ӧ���ź�
    Stop();
	return true;
}
/**
  * @brief  Description "RTC8025״̬λ��λ����"
  * @param  None
  * @retval bool		RTC8025�������Ƿ�ɹ�
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
		/* ���� RTC8025 �̶������жϹ��� */
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
