#define __PERIPHERAL_C__

#include "Typedef.h"
#include "Key.h"
#include "Led.h"
#include "Sys.h"
#include "Peripheral.h"
#include "Wifi.h"
#include "stm8s.h"
#include "Log.h"

static void ADJLuminSmoothRun();
static void ADJIncDecRun();

static void JKInit(void)
{	
	GPIO_Init(GPIOD, GPIO_PIN_7, GPIO_MODE_OUT_PP_HIGH_SLOW);
	
	GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_IN_PU_IT);
	
	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_FALL_ONLY);
	EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_FALL_ONLY);
}
void PeriphralInit(void)
{
	KeyInit();
	LedInit(); 
	JKInit();
	g_ADJ.enable = 0;
	g_ADJ.lumin = 0;
	g_ADJ.ledIncTime = 0;
	g_ADJ.ledDecTime = 0;
	g_ADJ.runTime = WORK_TIME_TOTAL;
	g_ADJ.setTime = WORK_TIME_TOTAL;
}


void ADJHandle(const MSG_t *const pMsg)
{	

//	Log("msgParm:%bx\r\n", (uint8)(pMsg->Param));
	switch (pMsg->Param)
	{
		case LED_SWITCH:
			BrightLevelShow();
			break;
		
		case BRIGHT_VALUE:
			BrightLevelShow();
		 	break;
		
		case LED_SWITCH_BRIGHT_VALUE:
			BrightLevelShow();
			break;
		
		default:
			BrightLevelShow();
			break;
	}
}
sysServerTO_t ADJServer(void)
{
	ADJLuminSmoothRun();
	ADJIncDecRun();
	return ADJ_SERVER_TICK;
}
void ADJWorkSmooth(void)
{
	uint16 tim1Cnt;
	if(g_ADJ.enable)
	{
		tim1Cnt = TIM1_GetCounter();
		if((tim1Cnt > g_ADJ.runTime) && (tim1Cnt < WORK_TIME_TOTAL))
		{
			JK_HIGH();
		}
		else
		{
			JK_LOW();
		}
		Log("  tim1Cnt;%d\r\n", tim1Cnt);
	}
	else
	{
		JK_LOW();
	}
}
static void ADJLuminSmoothRun()
{
	if(g_ADJ.runTime < g_ADJ.setTime)
	{
		if((g_ADJ.runTime + (2*WORK_TIME_UNIT)) < g_ADJ.setTime)
		{
			g_ADJ.runTime += (2*WORK_TIME_UNIT);
		}
		else
		{
			g_ADJ.runTime += WORK_TIME_UNIT;
		}
	}
	
	if(g_ADJ.runTime > g_ADJ.setTime)
	{
		if(g_ADJ.runTime > (g_ADJ.setTime+(2*WORK_TIME_UNIT)))
		{
			g_ADJ.runTime -= (2*WORK_TIME_UNIT);
		}
		else
		{
			g_ADJ.runTime -= WORK_TIME_UNIT;
		}
	}
}
static void ADJIncDecRun()
{
	if(g_ADJ.ledIncTime)
	{
		g_ADJ.ledIncTime--;
		LED_INC_ON();
	}
	else
	{
		LED_INC_OFF();
	}
	
	if(g_ADJ.ledDecTime)
	{
		g_ADJ.ledDecTime--;
		LED_DEC_ON();
	}
	else
	{
		LED_DEC_OFF();
	}
}

