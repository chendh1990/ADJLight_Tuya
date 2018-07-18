#ifndef __PACKET_QUEUE_H__
#define __PACKET_QUEUE_H__
#include "Typedef.h"

#ifndef __PACKET_QUEUE_C__
	#define PACKET_QUEUE_EXTERN	extern
#else
	#define PACKET_QUEUE_EXTERN
#endif

/***********************宏定义****************************/
#define PACKET_QUEUE_MAX		20


/***********************数据结构定义************************/

typedef struct
{
	uint8 buffer[PACKET_QUEUE_MAX];
	uint8 in;
	uint8 out;
	uint8 size;
}PacketQueue_t;


/********************全局变量声明与定义***********************/
PACKET_QUEUE_EXTERN PacketQueue_t  g_PacketQueue;


/***************************函数声明***************************/
PACKET_QUEUE_EXTERN void QueueInit(void);
PACKET_QUEUE_EXTERN uint8 QueuePost(uint8 dat);
PACKET_QUEUE_EXTERN uint8 QueuePend(uint8 *dat);
PACKET_QUEUE_EXTERN uint8 QueueSize(void);
#endif

