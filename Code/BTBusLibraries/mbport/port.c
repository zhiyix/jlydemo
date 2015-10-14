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
  * File: $Id: portevent.c,v 1.60 2013/12/28 9:18:05 Armink $
  */

/* ----------------------- System includes --------------------------------*/

/* ----------------------- Modbus includes ----------------------------------*/
#include <stdarg.h>
#include <stdlib.h>
#include "porttype.h"

/*============================ TYPES =========================================*/

/**
  * @brief  "Temp Variable Buffer"
  * @param  None
  * @retval None
  */
#define APP_DEBUG_CUR				DEBUG_ALL
#define APP_DEBUG_SIZE				0x200

/* ----------------------- Variables ----------------------------------------*/
static char 			   str[APP_DEBUG_SIZE];

/* ----------------------- Start implementation -----------------------------*/
void EnterCriticalSection(void)
{
	//关闭全局中断
	__disable_irq();
}

void ExitCriticalSection(void)
{
	//开启全局中断
	__enable_irq();
}

void vMBDelay(ULONG nCount)
{
	for(; nCount > 0;nCount--);
}

/**
  * @brief  Description "调试信息输出--字符串输出函数"
  * @param  ch			输出字符
  * @retval None
  */
int xMBOutput(CHAR deg, const char *format, ...)
{
    int done = 0;

    char *ptr;
    ptr = str;
    if(deg)
    {
        {
            va_list arg;
            va_start(arg, format);
            done = vsprintf(ptr, format, arg);
            done = done >= APP_DEBUG_SIZE ? APP_DEBUG_SIZE - 1 : done;
            str[done] = '\0';
            va_end(arg);
            printf(ptr);
        }
    }

    return done;
}
