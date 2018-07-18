#ifndef __TIMER_UNIT_H__
#define __TIMER_UNIT_H__
#include "Typedef.h"
#include "QMsg.h"

#ifndef __TIMER_UNIT_C__
	#define TIMER_UNIT_EXTERN	extern
#else
	#define TIMER_UNIT_EXTERN
#endif

/***********************宏定义****************************/
#define TIMER_UNIT_SERVER_TICK	10


#define SEC(x)			((x)*10)
#define MIN(x)			((x)*SEC(60))
#define HOUR(x)			((x)*MIN(60))


/***********************数据结构定义************************/
typedef enum{
	TIMER_WINDOW_CTR_ID = 0,
	TIMER_TABLE_MAX
}TIMER_UNIT_ID_t;
typedef struct{
	MSG_Q_t	*pQmsg;
	MSG_t msg;
	uint16 TO;
	uint8 Id;
	uint8 enable;
}Timer_t;

typedef struct{
	Timer_t TServer[TIMER_TABLE_MAX];
	uint8	Total;
}TimerServer_t;

/********************全局变量声明与定义***********************/
TIMER_UNIT_EXTERN  TimerServer_t g_TimerServer;


/***************************函数声明***************************/
TIMER_UNIT_EXTERN sysServerTO_t SysTimerUnitServer(void);
TIMER_UNIT_EXTERN void TimerUnitInit(TimerServer_t *this);
TIMER_UNIT_EXTERN void TimerUnitDel(TimerServer_t *this, uint8 Id);
TIMER_UNIT_EXTERN void TimerUnitAdd(TimerServer_t *this, uint8 Id, MSG_Q_t *pqmsg, MSG_t *pmsg, uint16 TO);
TIMER_UNIT_EXTERN void TimerUnitEnable(TimerServer_t *this, uint8 Id, uint8 enable);
TIMER_UNIT_EXTERN uint8 TimerUnitIsRun(TimerServer_t *this, uint8 Id);
TIMER_UNIT_EXTERN uint16 TimerUnitGetTO(TimerServer_t *this, uint8 Id);

#endif
