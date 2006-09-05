
#ifdef __cplusplus
extern "C" {
#endif

#include "PA_Timer.h"

#define TICK ((TIMER1_DATA>>5) + (TIMER2_DATA<<11))
struct tTimeMgr gTime;

void StartTime(u8 bNew)
{
	struct tTimer * pTimer = &(gTime.rBank[0]);
	ASSERT(pTimer != NULL);

	pTimer->bRunning = true;

	if (!bNew)
	{
		pTimer->nRefTick -= TICK-pTimer->nRefTick;
		return;
	}

	pTimer->nRefTick	= 0;

	TIMER1_DATA=0;
	TIMER2_DATA=0;
	TIMER1_CR=TIMER_DIV_1024;
	TIMER2_CR=TIMER_CASCADE;

	gTime.nCount		= 1;

}

void PauseTime()
{
	struct tTimer * pTimer = &(gTime.rBank[0]);
	ASSERT(pTimer != NULL);

	pTimer->bRunning = false;

}

u8	NewTimer(u8 bStarted)
{
	struct tTimer * pTimer = NULL;

	ASSERT(gTime.nCount < TIME_MGR_CAPACITY-1);

	pTimer = &(gTime.rBank[gTime.nCount]);
	ASSERT(pTimer != NULL);

	pTimer->bRunning = bStarted;
	pTimer->nRefTick = 0;

	if (bStarted)
		pTimer->nRefTick = TICK;

	return gTime.nCount++;	
}

void	StartTimer(u8 nTimerId)
{
	struct tTimer * pTimer = &(gTime.rBank[nTimerId]);
	ASSERT(pTimer != NULL);

	pTimer->bRunning = true;
	pTimer->nRefTick = TICK-pTimer->nRefTick;
}

u32	Tick(u8	nTimerId)
{
	struct tTimer * pTimer = &(gTime.rBank[nTimerId]);
	ASSERT(pTimer != NULL);

	if (pTimer->bRunning == true)
		return TICK-pTimer->nRefTick;

	return pTimer->nRefTick;
}

void PauseTimer(u8 nTimerId)
{
	struct tTimer * pTimer = &(gTime.rBank[nTimerId]);
	ASSERT(pTimer != NULL);

	pTimer->nPauseTick = TICK;

	pTimer->bRunning = false;	
}

void UnpauseTimer(u8 nTimerId)
{
	struct tTimer * pTimer = &(gTime.rBank[nTimerId]);
	ASSERT(pTimer != NULL);


	pTimer->nRefTick += TICK - pTimer->nPauseTick;

	pTimer->bRunning = true;	
}

void ResetTimer(u8 nTimerId)
{
	struct tTimer * pTimer = &(gTime.rBank[nTimerId]);
	ASSERT(pTimer != NULL);

	pTimer->nRefTick = TICK;
}

#ifdef __cplusplus
}
#endif
