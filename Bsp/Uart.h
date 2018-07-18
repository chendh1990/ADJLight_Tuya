#ifndef __UART_H__
#define __UART_H__

#ifndef __UART_C__
	#define UART_EXTERN	extern
#else
	#define UART_EXTERN
#endif

/**********************数据结构定义***********************/
typedef enum
{	
	UART_WIFI = 0,
	UART_DEBUG
}UartPort_t;
/***********************宏定义****************************/




/**********************函数声明**************************/
UART_EXTERN void UartInit(void);
UART_EXTERN void UartSendString(UartPort_t uport, uint8 const *str);
UART_EXTERN void UartSendBuffer(UartPort_t Port, const uint8 *const Buffer, uint8 length);
UART_EXTERN void UartSendHexBuffer(UartPort_t Port, const uint8 *des, const uint8 *const buf , uint8 len, uint8 dec, uint8 en);
UART_EXTERN void print(char* fmt, ...);

#endif
