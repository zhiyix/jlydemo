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

/**
  * @brief  Description char转换为int
  * @param  high        高位
  * @param  low         低位
  * @retval temp        int		
  */
unsigned int  Char_to_Int(unsigned int high,unsigned int low)
{
    unsigned int temp ;
    temp =  high*256 + low;
    return temp;
}
/**
  * @brief  Description 浮点数转U16
  * @param  FtTmp  		转换的浮点数  	
  * @retval UtTmp       转换后U16		
  */
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
/**
  * @brief  Description U16转10
  * @param  high  		U16高位
  * @param  low         U16低位
  * @retval Temp
  */
uint16_t U16ToValue10(uint8_t high,uint8_t low)
{
    uint16_t Temp;
    Temp= (high<<8)+low;
    return Temp;
}
/**
  * @brief  Description HEX->ASCII
  * @param  data        源数据指针		
  * @param  buffer		目标数据指针
  * @param  len	        转换的数据长度
  * @retval pos		    转换后数据长度
  */
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

#endif /* __BSPCLASS_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
