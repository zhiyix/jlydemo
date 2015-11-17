/**
  ******************************************************************************
  * @file    bsp_lcd.c
  * @author  
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   lcd应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:
  *
  ******************************************************************************
  */

#include "main.h"

/**
  * @brief LETTERS AND NUMBERS MAPPING DEFINITION
  */
uint32_t digit;     /* Digit LCD RAM buffer */
__I uint16_t mask[4] = {0xF000, 0x0F00, 0x00F0, 0x000F};
__I uint8_t shift[4] = {0x0C, 0x08, 0x04, 0x00};
  

__I uint16_t NumberMap1[10]= {0x77,0x12,0x6b,0x5b,0x1e,0x5d,0x7d,0x13,0x7f,0x5f};	
/**
  * @}
  */ 
  
/** @defgroup STM32L152_LCD_Private_Function_Prototypes
  * @{
  */ 
static void LCD_GPIOConfig(void);
static void Convert(uint8_t c);
static void Lcd_Dis1Value(uint8_t value);
static void Lcd_Dis2Value(uint8_t value);
static void Lcd_Dis3Value(uint8_t value);
static void Lcd_Dis4Value(uint8_t value);
static void Lcd_Dis5Value(uint8_t value);
static void Lcd_Dis6Value(uint8_t value);
static void Lcd_Dis7Value(uint8_t value);
static void Lcd_Dis8Value(uint8_t value);
static void Lcd_Dis9Value(uint8_t value);
static void Lcd_Dis10Value(uint8_t value);
static void Lcd_Dis11Value(uint8_t value);
static void Lcd_Dis12Value(uint8_t value);
static void Lcd_Dis13Value(uint8_t value);
static void Lcd_Dis14Value(uint8_t value);
static void Lcd_Dis15Value(uint8_t value);

/*******************************************************************************
  * @brief  Configures the LCD Segments and Coms GPIOs.
  * @caller 
  * @param None
  * @retval None
  ******************************************************************************/
static void LCD_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
	/*!< Enable GPIOA, GPIOB, GPIOC, GPIOD and GPIOE AHB Clocks */	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
						  RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE , ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_LCD);	/*!< Connect PA.08 to LCD COM0 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_LCD);	/*!< Connect PA.09 to LCD COM1 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_LCD);	/*!< Connect PA.10 to LCD COM2 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_LCD);	/*!< Connect PB.09 to LCD COM3 */
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_LCD);	/*!< Connect PA.01 to LCD SEG0 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_LCD);	/*!< Connect PA.02 to LCD SEG1 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_LCD);	/*!< Connect PA.03 to LCD SEG2 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_LCD);	/*!< Connect PA.06 to LCD SEG3 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_LCD);	/*!< Connect PA.07 to LCD SEG4 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_LCD);	/*!< Connect PB.00 to LCD SEG5 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_LCD);	/*!< Connect PB.01 to LCD SEG6 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_LCD);	/*!< Connect PB.03 to LCD SEG7 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_LCD);	/*!< Connect PB.04 to LCD SEG8 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_LCD);	/*!< Connect PB.05 to LCD SEG9 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_LCD);	/*!< Connect PB.10 to LCD SEG10 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_LCD);	/*!< Connect PB.11 to LCD SEG11 */	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_LCD);	/*!< Connect PB.12 to LCD SEG12 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_LCD);	/*!< Connect PB.13 to LCD SEG13 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_LCD);	/*!< Connect PB.14 to LCD SEG14 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_LCD);	/*!< Connect PB.15 to LCD SEG15 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_LCD);	/*!< Connect PB.08 to LCD SEG16 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_LCD);	/*!< Connect PA.15 to LCD SEG17 */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource0, GPIO_AF_LCD);	/*!< Connect PC.00 to LCD SEG18 */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_LCD);	/*!< Connect PC.01 to LCD SEG19 */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_LCD);	/*!< Connect PC.02 to LCD SEG20 */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_LCD);	/*!< Connect PC.03 to LCD SEG21 */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_LCD);	/*!< Connect PC.04 to LCD SEG22 */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_LCD);	/*!< Connect PC.05 to LCD SEG23 */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_LCD);	/*!< Connect PC.06 to LCD SEG24 */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_LCD);	/*!< Connect PC.07 to LCD SEG25 */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_LCD);	/*!< Connect PC.08 to LCD SEG26 */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_LCD);	/*!< Connect PC.09 to LCD SEG27 */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_LCD);	/*!< Connect PD.08 to LCD SEG28 */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_LCD);	/*!< Connect PD.09 to LCD SEG29 */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_LCD);	/*!< Connect PD.10 to LCD SEG30 */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_LCD);	/*!< Connect PD.11 to LCD SEG31 */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_LCD);	/*!< Connect PD.12 to LCD SEG32 */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_LCD);	/*!< Connect PD.13 to LCD SEG33 */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_LCD);	/*!< Connect PD.14 to LCD SEG34 */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_LCD);	/*!< Connect PD.15 to LCD SEG35 */
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_LCD);	/*!< Connect PE.00 to LCD SEG36 */
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_LCD);	/*!< Connect PE.01 to LCD SEG37 */
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_LCD);	/*!< Connect PE.02 to LCD SEG38 */
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource3, GPIO_AF_LCD);	/*!< Connect PE.03 to LCD SEG39 */
	
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_LCD);	/*!< Connect PC.10 to LCD SEG40 */
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_LCD);	/*!< Connect PC.11 to LCD SEG41 */
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_LCD);	/*!< Connect PC.12 to LCD SEG42 */
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_LCD);	/*!< Connect PD.02 to LCD SEG43 */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | \
                                  GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9 | \
                                  GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | \
                                  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | \
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | \
                                  GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/*******************************************************************************
  * @brief  Configures the lcd.
  * @param  None
  * @retval None
  ******************************************************************************/
void LCD_GLASS_Init(void)
{
	LCD_InitTypeDef LCD_InitStructure;
	
//	RCC_LSICmd(ENABLE);
//	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
//	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	
	LCD_GPIOConfig();/*!< Configure the LCD Glass GPIO pins */
	
    LCD_StructInit(&LCD_InitStructure);
	/*!< Configure the LCD interface -------------------------------------------*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_LCD, ENABLE);/*!< Enable LCD APB1 Clock */

	LCD_InitStructure.LCD_Prescaler = LCD_Prescaler_8;
	LCD_InitStructure.LCD_Divider = LCD_Divider_31;
	LCD_InitStructure.LCD_Duty = LCD_Duty_1_4;
	LCD_InitStructure.LCD_Bias = LCD_Bias_1_3;/*选择了COM0~COM3，偏压选LCD_Bias_1_3*/
	LCD_InitStructure.LCD_VoltageSource = LCD_VoltageSource_Internal;
	LCD_Init(&LCD_InitStructure);

	/*!< Configure the Pulse On Duration */
	LCD_PulseOnDurationConfig(LCD_PulseOnDuration_1); //配置LCD脉冲持续时间 LCD_PulseOnDuration_0
  
	/*!< Configure the LCD Contrast (3.25V) */
	LCD_ContrastConfig(LCD_Contrast_Level_7); //配置LCD对比度 LCD_Contrast_Level_7

	/*!< Wait Until the LCD FCR register is synchronized */
	LCD_WaitForSynchro();
  
	/*!< Enable LCD peripheral */
	LCD_Cmd(ENABLE);
  
	/*!< Wait Until the LCD is enabled */
	while(LCD_GetFlagStatus(LCD_FLAG_ENS) == RESET)
	{
	}
	/*!< Wait Until the LCD Booster is ready */  
	while(LCD_GetFlagStatus(LCD_FLAG_RDY) == RESET)
	{
	}    
}
/*******************************************************************************
  * @brief  This function Clear the whole LCD RAM.
  * @param  None
  * @retval None
  ******************************************************************************/
void LCD_GLASS_Clear(void)
{
  uint8_t counter = 0;

  /* TO wait LCD Ready */  
  while( LCD_GetFlagStatus (LCD_FLAG_UDR) != RESET) ;
  
  for (counter = LCD_RAMRegister_0; counter <= LCD_RAMRegister_7; counter++)
  {
    LCD->RAM[counter] = 0;
  }

  /* Update the LCD display */
  LCD_UpdateDisplayRequest();
  
}

/*******************************************************************************
  * @brief  This function writes a char in the LCD RAM.
  * @param  ptr: Pointer to string to display on the LCD Glass.
  * @retval None
  ******************************************************************************/
void LCD_GLASS_DisplayChar(uint8_t ch,uint8_t position)
{
  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
  /*!< LCD Write Char */
  LCD_GLASS_WriteChar(ch,position);
  /*!< Requesy LCD RAM update */
  LCD_UpdateDisplayRequest();  
}

/**
  * @brief  This function writes a char in the LCD RAM.
  * @param  ch: The character to dispaly.
  * @param  point: A point to add in front of char.
  *          This parameter  can be one of the following values:  
  *              @arg POINT_OFF : No point to add in front of char.
  *              @arg POINT_ON : Add a point in front of char.
  * @param  apostrophe: Flag indicating if a apostrophe has to be add in front 
  *                     of displayed character.
  *          This parameter  can be one of the following values:
  *              @arg APOSTROPHE_OFF : No apostrophe to add in back of char.
  *              @arg APOSTROPHE_ON : Add an apostrophe in back of char.
  * @param  position: Position in the LCD of the caracter to write.
  *          This parameter can be any value in range [0:7].
  * @retval None
  */
void LCD_GLASS_WriteChar(uint8_t ch, uint8_t position)
{ 
	Convert(ch); /*!< Convert the corresponding character */

	switch (position)
	{     
		case 0:
		/*!< Clear the corresponding segments (SEG24--SEG31) */
		LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0x00);
		LCD->RAM[LCD_RAMRegister_1] &= (uint32_t)(0x00);
		/*!< Write the corresponding segments (SEG24--SEG31) */
		LCD->RAM[LCD_RAMRegister_0] = 0xFFFFFFFF;//|= digit;
		LCD->RAM[LCD_RAMRegister_1] = 0x000000FF;
		break;

		case 1:
		/*!< Clear the corresponding segments (SEG24--SEG31) */
		LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0x00);
		LCD->RAM[LCD_RAMRegister_3] &= (uint32_t)(0x00);
		/*!< Write the corresponding segments (SEG24--SEG31) */
		LCD->RAM[LCD_RAMRegister_2] = 0xFFFFFFFF;
		LCD->RAM[LCD_RAMRegister_3] = 0x000000FF;
		break;

		case 2:
		/*!< Clear the corresponding segments (SEG24--SEG31) */
		LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0x00);
		LCD->RAM[LCD_RAMRegister_5] &= (uint32_t)(0x00);
		/*!< Write the corresponding segments (SEG24--SEG31) */
		LCD->RAM[LCD_RAMRegister_4] = 0xFFFFFFFF;
		LCD->RAM[LCD_RAMRegister_5] = 0x000000FF;	  
		break;

		case 3:
		/*!< Clear the corresponding segments (SEG24--SEG31) */
		LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0x00);
		LCD->RAM[LCD_RAMRegister_7] &= (uint32_t)(0x00);
		/*!< Write the corresponding segments (SEG24--SEG31) */
		LCD->RAM[LCD_RAMRegister_6] = 0xFFFFFFFF;
		LCD->RAM[LCD_RAMRegister_7] = 0x000000FF;
		break;
	}
}


void LCD_GLASS_ClearChar(uint8_t position)
{
	/*!< Wait Until the last LCD RAM update finish */
	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
	switch (position)
	{
		case 0:
		  /*!< Clear the corresponding segments (SEG24--SEG31) */
		  LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0x00);
		  LCD->RAM[LCD_RAMRegister_1] &= (uint32_t)(0x00);
		  break;
		  
		case 1:
		  /*!< Clear the corresponding segments (SEG24--SEG31) */
		  LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0x00);
		  LCD->RAM[LCD_RAMRegister_3] &= (uint32_t)(0x00);
		  break;
		  
		case 2:
		  /*!< Clear the corresponding segments (SEG24--SEG31)) */
		  LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0x00);
		  LCD->RAM[LCD_RAMRegister_5] &= (uint32_t)(0x00);
		  break;
		  
		case 3:
		  /*!< Clear the corresponding segments (SEG24--SEG31) */
		  LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0x00);
		  LCD->RAM[LCD_RAMRegister_7] &= (uint32_t)(0x00);
		  break;
	}
	/*!< Requesy LCD RAM update */
	LCD_UpdateDisplayRequest();  
}
/**
  * @brief  Converts an ascii char to an LCD digit.
  * @param  c: Char to display.
  * @param  point: A point to add in front of char.
  *          This parameter  can be one of the following values:  
  *              @arg POINT_OFF : No point to add in front of char.
  *              @arg POINT_ON : Add a point in front of char.
  * @param  apostrophe: Flag indicating if a apostrophe has to be add in front 
  *                     of displayed character.
  *          This parameter  can be one of the following values:
  *              @arg APOSTROPHE_OFF : No apostrophe to add in back of char.
  *              @arg APOSTROPHE_ON : Add an apostrophe in back of char.
  * @retval None
  */
static void Convert(uint8_t c)
{
  uint32_t ch = 0;
  /*!< The character c is a number*/
  if ((c < 0x3A) & (c > 0x2F))
  {
    ch = NumberMap1[c - 0x30];
  }
  
  /*!< The character c is a space character */
  if (c == 0x20)
  {
    ch =0x00;
  }
	digit = ch << 24;
}

/*******************************************************************************
  * @brief  液晶1显示数字,加入通道显示
  * @param  None
  * @retval None
  ******************************************************************************/
static void Lcd_Dis1Value(uint8_t value)
{
	/*!< Wait Until the last LCD RAM update finish */
//	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
	switch(value)
	{
		case 0x00:
			LCD->RAM[0] |= D_BIT3;LCD->RAM[2] |= D_BIT2+D_BIT3;LCD->RAM[4] |= D_BIT3;LCD->RAM[6] |= D_BIT2+D_BIT3;
			break;
		case 0x01:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT3;	   LCD->RAM[4] |= D_BIT3;LCD->RAM[6] |= D_ZERO;
			break;
		case 0x02:
			LCD->RAM[0] |= D_BIT3;LCD->RAM[2] |= D_BIT3;	   LCD->RAM[4] |= D_BIT2;LCD->RAM[6] |= D_BIT2+D_BIT3;
			break;
		case 0x03:
			LCD->RAM[0] |= D_BIT3;LCD->RAM[2] |= D_BIT3;	   LCD->RAM[4] |= D_BIT2+D_BIT3;LCD->RAM[6] |= D_BIT3;
			break;
		case 0x04:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT2+D_BIT3;LCD->RAM[4] |= D_BIT2+D_BIT3;LCD->RAM[6] |= D_ZERO;
			break;
		case 0x05:
			LCD->RAM[0] |= D_BIT3;LCD->RAM[2] |= D_BIT2;LCD->RAM[4] |= D_BIT2+D_BIT3;LCD->RAM[6] |= D_BIT3;
			break;
		case 0x06:
			LCD->RAM[0] |= D_BIT3;LCD->RAM[2] |= D_BIT2;LCD->RAM[4] |= D_BIT2+D_BIT3;LCD->RAM[6] |= D_BIT2+D_BIT3;
			break;
		case 0x07:
			LCD->RAM[0] |= D_BIT3;LCD->RAM[2] |= D_BIT3;	   LCD->RAM[4] |= D_BIT3;LCD->RAM[6] |= D_ZERO;
			break;
		case 0x08:
			LCD->RAM[0] |= D_BIT3;LCD->RAM[2] |= D_BIT2+D_BIT3;LCD->RAM[4] |= D_BIT2+D_BIT3;LCD->RAM[6] |= D_BIT2+D_BIT3;
			break;
		case 0x09:
			LCD->RAM[0] |= D_BIT3;LCD->RAM[2] |= D_BIT2+D_BIT3;LCD->RAM[4] |= D_BIT2+D_BIT3;LCD->RAM[6] |= D_BIT3;
			break;
		
		case 0xFF:
			LCD->RAM[0] &= ~D_BIT3;LCD->RAM[2] &= ~(D_BIT2+D_BIT3);LCD->RAM[4] &= ~(D_BIT2+D_BIT3);LCD->RAM[6] &= ~(D_BIT2+D_BIT3);
			break;
		default:
			break;
	}
	/*!< Requesy LCD RAM update */
//	LCD_UpdateDisplayRequest();  
}
/*******************************************************************************
  * @brief  液晶2显示数字
  * @param  None
  * @retval None
  ******************************************************************************/
static void Lcd_Dis2Value(uint8_t value)
{
	/*!< Wait Until the last LCD RAM update finish */
//	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
	switch(value)
	{
		case 0x00:
			LCD->RAM[0] |= D_BIT5;LCD->RAM[2] |= D_BIT4+D_BIT5;LCD->RAM[4] |= D_BIT5;LCD->RAM[6] |= D_BIT4+D_BIT5;
			break;
		case 0x01:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT5;	   LCD->RAM[4] |= D_BIT5;LCD->RAM[6] |= D_ZERO;
			break;
		case 0x02:
			LCD->RAM[0] |= D_BIT5;LCD->RAM[2] |= D_BIT5;	   LCD->RAM[4] |= D_BIT4;LCD->RAM[6] |= D_BIT4+D_BIT5;
			break;
		case 0x03:
			LCD->RAM[0] |= D_BIT5;LCD->RAM[2] |= D_BIT5;	   LCD->RAM[4] |= D_BIT4+D_BIT5;LCD->RAM[6] |= D_BIT5;
			break;
		case 0x04:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT4+D_BIT5;LCD->RAM[4] |= D_BIT4+D_BIT5;LCD->RAM[6] |= D_ZERO;
			break;
		case 0x05:
			LCD->RAM[0] |= D_BIT5;LCD->RAM[2] |= D_BIT4;LCD->RAM[4] |= D_BIT4+D_BIT5;LCD->RAM[6] |= D_BIT5;
			break;
		case 0x06:
			LCD->RAM[0] |= D_BIT5;LCD->RAM[2] |= D_BIT4;LCD->RAM[4] |= D_BIT4+D_BIT5;LCD->RAM[6] |= D_BIT4+D_BIT5;
			break;
		case 0x07:
			LCD->RAM[0] |= D_BIT5;LCD->RAM[2] |= D_BIT5;	   LCD->RAM[4] |= D_BIT5;LCD->RAM[6] |= D_ZERO;
			break;
		case 0x08:
			LCD->RAM[0] |= D_BIT5;LCD->RAM[2] |= D_BIT4+D_BIT5;LCD->RAM[4] |= D_BIT4+D_BIT5;LCD->RAM[6] |= D_BIT4+D_BIT5;
			break;
		case 0x09:
			LCD->RAM[0] |= D_BIT5;LCD->RAM[2] |= D_BIT4+D_BIT5;LCD->RAM[4] |= D_BIT4+D_BIT5;LCD->RAM[6] |= D_BIT5;
			break;
		case '-':
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_ZERO;LCD->RAM[4] |= D_BIT4;LCD->RAM[6] |= D_ZERO;
			break;
		case 'n':
			LCD->RAM[0] |= D_BIT5;LCD->RAM[2] |= D_BIT4+D_BIT5;LCD->RAM[4] |= D_BIT5;LCD->RAM[6] |= D_BIT4;
			break;
		case 'L':
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT4;LCD->RAM[4] |= D_ZERO;LCD->RAM[6] |= D_BIT4+D_BIT5;
			break;
		
		case 0xFF:
			LCD->RAM[0] &= ~D_BIT5;LCD->RAM[2] &= ~(D_BIT4+D_BIT5);LCD->RAM[4] &= ~(D_BIT4+D_BIT5);LCD->RAM[6] &= ~(D_BIT4+D_BIT5);
			break;
		default:
			break;
	}
	/*!< Requesy LCD RAM update */
//	LCD_UpdateDisplayRequest();  
}
/*******************************************************************************
  * @brief  液晶3显示数字
  * @param  None
  * @retval None
  ******************************************************************************/
static void Lcd_Dis3Value(uint8_t value)
{
	/*!< Wait Until the last LCD RAM update finish */
//	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
	switch(value)
	{
		case 0x00:
			LCD->RAM[0] |= D_BIT7;LCD->RAM[2] |= D_BIT6+D_BIT7;LCD->RAM[4] |= D_BIT7;LCD->RAM[6] |= D_BIT6+D_BIT7;
			break;
        case 0x01:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT7;LCD->RAM[4] |= D_BIT7;LCD->RAM[6] |= D_ZERO;
			break;
        case 0x02:
			LCD->RAM[0] |= D_BIT7;LCD->RAM[2] |= D_BIT7;LCD->RAM[4] |= D_BIT6;LCD->RAM[6] |= D_BIT6+D_BIT7;
			break;   
        case 0x03:
			LCD->RAM[0] |= D_BIT7;LCD->RAM[2] |= D_BIT7;LCD->RAM[4] |= D_BIT6+D_BIT7;LCD->RAM[6] |= D_BIT7;
			break;
        case 0x04:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT6+D_BIT7;LCD->RAM[4] |= D_BIT6+D_BIT7;LCD->RAM[6] |= D_ZERO;
			break;
        case 0x05:
			LCD->RAM[0] |= D_BIT7;LCD->RAM[2] |= D_BIT6;LCD->RAM[4] |= D_BIT6+D_BIT7;LCD->RAM[6] |= D_BIT7;
			break;  
        case 0x06:
			LCD->RAM[0] |= D_BIT7;LCD->RAM[2] |= D_BIT6;LCD->RAM[4] |= D_BIT6+D_BIT7;LCD->RAM[6] |= D_BIT6+D_BIT7;
			break; 
        case 0x07:
			LCD->RAM[0] |= D_BIT7;LCD->RAM[2] |= D_BIT7;LCD->RAM[4] |= D_BIT7;LCD->RAM[6] |= D_ZERO;
			break;  
        case 0x08:
			LCD->RAM[0] |= D_BIT7;LCD->RAM[2] |= D_BIT6+D_BIT7;LCD->RAM[4] |= D_BIT6+D_BIT7;LCD->RAM[6] |= D_BIT6+D_BIT7;
			break;  
        case 0x09:
			LCD->RAM[0] |= D_BIT7;LCD->RAM[2] |= D_BIT6+D_BIT7;LCD->RAM[4] |= D_BIT6+D_BIT7;LCD->RAM[6] |= D_BIT7;
			break; 
		case 'E':
			LCD->RAM[0] |= D_BIT7;LCD->RAM[2] |= D_BIT6;LCD->RAM[4] |= D_BIT6;LCD->RAM[6] |= D_BIT6+D_BIT7;
			break;
		case 'F':
			LCD->RAM[0] |= D_BIT7;LCD->RAM[2] |= D_BIT6;LCD->RAM[4] |= D_BIT6;LCD->RAM[6] |= D_BIT6;
			break;
		case 'O':
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_ZERO;LCD->RAM[4] |= D_BIT6+D_BIT7;LCD->RAM[6] |= D_BIT6+D_BIT7;
			break; 
		case '-':
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_ZERO;LCD->RAM[4] |= D_BIT6;LCD->RAM[6] |= D_ZERO;
			break;
		case 'U':
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT6+D_BIT7;LCD->RAM[4] |= D_BIT7;LCD->RAM[6] |= D_BIT6+D_BIT7;
			break;
		
        case 0xFF:
            LCD->RAM[0] &= ~D_BIT7;LCD->RAM[2] &= ~(D_BIT6+D_BIT7);LCD->RAM[4] &= ~(D_BIT6+D_BIT7);LCD->RAM[6] &= ~(D_BIT6+D_BIT7);
            break;
		default:
			break;
	}
	/*!< Requesy LCD RAM update */
//	LCD_UpdateDisplayRequest();  
}

/*******************************************************************************
  * @brief  液晶4显示数字
  * @param  None
  * @retval None
  ******************************************************************************/
static void Lcd_Dis4Value(uint8_t value)
{
    /*!< Wait Until the last LCD RAM update finish */
//	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);    
	switch(value)
	{
        case 0x00:
			LCD->RAM[0] |= D_BIT9;LCD->RAM[2] |= D_BIT8+D_BIT9;LCD->RAM[4] |= D_BIT9;LCD->RAM[6] |= D_BIT8+D_BIT9;
			break;
        case 0x01:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT9;LCD->RAM[4] |= D_BIT9;LCD->RAM[6] |= D_ZERO;
			break;
        case 0x02:
			LCD->RAM[0] |= D_BIT9;LCD->RAM[2] |= D_BIT9;LCD->RAM[4] |= D_BIT8;LCD->RAM[6] |= D_BIT8+D_BIT9;
			break;   
        case 0x03:
			LCD->RAM[0] |= D_BIT9;LCD->RAM[2] |= D_BIT9;LCD->RAM[4] |= D_BIT8+D_BIT9;LCD->RAM[6] |= D_BIT9;
			break;
        case 0x04:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT8+D_BIT9;LCD->RAM[4] |= D_BIT8+D_BIT9;LCD->RAM[6] |= D_ZERO;
			break;
        case 0x05:
			LCD->RAM[0] |= D_BIT9;LCD->RAM[2] |= D_BIT8;LCD->RAM[4] |= D_BIT8+D_BIT9;LCD->RAM[6] |= D_BIT9;
			break;  
        case 0x06:
			LCD->RAM[0] |= D_BIT9;LCD->RAM[2] |= D_BIT8;LCD->RAM[4] |= D_BIT8+D_BIT9;LCD->RAM[6] |= D_BIT8+D_BIT9;
			break; 
        case 0x07:
			LCD->RAM[0] |= D_BIT9;LCD->RAM[2] |= D_BIT9;LCD->RAM[4] |= D_BIT9;LCD->RAM[6] |= D_ZERO;
			break;  
        case 0x08:
			LCD->RAM[0] |= D_BIT9;LCD->RAM[2] |= D_BIT8+D_BIT9;LCD->RAM[4] |= D_BIT8+D_BIT9;LCD->RAM[6] |= D_BIT8+D_BIT9;
			break;  
        case 0x09:
			LCD->RAM[0] |= D_BIT9;LCD->RAM[2] |= D_BIT8+D_BIT9;LCD->RAM[4] |= D_BIT8+D_BIT9;LCD->RAM[6] |= D_BIT9;
			break; 
		case 'r':
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_ZERO;LCD->RAM[4] |= D_BIT8;LCD->RAM[6] |= D_BIT8;
			break; 
		case 'F':
			LCD->RAM[0] |= D_BIT9;LCD->RAM[2] |= D_BIT8;LCD->RAM[4] |= D_BIT8;LCD->RAM[6] |= D_BIT8;
			break;	
		case '-':
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_ZERO;LCD->RAM[4] |= D_BIT8;LCD->RAM[6] |= D_ZERO;
			break;
		case 'L':
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT8;LCD->RAM[4] |= D_ZERO;LCD->RAM[6] |= D_BIT8+D_BIT9;
			break;
		
        case 0xFF:
            LCD->RAM[0] &= ~D_BIT9;LCD->RAM[2] &= ~(D_BIT8+D_BIT9);LCD->RAM[4] &= ~(D_BIT8+D_BIT9);LCD->RAM[6] &= ~(D_BIT8+D_BIT9);
            break;    
        default:
			break;
    }
    /*!< Requesy LCD RAM update */
//	LCD_UpdateDisplayRequest();  
}

/*******************************************************************************
  * @brief  液晶5显示数字
  * @param  None
  * @retval None
  ******************************************************************************/
static void Lcd_Dis5Value(uint8_t value)
{
    /*!< Wait Until the last LCD RAM update finish */
//	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);   
	switch(value)
	{
        case 0x00:
			LCD->RAM[0] |= D_BIT23+D_BIT25;LCD->RAM[2] |= D_BIT25;LCD->RAM[4] |= D_BIT23+D_BIT25;LCD->RAM[6] |= D_BIT25;
			break;
        case 0x01:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT25;LCD->RAM[4] |= D_BIT25;LCD->RAM[6] |= D_ZERO;
			break;
        case 0x02:
			LCD->RAM[0] |= D_BIT23+D_BIT25;LCD->RAM[2] |= D_BIT23;LCD->RAM[4] |= D_BIT25;LCD->RAM[6] |= D_BIT25;
			break;   
        case 0x03:
			LCD->RAM[0] |= D_BIT25;LCD->RAM[2] |= D_BIT23+D_BIT25;LCD->RAM[4] |= D_BIT25;LCD->RAM[6] |= D_BIT25;
			break;
        case 0x04:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT23+D_BIT25;LCD->RAM[4] |= D_BIT23+D_BIT25;LCD->RAM[6] |= D_ZERO;
			break;
        case 0x05:
			LCD->RAM[0] |= D_BIT25;LCD->RAM[2] |= D_BIT23+D_BIT25;LCD->RAM[4] |= D_BIT23;LCD->RAM[6] |= D_BIT25;
			break;  
        case 0x06:
			LCD->RAM[0] |= D_BIT23+D_BIT25;LCD->RAM[2] |= D_BIT23+D_BIT25;LCD->RAM[4] |= D_BIT23;LCD->RAM[6] |= D_BIT25;
			break; 
        case 0x07:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT25;LCD->RAM[4] |= D_BIT25;LCD->RAM[6] |= D_BIT25;
			break;  
        case 0x08:
			LCD->RAM[0] |= D_BIT23+D_BIT25;LCD->RAM[2] |= D_BIT23+D_BIT25;LCD->RAM[4] |= D_BIT23+D_BIT25;LCD->RAM[6] |= D_BIT25;
			break;  
        case 0x09:
			LCD->RAM[0] |= D_BIT25;LCD->RAM[2] |= D_BIT23+D_BIT25;LCD->RAM[4] |= D_BIT23+D_BIT25;LCD->RAM[6] |= D_BIT25;
			break; 
        case 0x0c:
            LCD->RAM[0] |= D_BIT23+D_BIT25;LCD->RAM[2] |= D_ZERO;LCD->RAM[4] |= D_BIT23;LCD->RAM[6] |= D_BIT25;
            break;
        case 0xFF:
            LCD->RAM[0] &= ~(D_BIT23+D_BIT25);LCD->RAM[2] &= ~(D_BIT23+D_BIT25);LCD->RAM[4] &= ~(D_BIT23+D_BIT25);LCD->RAM[6] &= ~D_BIT25;
            break;    
        default:
			break;
    }
    /*!< Requesy LCD RAM update */
//	LCD_UpdateDisplayRequest();  
}

/*******************************************************************************
  * @brief  液晶6显示数字
  * @param  None
  * @retval None
  ******************************************************************************/
static void Lcd_Dis6Value(uint8_t value)
{
    /*!< Wait Until the last LCD RAM update finish */
//	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);    
	switch(value)
	{
        case 0x00:
			LCD->RAM[1] |= D_BIT1+D_BIT2;LCD->RAM[3] |= D_BIT1;LCD->RAM[5] |= D_BIT1+D_BIT2;LCD->RAM[7] |= D_BIT1;
			break;
        case 0x01:
			LCD->RAM[1] |= D_ZERO;LCD->RAM[3] |= D_BIT1;LCD->RAM[5] |= D_BIT1;LCD->RAM[7] |= D_ZERO;
			break;
        case 0x02:
			LCD->RAM[1] |= D_BIT1+D_BIT2;LCD->RAM[3] |= D_BIT2;LCD->RAM[5] |= D_BIT1;LCD->RAM[7] |= D_BIT1;
			break;   
        case 0x03:
			LCD->RAM[1] |= D_BIT1;LCD->RAM[3] |= D_BIT1+D_BIT2;LCD->RAM[5] |= D_BIT1;LCD->RAM[7] |= D_BIT1;
			break;
        case 0x04:
			LCD->RAM[1] |= D_ZERO;LCD->RAM[3] |= D_BIT1+D_BIT2;LCD->RAM[5] |= D_BIT1+D_BIT2;LCD->RAM[7] |= D_ZERO;
			break;
        case 0x05:
			LCD->RAM[1] |= D_BIT1;LCD->RAM[3] |= D_BIT1+D_BIT2;LCD->RAM[5] |= D_BIT2;LCD->RAM[7] |= D_BIT1;
			break;  
        case 0x06:
			LCD->RAM[1] |= D_BIT1+D_BIT2;LCD->RAM[3] |= D_BIT1+D_BIT2;LCD->RAM[5] |= D_BIT2;LCD->RAM[7] |= D_BIT1;
			break; 
        case 0x07:
			LCD->RAM[1] |= D_ZERO;LCD->RAM[3] |= D_BIT1;LCD->RAM[5] |= D_BIT1;LCD->RAM[7] |= D_BIT1;
			break;  
        case 0x08:
			LCD->RAM[1] |= D_BIT1+D_BIT2;LCD->RAM[3] |= D_BIT1+D_BIT2;LCD->RAM[5] |= D_BIT1+D_BIT2;LCD->RAM[7] |= D_BIT1;
			break;  
        case 0x09:
			LCD->RAM[1] |= D_BIT1;LCD->RAM[3] |= D_BIT1+D_BIT2;LCD->RAM[5] |= D_BIT1+D_BIT2;LCD->RAM[7] |= D_BIT1;
			break; 
        case 0xFF:
            LCD->RAM[1] &= ~(D_BIT1+D_BIT2);LCD->RAM[3] &= ~(D_BIT1+D_BIT2);LCD->RAM[5] &= ~(D_BIT1+D_BIT2);LCD->RAM[7] &= ~D_BIT1;
            break;    
        default:
			break;
    }
    /*!< Requesy LCD RAM update */
//	LCD_UpdateDisplayRequest();  
}

/*******************************************************************************
  * @brief  液晶7显示数字
  * @param  None
  * @retval None
  ******************************************************************************/
static void Lcd_Dis7Value(uint8_t value)
{
    /*!< Wait Until the last LCD RAM update finish */
//	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);  
	switch(value)
	{
        case 0x00:
			LCD->RAM[0] |= D_BIT31+D_BIT32;LCD->RAM[2] |= D_BIT31;LCD->RAM[4] |= D_BIT31+D_BIT32;LCD->RAM[6] |= D_BIT31;
			break;
        case 0x01:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT31;LCD->RAM[4] |= D_BIT31;LCD->RAM[6] |= D_ZERO;
			break;
        case 0x02:
			LCD->RAM[0] |= D_BIT31+D_BIT32;LCD->RAM[2] |= D_BIT32;LCD->RAM[4] |= D_BIT31;LCD->RAM[6] |= D_BIT31;
			break;   
        case 0x03:
			LCD->RAM[0] |= D_BIT31;LCD->RAM[2] |= D_BIT31+D_BIT32;LCD->RAM[4] |= D_BIT31;LCD->RAM[6] |= D_BIT31;
			break;
        case 0x04:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT31+D_BIT32;LCD->RAM[4] |= D_BIT31+D_BIT32;LCD->RAM[6] |= D_ZERO;
			break;
        case 0x05:
			LCD->RAM[0] |= D_BIT31;LCD->RAM[2] |= D_BIT31+D_BIT32;LCD->RAM[4] |= D_BIT32;LCD->RAM[6] |= D_BIT31;
			break;  
        case 0x06:
			LCD->RAM[0] |= D_BIT31+D_BIT32;LCD->RAM[2] |= D_BIT31+D_BIT32;LCD->RAM[4] |= D_BIT32;LCD->RAM[6] |= D_BIT31;
			break; 
        case 0x07:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT31;LCD->RAM[4] |= D_BIT31;LCD->RAM[6] |= D_BIT31;
			break;  
        case 0x08:
			LCD->RAM[0] |= D_BIT31+D_BIT32;LCD->RAM[2] |= D_BIT31+D_BIT32;LCD->RAM[4] |= D_BIT31+D_BIT32;LCD->RAM[6] |= D_BIT31;
			break;  
        case 0x09:
			LCD->RAM[0] |= D_BIT31;LCD->RAM[2] |= D_BIT31+D_BIT32;LCD->RAM[4] |= D_BIT31+D_BIT32;LCD->RAM[6] |= D_BIT31;
			break; 
        case 0xFF:
            LCD->RAM[0] &= ~(D_BIT31+D_BIT32);LCD->RAM[2] &= ~(D_BIT31+D_BIT32);LCD->RAM[4] &= ~(D_BIT31+D_BIT32);LCD->RAM[6] &= ~D_BIT31;
            break;    
        default:
			break;
    }
    /*!< Requesy LCD RAM update */
//	LCD_UpdateDisplayRequest();  
}
/*******************************************************************************
  * @brief  液晶8显示数字
  * @param  None
  * @retval None
  ******************************************************************************/
static void Lcd_Dis8Value(uint8_t value)
{
    /*!< Wait Until the last LCD RAM update finish */
//	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);  
	switch(value)
	{
        case 0x00:
			LCD->RAM[0] |= D_BIT29+D_BIT30;LCD->RAM[2] |= D_BIT29;LCD->RAM[4] |= D_BIT29+D_BIT30;LCD->RAM[6] |= D_BIT29;
			break;
        case 0x01:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT29;LCD->RAM[4] |= D_BIT29;LCD->RAM[6] |= D_ZERO;
			break;
        case 0x02:
			LCD->RAM[0] |= D_BIT29+D_BIT30;LCD->RAM[2] |= D_BIT30;LCD->RAM[4] |= D_BIT29;LCD->RAM[6] |= D_BIT29;
			break;   
        case 0x03:
			LCD->RAM[0] |= D_BIT29;LCD->RAM[2] |= D_BIT29+D_BIT30;LCD->RAM[4] |= D_BIT29;LCD->RAM[6] |= D_BIT29;
			break;
        case 0x04:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT29+D_BIT30;LCD->RAM[4] |= D_BIT29+D_BIT30;LCD->RAM[6] |= D_ZERO;
			break;
        case 0x05:
			LCD->RAM[0] |= D_BIT29;LCD->RAM[2] |= D_BIT29+D_BIT30;LCD->RAM[4] |= D_BIT30;LCD->RAM[6] |= D_BIT29;
			break;  
        case 0x06:
			LCD->RAM[0] |= D_BIT29+D_BIT30;LCD->RAM[2] |= D_BIT29+D_BIT30;LCD->RAM[4] |= D_BIT30;LCD->RAM[6] |= D_BIT29;
			break; 
        case 0x07:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT29;LCD->RAM[4] |= D_BIT29;LCD->RAM[6] |= D_BIT29;
			break;  
        case 0x08:
			LCD->RAM[0] |= D_BIT29+D_BIT30;LCD->RAM[2] |= D_BIT29+D_BIT30;LCD->RAM[4] |= D_BIT29+D_BIT30;LCD->RAM[6] |= D_BIT29;
			break;  
        case 0x09:
			LCD->RAM[0] |= D_BIT29;LCD->RAM[2] |= D_BIT29+D_BIT30;LCD->RAM[4] |= D_BIT29+D_BIT30;LCD->RAM[6] |= D_BIT29;
			break; 
        case 0xFF:
            LCD->RAM[0] &= ~(D_BIT29+D_BIT30);LCD->RAM[2] &= ~(D_BIT29+D_BIT30);LCD->RAM[4] &= ~(D_BIT29+D_BIT30);LCD->RAM[6] &= ~D_BIT29;
            break;    
        default:
			break;
    }
    /*!< Requesy LCD RAM update */
//	LCD_UpdateDisplayRequest();  
}
/*******************************************************************************
  * @brief  液晶9显示数字
  * @param  None
  * @retval None
  ******************************************************************************/
static void Lcd_Dis9Value(uint8_t value)
{
    /*!< Wait Until the last LCD RAM update finish */
//	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);   
	switch(value)
	{
        case 0x00:
			LCD->RAM[0] |= D_BIT27+D_BIT28;LCD->RAM[2] |= D_BIT27;LCD->RAM[4] |= D_BIT27+D_BIT28;LCD->RAM[6] |= D_BIT27;
			break;
        case 0x01:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT27;LCD->RAM[4] |= D_BIT27;LCD->RAM[6] |= D_ZERO;
			break;
        case 0x02:
			LCD->RAM[0] |= D_BIT27+D_BIT28;LCD->RAM[2] |= D_BIT28;LCD->RAM[4] |= D_BIT27;LCD->RAM[6] |= D_BIT27;
			break;   
        case 0x03:
			LCD->RAM[0] |= D_BIT27;LCD->RAM[2] |= D_BIT27+D_BIT28;LCD->RAM[4] |= D_BIT27;LCD->RAM[6] |= D_BIT27;
			break;
        case 0x04:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT27+D_BIT28;LCD->RAM[4] |= D_BIT27+D_BIT28;LCD->RAM[6] |= D_ZERO;
			break;
        case 0x05:
			LCD->RAM[0] |= D_BIT27;LCD->RAM[2] |= D_BIT27+D_BIT28;LCD->RAM[4] |= D_BIT28;LCD->RAM[6] |= D_BIT27;
			break;  
        case 0x06:
			LCD->RAM[0] |= D_BIT27+D_BIT28;LCD->RAM[2] |= D_BIT27+D_BIT28;LCD->RAM[4] |= D_BIT28;LCD->RAM[6] |= D_BIT27;
			break; 
        case 0x07:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT27;LCD->RAM[4] |= D_BIT27;LCD->RAM[6] |= D_BIT27;
			break;  
        case 0x08:
			LCD->RAM[0] |= D_BIT27+D_BIT28;LCD->RAM[2] |= D_BIT27+D_BIT28;LCD->RAM[4] |= D_BIT27+D_BIT28;LCD->RAM[6] |= D_BIT27;
			break;  
        case 0x09:
			LCD->RAM[0] |= D_BIT27;LCD->RAM[2] |= D_BIT27+D_BIT28;LCD->RAM[4] |= D_BIT27+D_BIT28;LCD->RAM[6] |= D_BIT27;
			break; 
        case 0xFF:
            LCD->RAM[0] &= ~(D_BIT27+D_BIT28);LCD->RAM[2] &= ~(D_BIT27+D_BIT28);LCD->RAM[4] &= ~(D_BIT27+D_BIT28);LCD->RAM[6] &= ~D_BIT27;
            break;    
        default:
			break;
    }
    /*!< Requesy LCD RAM update */
//	LCD_UpdateDisplayRequest();  
}
/*******************************************************************************
  * @brief  液晶10显示数字
  * @param  None
  * @retval None
  ******************************************************************************/
static void Lcd_Dis10Value(uint8_t value)
{
    /*!< Wait Until the last LCD RAM update finish */
//	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
	switch(value)
	{
        case 0x00:
			LCD->RAM[1] |= D_BIT7+D_BIT8;LCD->RAM[3] |= D_BIT7;LCD->RAM[5] |= D_BIT7+D_BIT8;LCD->RAM[7] |= D_BIT7;
			break;
        case 0x01:
			LCD->RAM[1] |= D_ZERO;LCD->RAM[3] |= D_BIT7;LCD->RAM[5] |= D_BIT7;LCD->RAM[7] |= D_ZERO;
			break;
        case 0x02:
			LCD->RAM[1] |= D_BIT7+D_BIT8;LCD->RAM[3] |= D_BIT8;LCD->RAM[5] |= D_BIT7;LCD->RAM[7] |= D_BIT7;
			break;   
        case 0x03:
			LCD->RAM[1] |= D_BIT7;LCD->RAM[3] |= D_BIT7+D_BIT8;LCD->RAM[5] |= D_BIT7;LCD->RAM[7] |= D_BIT7;
			break;
        case 0x04:
			LCD->RAM[1] |= D_ZERO;LCD->RAM[3] |= D_BIT7+D_BIT8;LCD->RAM[5] |= D_BIT7+D_BIT8;LCD->RAM[7] |= D_ZERO;
			break;
        case 0x05:
			LCD->RAM[1] |= D_BIT7;LCD->RAM[3] |= D_BIT7+D_BIT8;LCD->RAM[5] |= D_BIT8;LCD->RAM[7] |= D_BIT7;
			break;  
        case 0x06:
			LCD->RAM[1] |= D_BIT7+D_BIT8;LCD->RAM[3] |= D_BIT7+D_BIT8;LCD->RAM[5] |= D_BIT8;LCD->RAM[7] |= D_BIT7;
			break; 
        case 0x07:
			LCD->RAM[1] |= D_ZERO;LCD->RAM[3] |= D_BIT7;LCD->RAM[5] |= D_BIT7;LCD->RAM[7] |= D_BIT7;
			break;  
        case 0x08:
			LCD->RAM[1] |= D_BIT7+D_BIT8;LCD->RAM[3] |= D_BIT7+D_BIT8;LCD->RAM[5] |= D_BIT7+D_BIT8;LCD->RAM[7] |= D_BIT7;
			break;  
        case 0x09:
			LCD->RAM[1] |= D_BIT7;LCD->RAM[3] |= D_BIT7+D_BIT8;LCD->RAM[5] |= D_BIT7+D_BIT8;LCD->RAM[7] |= D_BIT7;
			break; 
        case 0xFF:
            LCD->RAM[1] &= ~(D_BIT7+D_BIT8);LCD->RAM[3] &= ~(D_BIT7+D_BIT8);LCD->RAM[5] &= ~(D_BIT7+D_BIT8);LCD->RAM[7] &= ~D_BIT7;
            break;    
        default:
			break;
    }
    /*!< Requesy LCD RAM update */
//	LCD_UpdateDisplayRequest();  
}

/*******************************************************************************
  * @brief  液晶11显示数字
  * @param  None
  * @retval None
  ******************************************************************************/
static void Lcd_Dis11Value(uint8_t value)
{
    /*!< Wait Until the last LCD RAM update finish */
//	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
   	switch(value)
	{
        case 0x00:
			LCD->RAM[1] |= D_BIT5+D_BIT6;LCD->RAM[3] |= D_BIT5;LCD->RAM[5] |= D_BIT5+D_BIT6;LCD->RAM[7] |= D_BIT5;
			break;
        case 0x01:
			LCD->RAM[1] |= D_ZERO;LCD->RAM[3] |= D_BIT5;LCD->RAM[5] |= D_BIT5;LCD->RAM[7] |= D_ZERO;
			break;
        case 0x02:
			LCD->RAM[1] |= D_BIT5+D_BIT6;LCD->RAM[3] |= D_BIT6;LCD->RAM[5] |= D_BIT5;LCD->RAM[7] |= D_BIT5;
			break;   
        case 0x03:
			LCD->RAM[1] |= D_BIT5;LCD->RAM[3] |= D_BIT5+D_BIT6;LCD->RAM[5] |= D_BIT5;LCD->RAM[7] |= D_BIT5;
			break;
        case 0x04:
			LCD->RAM[1] |= D_ZERO;LCD->RAM[3] |= D_BIT5+D_BIT6;LCD->RAM[5] |= D_BIT5+D_BIT6;LCD->RAM[7] |= D_ZERO;
			break;
        case 0x05:
			LCD->RAM[1] |= D_BIT5;LCD->RAM[3] |= D_BIT5+D_BIT6;LCD->RAM[5] |= D_BIT6;LCD->RAM[7] |= D_BIT5;
			break;  
        case 0x06:
			LCD->RAM[1] |= D_BIT5+D_BIT6;LCD->RAM[3] |= D_BIT5+D_BIT6;LCD->RAM[5] |= D_BIT6;LCD->RAM[7] |= D_BIT5;
			break; 
        case 0x07:
			LCD->RAM[1] |= D_ZERO;LCD->RAM[3] |= D_BIT5;LCD->RAM[5] |= D_BIT5;LCD->RAM[7] |= D_BIT5;
			break;  
        case 0x08:
			LCD->RAM[1] |= D_BIT5+D_BIT6;LCD->RAM[3] |= D_BIT5+D_BIT6;LCD->RAM[5] |= D_BIT5+D_BIT6;LCD->RAM[7] |= D_BIT5;
			break;  
        case 0x09:
			LCD->RAM[1] |= D_BIT5;LCD->RAM[3] |= D_BIT5+D_BIT6;LCD->RAM[5] |= D_BIT5+D_BIT6;LCD->RAM[7] |= D_BIT5;
			break; 
        case 0xFF:
            LCD->RAM[1] &= ~(D_BIT5+D_BIT6);LCD->RAM[3] &= ~(D_BIT5+D_BIT6);LCD->RAM[5] &= ~(D_BIT5+D_BIT6);LCD->RAM[7] &= ~D_BIT5;
            break;    
        default:
			break;
    }
    /*!< Requesy LCD RAM update */
//	LCD_UpdateDisplayRequest();  
}
/*******************************************************************************
  * @brief  液晶12显示数字
  * @param  None
  * @retval None
  ******************************************************************************/
static void Lcd_Dis12Value(uint8_t value)
{
    /*!< Wait Until the last LCD RAM update finish */
//	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);    
	switch(value)
	{
        case 0x00:
			LCD->RAM[1] |= D_BIT3+D_BIT4;LCD->RAM[3] |= D_BIT3;LCD->RAM[5] |= D_BIT3+D_BIT4;LCD->RAM[7] |= D_BIT3;
			break;
        case 0x01:
			LCD->RAM[1] |= D_ZERO;LCD->RAM[3] |= D_BIT3;LCD->RAM[5] |= D_BIT3;LCD->RAM[7] |= D_ZERO;
			break;
        case 0x02:
			LCD->RAM[1] |= D_BIT3+D_BIT4;LCD->RAM[3] |= D_BIT4;LCD->RAM[5] |= D_BIT3;LCD->RAM[7] |= D_BIT3;
			break;   
        case 0x03:
			LCD->RAM[1] |= D_BIT3;LCD->RAM[3] |= D_BIT3+D_BIT4;LCD->RAM[5] |= D_BIT3;LCD->RAM[7] |= D_BIT3;
			break;
        case 0x04:
			LCD->RAM[1] |= D_ZERO;LCD->RAM[3] |= D_BIT3+D_BIT4;LCD->RAM[5] |= D_BIT3+D_BIT4;LCD->RAM[7] |= D_ZERO;
			break;
        case 0x05:
			LCD->RAM[1] |= D_BIT3;LCD->RAM[3] |= D_BIT3+D_BIT4;LCD->RAM[5] |= D_BIT4;LCD->RAM[7] |= D_BIT3;
			break;  
        case 0x06:
			LCD->RAM[1] |= D_BIT3+D_BIT4;LCD->RAM[3] |= D_BIT3+D_BIT4;LCD->RAM[5] |= D_BIT4;LCD->RAM[7] |= D_BIT3;
			break; 
        case 0x07:
			LCD->RAM[1] |= D_ZERO;LCD->RAM[3] |= D_BIT3;LCD->RAM[5] |= D_BIT3;LCD->RAM[7] |= D_BIT3;
			break;  
        case 0x08:
			LCD->RAM[1] |= D_BIT3+D_BIT4;LCD->RAM[3] |= D_BIT3+D_BIT4;LCD->RAM[5] |= D_BIT3+D_BIT4;LCD->RAM[7] |= D_BIT3;
			break;  
        case 0x09:
			LCD->RAM[1] |= D_BIT3;LCD->RAM[3] |= D_BIT3+D_BIT4;LCD->RAM[5] |= D_BIT3+D_BIT4;LCD->RAM[7] |= D_BIT3;
			break; 
        case 0xFF:
            LCD->RAM[1] &= ~(D_BIT3+D_BIT4);LCD->RAM[3] &= ~(D_BIT3+D_BIT4);LCD->RAM[5] &= ~(D_BIT3+D_BIT4);LCD->RAM[7] &= ~D_BIT3;
            break;    
        default:
			break;
    }
    /*!< Requesy LCD RAM update */
//	LCD_UpdateDisplayRequest();  
}
/*******************************************************************************
  * @brief  液晶13显示数字
  * @param  None
  * @retval None
  ******************************************************************************/
static void Lcd_Dis13Value(uint8_t value)
{
    /*!< Wait Until the last LCD RAM update finish */
//	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);  
	switch(value)
	{
        case 0x00:
			LCD->RAM[0] |= D_BIT17+D_BIT18;LCD->RAM[2] |= D_BIT17;LCD->RAM[4] |= D_BIT17+D_BIT18;LCD->RAM[6] |= D_BIT17;
			break;
        case 0x01:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT17;LCD->RAM[4] |= D_BIT17;LCD->RAM[6] |= D_ZERO;
			break;
        case 0x02:
			LCD->RAM[0] |= D_BIT17+D_BIT18;LCD->RAM[2] |= D_BIT18;LCD->RAM[4] |= D_BIT17;LCD->RAM[6] |= D_BIT17;
			break;   
        case 0x03:
			LCD->RAM[0] |= D_BIT17;LCD->RAM[2] |= D_BIT17+D_BIT18;LCD->RAM[4] |= D_BIT17;LCD->RAM[6] |= D_BIT17;
			break;
        case 0x04:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT17+D_BIT18;LCD->RAM[4] |= D_BIT17+D_BIT18;LCD->RAM[6] |= D_ZERO;
			break;
        case 0x05:
			LCD->RAM[0] |= D_BIT17;LCD->RAM[2] |= D_BIT17+D_BIT18;LCD->RAM[4] |= D_BIT18;LCD->RAM[6] |= D_BIT17;
			break;  
        case 0x06:
			LCD->RAM[0] |= D_BIT17+D_BIT18;LCD->RAM[2] |= D_BIT17+D_BIT18;LCD->RAM[4] |= D_BIT18;LCD->RAM[6] |= D_BIT17;
			break; 
        case 0x07:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT17;LCD->RAM[4] |= D_BIT17;LCD->RAM[6] |= D_BIT17;
			break;  
        case 0x08:
			LCD->RAM[0] |= D_BIT17+D_BIT18;LCD->RAM[2] |= D_BIT17+D_BIT18;LCD->RAM[4] |= D_BIT17+D_BIT18;LCD->RAM[6] |= D_BIT17;
			break;  
        case 0x09:
			LCD->RAM[0] |= D_BIT17;LCD->RAM[2] |= D_BIT17+D_BIT18;LCD->RAM[4] |= D_BIT17+D_BIT18;LCD->RAM[6] |= D_BIT17;
			break; 
        case 0xFF:
            LCD->RAM[0] &= ~(D_BIT17+D_BIT18);LCD->RAM[2] &= ~(D_BIT17+D_BIT18);LCD->RAM[4] &= ~(D_BIT17+D_BIT18);LCD->RAM[6] &= ~D_BIT17;
            break;    
        default:
			break;
    }
    /*!< Requesy LCD RAM update */
//	LCD_UpdateDisplayRequest();  
}
/*******************************************************************************
  * @brief  液晶14显示数字
  * @param  None
  * @retval None
  ******************************************************************************/
static void Lcd_Dis14Value(uint8_t value)
{
    /*!< Wait Until the last LCD RAM update finish */
//	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET);
	switch(value)
	{
        case 0x00:
			LCD->RAM[0] |= D_BIT12+D_BIT14;LCD->RAM[2] |= D_BIT14;LCD->RAM[4] |= D_BIT12+D_BIT14;LCD->RAM[6] |= D_BIT14;
			break;
        case 0x01:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT14;LCD->RAM[4] |= D_BIT14;LCD->RAM[6] |= D_ZERO;
			break;
        case 0x02:
			LCD->RAM[0] |= D_BIT12+D_BIT14;LCD->RAM[2] |= D_BIT12;LCD->RAM[4] |= D_BIT14;LCD->RAM[6] |= D_BIT14;
			break;   
        case 0x03:
			LCD->RAM[0] |= D_BIT14;LCD->RAM[2] |= D_BIT12+D_BIT14;LCD->RAM[4] |= D_BIT14;LCD->RAM[6] |= D_BIT14;
			break;
        case 0x04:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT12+D_BIT14;LCD->RAM[4] |= D_BIT12+D_BIT14;LCD->RAM[6] |= D_ZERO;
			break;
        case 0x05:
			LCD->RAM[0] |= D_BIT14;LCD->RAM[2] |= D_BIT12+D_BIT14;LCD->RAM[4] |= D_BIT12;LCD->RAM[6] |= D_BIT14;
			break;  
        case 0x06:
			LCD->RAM[0] |= D_BIT12+D_BIT14;LCD->RAM[2] |= D_BIT12+D_BIT14;LCD->RAM[4] |= D_BIT12;LCD->RAM[6] |= D_BIT14;
			break; 
        case 0x07:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT14;LCD->RAM[4] |= D_BIT14;LCD->RAM[6] |= D_BIT14;
			break;  
        case 0x08:
			LCD->RAM[0] |= D_BIT12+D_BIT14;LCD->RAM[2] |= D_BIT12+D_BIT14;LCD->RAM[4] |= D_BIT12+D_BIT14;LCD->RAM[6] |= D_BIT14;
			break;  
        case 0x09:
			LCD->RAM[0] |= D_BIT14;LCD->RAM[2] |= D_BIT12+D_BIT14;LCD->RAM[4] |= D_BIT12+D_BIT14;LCD->RAM[6] |= D_BIT14;
			break; 
        case 0xFF:
            LCD->RAM[0] &= ~(D_BIT12+D_BIT14);LCD->RAM[2] &= ~(D_BIT12+D_BIT14);LCD->RAM[4] &= ~(D_BIT12+D_BIT14);LCD->RAM[6] &= ~D_BIT14;
            break;    
        default:
			break;
    }
    /*!< Requesy LCD RAM update */
//	LCD_UpdateDisplayRequest();  
}

/*******************************************************************************
  * @brief  液晶15显示数字
  * @param  None
  * @retval None
  ******************************************************************************/
static void Lcd_Dis15Value(uint8_t value)
{
    /*!< Wait Until the last LCD RAM update finish */
//	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET); 
	switch(value)
	{
        case 0x00:
			LCD->RAM[0] |= D_BIT16+D_BIT20;LCD->RAM[2] |= D_BIT20;LCD->RAM[4] |= D_BIT16+D_BIT20;LCD->RAM[6] |= D_BIT20;
			break;
        case 0x01:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT20;LCD->RAM[4] |= D_BIT20;LCD->RAM[6] |= D_ZERO;
			break;
        case 0x02:
			LCD->RAM[0] |= D_BIT16+D_BIT20;LCD->RAM[2] |= D_BIT16;LCD->RAM[4] |= D_BIT20;LCD->RAM[6] |= D_BIT20;
			break;   
        case 0x03:
			LCD->RAM[0] |= D_BIT20;LCD->RAM[2] |= D_BIT16+D_BIT20;LCD->RAM[4] |= D_BIT20;LCD->RAM[6] |= D_BIT20;
			break;
        case 0x04:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT16+D_BIT20;LCD->RAM[4] |= D_BIT16+D_BIT20;LCD->RAM[6] |= D_ZERO;
			break;
        case 0x05:
			LCD->RAM[0] |= D_BIT20;LCD->RAM[2] |= D_BIT16+D_BIT20;LCD->RAM[4] |= D_BIT16;LCD->RAM[6] |= D_BIT20;
			break;  
        case 0x06:
			LCD->RAM[0] |= D_BIT16+D_BIT20;LCD->RAM[2] |= D_BIT16+D_BIT20;LCD->RAM[4] |= D_BIT16;LCD->RAM[6] |= D_BIT20;
			break; 
        case 0x07:
			LCD->RAM[0] |= D_ZERO;LCD->RAM[2] |= D_BIT20;LCD->RAM[4] |= D_BIT20;LCD->RAM[6] |= D_BIT20;
			break;  
        case 0x08:
			LCD->RAM[0] |= D_BIT16+D_BIT20;LCD->RAM[2] |= D_BIT16+D_BIT20;LCD->RAM[4] |= D_BIT16+D_BIT20;LCD->RAM[6] |= D_BIT20;
			break;  
        case 0x09:
			LCD->RAM[0] |= D_BIT20;LCD->RAM[2] |= D_BIT16+D_BIT20;LCD->RAM[4] |= D_BIT16+D_BIT20;LCD->RAM[6] |= D_BIT20;
			break; 
        case 0xFF:
            LCD->RAM[0] &= ~(D_BIT16+D_BIT20);LCD->RAM[2] &= ~(D_BIT16+D_BIT20);LCD->RAM[4] &= ~(D_BIT16+D_BIT20);LCD->RAM[6] &= ~D_BIT20;
            break;    
        default:
			break;
    }
    /*!< Requesy LCD RAM update */
//	LCD_UpdateDisplayRequest();  
}

/*******************************************************************************
  * @brief  Description 显示年
  * @param  None
  * @retval None
  ******************************************************************************/
void displayYEAR(unsigned char year)
{
	Lcd_Dis12Value(0xff);
	Lcd_Dis13Value(0xff);
	Lcd_Dis14Value(0xff);
	Lcd_Dis15Value(0xff);
	
    Lcd_Dis12Value(2);//1  
    Lcd_Dis13Value(0);//2
    if(year>9)
    {
        Lcd_Dis14Value(year/16); 
        Lcd_Dis15Value(year%16);
    }
    else
    {
        Lcd_Dis14Value(0);// 
        Lcd_Dis15Value(year%16);
    }
}
/*******************************************************************************
  * @brief  Description 显示日期
  * @param  None
  * @retval None
  ******************************************************************************/
void displayDAY(uint8_t month,uint8_t day)
{
	Lcd_Dis12Value(0xff);
	Lcd_Dis13Value(0xff);
	Lcd_Dis14Value(0xff);
	Lcd_Dis15Value(0xff);
    if(month>9)
    {
        Lcd_Dis13Value(month%16);//2 个位
        month=month/16;
        Lcd_Dis12Value(month);//1 
    }
    else
    {
        Lcd_Dis13Value(month);//2
        Lcd_Dis12Value(0);//0
    }
    if(day>9)
    {
        Lcd_Dis15Value(day%16);//2 个位
        day=day/16;
        Lcd_Dis14Value(day);//1 
    }
    else
    {
        Lcd_Dis15Value(day);//2
        Lcd_Dis14Value(0);//0
    }
}
/*******************************************************************************
  * @brief  Description 显示时间
  * @param  None
  * @retval None
  ******************************************************************************/
void displayTIME(uint8_t hour,uint8_t min)
{
	Lcd_Dis12Value(0xff);
	Lcd_Dis13Value(0xff);
	Lcd_Dis14Value(0xff);
	Lcd_Dis15Value(0xff);
    if(hour>9)
    {
        Lcd_Dis13Value(hour%16);//2 个位
        hour=hour/16;
        Lcd_Dis12Value(hour);//1 
    }
    else
    {
        Lcd_Dis13Value(hour);//2
        Lcd_Dis12Value(0);//0
    }
    if(min>9)
    {
        Lcd_Dis15Value(min%16);//2 个位
        min=min/16;
        Lcd_Dis14Value(min);//1 
    }
    else
    {
        Lcd_Dis15Value(min);//2
        Lcd_Dis14Value(0);//0
    }
}
/*******************************************************************************
  * @brief  Description  显示信号强度
  * @param  signal_value 信号值
  * @retval None
  ******************************************************************************/
void Display_Signal(uint8_t signal_value)
{
	/*!< Wait Until the last LCD RAM update finish */
	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){} 
	
	if((signal_value >0)&&(signal_value<=6))
	{showS1;}
	if((signal_value>=7)&&(signal_value<=13))
	{showS12;}
	if((signal_value>=14)&&(signal_value<=20))
	{showS123;}
	if((signal_value>=21)&&(signal_value<=27))
	{showS1234;}
	if((signal_value>=28)&&(signal_value<=31))
	{showS12345;}
	/*!< Requesy LCD RAM update */
	LCD_UpdateDisplayRequest();		
}
/*******************************************************************************
  * @brief  显示存储容量
  * @param  None
  * @retval None
  ******************************************************************************/
void Display_Mem(void) 
{
	/*!< Wait Until the last LCD RAM update finish */
	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){}
    if(Queue.FlashRecOverFlow == 0)/*未溢出过*/
    {
        if(Queue.WriteFlashDataPointer<=(FLASH_RecMaxSize/5)) /**/
            {showFlashMEM1;}
        else if((Queue.WriteFlashDataPointer<=(FLASH_RecMaxSize*2/5))&&(Queue.WriteFlashDataPointer>(FLASH_RecMaxSize/5)))
            {showFlashMEM2;}
        else if((Queue.WriteFlashDataPointer<=(FLASH_RecMaxSize*3/5))&&(Queue.WriteFlashDataPointer>(FLASH_RecMaxSize*2/5)))
            {showFlashMEM3;}
        else if((Queue.WriteFlashDataPointer<=(FLASH_RecMaxSize*4/5))&&(Queue.WriteFlashDataPointer>(FLASH_RecMaxSize*3/5)))
            {showFlashMEM4;}
        else if((Queue.WriteFlashDataPointer<(FLASH_RecMaxSize))&&(Queue.WriteFlashDataPointer>(FLASH_RecMaxSize*4/5)))
            {showFlashMEM5;}
    }
    else/*溢出，显示满格*/
    {
        showFlashMEM5;
    }
    /*!< Requesy LCD RAM update */
	LCD_UpdateDisplayRequest();  
}
/*******************************************************************************
  * @brief  显示NUL
  * @param  None
  * @retval None
  ******************************************************************************/
void Display_NUL(void)
{
	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET); 
	
	/*先清数据*/
	Lcd_Dis2Value(0xFF);
	Lcd_Dis3Value(0xFF);
	Lcd_Dis4Value(0xFF);
	
	Lcd_Dis2Value('n');
	Lcd_Dis3Value('U');
	Lcd_Dis4Value('L');
	
	/*!< Requesy LCD RAM update */
	LCD_UpdateDisplayRequest();  
}
/*******************************************************************************
  * @brief  显示低功耗标志 LO.
  * @param  None
  * @retval None
  ******************************************************************************/
void Display_LOW(void)
{
	LCD_GLASS_Clear();
	
	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET); 
	/*先清数据*/
	
	Lcd_Dis2Value('L');
	Lcd_Dis3Value('O');
	showP2;
	/*!< Requesy LCD RAM update */
	LCD_UpdateDisplayRequest();  
	
}
/*******************************************************************************
  * @brief  首次上电显示 ---,SN号,仪器时间
  * @param  None
  * @retval None
  ******************************************************************************/
void FisrtPowerOnDisplay(void)
{
	Display_SN();//显示SN号
	/*显示时间*/
	read_time();
    while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET); 
	
	displayTIME(Rtc.Hour,Rtc.Minute);
    showTIME;	/*显示钟表符号*/
	
	Lcd_Dis2Value('-');
	Lcd_Dis3Value('-');
	Lcd_Dis4Value('-');
	
	LCD_UpdateDisplayRequest();
}
/*******************************************************************************
  * @brief  显示仪器SN号
  * @param  None
  * @retval None
  ******************************************************************************/
void Display_SN(void)
{
//    unsigned char year,month;
    uint16_t value;
    
//    year = *((char*)JLY_SN_ADDR);
//    month = *((char*)JLY_SN_ADDR+1);
//    num_H = *((char*)JLY_SN_ADDR+2);
//    num_L = *((char*)JLY_SN_ADDR+3);
//    value = (num_H<<8)+num_L;
    value = Conf.Basic.Sn[0]+Conf.Basic.Sn[1];
    
    
    
    //LCDMEM[2]=digit[year%16];
//    LCDMEM[2]|=digit[month/16];
    /*!< Wait Until the last LCD RAM update finish */
	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET){} 
    
	Lcd_Dis5Value(0xFF);	
	Lcd_Dis6Value(0xFF);	
	Lcd_Dis7Value(0xFF);	
	Lcd_Dis8Value(0xFF);	
	Lcd_Dis9Value(0xFF);	
	Lcd_Dis10Value(0xFF);	
	Lcd_Dis11Value(0xFF);	
	
    showID;
//    LCDMEM[3]|=digit[month%16];
    Lcd_Dis5Value(0x0c);
    Lcd_Dis6Value(3);
    if(value/10000>0)
        Lcd_Dis7Value(value/10000);
    else
        Lcd_Dis7Value(0);
    if(value/1000>0)
        Lcd_Dis8Value((value/1000)%10);
    else
        Lcd_Dis8Value(0);
    if(value/100>0)
        Lcd_Dis9Value((value/100)%10);
    else
        Lcd_Dis9Value(0);
    if(value/10)
        Lcd_Dis10Value((value/10)%10);
    else
        Lcd_Dis10Value(0);
    
    Lcd_Dis11Value(value%10);
    
    /*!< Requesy LCD RAM update */
	LCD_UpdateDisplayRequest();  
}
/*******************************************************************************
  * @brief  Description 显示传感器故障
  * @param  None
  * @retval None
  ******************************************************************************/
void displayErr(uint8_t Er)
{
	/*!< Wait Until the last LCD RAM update finish */
	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET); 
//    if(FlagDownIng==0)
//    {
        /* 2[8] 3[8] 4[8]清零 */
        Lcd_Dis2Value(0XFF);
        Lcd_Dis3Value(0XFF);
        Lcd_Dis4Value(0XFF);
		/* 清报警状态 */
        clearAlarmStatus;
		/* 清 "设置上限"，“设置下限" */	
        clearshangxian;clearxiaxian;		
        /* 清符号 */
        clearFUHAO;clearAlarmStatus;clearJINBAO;
        //clearRH;
		//showC;
		clearP1;clearP2;
        /* 显示 Er */
        Lcd_Dis2Value(Er);
        Lcd_Dis3Value('E');
        Lcd_Dis4Value('r');
//    }
	/*!< Requesy LCD RAM update */
	LCD_UpdateDisplayRequest();  
}
/*******************************************************************************
  * @brief  Description 关闭LCD
  * @param  None
  * @retval None
  ******************************************************************************/
void lcd_OFF(uint8_t offcode)
{
	/*!< Wait Until the last LCD RAM update finish */
	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET); 
	/* 2[8] 3[8] 4[8]清零 */
	Lcd_Dis1Value(0XFF);
	Lcd_Dis2Value(0XFF);
	Lcd_Dis3Value(0XFF);
	Lcd_Dis4Value(0XFF);
	
	clearS12345;
	clear_NFC;
	clear_BOX;
	clear_GPRS;
	clear_GPS;
	//clearBATT;
	clearFlashMEM;
	cleartongdao;
	clearJINBAO;
	clearAlarmStatus;
	clearshangxian;
	clearxiaxian;
	clearFUHAO;
	clearP1;
	clearP2;
	clearC;
	clearRH;
	
	if(offcode==0xFF)
	{
		Lcd_Dis1Value(0xFF);
	}
	else
	{
		Lcd_Dis1Value(offcode);
	}
	/*显示0FF*/
	Lcd_Dis2Value(0);
	Lcd_Dis3Value('F');
	Lcd_Dis4Value('F');
	
	/*!< Requesy LCD RAM update */
	LCD_UpdateDisplayRequest();  
}
/*******************************************************************************
  * @brief  Description 显示指定通道数据
  * @param  temp 		通道
  * @param  humi		温湿度数据
  * @retval None
  ******************************************************************************/
static void Lcd_ChannelValue(uint8_t temp,float humi)
{
    uint16_t value;
    uint8_t value_fuhao,chanel;
    float ft;
    
    chanel=temp;
    
	/*!< Wait Until the last LCD RAM update finish */
	while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET); 
	
    Lcd_Dis1Value(0XFF);
	showtongdao;
    Lcd_Dis1Value(chanel);	//显示通道号
    Lcd_Dis2Value(0XFF);
    Lcd_Dis3Value(0XFF);
    Lcd_Dis4Value(0XFF);
	//clearS12345;
	clear_NFC;
    clearAlarmStatus;
    clearshangxian;
    clearxiaxian;
    
    clearFUHAO;
    //clearRH;
	clearJINBAO;
    //clearC;
	
    if(Conf.Sensor[chanel-1].SensorType ==0x00 )    
	{
		clearP2;
		Display_NUL();
	}else{
		if(FlagSeniorErr[chanel-1]==1)
		{
			/*!< 显示传感器故障 */
			displayErr(2);
		}
		else
		{
			if(humi<0)
			{
				value_fuhao=1;
				ft=humi*(-100.0);
			}
			else
			{
				value_fuhao=0;
				ft=humi*100.0;
			}
			value=(unsigned int)ft;
			if(value%10>=4)value=value+10;//四舍五入
			value=value/10;
			if(value >= 1000)
				value = 999;
			/*
			if(value/10000>0)
				digit8_12(value/10000);//10
			else
				digit8_12(0XFF);
			if(value/1000>0)
				digit8_13((value/1000)%10);//11
			else
				digit8_13(0XFF);
			*/
			if(value/100>0)
				Lcd_Dis2Value((value/100)%10);
			else
				Lcd_Dis2Value(0XFF);
			if(value/10)
				Lcd_Dis3Value((value/10)%10);
			else
				Lcd_Dis3Value(0);
			
			Lcd_Dis4Value(value%10);
			
			if(value_fuhao==1)
				showFUHAO;
			
			showP2;
		}
	}
	/*!< Requesy LCD RAM update */
	LCD_UpdateDisplayRequest();  
}

/*******************************************************************************
  * @brief  Description 		显示通道数据
  * @param  started_channel0 	通道
  * @retval None
  ******************************************************************************/
void Display_ChannelValue(uint8_t started_channel0)
{
    uint8_t channel_cp;
    
    if(!(display_ct%channeldisplaytime)&&Flag.IsDisplayRightNow)
    {
        
        channel_cp = started_channel0;
        
        while(channel_cp & 0x01)
        {
			ChannelForDisplay++;//ChannelForDisplay
			Lcd_ChannelValue(ChannelForDisplay,ChannelDataFloat[ChannelForDisplay-1]);
			AlarmDeal(ChannelForDisplay);
            
            if(Conf.Sensor[ChannelForDisplay-1].SensorType == SENSOR_HUMI)	/*湿度*/
            {
                /*!< Wait Until the last LCD RAM update finish */
                while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET); 
                clearC;  
				showRH;
                /*!< Requesy LCD RAM update */
                LCD_UpdateDisplayRequest();  
            }else{
				while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET); 
				clearRH;
				showC;
				LCD_UpdateDisplayRequest();  
			}
            break;
        }
        
        StartedChannelForDisplay=StartedChannelForDisplay>>1;
        
        if(!StartedChannelForDisplay)
        {
            StartedChannelForDisplay = Started_Channel;
            ChannelForDisplay = 0;
        }
        
//        Transact_alarm(AlarmCode);
    }
    Flag.IsDisplayRightNow = 0;
}

