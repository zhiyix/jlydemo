/**
  ******************************************************************************
  * @file    Project/STM32L1xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    16-May-2015
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "freemodbus.h"

/** @addtogroup Template_Project
  * @{
  */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32l1xx_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32l1xx.c file
     */ 

	PeripheralInit();
    SysInit();
	
	printf("\r\n this is a 32bits  demo \r\n");
	
  /* Add your application code here
     */

//    SPI_FLASH_Test();
//    SPI_Flash_PowerDown();  
//    Fram_Test();
	
	freemodbus_init();
  /* Infinite loop */
  while (1)
  {
	  
//	  LED1_ON;Delay_ms(1000);LED1_OFF;
//	  
//	  LED2_ON;Delay_ms(1000);LED2_OFF;
	  
//	  printf("\r\n 温度 0x%04X,%f",ADC_ConvertedValue[0],ChannelDataFloat[0]);
//	  printf("\r\n 湿度 0x%04X,%f",ADC_ConvertedValue[1],ChannelDataFloat[1]);
//	  printf("\r\n 电压 0x%04X,%f",ADC_ConvertedValue[2],ChannelDataFloat[2]);
	  
//	  Fram_Test();
//	  Delay_ms(1000);
      
	  JlySerialDeal();
	  
      JlySecDeal();
	  if(Flag.SysTickSec ==1)//模拟10s保存数据
	  {
         Flag.SysTickSec = 0;
         
         ReadFramHisDataToRam();
//         Down_HisData();
	  }
      if(Flag.KeyDuanAn ==1)
      {
//          Fram_Test();
          Down_HisData();
//		  DownFlash_HisData();
      }
	  freemodbus_main();
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
