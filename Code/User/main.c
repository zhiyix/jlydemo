/**
  ******************************************************************************
  * @file    Project/STM32L1xx_JLY/main.c 
  * @author  MCD Application Team
  * @version V
  * @date    
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

/******************************************************************************
  * @brief  Main program.
  * @param  None
  * @retval None
  *****************************************************************************/
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
	printf("\r\n struct SensorChanelConfDataStr size:%d \r\n",sizeof(struct SensorChanelConfDataStr));
	printf("\r\n Conf.Sensor[0].SensorAlarm_High.wd:%d \r\n",Conf.Sensor[0].SensorAlarm_High.wd);
	printf("\r\n Conf.Sensor[0].SensorAlarm_High.ft:%f \r\n",Conf.Sensor[0].SensorAlarm_High.ft);
  /* Add your application code here
     */

	freemodbus_init();
  /* Infinite loop */
  while (1)
  {
	  
      
	  //JlySerialDeal();
	  
      JlySecDeal();
//	  if(Flag.SysTickSec ==1)//模拟10s 数据
//	  {
//         Flag.SysTickSec = 0;
//         
//         ReadFramHisDataToRam();
//         Down_HisData();
//	  }
      if(Flag.TouchKey1DuanAn ==1)
      {
         //Down_HisData();
		  printf("Queue.FlashRecOverFlow %d\r\n",Queue.FlashRecOverFlow);
		  printf("Queue.FlashSectorPointer %d\r\n",Queue.FlashSectorPointer);
		  printf("Queue.WriteFlashDataPointer %d\r\n",Queue.WriteFlashDataPointer);
		  printf("Queue.FlashReadDataBeginPointer %d\r\n",Queue.FlashReadDataBeginPointer);
		  printf("Queue.ReadFlashDataPointer %d\r\n",Queue.ReadFlashDataPointer);
		  printf("Queue.FlashNoReadingDataNum %d\r\n",Queue.FlashNoReadingDataNum);
		  DownFlash_HisData();
		  rtc_deel();
      }
	  if(Flag.TouchKey2DuanAn ==1)
	  {
		  Flag.TouchKey2DuanAn =0;
		  
		  Queue.FlashSectorPointer = 0;
		  Queue.WriteFlashDataPointer =0;
		  Queue.FlashReadDataBeginPointer =0;
		  Queue.ReadFlashDataPointer = 0;
		  Queue.FlashNoReadingDataNum = 0;
		  
		  SetFlashOverFlow(0);//清除flash溢出标志
		  WriteU16Pointer(FLASH_SectorWriteAddr_Lchar,0);
		  WriteU32Pointer(FLASH_WriteDataAddr_Lchar,0);
		  WriteU32Pointer(FLASH_ReadDataBeginAddr_Lchar,0);
		  WriteU32Pointer(FLASH_ReadDataAddr_Lchar,0);
		  WriteU32Pointer(FLASH_NoReadingDataNumAddr_Lchar,0);
	  }
	  freemodbus_main();
  }
}

#ifdef  USE_FULL_ASSERT

/******************************************************************************
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  *****************************************************************************/
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
