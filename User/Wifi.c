#define __WIFI_C__
#include "Wifi.h"
#include "PacketQueue.h"
#include "Common.h"
#include "Uart.h"
#include "Sys.h"
#include "Peripheral.h"
#include "Led.h"
#include "Log.h"
#include "stm8s.h"


static WifiPacketCfg_t  s_WifiPacketCfg;
#if defined(UART_PROTOCOL_TUYA)
static Wifi_t  s_Wifi;

static DownloadCmd_t const  s_DownloadCmd[] =
{
	{DPID_LED_SWITCH, DP_TYPE_BOOL},
	{DPID_BRIGHT_VALUE, DP_TYPE_VALUE},
};
#endif	
static void WifiPacketInit(void);
static void WifiPacketHandle(void);

#if defined(UART_PROTOCOL_TUYA)
static uint8 GetDownloadCmdTotal(void);
static unsigned char GetDowmloadDataPointIdIndex(uint8 dpid);
static uint8 DownloadDataPointHandle(const uint8  *pVal, uint8  dpid, uint8  length);
static uint8 OneDataPointHand(uint8 const * pDPVal);
static void AllDataPointHand(void);
static void BoolDataPointUpdate(uint8  *const pbuffer, uint8  dpid, uint8  value);
static void ValueDataPointUpdate(uint8  *const pbuffer, uint8  dpid, uint32 value);
static void AllDataUpdate(void);
static void HeatBeatCheck(void);
static void ProductInfoUpdate(void);
static void WIFIWortModeSet(void);
static void AllDataUpdate(void);
#endif
static void SetTxPacketData(uint8 const* const src, uint8  length);
static void SendTxPacketData(uint8  cmd);


void WifiInit(void)
{
#if defined(UART_PROTOCOL_TUYA)
	s_Wifi.CurrState = WIFI_SATE_UNKNOW;
	s_Wifi.BackState = WIFI_SATE_UNKNOW;
	s_Wifi.resetFlag = RESET_WIFI_SUCCESS;
	s_Wifi.setModeFlag = SET_WIFICONFIG_SUCCESS;
#endif
	WifiPacketInit();
}
static void WifiPacketInit(void)
{
	s_WifiPacketCfg.rxCnt = 0;
	s_WifiPacketCfg.rxStatus = RX_IDDLE;
	s_WifiPacketCfg.rxPack.head_0 = P_HEAD_0;
	s_WifiPacketCfg.rxPack.head_1 = P_HEAD_1;
#if defined(UART_PROTOCOL_TUYA)	
	s_WifiPacketCfg.rxPack.ver = P_VER;
#endif	
	s_WifiPacketCfg.txCnt = 0;
	s_WifiPacketCfg.txStatus = RX_IDDLE;
	s_WifiPacketCfg.txPack.head_0 = P_HEAD_0;
	s_WifiPacketCfg.txPack.head_1 = P_HEAD_1;
#if defined(UART_PROTOCOL_TUYA)	
	s_WifiPacketCfg.txPack.ver = P_VER;
#endif	
	s_WifiPacketCfg.txLength = 0;
}
void WifiPacketParse(void)
{
  	uint8  dat;
	uint8  checkSum;
	if(QueuePend(&dat) != 0)
	{	
		return ;
	}
//	putchar(getHex(HI_UINT8(dat)));
//	putchar(getHex(LO_UINT8(dat)));
//	putchar(' ');
#ifdef UART_PROTOCOL_HZ
	switch(s_WifiPacketCfg.rxStatus)
	{
		case RX_IDDLE:
		{
			if(dat == P_HEAD_0)
			{
				s_WifiPacketCfg.rxStatus = RX_HEAD_0;
			}
			break;
		}

		case RX_HEAD_0:
		{
			if(dat == P_HEAD_1)
			{
				s_WifiPacketCfg.rxStatus = RX_CMD;
			}
			else
			{
				s_WifiPacketCfg.rxStatus = RX_IDDLE;
			}
			break;
		}	
		case RX_CMD:
		{
			s_WifiPacketCfg.rxPack.cmd = dat; //LENGTH0
			s_WifiPacketCfg.rxLength = 2;
			s_WifiPacketCfg.rxStatus = RX_BUSY;
			break;
		}
		case RX_BUSY:
		{
		//	putchar('7');
		//	putchar('\r');
		//	putchar('\n');
			s_WifiPacketCfg.rxPack.dat[s_WifiPacketCfg.rxCnt++] = dat;
			if(s_WifiPacketCfg.rxCnt >= (s_WifiPacketCfg.rxLength))
			{
				s_WifiPacketCfg.rxStatus = RX_VERIFY;
			};
			break;
		}
		case RX_VERIFY:
		{
			//putchar('8');
			//putchar('\r');
			//putchar('\n');
			//putchar(getHex(HI_UINT8(dat)));
			//putchar(getHex(LO_UINT8(dat)));
			//putchar('\r');
			//putchar('\n');

			checkSum = getCheckSum((uint8  *)(&(s_WifiPacketCfg.rxPack.head_0)), s_WifiPacketCfg.rxCnt+3);
		//	checkSum = SUM_HEAD_VER;
			if(checkSum == dat)
			{
			//	uint8  s[] =  "+verify ok!\r\n";
				//	UartSendHexBuffer(UART_WIFI, "+results:", (uint8  *)(&(checkSum)), 1, ' ', true);
			//	UartSendString(UART_WIFI, s);
				WifiPacketHandle();
			//	UartSendHexBuffer(UART_WIFI, "+raw:", (uint8  *)(&(s_WifiPacketCfg.rxPack.cmd)), s_WifiPacketCfg.rxCnt+3, ' ', true);
			
			}
			else
			{
			//	uint8  s[] =  "-verify fail!\r\n";
			//	UartSendHexBuffer(UART_WIFI, "-results:", (uint8  *)(&(checkSum)), 1, ' ', true);
			//	checkSum = s_WifiPacketCfg.rxPack.dat[0] + s_WifiPacketCfg.rxPack.dat[1] + s_WifiPacketCfg.rxPack.dat[2];
			//	UartSendHexBuffer(UART_WIFI, "--results:", (uint8  *)(&(checkSum)), 1, ' ', true);
			//	UartSendString(UART_WIFI, s);
			//	UartSendHexBuffer(UART_WIFI, "-raw:", (uint8  *)(&(s_WifiPacketCfg.rxPack.cmd)), s_WifiPacketCfg.rxCnt+3, ' ', true);
			}
			s_WifiPacketCfg.rxStatus = RX_IDDLE;
			s_WifiPacketCfg.rxCnt = 0;
			break;
		}
		default:
		{
			s_WifiPacketCfg.rxStatus = RX_IDDLE;
			s_WifiPacketCfg.rxCnt = 0;
			break;
		}
	} 
#elif defined(UART_PROTOCOL_TUYA)
	switch(s_WifiPacketCfg.rxStatus)
	{
		case RX_IDDLE:
		{
			if(dat == P_HEAD_0)
			{
				s_WifiPacketCfg.rxStatus = RX_HEAD_0;
			}
			break;
		}

		case RX_HEAD_0:
		{
			if(dat == P_HEAD_1)
			{
				s_WifiPacketCfg.rxStatus = RX_HEAD_1;
			}
			else
			{
				s_WifiPacketCfg.rxStatus = RX_IDDLE;
			}
			break;
		}	
		case RX_HEAD_1:
		{
			if(dat == P_VER)
			{
				s_WifiPacketCfg.rxStatus = RX_VER;
			}
			else
			{
				s_WifiPacketCfg.rxStatus = RX_IDDLE;
			}
			break;
		}
		case RX_VER:
		{
			s_WifiPacketCfg.rxPack.cmd = dat;	//CMD
			s_WifiPacketCfg.rxStatus = RX_CMD;
			break;
		}
		case RX_CMD:
		{
			s_WifiPacketCfg.rxPack.length[0] = dat;	//LENGTH0
			s_WifiPacketCfg.rxStatus = RX_LEN_0;

			break;
		}
		case RX_LEN_0:
		{
			s_WifiPacketCfg.rxPack.length[1] = dat;	//LENGTH1
			s_WifiPacketCfg.rxLength = BUILD_UINT16(s_WifiPacketCfg.rxPack.length[1], s_WifiPacketCfg.rxPack.length[0]);
			if(s_WifiPacketCfg.rxLength <= (UART_PACKET_DAT_MAX))
			{
				if(s_WifiPacketCfg.rxLength != 0)
				{
					s_WifiPacketCfg.rxStatus = RX_BUSY;
				}
				else
				{
					s_WifiPacketCfg.rxStatus = RX_VERIFY;
				}
			}
			else
			{
				s_WifiPacketCfg.rxStatus = RX_IDDLE;	
				s_WifiPacketCfg.rxCnt = 0;
				s_WifiPacketCfg.rxLength = 0;
			}
			break;
		}
		case RX_BUSY:
		{
		//	putchar('7');
		//	putchar('\r');
		//	putchar('\n');
			s_WifiPacketCfg.rxPack.dat[s_WifiPacketCfg.rxCnt++] = dat;
			if(s_WifiPacketCfg.rxCnt >= (s_WifiPacketCfg.rxLength))
			{
				s_WifiPacketCfg.rxStatus = RX_VERIFY;
			};
			break;
		}
		case RX_VERIFY:
		{
		//	putchar('8');
		//	putchar('\r');
		//	putchar('\n');
		//	putchar(getHex(HI_UINT8(dat)));
		//	putchar(getHex(LO_UINT8(dat)));
		//	putchar('\r');
		//	putchar('\n');

			checkSum = getCheckSum((uint8  *)(&(s_WifiPacketCfg.rxPack.head_0)), s_WifiPacketCfg.rxCnt+6);
		//	checkSum = SUM_HEAD_VER;
			if(checkSum == dat)
			{
			//	uint8  s[] =  "+verify ok!\r\n";
				//	UartSendHexBuffer(UART_WIFI, "+results:", (uint8  *)(&(checkSum)), 1, ' ', true);
			//	UartSendString(UART_WIFI, s);
				WifiPacketHandle();
			//	UartSendHexBuffer(UART_WIFI, "+raw:", (uint8  *)(&(s_WifiPacketCfg.rxPack.cmd)), s_WifiPacketCfg.rxCnt+3, ' ', true);
			
			}
			else
			{
			//	uint8  s[] =  "-verify fail!\r\n";
			//	UartSendHexBuffer(UART_WIFI, "-results:", (uint8  *)(&(checkSum)), 1, ' ', true);
			//	checkSum = s_WifiPacketCfg.rxPack.dat[0] + s_WifiPacketCfg.rxPack.dat[1] + s_WifiPacketCfg.rxPack.dat[2];
			//	UartSendHexBuffer(UART_WIFI, "--results:", (uint8  *)(&(checkSum)), 1, ' ', true);
			//	UartSendString(UART_WIFI, s);
			//	UartSendHexBuffer(UART_WIFI, "-raw:", (uint8  *)(&(s_WifiPacketCfg.rxPack.cmd)), s_WifiPacketCfg.rxCnt+3, ' ', true);
			}
			s_WifiPacketCfg.rxStatus = RX_IDDLE;
			s_WifiPacketCfg.rxCnt = 0;
			break;
		}
		default:
		{
			s_WifiPacketCfg.rxStatus = RX_IDDLE;
			s_WifiPacketCfg.rxCnt = 0;
			break;
		}
	} 
#endif
}
/*****************************************************************************
函数名称 : data_handle
功能描述 : 数据帧处理
输入参数 : offset:数据起始位
返回参数 : 无
*****************************************************************************/
void WifiPacketHandle(void)
{
#ifdef SUPPORT_MCU_FIRM_UPDATE
  unsigned char *firmware_addr;
  static unsigned long firm_length;                                             //MCU升级文件长度
  static unsigned char firm_update_flag;                                        //MCU升级标志
  unsigned long dp_len;
#else
//  unsigned short dp_len;
#endif
  
  //unsigned char ret;
  //unsigned short i,total_len;
#ifdef WIFI_TEST_ENABLE
  //unsigned char result;
  //unsigned char rssi;
#endif

#if defined(UART_PROTOCOL_TUYA)
  switch(s_WifiPacketCfg.rxPack.cmd)
  {
  case HEAT_BEAT_CMD:                                   //心跳包
	HeatBeatCheck();
	SendTxPacketData(HEAT_BEAT_CMD);
    break;
 
  case PRODUCT_INFO_CMD:                                //产品信息
    ProductInfoUpdate();
  	SendTxPacketData(PRODUCT_INFO_CMD);
    break;
 
  case WORK_MODE_CMD:                                   //查询MCU设定的模块工作模式
	WIFIWortModeSet();
	SendTxPacketData(WORK_MODE_CMD);
    break;
 
  case WIFI_STATE_CMD:                                  //wifi工作状态	
    s_Wifi.CurrState = s_WifiPacketCfg.rxPack.dat[0];
    SendTxPacketData(WIFI_STATE_CMD);
    break;

  case WIFI_RESET_CMD:                                  //重置wifi(wifi返回成功)
    s_Wifi.resetFlag = RESET_WIFI_SUCCESS;
    break;
   
  case WIFI_MODE_CMD:                                   //选择smartconfig/AP模式(wifi返回成功)	
    s_Wifi.setModeFlag = SET_WIFICONFIG_SUCCESS;
    break;

  case DATA_QUERT_CMD:                                  //命令下发
	AllDataPointHand();
	AllDataUpdate();
	SendTxPacketData(STATE_UPLOAD_CMD);
    break;

  case STATE_QUERY_CMD:                                 //状态查询
    AllDataUpdate();
	SendTxPacketData(STATE_UPLOAD_CMD);
    break;
 #if 0  
#ifdef SUPPORT_MCU_FIRM_UPDATE
  case UPDATE_START_CMD:                                //升级开始
    firm_length = wifi_uart_rx_buf[offset + DATA_START];
    firm_length <<= 8;
    firm_length |= wifi_uart_rx_buf[offset + DATA_START + 1];
    firm_length <<= 8;
    firm_length |= wifi_uart_rx_buf[offset + DATA_START + 2];
    firm_length <<= 8;
    firm_length |= wifi_uart_rx_buf[offset + DATA_START + 3];
    //
    wifi_uart_write_frame(UPDATE_START_CMD,0);
    firm_update_flag = UPDATE_START_CMD;
     break;
    
  case UPDATE_TRANS_CMD:                                //升级传输
    if(firm_update_flag == UPDATE_START_CMD)
    {
      //停止一切数据上报
      stop_update_flag = ENABLE;                                                 
      
      total_len = wifi_uart_rx_buf[offset + LENGTH_HIGH] * 0x100;
      total_len += wifi_uart_rx_buf[offset + LENGTH_LOW];
      
      dp_len = wifi_uart_rx_buf[offset + DATA_START];
      dp_len <<= 8;
      dp_len |= wifi_uart_rx_buf[offset + DATA_START + 1];
      dp_len <<= 8;
      dp_len |= wifi_uart_rx_buf[offset + DATA_START + 2];
      dp_len <<= 8;
      dp_len |= wifi_uart_rx_buf[offset + DATA_START + 3];
      
      firmware_addr = (unsigned char *)wifi_uart_rx_buf;
      firmware_addr += (offset + DATA_START + 4);
      if((total_len == 4) && (dp_len == firm_length))
      {
        //最后一包
        ret = mcu_firm_update_handle(firmware_addr,dp_len,0);
        
        firm_update_flag = 0;
      }
      else if((total_len - 4) <= FIRM_UPDATA_SIZE)
      {
        ret = mcu_firm_update_handle(firmware_addr,dp_len,total_len - 4);
      }
      else
      {
        firm_update_flag = 0;
        ret = ERROR;
      }
      
      if(ret == SUCCESS)
      {
        wifi_uart_write_frame(UPDATE_TRANS_CMD,0);
      }
      //恢复一切数据上报
      stop_update_flag = DISABLE;    
    }
    break;
#endif      
    
#ifdef SUPPORT_MCU_RTC_CHECK
  case GET_LOCAL_TIME_CMD:                             //获取本地时间
      mcu_write_rtctime(wifi_uart_rx_buf + offset + DATA_START);
    break;
#endif
 
#ifdef WIFI_TEST_ENABLE
  case WIFI_TEST_CMD:                                   //wifi功能测试
    result = wifi_uart_rx_buf[offset + DATA_START];
    rssi = wifi_uart_rx_buf[offset + DATA_START + 1];
    wifi_test_result(result, rssi);
    break;
#endif
#endif 
  default:
    break;
 }
 #elif defined(UART_PROTOCOL_HZ)
	switch (s_WifiPacketCfg.rxPack.cmd)
	{
		case LED_SWITCH_BRIGHT_CTR_CMD:
			if(g_ADJ.lumin < s_WifiPacketCfg.rxPack.dat[0])
			{
				g_ADJ.ledIncTime = LED_INC_TIME;
				g_ADJ.ledDecTime = 0; 
			}
			else if(g_ADJ.lumin > s_WifiPacketCfg.rxPack.dat[0])
			{
				g_ADJ.ledIncTime = 0;
				g_ADJ.ledDecTime = LED_INC_TIME; 
			}
			else
			{
				g_ADJ.ledIncTime = 0;
				g_ADJ.ledDecTime = 0;
			}
			g_ADJ.lumin = s_WifiPacketCfg.rxPack.dat[0];
			if(g_ADJ.lumin == 0)
			{
				g_ADJ.lumin = 1;
			}
			g_ADJ.setTime = WORK_TIME_TOTAL - (g_ADJ.lumin*WORK_TIME_UNIT);
			
			g_ADJ.enable = s_WifiPacketCfg.rxPack.dat[1];
			{
				uint8 dat[] = {0, 0};
				SetTxPacketData(dat, 2);
				SendTxPacketData(LED_SWITCH_BRIGHT_CTR_CMD|0xf0);
			}
			
			QMsgPostSimple(&g_QMsg, SYS_MSG_ADJ_ID, LED_SWITCH_BRIGHT_VALUE);
			break;
		case LED_SWITCH_BRIGHT_QUE_CMD:
			{
				uint8 dat[] = {0, 0};
				dat[0] = g_ADJ.lumin;
				dat[1] = g_ADJ.enable;
				SetTxPacketData(dat, 2);
				SendTxPacketData(LED_SWITCH_BRIGHT_QUE_CMD|0xf0);
			}
			break;
	}
 #endif
}

#if defined(UART_PROTOCOL_TUYA)
/*****************************************************************************
函数名称 : GetDownloadCmdTotal
功能描述 : 获取所有dp命令总和
输入参数 : 无
返回参数 : 下发命令总和
使用说明 : 
*****************************************************************************/
static uint8 GetDownloadCmdTotal(void)
{
	return(sizeof(s_DownloadCmd) / sizeof(s_DownloadCmd[0]));
}

/*****************************************************************************
函数名称 : get_update_dpid_index
功能描述 : 获取制定DPID在数组中的序号
输入参数 : dpid:dpid
返回参数 : index:dp序号
*****************************************************************************/
static uint8 GetDowmloadDataPointIdIndex(uint8 dpid)
{
	uint8 index;
	uint8 total = GetDownloadCmdTotal();
  
	for(index = 0; index < total; index ++)
	{
		if(s_DownloadCmd[index].dp_id == dpid)
		{
			break;
		}
	}
	return index;
}
/*****************************************************************************
函数名称 : dp_download_handle
功能描述 : dp下发处理函数
输入参数 : dpid:DP序号
           value:dp数据缓冲区地址
           length:dp数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERRO
使用说明 : 该函数用户不能修改
*****************************************************************************/
static uint8 DownloadDataPointHandle(const uint8  *pVal, uint8  dpid, uint8  length)
{
	/*********************************
	当前函数处理可下发/可上报数据调用                    
	具体函数内需要实现下发数据处理
	完成用需要将处理结果反馈至APP端,否则APP会认为下发失败
	***********************************/
	MSG_t msg;
	if(!pVal)
	{
		return false;
	}
	msg.msgID = SYS_MSG_ADJ_ID;
	switch(dpid)
	{
		case DPID_LED_SWITCH:
			//开关处理函数
			if(length == 1)
			{
				g_ADJ.enable = pVal[0];
			
				msg.Param = LED_SWITCH;
			
				QMsgPost(&g_QMsg, msg);
			}
			break;
		case DPID_BRIGHT_VALUE:		
			if(length == 4)
			{
				if (pVal[3] >= 25)
				{
					g_ADJ.lumin = pVal[3] - 25;
					
					msg.Param = BRIGHT_VALUE;
					
					QMsgPost(&g_QMsg, msg);
				}
			}
			break;
		default:
			break;
	}
	return true;
}



static void HeatBeatCheck(void)
{
	uint8  length = 1;
	static uint8  resetState = FALSE;
  
	if(FALSE == resetState)
	{
		SetTxPacketData(&resetState, length);
		resetState = TRUE;
	}
	else
	{
		SetTxPacketData(&resetState, length);
	}
}
static void ProductInfoUpdate(void)
{
	//SetTxPacketData((uint8 const* const)(PRODUCT_INFO), my_strlen((uint8 *)PRODUCT_INFO));
	SetTxPacketData(PRODUCT_INFO, my_strlen((uint8 *)PRODUCT_INFO));

}
static void WIFIWortModeSet(void)
{
	u8 dat[] = {5, 14};
	SetTxPacketData(dat, 2);
}

static void AllDataPointHand(void)
{	
	uint8 i;
	uint8 dplength;
	for(i = 0; i < s_WifiPacketCfg.rxCnt; i++)
	{
		dplength = s_WifiPacketCfg.rxPack.dat[i+2]<<8;
		dplength |= s_WifiPacketCfg.rxPack.dat[i+3];
		OneDataPointHand(s_WifiPacketCfg.rxPack.dat);
		i += (dplength + 4);
	}
}
static uint8 OneDataPointHand(uint8 const *pDPVal)
{
	uint8  dpID;
	uint8  dpIndex;
	uint8  dpType;
	uint16  dpLength;
	uint8 const * pVal;
	uint8  ret = FALSE;
	if(!pDPVal)
	{
		return ret;
	}
	dpID = pDPVal[0];
	dpType = pDPVal[1];
	dpLength = BUILD_UINT16(pDPVal[3], pDPVal[2]);
	pVal = pDPVal + 4;
	dpIndex = GetDowmloadDataPointIdIndex(dpID);
	if(s_DownloadCmd[dpIndex].dp_type == dpType)
	{
		return DownloadDataPointHandle(pVal, dpID, dpLength);
	}
	return ret;
}


static void BoolDataPointUpdate(uint8  *const pbuffer, uint8  dpid, uint8  value)
{
	if(!pbuffer)
	{
		return ;
	}
	pbuffer[0] = dpid;
	pbuffer[1] = DP_TYPE_BOOL;
	pbuffer[2] = 0x00;
	pbuffer[3] = 0x01;
	pbuffer[4] = value ? 1 : 0;
}
static void ValueDataPointUpdate(uint8  *const pbuffer, uint8  dpid, uint32 value)
{
	if(!pbuffer)
	{
		return ;
	}
	pbuffer[0] = dpid;
	pbuffer[1] = DP_TYPE_VALUE;
	pbuffer[2] = 0x00;
	pbuffer[3] = 0x04;
	pbuffer[4] = (value>>24)&0xff;
	pbuffer[5] = (value>>16)&0xff;
	pbuffer[6] = (value>>8)&0xff;
	pbuffer[7] = (value>>0)&0xff;
}
static void AllDataUpdate(void)
{
	uint8  buffer[13];
	uint8  length = 0;
	if(g_ADJ.enable) 
	{
		BoolDataPointUpdate(buffer, DPID_LED_SWITCH, 1);
		length = 5;
	}
	else
	{
		BoolDataPointUpdate(buffer, DPID_LED_SWITCH, 0);
		length = 5;
	}
	ValueDataPointUpdate(buffer+length, DPID_BRIGHT_VALUE, (uint32)(g_ADJ.lumin+25));
	length += 8;
	SetTxPacketData(buffer, length);
}

void WifiHandle(const MSG_t *const pMsg)
{
	switch (pMsg->Param)
	{
		case WIFI_RESET:			//Wifi reset
			Log("WIFI_RESET\r\n");
			s_Wifi.resetFlag = RESET_WIFI_ERROR;
			SetTxPacketData(null, 0);
			SendTxPacketData(WIFI_RESET_CMD);
			break;
		case WIFI_SMART_CONFIG:		//Wifi smartconfig
			Log("WIFI_SMART_CONFIG\r\n");
			{
				uint8  dat = 0x00;
				s_Wifi.CurrState = SMART_CONFIG_STATE;
				s_Wifi.setModeFlag = RESET_WIFI_ERROR;
				SetTxPacketData(&dat, 1);
				SendTxPacketData(WIFI_MODE_CMD);
			}
			break;
		case WIFI_AP:				//Wifi ap
			Log("WIFI_AP\r\n");
			{
				uint8  dat = 0x01;
				s_Wifi.CurrState = AP_STATE;
				s_Wifi.setModeFlag = RESET_WIFI_ERROR;
				SetTxPacketData(&dat, 1);
				SendTxPacketData(WIFI_MODE_CMD);
			}
			break;
		case WIFI_UPLOAD:
			Log("WIFI_UPLOAD\r\n");
			{
				AllDataUpdate();
				SendTxPacketData(STATE_UPLOAD_CMD);
			}
			break;
		default:
			break;
	}			
}

void WifiStatusShow(void)
{
#if 0
	if(s_Wifi.CurrState != s_Wifi.BackState)
	{
		s_Wifi.BackState = s_Wifi.CurrState;
		switch (s_Wifi.CurrState)
		{
			case SMART_CONFIG_STATE:
				Log("SMART_CONFIG_STATE\r\n");
				BrightLevelShowFlash(LED_WIFI_ID, LED_FLASH_ALWAYS, 2, 2);
				break;
			case AP_STATE:
				Log("AP_STATE\r\n");
				BrightLevelShowFlash(LED_WIFI_ID, LED_FLASH_ALWAYS, 10, 10);
				break;
			case WIFI_NOT_CONNECTED:
				Log("WIFI_NOT_CONNECTED\r\n");
				LedSetLevel(LED_WIFI_ID, ON, true);
				break;
				
			case WIFI_CONNECTED:
				Log("WIFI_CONNECTED\r\n");
				LedSetLevel(LED_WIFI_ID, OFF, true);
				break;

			default:
				break;
		}
	}
#else
	s_Wifi.BackState= s_Wifi.CurrState;
#endif
}
#endif
static void SetTxPacketData(uint8 const* const src, uint8  length)
{
	my_memcpy(&s_WifiPacketCfg.txPack.dat, src, length);
	s_WifiPacketCfg.txLength = length;
}

static void SendTxPacketData(uint8  cmd)
{
#if defined(UART_PROTOCOL_TUYA)
	uint8  txLength = s_WifiPacketCfg.txLength+6;
	s_WifiPacketCfg.txPack.cmd = cmd;
	s_WifiPacketCfg.txPack.length[0] = HI_UINT16(s_WifiPacketCfg.txLength);
	s_WifiPacketCfg.txPack.length[1] = LO_UINT16(s_WifiPacketCfg.txLength);
	s_WifiPacketCfg.txPack.dat[s_WifiPacketCfg.txLength] = getCheckSum((uint8 const *)&s_WifiPacketCfg.txPack.head_0, txLength++);
	UartSendBuffer(UART_WIFI, (const uint8 *)&s_WifiPacketCfg.txPack.head_0, txLength);

//	UartSendHexBuffer(UART_WIFI, "res:", (const uint8 *const)&s_WifiPacketCfg.txPack.head_0, txLength, ' ', true);

	s_WifiPacketCfg.txLength = 0;
#elif defined(UART_PROTOCOL_HZ)
	uint8  txLength = s_WifiPacketCfg.txLength+3;
	s_WifiPacketCfg.txPack.cmd = cmd;
	s_WifiPacketCfg.txPack.dat[s_WifiPacketCfg.txLength] = getCheckSum((uint8 const *)&s_WifiPacketCfg.txPack.head_0, txLength++);
	UartSendBuffer(UART_WIFI, (const uint8 *)&s_WifiPacketCfg.txPack.head_0, txLength);
	
//	UartSendHexBuffer(UART_WIFI, "res:", (const uint8 *const)&s_WifiPacketCfg.txPack.head_0, txLength, ' ', true);

	s_WifiPacketCfg.txLength = 0;

#endif
}

