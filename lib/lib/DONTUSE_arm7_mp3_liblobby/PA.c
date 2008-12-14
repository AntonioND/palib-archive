#include "PA7.h"
#ifdef __cplusplus
extern "C" {
#endif
#include <nds.h>
//#include "command.h"

// precalculate some values
#define TOUCH_CNTRL_X1   (*(vu8*)0x027FFCDC)
#define TOUCH_CNTRL_Y1   (*(vu8*)0x027FFCDD)
#define TOUCH_CNTRL_X2   (*(vu8*)0x027FFCE2)
#define TOUCH_CNTRL_Y2   (*(vu8*)0x027FFCE3)

#define DISP_SR			(*(vuint16*)0x04000004)

// DS Lite screen brightness registers
#define PM_DSLITE_REG   (4)
#define PM_IS_LITE      BIT(6)
#define PM_BACKLIGHTS   (PM_BACKLIGHT_BOTTOM | PM_BACKLIGHT_TOP)

int16 CNTRL_WIDTH;
int16 CNTRL_HEIGHT;

s32 TOUCH_WIDTH;
s32 TOUCH_HEIGHT;

u16 PA_ReadSPI(void);

u16 PA_NewSPI;

volatile PA_IPCType *PA_IPC;
u8 PA_SoundBusyInit;

void PA_Init(void){
 	rtcReset();

	//enable sound
	powerON(POWER_SOUND);
	SOUND_CR = SOUND_ENABLE | SOUND_VOL(0x7F);
	IPC->soundData = 0;
	
	PA_NewSPI = PA_ReadSPI();
	IPC->aux = PA_NewSPI;
	
	MIC_On();
	PM_SetAmp(1);
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
touchPosition tempPos;


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


void PA_ScreenLight(void){
	SerialWaitBusy();
	REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHz | SPI_CONTINUOUS;
	REG_SPIDATA = 0;
	SerialWaitBusy();
	REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHz;
	
	REG_SPIDATA = PA_NewSPI; // On met en fonction de ce qu'on a dans l'IPC
}


void PA_SetDSLiteBrightness(u8 level){
	if (level > 3) level = 3;
    writePowerManagement(PM_DSLITE_REG, level);
}


#ifdef __cplusplus
}
#endif

