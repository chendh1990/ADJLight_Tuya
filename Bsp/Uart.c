#define __UART_C__
#include <stdio.h>
#include <stdarg.h>
#include "stm8s.h"
#include "Typedef.h"
#include "Common.h"
#include "Wifi.h"
#include "Uart.h"

#ifdef _RAISONANCE_
#define PUTCHAR_PROTOTYPE int putchar (char c)
#define GETCHAR_PROTOTYPE int getchar (void)
#elif defined (_COSMIC_)
#define PUTCHAR_PROTOTYPE char putchar (char c)
#define GETCHAR_PROTOTYPE char getchar (void)
#else /* _IAR_ */
#define PUTCHAR_PROTOTYPE int putchar (int c)
#define GETCHAR_PROTOTYPE int getchar (void)
#endif /* _RAISONANCE_ */

static void printch(char ch);
static void printdec(int dec);
static void printflt(double flt);
static void printstr(char* str);
static void printbin(int bin);
static void printhex(int hex);

static void InitialUART1(void)
{
	UART1_DeInit();
#if defined(UART_PROTOCOL_TUYA)
	UART1_Init((u32)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, \
   				 	UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, \
   				 	UART1_MODE_TXRX_ENABLE);
#elif defined(UART_PROTOCOL_HZ)
	UART1_Init((u32)19200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, \
					UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, \
					UART1_MODE_TXRX_ENABLE);
#endif
	UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
	UART1_Cmd(ENABLE);
}
void UartInit(void)
{		
	InitialUART1();
}

/**
  * @brief Retargets the C library printf function to the UART.
  * @param c Character to send
  * @retval char Character sent
  */
PUTCHAR_PROTOTYPE
{
  /* Write a character to the UART1 */
  UART1_SendData8(c);
  /* Loop until the end of transmission */
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);

  return (c);
}


/**
  * @brief Retargets the C library scanf function to the USART.
  * @param None
  * @retval char Character to Read
  */
GETCHAR_PROTOTYPE
{
#ifdef _COSMIC_
  char c = 0;
#else
  int c = 0;
#endif
  /* Loop until the Read data register flag is SET */
  while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET);
    c = UART1_ReceiveData8();
  return (c);
}


static void UartSendByte(UartPort_t Port, char c)
{
	switch (Port)
	{
		case UART_WIFI:
			putchar(c);
			break;
		case UART_DEBUG:
			putchar(c);
			break;
		default:
			break;
	}
}

void UartSendString(UartPort_t Port, uint8 const *str)
{
	if(!str)
	{
		return ;
	}
	while(*str)
	{
		UartSendByte(Port, *str++);
	}
}
void UartSendBuffer(UartPort_t Port, const uint8 *const Buffer, uint8 length)
{
	uint8 i;
	for(i = 0; i < length; i++)
	{
		UartSendByte(Port, Buffer[i]);
	}
}
void UartSendHexBuffer(UartPort_t Port, const uint8 *des, const uint8 *const buf , uint8 len, uint8 dec, uint8 en)
{
	uint8 i;
	uint8 MSB;
	uint8 LSB;
	if(des)
	{
		UartSendString(Port, des);
	}
	for(i = 0; i < len; i++)
	{
		MSB = getHex(HI_UINT8(buf[i]));
		LSB = getHex(LO_UINT8(buf[i]));
		UartSendByte(Port, MSB);
		UartSendByte(Port, LSB);
		UartSendByte(Port, dec);
	}

	if(en)
	{
		UartSendByte(Port, '\r');
		UartSendByte(Port, '\n');
	}
}

void print(char* fmt, ...)
{
    double vargflt = 0;
    int  vargint = 0;
    char* vargpch = NULL;
    char vargch = 0;
    char* pfmt = NULL;
    va_list vp;

    va_start(vp, fmt);
    pfmt = fmt;

    while(*pfmt)
    {
        if(*pfmt == '%')
        {
            switch(*(++pfmt))
            {
                
                case 'c':
                    vargch = va_arg(vp, int); 
                    /*    va_arg(ap, type), if type is narrow type (char, short, float) an error is given in strict ANSI
                        mode, or a warning otherwise.In non-strict ANSI mode, 'type' is allowed to be any expression. */
                    printch(vargch);
                    break;
                case 'd':
                case 'i':
                    vargint = va_arg(vp, int);
                    printdec(vargint);
                    break;
                case 'f':
                    vargflt = va_arg(vp, double);
                    /*    va_arg(ap, type), if type is narrow type (char, short, float) an error is given in strict ANSI
                        mode, or a warning otherwise.In non-strict ANSI mode, 'type' is allowed to be any expression. */
                    printflt(vargflt);
                    break;
                case 's':
                    vargpch = va_arg(vp, char*);
                    printstr(vargpch);
                    break;
                case 'b':
                case 'B':
                    vargint = va_arg(vp, int);
                    printbin(vargint);
                    break;
                case 'x':
                case 'X':
                    vargint = va_arg(vp, int);
                    printhex(vargint);
                    break;
                case '%':
                    printch('%');
                    break;
                default:
                    break;
            }
            pfmt++;
        }
        else
        {
            printch(*pfmt++);
        }
    }
    va_end(vp);
}

static void printch(char ch)
{
    putchar(ch);
}

static void printdec(int dec)
{
    if(dec==0)
    {
        return;
    }
    printdec(dec/10);
    printch( (char)(dec%10 + '0'));
}

static void printflt(double flt)
{
    int tmpint = 0;
    
    tmpint = (int)flt;
    printdec(tmpint);
    printch('.');
    flt = flt - tmpint;
    tmpint = (int)(flt * 1000000);
    printdec(tmpint);
}

static void printstr(char* str)
{
    while(*str)
    {
        printch(*str++);
    }
}

static void printbin(int bin)
{
    if(bin == 0)
    {
        printstr("0b");
        return;
    }
    printbin(bin/2);
    printch( (char)(bin%2 + '0'));
}

static void printhex(int hex)
{
    if(hex==0)
    {
        printstr("0x");
        return;
    }
    printhex(hex/16);
    if(hex < 10)
    {
        printch((char)(hex%16 + '0'));
    }
    else
    {
        printch((char)(hex%16 - 10 + 'a' ));
    }
}
