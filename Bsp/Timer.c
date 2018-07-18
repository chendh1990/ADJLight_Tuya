#define __TIMER_C__
#include "Typedef.h"
#include "stm8s.h"
#include "Timer.h"

/************************************************************************************************************
*    Main function 
************************************************************************************************************/
static void Timer1Init(void)
{
	TIM1_DeInit();
	TIM1_TimeBaseInit(15, TIM1_COUNTERMODE_UP, 50000, 0);	//50ms
/*
	TIM1_OC4Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, \
						0, \
						TIM1_OCPOLARITY_HIGH,
						TIM1_OCIDLESTATE_RESET);
*/	
	TIM1_ClearFlag(TIM1_FLAG_UPDATE);
	TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
  	TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
	TIM1_ARRPreloadConfig(ENABLE);
	TIM1_Cmd(ENABLE);
/*
	TIM1_CtrlPWMOutputs(DISABLE);
*/
}
static void Timer2Init(void)
{   	
	TIM2_DeInit();
	TIM2_TimeBaseInit(TIM2_PRESCALER_256, TIMER_10MS);
	TIM2_ClearFlag(TIM2_FLAG_UPDATE);
	TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
  	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
	TIM2_ARRPreloadConfig(ENABLE);
	TIM2_Cmd(ENABLE);
}

void TimerInit(void)
{
   	Timer1Init();
   	Timer2Init();	
}
