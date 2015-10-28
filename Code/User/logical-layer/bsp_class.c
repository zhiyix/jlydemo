/**
  ******************************************************************************
  * @file              : bsp_class.c
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
#ifndef __BSPCLASS_C
#define __BSPCLASS_C

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
  * @brief  Description charת��Ϊint
  * @param  high        ��λ
  * @param  low         ��λ
  * @retval temp        int		
  *****************************************************************************/
unsigned int  Char_to_Int(unsigned int high,unsigned int low)
{
    unsigned int temp ;
    temp =  high*256 + low;
    return temp;
}
/******************************************************************************
  * @brief  Description ������תU16
  * @param  FtTmp  		ת���ĸ�����  	
  * @retval UtTmp       ת����U16		
  *****************************************************************************/
uint16_t ValueFtToU16(float FtTmp)
{
    uint16_t UtTmp;
    if(FtTmp<0)
    {
        FtTmp=FtTmp*(-100.0);
        UtTmp=(uint16_t)FtTmp;
        if(UtTmp%10>=4)
            UtTmp=UtTmp+10;
        UtTmp=UtTmp/10;
        UtTmp=~UtTmp+1;
    }
    else
    {
        FtTmp=FtTmp*(100.0);
        UtTmp=(uint16_t)FtTmp;
        if(UtTmp%10>=4)
            UtTmp=UtTmp+10;
        UtTmp=UtTmp/10;
    }
    return UtTmp;
}
/******************************************************************************
  * @brief  Description U16ת10
  * @param  high  		U16��λ
  * @param  low         U16��λ
  * @retval Temp
  *****************************************************************************/
uint16_t U16ToValue10(uint8_t high,uint8_t low)
{
    uint16_t Temp;
    Temp= (high<<8)+low;
    return Temp;
}
/******************************************************************************
  * @brief  Description HEX->ASCII
  * @param  data        Դ����ָ��		
  * @param  buffer		Ŀ������ָ��
  * @param  len	        ת�������ݳ���
  * @retval pos		    ת�������ݳ���
  *****************************************************************************/
int hex_2_ascii(uint8_t *data, uint8_t *buffer, uint16_t len) 
{ 
    int i, pos; 
    const unsigned char ascTable[17] = {"0123456789ABCDEF"}; 
    unsigned char *tmp_p = buffer; 
    pos = 0; 
    for(i = 0; i < len; i++) 
    { 
        tmp_p[pos++] = ascTable[data[i] >> 4]; 
        tmp_p[pos++] = ascTable[data[i] & 0x0f]; 
    } 
    tmp_p[pos] = '\0'; 
    return pos; 
}
/******************************************************************************
  * @brief  Description ��ʱ����ʱ��ת��Ϊ����
  * @param  hour  		
  * @param  min         
  * @param  sec         
  * @retval ����
  *****************************************************************************/
uint32_t Change3BytesTimeToLong(uint8_t hour,uint8_t min,uint8_t sec)
{
    uint32_t save_time;
    save_time = hour*36;
    save_time = save_time*100;
    save_time = save_time+min*60+sec;
    return save_time;
}
/******************************************************************************
  * @brief  Description ��¼���ʱ��תΪ����
  * @param  hour  		
  * @param  min         
  * @param  sec         
  * @retval ����
  *****************************************************************************/
uint32_t GetRecIntervalTime(uint8_t day,uint8_t hour,uint8_t min,uint8_t sec)
{
	uint8_t temp;
	uint8_t buf[4]={0};
	uint32_t save_time;
	temp = day*24 + hour;//��¼�������һ��
	save_time=Change3BytesTimeToLong(temp,min,sec);
	if(save_time <= 0)
	{
		//save_time = 600;
		buf[0] = 0;
		buf[1] = 10;
		buf[2] = 0;
		buf[3] = 0;
		/*д�롢�֡�ʱ���쵽Fram�� ,10����*/
		Fram_Write(buf,FRAM_JlyTimeConfAddr,4);
	}
	return save_time;
}
#endif /* __BSPCLASS_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
