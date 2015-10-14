/**
  ******************************************************************************
  * @file              : porttype.h
  * @author            : HZ Zhiyix Team
  * @version           : V1.1.0.0
  * @date              : 2015-10-13
  * @brief             : h file
  * @description       : 定义Freemodbus协议 PORT层的数据类型
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 STMicroelectronics
  *
  *
  ******************************************************************************
  */

#ifndef MB_PORTTYPE_H
#define MB_PORTTYPE_H

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

/* ----------------------- Defines -------------------------------------------*/
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
#endif
	
#if (false)
 #define assert(expr)					assert_param(expr)
#else
 #define assert(expr)					(void)0
#endif
	
#if (false)
 #define xMB_Output(expr)				printf(expr)
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

typedef enum
{
    mb_rt_err          = -1,    		//!< fsm error, error code can be get from other interface
    mb_rt_cpl          = 0,     		//!< fsm complete
    mb_rt_on_going     = 1,     		//!< fsm on-going
    mb_rt_wait_for_obj = 2,     		//!< fsm wait for object
    mb_rt_asyn         = 3,     		//!< fsm asynchronose complete, you can check it later.
} xMBRetStatus;


extern int xMBOutput(CHAR deg, const char *format, ...);

#endif	/* MB_PORTTYPE_H */
