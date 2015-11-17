/**
  ******************************************************************************
  * @file              : 
  * @author            : 
  * @version           : V1.0.
  * @date              : 
  * @brief             : h file
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
#ifndef __LCD_H
#define	__LCD_H

//#ifdef __cplusplus
// extern "C" {
//#endif

/*============================ INCLUDES ======================================*/
//! @name include
//! @{
#include "stm32l1xx.h"

//! @}

/*============================ MACROS ========================================*/
//! @{
//! \brief  
//!
/*
 *LCD显示位定义
 */
#define D_ZERO		0x00000000
#define D_BIT1 		0x00000001
#define D_BIT2 		0x00000002
#define D_BIT3 		0x00000004
#define D_BIT4 		0x00000008
#define D_BIT5 		0x00000010
#define D_BIT6 		0x00000020
#define D_BIT7 		0x00000040
#define D_BIT8 		0x00000080
#define D_BIT9 		0x00000100
#define D_BIT10 	0x00000200
#define D_BIT11 	0x00000400
#define D_BIT12 	0x00000800
#define D_BIT13 	0x00001000
#define D_BIT14 	0x00002000
#define D_BIT15 	0x00004000
#define D_BIT16 	0x00008000
#define D_BIT17 	0x00010000
#define D_BIT18 	0x00020000
#define D_BIT19 	0x00040000
#define D_BIT20 	0x00080000
#define D_BIT21 	0x00100000
#define D_BIT22 	0x00200000
#define D_BIT23 	0x00400000
#define D_BIT24 	0x00800000
#define D_BIT25 	0x01000000
#define D_BIT26 	0x02000000
#define D_BIT27 	0x04000000
#define D_BIT28 	0x08000000
#define D_BIT29 	0x10000000
#define D_BIT30 	0x20000000
#define D_BIT31 	0x40000000
#define D_BIT32 	0x80000000
//!
//LCD 信号显示
#define clearS12345	LCD->RAM[0]&=~(D_BIT1+D_BIT2+D_BIT4+D_BIT6);LCD->RAM[2]&=~D_BIT1
#define showS1      clearS12345;LCD->RAM[0]|=D_BIT1
#define showS12     clearS12345;LCD->RAM[0]|=D_BIT1;LCD->RAM[2]|=D_BIT1
#define showS123    clearS12345;LCD->RAM[0]|=(D_BIT1+D_BIT2);LCD->RAM[2]|=D_BIT1
#define showS1234   clearS12345;LCD->RAM[0]|=(D_BIT1+D_BIT2+D_BIT4);LCD->RAM[2]|=D_BIT1
#define showS12345  LCD->RAM[0]|=(D_BIT1+D_BIT2+D_BIT4+D_BIT6);LCD->RAM[2]|=D_BIT1

#define show_NFC		LCD->RAM[0]|=D_BIT8
#define clear_NFC		LCD->RAM[0]&=~D_BIT8
//! \brief 保温箱 s6 s7 s8
#define show_BOX_open	LCD->RAM[0]|=D_BIT10;LCD->RAM[2]&=~D_BIT10;LCD->RAM[4]|=D_BIT10
#define show_BOX_close  LCD->RAM[0]&=~D_BIT10;LCD->RAM[2]|=D_BIT10;LCD->RAM[4]|=D_BIT10
#define clear_BOX		LCD->RAM[0]&=~D_BIT10;LCD->RAM[2]&=~D_BIT10;LCD->RAM[4]&=~D_BIT10
//! \brief k1 s9
#define show_GPRS     LCD->RAM[0]|=D_BIT11;LCD->RAM[2]|=D_BIT11		/*!< E表示 GPRS符号 */
#define clear_GPRS    LCD->RAM[0]&=~D_BIT11;LCD->RAM[2]&=~D_BIT11
//! \brief k2 s10
#define show_GPS      LCD->RAM[4]|=D_BIT11;LCD->RAM[6]|=D_BIT11	/*!< GPS符号 */
#define clear_GPS     LCD->RAM[4]&=~D_BIT11;LCD->RAM[6]&=~D_BIT11

//! \brief  电池符号显示
#define clearBATT   LCD->RAM[6]&=~D_BIT26;LCD->RAM[4]&=~D_BIT26;LCD->RAM[2]&=~D_BIT26;LCD->RAM[0]&=~D_BIT26
#define showBATT0   clearBATT;LCD->RAM[6]|=D_BIT26   //空格,show N1
#define showBATT1   clearBATT;LCD->RAM[6]|=D_BIT26;LCD->RAM[4]|=D_BIT26  //1格,show N1 N2
#define showBATT2   clearBATT;LCD->RAM[6]|=D_BIT26;LCD->RAM[4]|=D_BIT26;LCD->RAM[2]|=D_BIT26 //2格,show N1 N2 N3
#define showBATT    LCD->RAM[6]|=D_BIT26;LCD->RAM[4]|=D_BIT26;LCD->RAM[2]|=D_BIT26;LCD->RAM[0]|=D_BIT26 //满格显示,show N1 N2 N3 N4
//! \brief 显示Flash 存储 N5 N6 N7 N8 N9 N10
#define clearFlashMEM	LCD->RAM[0]&=~(D_BIT22+D_BIT24);LCD->RAM[2]&=~(D_BIT22+D_BIT24);LCD->RAM[4]&=~(D_BIT22+D_BIT24)
#define showFlashMEM1   clearFlashMEM;LCD->RAM[0]|=(D_BIT22+D_BIT24)
#define showFlashMEM2   clearFlashMEM;LCD->RAM[0]|=(D_BIT22+D_BIT24);LCD->RAM[2]|=D_BIT24
#define showFlashMEM3   clearFlashMEM;LCD->RAM[0]|=(D_BIT22+D_BIT24);LCD->RAM[2]|=D_BIT24;LCD->RAM[4]|=D_BIT24
#define showFlashMEM4   clearFlashMEM;LCD->RAM[0]|=(D_BIT22+D_BIT24);LCD->RAM[2]|=D_BIT24;LCD->RAM[4]|=(D_BIT22+D_BIT24)
#define showFlashMEM5   LCD->RAM[0]|=(D_BIT22+D_BIT24);LCD->RAM[2]|=(D_BIT22+D_BIT24);LCD->RAM[4]|=(D_BIT22+D_BIT24)

#define showtongdao	 LCD->RAM[4]|=D_BIT1   
#define cleartongdao LCD->RAM[4]&=~D_BIT1    
//! \brief  显示报警符号 s11
#define showJINBAO	LCD->RAM[6]|=D_BIT30
#define clearJINBAO LCD->RAM[6]&=~D_BIT30
//! \brief  显示报警状态 
#define showAlarmStatus   LCD->RAM[6]|=D_BIT1
#define clearAlarmStatus  LCD->RAM[6]&=~D_BIT1
//! \brief  显示 设置上限
#define showshangxian   LCD->RAM[7]|=D_BIT2
#define clearshangxian  LCD->RAM[7]&=~D_BIT2
//! \brief  显示 设置下限
#define showxiaxian     LCD->RAM[6]|=D_BIT32
#define clearxiaxian    LCD->RAM[6]&=~D_BIT32
//显示负号 s12
#define showFUHAO	LCD->RAM[6]|=D_BIT28
#define clearFUHAO	LCD->RAM[6]&=~D_BIT28
//!第一行左边小数点 S13
#define showP1		LCD->RAM[7]|=D_BIT8
#define clearP1		LCD->RAM[7]&=~D_BIT8
//!第一行右边小数点 S14
#define showP2		LCD->RAM[7]|=D_BIT6
#define clearP2		LCD->RAM[7]&=~D_BIT6
//!温度符号显示
#define showC		LCD->RAM[6]|=D_BIT22
#define clearC		LCD->RAM[6]&=~D_BIT22
//!湿度符号显示
#define showRH	    LCD->RAM[6]|=D_BIT16
#define clearRH	    LCD->RAM[6]&=~D_BIT16

//! \brief  显示ID
#define showID      LCD->RAM[6]|=D_BIT23       
#define clearID     LCD->RAM[6]&=~D_BIT23       
//! \brief  显示时间S15符号
#define showTIME    LCD->RAM[7]|=D_BIT4
#define clearTIME   LCD->RAM[7]&=~D_BIT4
//! \brief  显示日期 点S17
#define showP       LCD->RAM[6]|=D_BIT12
#define clearP      LCD->RAM[6]&=~D_BIT12
//! \brief  显示时钟 : S16
#define showCOL     LCD->RAM[6]|=D_BIT18
#define clearCOL    LCD->RAM[6]&=~D_BIT18




//! \brief  显示报警状态

//! @}


/** @defgroup STM32L152_EVAL_GLASS_LCD_Exported_Types
  * @{
  */
typedef enum
{
  POINT_OFF = 0,
  POINT_ON = 1
}Point_Typedef;

typedef enum
{
  APOSTROPHE_OFF = 0,
  APOSTROPHE_ON = 1
}Apostrophe_Typedef;   

extern uint8_t        		ChannelForDisplay; 
extern uint8_t        		StartedChannelForDisplay;
extern uint8_t        		Started_Channel;
extern volatile uint8_t 	FlagSeniorErr[32];
extern const uint32_t 		lcd_test[];
/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/

/*============================ EXTERN FUNCTIONS ==============================*/

void LCD_GLASS_Init(void);
void LCD_GLASS_Clear(void);
//void LCD_GLASS_DisplayString(uint8_t* ptr);
void LCD_GLASS_DisplayChar(uint8_t ch,uint8_t position);
//void LCD_GLASS_ScrollString(uint8_t* ptr, uint16_t nScroll, uint16_t ScrollSpeed);
void LCD_GLASS_WriteChar(uint8_t ch, uint8_t position);
void LCD_GLASS_ClearChar(uint8_t position);


//void Lcd_Test(void);
void Display_Signal(uint8_t signal_value);
void Display_Mem(void);
void Display_SN(void);
void Display_NUL(void);
void Display_LOW(void);
void displayErr(uint8_t Er);
void lcd_OFF(uint8_t offcode);
void displayYEAR(unsigned char year);
void displayDAY(uint8_t month,uint8_t day);
void displayTIME(uint8_t hour,uint8_t min);
void Display_ChannelValue(uint8_t started_channel0);
void FisrtPowerOnDisplay(void);
//#ifdef __cplusplus
//}
//#endif

#endif /* __LCD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
