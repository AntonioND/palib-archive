#include "PA7.h"
#ifdef __cplusplus
extern "C" {
#endif
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


u16 screenlights;

void PA_Init(void){
//	PA_InitSoundSystem(); // Initialisation des docs de son
//PA_SoundOk = 0;
//PA_InitVBL();

// precalculate some values
/*
TOUCH_WIDTH  = TOUCH_CAL_X2 - TOUCH_CAL_X1;
TOUCH_HEIGHT = TOUCH_CAL_Y2 - TOUCH_CAL_Y1;
CNTRL_WIDTH  = TOUCH_CNTRL_X2 - TOUCH_CNTRL_X1;
CNTRL_HEIGHT = TOUCH_CNTRL_Y2 - TOUCH_CNTRL_Y1;
*/

//screenlights = (IPC->aux&0xC);  // init the screen lights ok


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



static bool touchInit = false;
static s32 xscale, yscale;
static s32 xoffset, yoffset;

// stylus code from libnds
//---------------------------------------------------------------------------------
void PA_UpdateStylus(void){
//---------------------------------------------------------------------------------


	touchPosition touchPos;


	if ( !touchInit ) {


		xscale = ((PersonalData->calX2px - PersonalData->calX1px) << 19) / ((PersonalData->calX2) - (PersonalData->calX1));
		yscale = ((PersonalData->calY2px - PersonalData->calY1px) << 19) / ((PersonalData->calY2) - (PersonalData->calY1));

//		xoffset = (PersonalData->calX1) * xscale - (PersonalData->calX1px << 19);
//		yoffset = (PersonalData->calY1) * yscale - (PersonalData->calY1px << 19);


		xoffset = ((PersonalData->calX1 + PersonalData->calX2) * xscale  - ((PersonalData->calX1px + PersonalData->calX2px) << 19) ) / 2;
		yoffset = ((PersonalData->calY1 + PersonalData->calY2) * xscale  - ((PersonalData->calY1px + PersonalData->calY2px) << 19) ) / 2;
		touchInit = true;

	}

	s32 x,y;
	
	x =  touchRead(TSC_MEASURE_X | 1);
	y =  touchRead(TSC_MEASURE_Y | 1);
	x += 3 * touchRead(TSC_MEASURE_X | 1);
	y += 3 * touchRead(TSC_MEASURE_Y | 1);
	x += 5 * touchRead(TSC_MEASURE_X | 1);
	y += 5 * touchRead(TSC_MEASURE_Y | 1);
	x += 7 * touchRead(TSC_MEASURE_X);
	y += 7 * touchRead(TSC_MEASURE_Y);

	touchPos.x = x/16;
	touchPos.y = y/16;

	s16 px = ( touchPos.x * xscale - xoffset + xscale/2 ) >>19;
	s16 py = ( touchPos.y * yscale - yoffset + yscale/2 ) >>19;
	
	py+=40;

	if ( px < 0) px = 0;
	if ( py < 0) py = 0;
	if ( px > (SCREEN_WIDTH -1)) px = SCREEN_WIDTH -1;
	if ( py > (SCREEN_HEIGHT -1)) py = SCREEN_HEIGHT -1;

	touchPos.px = px;
	touchPos.py = py;
	
	IPC->touchX    = touchPos.x;
    IPC->touchY    = touchPos.y;
    IPC->touchXpx  = px;
    IPC->touchYpx  = py;	

	//return touchPos;

}


/*

void PA_UpdateStylus(void){

s32 x, y, xpx, ypx, z1, z2;

	// New touch code...
	touchPosition tempPos = touchReadXY();

		x = tempPos.x;
		y = tempPos.y;
		xpx = tempPos.px;
		ypx = tempPos.py+40;
		
 	  
	  
	  if (xpx < 0) xpx = 0;
	  if (xpx > 255) xpx = 255;
	  if (ypx < 0) ypx = 0;
	  if (ypx > 191) ypx = 191;	  
	  
	  
	  
      z1 = touchRead(TSC_MEASURE_Z1);
      z2 = touchRead(TSC_MEASURE_Z2);

    IPC->touchX    = x;
    IPC->touchY    = y;
    IPC->touchXpx  = xpx;
    IPC->touchYpx  = ypx;	

    IPC->touchZ1   = z1;
    IPC->touchZ2   = z2;
}*/


void PA_ScreenLight(void){
	SerialWaitBusy();
	REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHz | SPI_CONTINUOUS;
	REG_SPIDATA = 0;
	SerialWaitBusy();
	REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHz;
	REG_SPIDATA = screenlights; // On met en fonction de ce qu'on a dans l'IPC
}


void PA_Poweroff(void)
{
	SerialWaitBusy();
    REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHz | SPI_CONTINUOUS;
    REG_SPIDATA = 0;
    SerialWaitBusy();
    REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHz;
    REG_SPIDATA = PM_SYSTEM_PWR;
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




#ifdef __cplusplus
}
#endif

