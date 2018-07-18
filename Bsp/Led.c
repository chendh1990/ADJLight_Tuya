#define __LED_C__
#include "Led.h"
#include "Sys.h"
#include "Log.h"

static Led_Status_t  s_LedStatus[LED_MAX_ID];

void LedInit(void)
{
	uint8 i;
	GPIO_Init(GPIOA, GPIO_PIN_1, GPIO_MODE_OUT_PP_HIGH_SLOW);
	GPIO_Init(GPIOA, GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_SLOW);
	GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_SLOW);
	
	GPIO_Init(GPIOB, GPIO_PIN_ALL, GPIO_MODE_OUT_PP_HIGH_SLOW);

	GPIO_Init(GPIOC, GPIO_PIN_ALL, GPIO_MODE_OUT_PP_HIGH_SLOW);

	GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_HIGH_SLOW);
	GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_SLOW);
	GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_SLOW);

	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_SLOW);

	GPIO_Init(GPIOF, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_SLOW);

	for(i = 0; i < LED_MAX_ID; i++)
	{
	    s_LedStatus[i].count = 0;
	    s_LedStatus[i].timerOn = 0;
	    s_LedStatus[i].timerOff = 0;
	    s_LedStatus[i].cnt = 0;
	}
}

sysServerTO_t LedShowServer(void)
{
	uint8 i;
	for(i = 0; i < LED_MAX_ID; i++)
	{
		if(s_LedStatus[i].cnt > 0)
		{
		    s_LedStatus[i].cnt--;
		    if(s_LedStatus[i].cnt == s_LedStatus[i].timerOff)
			{
		        LedSetLevel(i, LOW, false);
			}
		}
		else
		{
		    if(s_LedStatus[i].count == 0xFF)
		    {
		        LedSetLevel(i, HIGH, false);
		        s_LedStatus[i].cnt = s_LedStatus[i].timerOn + s_LedStatus[i].timerOff;
		    }
		    else if(s_LedStatus[i].count > 0)
		    {
		        LedSetLevel(i, HIGH, false);
		        s_LedStatus[i].count--;
		        s_LedStatus[i].cnt = s_LedStatus[i].timerOn + s_LedStatus[i].timerOff;
		    }
		}
	}
	return LED_SHOW_SERVER_TICK;
}


//count:闪烁次数,	其中count = 0xff为连续
//on_time:开时间,
//off_time关时间(单位10ms),
void LedSetFlash(uint8 id, uint8 count, uint16 ontime, uint16 offtime)     
{
	if(id > LED_MAX_ID)
	{
		return;
	}	
    s_LedStatus[id].count = count;
    s_LedStatus[id].timerOn = ontime;
    s_LedStatus[id].timerOff = offtime;
    s_LedStatus[id].cnt = 0;	
    LedSetLevel(id, LOW, false);
}


void LedSetLevel(uint8 id, uint8 level, uint8 flag)
{
	if(id > LED_MAX_ID)
	{
		return ;
	}
	switch (id)
	{
		case LED_INC_ID:
			if(level)
		    {
		    	LED_INC_ON();
		    }
		    else
			{
		    	LED_INC_OFF();
			}
			break;
			
		case LED_DEC_ID:
			if(level)
		    {
		    	LED_DEC_ON() ;
		    }
		    else
			{
		    	LED_DEC_OFF();
			}
			break;
			
		case LED_POWER_ID:
			if(level)
		    {
		    	LED_POWER_ON() ;
		    }
		    else
			{
		    	LED_POWER_OFF();
			}
			break;
		default:
			break;
	}
	if(flag)
	{
		s_LedStatus[id].count = 0;
		s_LedStatus[id].timerOn = 0;
		s_LedStatus[id].timerOff = 0;
		s_LedStatus[id].cnt = 0;	
	}
}

void BrightLevelShow(void)
{
	LED0_OFF();
	LED1_OFF();
	LED2_OFF();
	LED3_OFF();
	LED4_OFF();
	LED5_OFF();
	LED6_OFF();
	LED7_OFF();
	LED8_OFF();
	LED9_OFF();
	LED10_OFF();
	LED11_OFF();
	LED12_OFF();
	LED13_OFF();
	LED14_OFF();
	LED15_OFF();
	LED16_OFF();
	LED17_OFF();
	LED18_OFF();
	LED19_OFF();
	LED4_OFF();
	LED4_OFF();
	LED_POWER_OFF();
	if(g_ADJ.enable == 0)
	{
		return ;
	}
	LED_POWER_ON();
	//if(g_ADJ.lumin >= BRIGHT_LEVEL_0)
	{
		LED0_ON();
	}
	if(g_ADJ.lumin >= BRIGHT_LEVEL_1)
	{
		LED1_ON();
	}
	if(g_ADJ.lumin >= BRIGHT_LEVEL_2)
	{
		LED2_ON();
	}
	if(g_ADJ.lumin >=BRIGHT_LEVEL_3)
	{
		LED3_ON();
	}
	if(g_ADJ.lumin >= BRIGHT_LEVEL_4)
	{
		LED4_ON();
	}
	if(g_ADJ.lumin >= BRIGHT_LEVEL_5)
	{
		LED5_ON();
	}
	if(g_ADJ.lumin >= BRIGHT_LEVEL_6)
	{
		LED6_ON();
	}
	if(g_ADJ.lumin >= BRIGHT_LEVEL_7)
	{
		LED7_ON();
	}
	if(g_ADJ.lumin >= BRIGHT_LEVEL_8)
	{
		LED8_ON();
	}
	if(g_ADJ.lumin >= BRIGHT_LEVEL_9)
	{
		LED9_ON();
	}
	if(g_ADJ.lumin >= BRIGHT_LEVEL_10)
	{
		LED10_ON();
	}
	if(g_ADJ.lumin >= BRIGHT_LEVEL_11)
	{
		LED11_ON();
	}
	if(g_ADJ.lumin >= BRIGHT_LEVEL_12)
	{
		LED12_ON();
	}
	if(g_ADJ.lumin >= BRIGHT_LEVEL_13)
	{
		LED13_ON();
	}
	if(g_ADJ.lumin >= BRIGHT_LEVEL_14)
	{
		LED14_ON();
	}
	if(g_ADJ.lumin >= BRIGHT_LEVEL_15)
	{
		LED15_ON();
	}
	if(g_ADJ.lumin >= BRIGHT_LEVEL_16)
	{
		LED16_ON();
	}
	if(g_ADJ.lumin >= BRIGHT_LEVEL_17)
	{
		LED17_ON();
	}
	if(g_ADJ.lumin >= BRIGHT_LEVEL_18)
	{
		LED18_ON();
	}
	if(g_ADJ.lumin >= BRIGHT_LEVEL_19)
	{
		LED19_ON();
	}
}
