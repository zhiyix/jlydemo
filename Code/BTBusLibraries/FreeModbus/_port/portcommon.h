/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
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
 * File: $Id: port.h,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#ifndef MB_PORT_H
#define MB_PORT_H

/*============================ INCLUDES ======================================*/
//! @name include
//! @{
// #include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
//! @}

/*============================ MACROS ========================================*/
/*######--------------------------------######--------------------------------*/
#ifndef TRUE
#define TRUE            				1
#endif

#ifndef FALSE
#define FALSE           				0
#endif

#define	INLINE                      	inline
	
#if (false)
 #define assert(expr)					assert_param(expr)
#else
 #define assert(expr)					(void)0
#endif

/*============================ TYPES =========================================*/
/*######--------------------------------######--------------------------------*/
typedef unsigned char   				UCHAR;
typedef char            				CHAR;

typedef uint16_t        				USHORT;
typedef int16_t         				SHORT;

typedef uint32_t        				ULONG;
typedef int32_t         				LONG;

typedef ULONG           				CORE_MB_SAFE;

typedef uint8_t       					BOOL;

/*============================ INTERFACE =====================================*/
/*######--------------------------------######--------------------------------*/
#if (false)
 #define ENTER_CRITICAL_SECTION( )		\
	do {  								\
		CORE_MB_SAFE core_sr = 			\
			__get_PRIMASK(); 			\
		__disable_irq();							

 #define EXIT_CRITICAL_SECTION( )  		\
		__set_PRIMASK(core_sr); 		\
		__enable_irq();					\
	} while (0);
#else
 #define ENTER_CRITICAL_SECTION( )		EnterCriticalSection()
 #define EXIT_CRITICAL_SECTION( )    	ExitCriticalSection()
 extern void EnterCriticalSection(void);
 extern void ExitCriticalSection(void);
 extern void vMBDelay(ULONG nCount);
#endif

#include "./nos/portctrl.h"
	
#endif	/* MB_PORT_H */
