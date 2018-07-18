#ifndef __COMMON_H__
#define __COMMON_H__
#include "Typedef.h"


#ifndef __COMMON_C__
	#define COMMON_EXTERN	extern
#else
	#define COMMON_EXTERN
#endif

/***********************宏定义****************************/

#define ENTER_CRITICAL()	//clr_EA
#define EXIT_CRITICAL()		//set_EA

#define FW_VER				0001




/***************************函数声明***************************/
COMMON_EXTERN uint8 getCheckSum(uint8 const *const pBuffer, uint8 length);
COMMON_EXTERN uint8 getHex(uint8 dat);
COMMON_EXTERN uint8 hex_to_bcd(uint8 Value_H, uint8 Value_L);
COMMON_EXTERN uint16 my_strlen(uint8 const *str);
//COMMON_EXTERN void *my_memset(void *src, uint8 ch, uint16 count);
COMMON_EXTERN void *my_memcpy(void *dest, const void *src, uint16 count);

#endif
