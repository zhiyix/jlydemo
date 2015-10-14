 /*
  * FreeModbus Libary: STM32 Port
  * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  *
  * File: $Id: portctrl.c,v 1.60 2013/12/28 9:18:05 Armink $
  */

/* ----------------------- System includes ----------------------------------*/

/* ----------------------- Platform includes --------------------------------*/
//#include ".\port.h"
#include ".\app_cfg.h"

/* ----------------------- Modbus includes ----------------------------------*/
// #include "mb.h"
// #include "mbport.h"
 #include ".\hal_gpio.h"

/* ----------------------- Variables ----------------------------------------*/
/*######--------------------------------######--------------------------------*/
#ifdef  IOPF
#undef  IOPF
#endif
#define IOPF							(*(GPIO_Typedef*)GPIOF_BASE)

/* ----------------------- Start implementation -----------------------------*/
#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif
	void MBMasterStatusIndicate(uint8_t ID, bool bl)
	{
		IOPF.WriteBits(((uint32_t)0x01UL << (ID & 0x1F)), bl);
	}

	void MBMasterStatusToggle(uint8_t ID, bool bl)
	{
		IOPF.WriteBits(((uint32_t)0x01UL << (ID & 0x1F)), bl);
		IOPF.ToggleBits(((uint32_t)0x01UL << (ID & 0x1F)));
	}

	void MBMasterNoStatusToggle(uint8_t ID)
	{
		IOPF.ToggleBits(((uint32_t)0x01UL << (ID & 0x1F)));
	}
#ifdef __cplusplus
PR_END_EXTERN_C
#endif


void IT_IND( int times )
{
	while (times --)
	{
		MB_STATUS_TOG2(MB_STA_IND_IT_CFG);
		MB_STATUS_TOG2(MB_STA_IND_IT_CFG);
	}
}
void CFG_IND( int times )
{
	while (times --)
	{
		MB_STATUS_TOG2(MB_STA_IND_IT_CFG);
		MB_STATUS_TOG2(MB_STA_IND_IT_CFG);
	}
}
