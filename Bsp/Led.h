#ifndef __LED_H__
#define __LED_H__

#include "Peripheral.h"
#include "Typedef.h"
#include "stm8s.h"


#ifndef __LED_C__
	#define LED_EXTERN	extern
#else
	#define LED_EXTERN
#endif


/**************************宏定义****************************/
#define LED_SHOW_SERVER_TICK		10

#define LED_FLASH_ALWAYS			0xff


/***********************数据结构定义************************/
typedef struct{
	uint16 timerOn;		//开时间
	uint16 timerOff;	//关时间
	uint16 cnt;			//计数器
	uint16 count;		//闪烁次数
}Led_Status_t;

typedef enum{
	LED_INC_ID = 0,
	LED_DEC_ID,
	LED_POWER_ID,
	LED_MAX_ID
}Led_Id_t;

/**********************全局变量定义与定义***********************/
#define BRIGHT_RADIX		(13)
#define BRIGHT_LEVEL_0		(0)
#define BRIGHT_LEVEL_1		(BRIGHT_RADIX*1)
#define BRIGHT_LEVEL_2		(BRIGHT_RADIX*2)
#define BRIGHT_LEVEL_3		(BRIGHT_RADIX*3)
#define BRIGHT_LEVEL_4		(BRIGHT_RADIX*4)
#define BRIGHT_LEVEL_5		(BRIGHT_RADIX*5)
#define BRIGHT_LEVEL_6		(BRIGHT_RADIX*6)
#define BRIGHT_LEVEL_7		(BRIGHT_RADIX*7)
#define BRIGHT_LEVEL_8		(BRIGHT_RADIX*8)
#define BRIGHT_LEVEL_9		(BRIGHT_RADIX*9)
#define BRIGHT_LEVEL_10		(BRIGHT_RADIX*10)
#define BRIGHT_LEVEL_11		(BRIGHT_RADIX*11)
#define BRIGHT_LEVEL_12		(BRIGHT_RADIX*12)
#define BRIGHT_LEVEL_13		(BRIGHT_RADIX*13)
#define BRIGHT_LEVEL_14		(BRIGHT_RADIX*14)
#define BRIGHT_LEVEL_15		(BRIGHT_RADIX*15)
#define BRIGHT_LEVEL_16		(BRIGHT_RADIX*16)
#define BRIGHT_LEVEL_17		(BRIGHT_RADIX*17)
#define BRIGHT_LEVEL_18		(BRIGHT_RADIX*18)
#define BRIGHT_LEVEL_19		(BRIGHT_RADIX*19)


/**********************函数声明**************************/
LED_EXTERN void LedInit(void);

LED_EXTERN sysServerTO_t LedShowServer(void);

LED_EXTERN void BrightLevelShowFlash(uint8 id, uint8 count, uint16 on_time, uint16 off_time);    

LED_EXTERN void LedSetLevel(uint8 id, uint8 level, uint8 flag);
LED_EXTERN void BrightLevelShow(void);

#endif
