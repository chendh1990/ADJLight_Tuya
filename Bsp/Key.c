#define __KEY_C__
#include "Key.h"
#include "Sys.h"
#include "Peripheral.h"
#include "Wifi.h"
#include "Log.h"

void KeyInit(void)
{

}
static uint8 ReadKey(void)
{
	uint8 key = 0x00;
	/*

	if(KEY_K1_PIN == 0)
	{
		key |= KEY_K1;
	}
	if(KEY_K2_PIN == 0)
	{
		key |= KEY_K2;
	}
	if(KEY_K3_PIN == 0)
	{
		key |= KEY_K3;
	}
	*/
	return key;
}

static uint8 KeyDetect(void)
{
	uint8 key;
	key = ReadKey();

	if(key == 0x00) 
	{
		key = NO_KEY;
	}
	return key;
}

sysServerTO_t KeyScanServer(void)
{
    static uint8 keyValue = 0;
	static uint16 keyCounter = 0;
    uint8 keyTmp = NO_KEY;
    uint8 key_return = NO_KEY;
	
    keyTmp = KeyDetect();
	if(keyTmp == NO_KEY )
	{
		if (keyCounter >= KEY_LONG_TIMES)  //长按抬键
		{
			key_return = keyValue | KEY_LONG_UP;
		}
		else if (keyCounter >= KEY_SCAN_TIMES)   //短按抬键
		{
			key_return = keyValue | KEY_SHORT_UP;
		}
		keyValue = keyTmp;
		keyCounter = 0;
	}
    else if(keyTmp != keyValue)
    {
		keyValue = keyTmp;
		keyCounter = 0;
    }	
    else
    {
		keyCounter++;
		if (keyCounter == KEY_SCAN_TIMES)			//去抖
		{
			key_return = keyValue;
		}
		else if (keyCounter == KEY_LONG_TIMES)				//长按
		{
			key_return = keyValue | KEY_LONG;
		}
		else if (keyCounter == (KEY_LONG_TIMES + KEY_HOLD_TIMES) )			//连按
		{
			key_return = keyValue | KEY_HOLD;
			keyCounter = KEY_LONG_TIMES;
		}
    }
    if (key_return != NO_KEY)
    {
		QMsgPostSimple(&g_QMsg, SYS_MSG_KEY_ID, key_return);
    }
	return KEY_SCAN_SERVER_TICK;
}
void KeyHandle(const MSG_t *const pMsg)
{
	if(!pMsg)
	{
		return ;
	}
	//Log("msgParm:%bx\r\n", (uint8)(pMsg->Param));
	switch (pMsg->Param)
	{
		case PS_K1:				//打开
			Log("PS_K1\r\n");
			break;
			
		case PS_K2:				//暂停
			Log("PS_K2\r\n");
			break;
			
		case PS_K3:				//关闭
			Log("PS_K3\r\n");	
			break;
			
		case PL_K1:				//smartconfig
			Log("PL_K1\r\n");
		//	QMsgPostSimple(&g_QMsg, SYS_MSG_WIFI_ID, WIFI_SMART_CONFIG);
			break;
			
		case PL_K2:				//smartconfig
			Log("PL_K2\r\n");
		//	QMsgPostSimple(&g_QMsg, SYS_MSG_WIFI_ID, WIFI_SMART_CONFIG);
			break;
			
		case PL_K3:				//smartconfig
			Log("PL_K3\r\n");
		//	QMsgPostSimple(&g_QMsg, SYS_MSG_WIFI_ID, WIFI_SMART_CONFIG);
			break;
			
		default:
			break;
	}
}

