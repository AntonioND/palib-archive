#ifndef _PA_Main
#define _PA_Main

#ifdef __cplusplus
extern "C" {
#endif
#include <nds.h>

#include <nds/bios.h>
#include <nds/arm7/touch.h>
#include <nds/arm7/clock.h>


#include "PA_IPC.h"

#include "arm7/PA_Sound.h"
#include "Sound7.h"

#define CODE_IN_IWRAM __attribute__ ((section (".iwram"), long_call))
#define IN_IWRAM __attribute__ ((section (".iwram")))
#define CODE_IN_EWRAM __attribute__ ((section (".ewram"), long_call))
#define IN_EWRAM __attribute__ ((section (".ewram")))
#define PACKED __attribute__ ((packed))


#define TOUCH_CAL_X1 (*(vs16*)0x027FFCD8)
#define TOUCH_CAL_Y1 (*(vs16*)0x027FFCDA)
#define TOUCH_CAL_X2 (*(vs16*)0x027FFCDE)
#define TOUCH_CAL_Y2 (*(vs16*)0x027FFCE0)
#define SCREEN_WIDTH    256
#define SCREEN_HEIGHT   192

extern s32 TOUCH_WIDTH;
extern s32 TOUCH_HEIGHT;
extern s32 TOUCH_OFFSET_X;
extern s32 TOUCH_OFFSET_Y;

extern u16 PA_NewSPI;


/*! \file PA7.h
    \brief Contains prototypes and macros for the arm7...

    Contains prototypes and macros for the arm7...
*/


extern inline u64 PA_Distance(s32 x1, s32 y1, s32 x2, s32 y2) {
   s64 h = x1 - x2;
   s64 v = y1 - y2;
   return(h*h + v*v);
}


void PA_Init(void);
//void InterruptHandler(void);
void PA_InitVBL(void);
void PA_UpdateStylus(void);
void PA_ScreenLight(void);








#ifdef __cplusplus
}
#endif


#endif



