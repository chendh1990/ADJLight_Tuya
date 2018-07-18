#ifndef __PERIPHERAL_H__
#define __PERIPHERAL_H__
#include "TimerUnit.h"
#include "QMsg.h"
#include "stm8s.h"

#ifndef __PERIPHERAL_C__
	#define PERIPHERAL_EXTERN extern
#else
	#define PERIPHERAL_EXTERN
#endif

/***********************宏定义****************************/
#ifndef ON
#define ON					1
#endif
#ifndef OFF
#define OFF					0
#endif

#define KEY_K1_PIN			
#define KEY_K2_PIN			
#define KEY_K3_PIN			

#define JK_HIGH()			GPIO_WriteHigh(GPIOD, GPIO_PIN_7);
#define JK_LOW()			GPIO_WriteLow(GPIOD, GPIO_PIN_7);

#define LED_INC_ON() 		GPIO_WriteLow(GPIOA, GPIO_PIN_3)
#define LED_INC_OFF() 		GPIO_WriteHigh(GPIOA, GPIO_PIN_3)

#define LED_DEC_ON() 		GPIO_WriteLow(GPIOA, GPIO_PIN_2)
#define LED_DEC_OFF() 		GPIO_WriteHigh(GPIOA, GPIO_PIN_2)

#define LED_POWER_ON() 		GPIO_WriteLow(GPIOC, GPIO_PIN_1)
#define LED_POWER_OFF()		GPIO_WriteHigh(GPIOC, GPIO_PIN_1)


#define LED0_OFF()		GPIO_WriteHigh(GPIOA, GPIO_PIN_1)
#define LED1_OFF()		GPIO_WriteHigh(GPIOD, GPIO_PIN_3)
#define LED2_OFF()		GPIO_WriteHigh(GPIOD, GPIO_PIN_2)
#define LED3_OFF()		GPIO_WriteHigh(GPIOD, GPIO_PIN_0)
#define LED4_OFF()		GPIO_WriteHigh(GPIOC, GPIO_PIN_7)
#define LED5_OFF()		GPIO_WriteHigh(GPIOC, GPIO_PIN_6)
#define LED6_OFF()		GPIO_WriteHigh(GPIOC, GPIO_PIN_5)
#define LED7_OFF()		GPIO_WriteHigh(GPIOC, GPIO_PIN_4)
#define LED8_OFF()		GPIO_WriteHigh(GPIOC, GPIO_PIN_3)
#define LED9_OFF()		GPIO_WriteHigh(GPIOC, GPIO_PIN_2)
#define LED10_OFF()		GPIO_WriteHigh(GPIOE, GPIO_PIN_5)
#define LED11_OFF()		GPIO_WriteHigh(GPIOB, GPIO_PIN_0)
#define LED12_OFF()		GPIO_WriteHigh(GPIOB, GPIO_PIN_1)
#define LED13_OFF()		GPIO_WriteHigh(GPIOB, GPIO_PIN_2)
#define LED14_OFF()		GPIO_WriteHigh(GPIOB, GPIO_PIN_3)
#define LED15_OFF()		GPIO_WriteHigh(GPIOB, GPIO_PIN_4)
#define LED16_OFF()		GPIO_WriteHigh(GPIOB, GPIO_PIN_5)
#define LED17_OFF()		GPIO_WriteHigh(GPIOB, GPIO_PIN_6)
#define LED18_OFF()		GPIO_WriteHigh(GPIOB, GPIO_PIN_7)
#define LED19_OFF()		GPIO_WriteHigh(GPIOF, GPIO_PIN_4)


#define LED0_ON()		GPIO_WriteLow(GPIOA, GPIO_PIN_1)
#define LED1_ON()		GPIO_WriteLow(GPIOD, GPIO_PIN_3)
#define LED2_ON()		GPIO_WriteLow(GPIOD, GPIO_PIN_2)
#define LED3_ON()		GPIO_WriteLow(GPIOD, GPIO_PIN_0)
#define LED4_ON()		GPIO_WriteLow(GPIOC, GPIO_PIN_7)
#define LED5_ON()		GPIO_WriteLow(GPIOC, GPIO_PIN_6)
#define LED6_ON()		GPIO_WriteLow(GPIOC, GPIO_PIN_5)
#define LED7_ON()		GPIO_WriteLow(GPIOC, GPIO_PIN_4)
#define LED8_ON()		GPIO_WriteLow(GPIOC, GPIO_PIN_3)
#define LED9_ON()		GPIO_WriteLow(GPIOC, GPIO_PIN_2)
#define LED10_ON()		GPIO_WriteLow(GPIOE, GPIO_PIN_5)
#define LED11_ON()		GPIO_WriteLow(GPIOB, GPIO_PIN_0)
#define LED12_ON()		GPIO_WriteLow(GPIOB, GPIO_PIN_1)
#define LED13_ON()		GPIO_WriteLow(GPIOB, GPIO_PIN_2)
#define LED14_ON()		GPIO_WriteLow(GPIOB, GPIO_PIN_3)
#define LED15_ON()		GPIO_WriteLow(GPIOB, GPIO_PIN_4)
#define LED16_ON()		GPIO_WriteLow(GPIOB, GPIO_PIN_5)
#define LED17_ON()		GPIO_WriteLow(GPIOB, GPIO_PIN_6)
#define LED18_ON()		GPIO_WriteLow(GPIOB, GPIO_PIN_7)
#define LED19_ON()		GPIO_WriteLow(GPIOF, GPIO_PIN_4)



#define ADJ_SERVER_TICK		(1)

#define WORK_TIME_TOTAL		(9945)
#define WORK_TIME_UNIT		(39)

/***********************数据结构定义************************/

typedef enum{
	LED_SWITCH = 0x00,
	BRIGHT_VALUE,
	LED_SWITCH_BRIGHT_VALUE,
}ADJ_State_t;

/********************全局变量声明与定义***********************/


PERIPHERAL_EXTERN void PeriphralInit(void);
PERIPHERAL_EXTERN void ADJHandle(const MSG_t *const pMsg);
PERIPHERAL_EXTERN sysServerTO_t ADJServer(void);
PERIPHERAL_EXTERN void ADJWorkSmooth(void);


#endif

