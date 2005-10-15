#include "PA7.h"

#include "PA/PA_Sound.c"
#include "command.h"
#include "microphone7.h"

// precalculate some values
#define TOUCH_CNTRL_X1   (*(vu8*)0x027FFCDC)
#define TOUCH_CNTRL_Y1   (*(vu8*)0x027FFCDD)
#define TOUCH_CNTRL_X2   (*(vu8*)0x027FFCE2)
#define TOUCH_CNTRL_Y2   (*(vu8*)0x027FFCE3)
int16 CNTRL_WIDTH;
int16 CNTRL_HEIGHT;

s32 TOUCH_WIDTH;
s32 TOUCH_HEIGHT;
/*s32 TOUCH_OFFSET_X;
s32 TOUCH_OFFSET_Y;*/


void PA_Init(void){
//	PA_InitSoundSystem(); // Initialisation des docs de son
//PA_SoundOk = 0;
//PA_InitVBL();

TOUCH_WIDTH  = TOUCH_CAL_X2 - TOUCH_CAL_X1;
TOUCH_HEIGHT = TOUCH_CAL_Y2 - TOUCH_CAL_Y1;/*
TOUCH_OFFSET_X = ( ((SCREEN_WIDTH -60) * TOUCH_CAL_X1) / TOUCH_WIDTH  ) - 28;
TOUCH_OFFSET_Y = ( ((SCREEN_HEIGHT-60) * TOUCH_CAL_Y1) / TOUCH_HEIGHT ) - 28;
*/
// precalculate some values
TOUCH_WIDTH  = TOUCH_CAL_X2 - TOUCH_CAL_X1;
TOUCH_HEIGHT = TOUCH_CAL_Y2 - TOUCH_CAL_Y1;
CNTRL_WIDTH  = TOUCH_CNTRL_X2 - TOUCH_CNTRL_X1;
CNTRL_HEIGHT = TOUCH_CNTRL_Y2 - TOUCH_CNTRL_Y1;


}



void PA_InitVBL(void){
  // Set up the interrupt handler
  REG_IME = 0;
 // IRQ_HANDLER = &InterruptHandler;
  REG_IE = IRQ_VBLANK;
  REG_IF = ~0;
  DISP_SR = DISP_VBLANK_IRQ;
  REG_IME = 1;
}


s32 oldx; s32 oldy; // Stylus positions...


void PA_UpdateStylus(void){

s32 x, y, xpx, ypx, z1, z2;

      // Read the touch screen
      x = touchRead(TSC_MEASURE_X);
      y = touchRead(TSC_MEASURE_Y);
      //xpx = ( ((SCREEN_WIDTH -60) * x) / TOUCH_WIDTH  ) - TOUCH_OFFSET_X;
      //ypx = ( ((SCREEN_HEIGHT-60) * y) / TOUCH_HEIGHT ) - TOUCH_OFFSET_Y;
	  
// reading pixel position:
	xpx = (x - (int16) TOUCH_CAL_X1) * CNTRL_WIDTH  / TOUCH_WIDTH  + (int16) TOUCH_CNTRL_X1;
	ypx = (y - (int16) TOUCH_CAL_Y1) * CNTRL_HEIGHT / TOUCH_HEIGHT + (int16) TOUCH_CNTRL_Y1;
 	  
	  
	  if (xpx < 0) xpx = 0;
	  if (xpx > 255) xpx = 255;
	  if (ypx < 0) ypx = 0;
	  if (ypx > 191) ypx = 191;	  
	  
	  
	  
      z1 = touchRead(TSC_MEASURE_Z1);
      z2 = touchRead(TSC_MEASURE_Z2);

    IPC->touchX    = x;
    IPC->touchY    = y;
    IPC->touchXpx    = xpx;
    IPC->touchYpx    = ypx;	
/*	
	if ((xpx > oldx+2)||(xpx < oldx-2)){
		IPC->touchXpx  = xpx;
		oldx = xpx;
	}
	if ((ypx > oldy+2)||(ypx < oldy-2)){
		IPC->touchYpx  = ypx;
		oldy = ypx;
	}	
	
	if (PA_Distance(xpx, ypx, oldx, oldy) > 3) { // Antitremblement...
		IPC->touchXpx  = xpx;
		IPC->touchYpx  = ypx;
		oldx = xpx;
		oldy = ypx;
	}*/
    IPC->touchZ1   = z1;
    IPC->touchZ2   = z2;


}



/*
void InterruptHandler(void) {
u8 i;
  static uint32 heartbeat = 0;

    s16 but=0, x=0, y=0, xpx=0, ypx=0, z1=0, z2=0, batt=0, aux=0;
    int t1=0, t2=0;
    uint32 temp=0;
    uint8 ct[sizeof(IPC->curtime)];

  if (IF & IRQ_VBLANK) {

    heartbeat++;
    IPC->heartbeat = heartbeat;

	//Si son non initialisé, on l'initialise
	if (IPC->mailRead == 2) PA_InitSoundSystem();

	// Si le son fonctionne, on regarde si on a lancé des nouveaux trucs...
	if (PA_SoundOk) {
		for (i = 0; i < 16; i++) {
			if(PA_StartSoundInfo[i].Action == ACTION_PLAY) {
				PA_StartSound(i, PA_StartSoundInfo[i].Sound, PA_StartSoundInfo[i].Volume);
			}
			else if (PA_StartSoundInfo[i].Action == ACTION_STOP) {
				PA_StopSound(i);
			}
		}
	}


	// Read the X/Y buttons and the /PENIRQ line
    but = XKEYS;
    if (!(but & 0x40)) {
		// Read the touch screen
		x = touchRead(TSC_MEASURE_X);
		y = touchRead(TSC_MEASURE_Y);
		xpx = ( ((SCREEN_WIDTH -60) * x) / TOUCH_WIDTH  ) - TOUCH_OFFSET_X;
		ypx = ( ((SCREEN_HEIGHT-60) * y) / TOUCH_HEIGHT ) - TOUCH_OFFSET_Y;
		z1 = touchRead(TSC_MEASURE_Z1);
		z2 = touchRead(TSC_MEASURE_Z2);
		if (xpx < 0) xpx = 0;
		if (xpx > SCREEN_WIDTH-1) xpx = SCREEN_WIDTH-1;
		if (ypx < 0) ypx = 0;
		if (ypx > SCREEN_HEIGHT-1) ypx = SCREEN_HEIGHT-1;
	}

    batt = touchRead(TSC_MEASURE_BATTERY);
    aux  = touchRead(TSC_MEASURE_AUX);

    // Read the time
    rtcGetTime((uint8 *)ct);
    BCDToInteger((uint8 *)&(ct[1]), 7);
 
    // Read the temperature
    temp = touchReadTemperature(&t1, &t2);
 
    // Update the IPC struct
    IPC->heartbeat = heartbeat;
    IPC->buttons   = but;
    IPC->touchX    = x;
    IPC->touchY    = y;
    IPC->touchXpx  = xpx;
    IPC->touchYpx  = ypx;
    IPC->touchZ1   = z1;
    IPC->touchZ2   = z2;
    IPC->battery   = batt;
    IPC->aux       = aux;

    for(u32 i=0; i<sizeof(ct); i++) {
      IPC->curtime[i] = ct[i];
    }

    IPC->temperature = temp;
    IPC->tdiode1 = t1;
    IPC->tdiode2 = t2;

  }

  // Acknowledge interrupts
  IF = IF;
}
*/
