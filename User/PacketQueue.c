#define __PACKET_QUEUE_C__
#include "PacketQueue.h"

void QueueInit(void)
{
	g_PacketQueue.in = 0;
	g_PacketQueue.out = 0;
	g_PacketQueue.size = 0;
}

uint8 QueuePost(uint8 dat)
{
	if(g_PacketQueue.size >= PACKET_QUEUE_MAX)
	{
		return 1;
	}
	if(g_PacketQueue.in >= PACKET_QUEUE_MAX)
	{
		g_PacketQueue.in = 0;
	}
	g_PacketQueue.buffer[g_PacketQueue.in++] = dat;
	g_PacketQueue.size++;
	return 0;
}
uint8 QueuePend(uint8 *dat)
{
	if(g_PacketQueue.size == 0)
	{
		return 1;
	}
	if(g_PacketQueue.out >= PACKET_QUEUE_MAX)
	{
		g_PacketQueue.out = 0;
	}
	*dat = g_PacketQueue.buffer[g_PacketQueue.out++];
	g_PacketQueue.size--;
	return 0;
}
uint8 QueueSize(void)
{
	return g_PacketQueue.size;	
}
