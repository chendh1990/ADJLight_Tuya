#ifndef __WIFI_H__
#define __WIFI_H__

#include "Typedef.h"
#include "QMsg.h"

#ifndef __WIFI_C__
	#define WIFI_EXTERN	extern
#else
	#define WIFI_EXTERN
#endif

/***********************宏定义****************************/
#define UART_PROTOCOL_HZ
//#define UART_PROTOCOL_TUYA

#if (!defined(UART_PROTOCOL_HZ) && !defined(UART_PROTOCOL_TUYA))
#error "Must define Uart protocol type!"
#endif
#ifdef UART_PROTOCOL_TUYA
#define P_HEAD_0		0x55
#define P_HEAD_1		0xaa
#define P_VER			0x00
#elif defined(UART_PROTOCOL_HZ)
#define P_HEAD_0		0xFA
#define P_HEAD_1		0x55
#endif
#define SUM_HEAD_VER	(P_HEAD_0+P_HEAD_1+P_VER)

#define UART_PACKET_DAT_MAX  	24


#define PRODUCT_KEY			"2JOFKB1AQyTmSde6"    //开发平台创建产品后生成的16位字符产品唯一标识
 
#define MCU_VER 			"1.0.0"                   //用户的软件版本,用于MCU固件升级,MCU升级版本需修改

#define PRODUCT_INFO		(PRODUCT_KEY MCU_VER)


//开关(可下发可上报)
//备注:
#define DPID_LED_SWITCH 		1
//暂停(可下发可上报)
//备注:
#define DPID_BRIGHT_VALUE 		3

//=============================================================================
//数据帧类型
//=============================================================================
#define         HEAT_BEAT_CMD                   0                               //心跳包
#define         PRODUCT_INFO_CMD                1                               //产品信息
#define         WORK_MODE_CMD                   2                               //查询MCU 设定的模块工作模式	
#define         WIFI_STATE_CMD                  3                               //wifi工作状态	
#define         WIFI_RESET_CMD                  4                               //重置wifi
#define         WIFI_MODE_CMD                   5                               //选择smartconfig/AP模式	
#define         DATA_QUERT_CMD                  6                               //命令下发
#define         STATE_UPLOAD_CMD                7                               //状态上报	 
#define         STATE_QUERY_CMD                 8                               //状态查询   
#define         UPDATE_QUERY_CMD                9                               //升级查询
#define         UPDATE_START_CMD                0x0a                            //升级开始
#define         UPDATE_TRANS_CMD                0x0b                            //升级传输
#define         GET_ONLINE_TIME_CMD             0x0c                            //获取系统时间(格林威治时间)
#define         FACTORY_MODE_CMD                0x0d                            //进入产测模式    
#define         WIFI_TEST_CMD                   0x0e                            //wifi功能测试
#define         GET_LOCAL_TIME_CMD              0x1c                            //获取本地时间                         

//=============================================================================
//dp数据点类型
//=============================================================================
#define         DP_TYPE_RAW                     0x00				//RAW型
#define         DP_TYPE_BOOL                    0x01	           	//布尔型
#define         DP_TYPE_VALUE                   0x02	         	//数值型
#define         DP_TYPE_STRING                  0x03				//字符串型
#define         DP_TYPE_ENUM                    0x04				//枚举型
#define         DP_TYPE_FAULT                   0x05				//故障型

//=============================================================================
//WIFI工作状态
//=============================================================================
#define         SMART_CONFIG_STATE              0x00
#define         AP_STATE                        0x01
#define         WIFI_NOT_CONNECTED              0x02
#define         WIFI_CONNECTED                  0x03
#define         WIFI_SATE_UNKNOW                0xff
//=============================================================================
//wifi配网的方式
//=============================================================================
#define         SMART_CONFIG                    0x0  
#define         AP_CONFIG                       0x1   

//=============================================================================
//wifi复位状态
//=============================================================================
#define         RESET_WIFI_ERROR                0
#define         RESET_WIFI_SUCCESS              1

//=============================================================================
//wifi复位状态
//=============================================================================
#define         SET_WIFICONFIG_ERROR            0
#define         SET_WIFICONFIG_SUCCESS          1



/***********************数据结构定义************************/
#ifdef UART_PROTOCOL_TUYA
typedef enum
{
	RX_IDDLE = 0,
	RX_HEAD_0,
	RX_HEAD_1,
	RX_VER,
	RX_CMD,
	RX_LEN_0,
	RX_LEN_1,
	RX_BUSY,
	RX_VERIFY
}UartRxState_t;

typedef struct{
	uint8 head_0;
	uint8 head_1;
	uint8 ver;
	uint8 cmd;
	uint8 length[2];
	uint8 dat[UART_PACKET_DAT_MAX];
}UartDataPacket_t;

typedef struct
{
	uint8 CurrState;
	uint8 BackState;
	uint8 resetFlag;
	uint8 setModeFlag;
}Wifi_t;


typedef struct {
  uint8 dp_id;                        //dp序号
  uint8 dp_type;                      //dp类型
}DownloadCmd_t;


typedef enum{
	WIFI_RESET = 0,
	WIFI_SMART_CONFIG,
	WIFI_AP,
	WIFI_UPLOAD
}Wifi_Cmd_t;
	
#elif defined(UART_PROTOCOL_HZ)

typedef enum
{
	RX_IDDLE = 0,
	RX_HEAD_0,
	RX_HEAD_1,
	RX_CMD,
	RX_BUSY,
	RX_VERIFY
}UartRxState_t;
typedef struct{
	uint8 head_0;
	uint8 head_1;
	uint8 cmd;
	uint8 dat[UART_PACKET_DAT_MAX];
}UartDataPacket_t;

#define LED_SWITCH_BRIGHT_CTR_CMD		(0x01)	
#define LED_SWITCH_BRIGHT_QUE_CMD		(0x02)	


#endif
typedef struct
{
	UartDataPacket_t rxPack;
	uint8 rxCnt;
	uint8 rxStatus;
	uint16 rxLength;

	UartDataPacket_t txPack;
	
	uint8 txCnt;
	uint8 txStatus;
	uint16 txLength;
} WifiPacketCfg_t;

/********************全局变量声明与定义***********************/


/***************************函数声明***************************/
WIFI_EXTERN void WifiInit(void);

WIFI_EXTERN void WifiPacketParse(void);

WIFI_EXTERN void WifiHandle(const MSG_t *const pMsg);

WIFI_EXTERN void WifiStatusShow(void);

#endif
