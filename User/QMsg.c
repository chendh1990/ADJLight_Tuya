#define __QMSG_C__
#include "QMsg.h"
#include "Common.h"
#include "Log.h"

#if 0
uint8 QMsgInit(MSG_Q_t  *const this, MSG_t  *const msgBuffer, uint8 cap)
{
	if(!this)
	{
		return 1;
	}
	if(!msgBuffer)
	{
		return 2;
	}
	if(cap == 0)
	{
		return 3;
	}
	g_QMsg.pStart = msgBuffer;
	g_QMsg.pEnd = msgBuffer + cap;
	g_QMsg.In = 0;
	g_QMsg.Out = 0;
	g_QMsg.Size = 0;
	g_QMsg.Capacity = cap;
	return 0;
}
uint8 QMsgPost(MSG_Q_t  *const this, MSG_t msg)
{
	if(!this)
	{
		Log("--[QMsgPost] this is null\r\n");
		Log("--[QMsgPost] this = %x\r\n", (uint16)this);
		Log("--[QMsgPost] &g_Qmsg = %x\r\n", (uint16)(&g_QMsg));
		return 1;
	}
	if(g_QMsg.Size < g_QMsg.Capacity)
	{
		if(g_QMsg.In >= g_QMsg.Capacity)
		{
			g_QMsg.In = 0;
		}
		g_QMsg.pStart[g_QMsg.In].msgID = msg.msgID;
		g_QMsg.pStart[g_QMsg.In].Param = msg.Param;
		g_QMsg.In++;
		ENTER_CRITICAL();
		g_QMsg.Size++;	
		EXIT_CRITICAL();
		//Log("QMsgPost: this = %x\r\n", this);
		//Log("QMsgPost:&g_Qmsg = %x\r\n", &g_QMsg);
		//Log("QMsgPost Qmsgid=%d, Qparam=%d\r\n", (uint16)(g_QMsg.pStart[g_QMsg.In-1].msgID), (uint16)(g_QMsg.pStart[g_QMsg.In-1].Param));
		//Log("QMsgPost msgid=%d, param=%d\r\n", (uint16)(msg.msgID), (uint16)(msg.Param));
		//Log("\r\n\tINT QMsgPost QSize:%d\r\n", (uint16)(g_QMsg.Size));
		//Log("QMsgPost QIn:%d\r\n\r\n", (uint16)(g_QMsg.In));
		return 0;
	}
	return 2;
}

uint8 QMsgPend(  MSG_Q_t  *const this, MSG_t  *const pMsg)
{
	MSG_t msg;
	
	Log("\r\n**[QMsgPend] this = %x\r\n", (uint16)this);
	Log("**[QMsgPend] &g_Qmsg = %x\r\n", (uint16)(&g_QMsg));
	Log("**[QMsgPend] sizeof(this)=%d\r\n", (uint16)sizeof(this));
	if((!this) || (this == null))
	{
		Log("--[QMsgPend] this is null\r\n");
		Log("--[QMsgPend] this = %x\r\n", (uint16)this);
		Log("--[QMsgPend] &g_Qmsg = %x\r\n", (uint16)(&g_QMsg));
		return 1;
	}
	if(!pMsg)
	{
		return 2;
	}
	if(g_QMsg.Size > 0)
	{
		if(g_QMsg.Out >= g_QMsg.Capacity)
		{
			g_QMsg.Out = 0;
		}
		pMsg->msgID = g_QMsg.pStart[g_QMsg.Out].msgID;
		pMsg->Param = g_QMsg.pStart[g_QMsg.Out].Param;
		msg.msgID = g_QMsg.pStart[g_QMsg.Out].msgID;
		msg.Param = g_QMsg.pStart[g_QMsg.Out].Param;
		g_QMsg.Out++;
		ENTER_CRITICAL();
		g_QMsg.Size--;	
		EXIT_CRITICAL();
		
		Log("\t[QMsgPend] this = %x\r\n", (uint16)this);
		Log("\t[QMsgPend] &g_Qmsg = %x\r\n", (uint16)(&g_QMsg));
		Log("\t[QMsgPend] Qmsgid=%d, Qparam=%d\r\n", (uint16)(g_QMsg.pStart[g_QMsg.Out-1].msgID), (uint16)(g_QMsg.pStart[g_QMsg.Out-1].Param));
		Log("\t[QMsgPend] msgid=%d, param=%d\r\n", (uint16)(pMsg->msgID), (uint16)(pMsg->Param));
		Log("\t[QMsgPend] interal msgid=%d, param=%d\r\n", (uint16)(msg.msgID), (uint16)(msg.Param));
		Log("\t[QMsgPend] QSize:%d\r\n", (uint16)(g_QMsg.Size));
		Log("\t[QMsgPend] QOut:%d\r\n\r\n", (uint16)(g_QMsg.Out));
		return 0;
	}
	return 3;
}

uint8 QMsgSize(const MSG_Q_t  *const this)
{
	if(!this)
	{
		return 0;
	}
	return g_QMsg.Size;
}
uint8 QMsgCapacity(const MSG_Q_t  *const this)
{
	if(!this)
	{
		return 0;
	}
	return g_QMsg.Capacity;
}
#else
uint8 QMsgInit(MSG_Q_t  *const this, MSG_t  *const msgBuffer, uint8 cap)
{
	if(!this)
	{
		return 1;
	}
	if(!msgBuffer)
	{
		return 2;
	}
	if(cap == 0)
	{
		return 3;
	}
	g_QMsg.pStart = msgBuffer;
	g_QMsg.pEnd = msgBuffer + cap;
	g_QMsg.In = 0;
	g_QMsg.Out = 0;
	g_QMsg.Size = 0;
	g_QMsg.Capacity = cap;
	return 0;
}
uint8 QMsgPostSimple(MSG_Q_t  *const this, uint16  msgID, uint16  param)
{
	if(!this)
	{
		return 1;
	}
	if(g_QMsg.Size < g_QMsg.Capacity)
	{
		if(g_QMsg.In >= g_QMsg.Capacity)
		{
			g_QMsg.In = 0;
		}
		g_QMsg.pStart[g_QMsg.In].msgID = msgID;
		g_QMsg.pStart[g_QMsg.In].Param = param;
		g_QMsg.In++;
		ENTER_CRITICAL();
		g_QMsg.Size++;	
		EXIT_CRITICAL();
		return 0;
	}
	return 2;
}
uint8 QMsgPost(MSG_Q_t  *const this, MSG_t msg)
{
	if(!this)
	{
		return 1;
	}
	if(g_QMsg.Size < g_QMsg.Capacity)
	{
		if(g_QMsg.In >= g_QMsg.Capacity)
		{
			g_QMsg.In = 0;
		}
		g_QMsg.pStart[g_QMsg.In].msgID = msg.msgID;
		g_QMsg.pStart[g_QMsg.In].Param = msg.Param;
		g_QMsg.In++;
		ENTER_CRITICAL();
		g_QMsg.Size++;	
		EXIT_CRITICAL();
		return 0;
	}
	return 2;
}

uint8 QMsgPend(  MSG_Q_t  *const this, MSG_t  *const pMsg)
{	
	if((!this) || (this == null))
	{
		return 1;
	}
	if(!pMsg)
	{
		return 2;
	}
	if(g_QMsg.Size > 0)
	{
		if(g_QMsg.Out >= g_QMsg.Capacity)
		{
			g_QMsg.Out = 0;
		}
		pMsg->msgID = g_QMsg.pStart[g_QMsg.Out].msgID;
		pMsg->Param = g_QMsg.pStart[g_QMsg.Out].Param;
		g_Msg.msgID = g_QMsg.pStart[g_QMsg.Out].msgID;
		g_Msg.Param = g_QMsg.pStart[g_QMsg.Out].Param;
		g_QMsg.Out++;
		ENTER_CRITICAL();
		g_QMsg.Size--;	
		EXIT_CRITICAL();
		return 0;
	}
	return 3;
}

uint8 QMsgSize(const MSG_Q_t  *const this)
{
	if(!this)
	{
		return 0;
	}
	return g_QMsg.Size;
}
uint8 QMsgCapacity(const MSG_Q_t  *const this)
{
	if(!this)
	{
		return 0;
	}
	return g_QMsg.Capacity;
}
#endif
