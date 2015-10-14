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

#ifndef _PORT_CTRL_H
#define _PORT_CTRL_H

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
#define PR_BEGIN_EXTERN_C           	extern "C" {			
#define	PR_END_EXTERN_C             	}




/*============================ TYPES =========================================*/
/*######--------------------------------######--------------------------------*/
enum mb_sta_ind_t
{
	MB_STA_IND_TRIG = 0,
	MB_STA_IND_TIMEN = 1,
	MB_STA_IND_TIMEOUT = 2,
	MB_STA_IND_UTX = 3,
	MB_STA_IND_URX = 4,
	MB_STA_IND_IT_CFG = 5,
	MB_STA_IND_TIRQ = 6,
	MB_STA_IND_UDIR = 7,
	MB_STA_IND_MAIN = 8,	//X
	MB_STA_IND_IRQ = 9,		//X
	MB_STA_IND_RESV = 10,	//X
	MB_STA_IND_UTXIRQ = 11,
	MB_STA_IND_URXIRQ = 12,
	MB_STA_IND_RESV1 = 13,
	MB_STA_IND_RET = 14,
	MB_STA_IND_ERROR = 15,
};

/*============================ INTERFACE =====================================*/
/*######--------------------------------######--------------------------------*/
#ifdef __cplusplus
 PR_BEGIN_EXTERN_C
#endif
 #define MB_STATUS_IND(__N,__S)		MBMasterStatusIndicate(__N,__S)
 #define MB_STATUS_TOG(__N,__S)		MBMasterStatusToggle(__N,__S)
 #define MB_STATUS_TOG2(__N)		MBMasterNoStatusToggle(__N)
	extern void 					MBMasterStatusIndicate(uint8_t ID, bool bl);
	extern void 					MBMasterStatusToggle(uint8_t ID, bool bl);
	extern void 					MBMasterNoStatusToggle(uint8_t ID);
#ifdef __cplusplus
 PR_END_EXTERN_C
#endif

	
#endif
