#include "PA7.h"
#ifdef __cplusplus
extern "C" {
#endif
#include <nds.h>
#include "PA/PA_Sound.c"
#include "command.h"
#include "microphone7.h"

// precalculate some values
#define TOUCH_CNTRL_X1   (*(vu8*)0x027FFCDC)
#define TOUCH_CNTRL_Y1   (*(vu8*)0x027FFCDD)
#define TOUCH_CNTRL_X2   (*(vu8*)0x027FFCE2)
#define TOUCH_CNTRL_Y2   (*(vu8*)0x027FFCE3)

#define DISP_SR			(*(vuint16*)0x04000004)
int16 CNTRL_WIDTH;
int16 CNTRL_HEIGHT;

s32 TOUCH_WIDTH;
s32 TOUCH_HEIGHT;
/*s32 TOUCH_OFFSET_X;
s32 TOUCH_OFFSET_Y;*/
u16 PA_ReadSPI(void);

u16 PA_NewSPI;

void PA_Init(void){
 	rtcReset();

	//enable sound
	powerON(POWER_SOUND);
	SOUND_CR = SOUND_ENABLE | SOUND_VOL(0x7F);
	IPC->soundData = 0;
/*	IPC->aux = 0x0F;
	PA_NewSPI = 0x0F;*/
	
	
	PA_NewSPI = PA_ReadSPI();
	IPC->aux = PA_NewSPI;
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



/*static u8 touchInit = false;
static s32 xscale, yscale;
static s32 xoffset, yoffset;*/

// stylus code from libnds
//---------------------------------------------------------------------------------
void PA_UpdateStylus(void){
//---------------------------------------------------------------------------------

	touchPosition tempPos = touchReadXY();
	
	IPC->touchX    = tempPos.x;
    IPC->touchY    = tempPos.y;
    IPC->touchXpx  = tempPos.px;
    IPC->touchYpx  = tempPos.py;	
	
	IPC->touchZ1 = touchRead(TSC_MEASURE_Z1);
	IPC->touchZ2 = touchRead(TSC_MEASURE_Z2);
}

/*
void PA_ScreenLight(void){
	SerialWaitBusy();
	REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHz | SPI_CONTINUOUS;
	REG_SPIDATA = 0;
	SerialWaitBusy();
	REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHz;
	//u8 temp = ~0x0C;
	
	REG_SPIDATA = PA_NewSPI; // On met en fonction de ce qu'on a dans l'IPC
}*/

u16 PA_ReadSPI(void){
	u8 pmData;
 
	SerialWaitBusy();
	
	REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHz | SPI_CONTINUOUS; 
	REG_SPIDATA = (1 << 7); 
 
	SerialWaitBusy();
 
	REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHz; // On revient en mode single byte ... 
	REG_SPIDATA = 0; // On indique à nouveau qu'on veut lire ... 
	
	SerialWaitBusy();
	
	pmData = REG_SPIDATA; // Et on récupère la valeur du registre ! 
	
	REG_SPICNT = 0; // Pour finit on arrête le SPI ... 
 
	return pmData;
}
/*
u16 PA_ReadSPI(u8 pmReg){
	u8 pmData;
 
	SerialWaitBusy();
	
	REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHz | SPI_CONTINUOUS; 
	REG_SPIDATA = pmReg | (1 << 7); 
 
	SerialWaitBusy();
 
	REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHz; // On revient en mode single byte ... 
	REG_SPIDATA = 0; // On indique à nouveau qu'on veut lire ... 
	
	SerialWaitBusy();
	
	pmData = REG_SPIDATA; // Et on récupère la valeur du registre ! 
	
	REG_SPICNT = 0; // Pour finit on arrête le SPI ... 
 
	return pmData;
}*/

void PA_WriteSPI(u8 pmReg, u8 pmData)
{
	SerialWaitBusy();
	
	REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHz | SPI_CONTINUOUS;
	REG_SPIDATA = pmReg;
	SerialWaitBusy();
 
	REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHz;
	REG_SPIDATA = pmData; 
	SerialWaitBusy();
}



/*
void PA_ScreenLight(void){

	u8 pmValue;
	pmValue = PA_ReadSPI(0);
	pmValue &= ~(1 << 2) | ~(1 << 3);  // shut down by default
	pmValue |= PA_NewSPI&0xC; // and the correct values...
 
	PA_WriteSPI(0, pmValue);
}*/

void PA_ScreenLight(void){
	SerialWaitBusy();
	REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHz | SPI_CONTINUOUS;
	REG_SPIDATA = 0;
	SerialWaitBusy();
	REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHz;
	//u8 temp = ~0x0C;
	
	REG_SPIDATA = PA_NewSPI; // On met en fonction de ce qu'on a dans l'IPC
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

