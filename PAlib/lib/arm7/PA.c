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

PA_IPCType *PA_IPC;
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

// stylus code from libnds
//---------------------------------------------------------------------------------
/*void PA_UpdateStylus(void){
//---------------------------------------------------------------------------------
	tempPos = touchReadXY();
	
	IPC->touchX    = tempPos.x;
    IPC->touchY    = tempPos.y;
    IPC->touchXpx  = tempPos.px;
    IPC->touchYpx  = tempPos.py;	
	
	IPC->touchZ1 = touchRead(TSC_MEASURE_Z1);
	IPC->touchZ2 = touchRead(TSC_MEASURE_Z2);
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
	



void PA_Poweroff(void)
{
	SerialWaitBusy();
    REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHz | SPI_CONTINUOUS;
    REG_SPIDATA = 0;
    SerialWaitBusy();
    REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER | SPI_BAUD_1MHz;
    REG_SPIDATA = PM_SYSTEM_PWR;
}



void PA_SetDSLiteBrightness(u8 level){
	if (level > 3) level = 3;
    writePowerManagement(PM_DSLITE_REG, level);
}



/*

void PA_DSLiteBrightness(u32 msg)
{
    if(msg == ISLITE_QUERY)
    {
        REG_IPC_FIFO_TX = (bool)(readPowerManagement(PM_DSLITE_REG) & PM_IS_LITE);
        return;
    }

    // Is Lite?
    if((bool)(readPowerManagement(PM_DSLITE_REG) & PM_IS_LITE))
    {
        if(msg == GET_BRIGHTNESS)
        {
            REG_IPC_FIFO_TX = readPowerManagement(PM_DSLITE_REG) - 64;
            return;
        }    
        else if(msg == SET_BRIGHTNESS_0)
        {
            writePowerManagement(PM_DSLITE_REG, 0);
            return;
        }
        else if(msg == SET_BRIGHTNESS_1)
        {
            writePowerManagement(PM_DSLITE_REG, 1);
            return;
        }
        else if(msg == SET_BRIGHTNESS_2)
        {
            writePowerManagement(PM_DSLITE_REG, 2);
            return;
        }
        else if(msg == SET_BRIGHTNESS_3)
        {
            writePowerManagement(PM_DSLITE_REG, 3);
            return;
        }
        else if(msg == SET_BRIGHTNESS_NEXT)
        {
            s32 nextlevel = 
                readPowerManagement(PM_DSLITE_REG) - 64 + 1;
            if(nextlevel > 3) 
                nextlevel = 0;
            writePowerManagement(PM_DSLITE_REG, nextlevel);
            return;
        }
        else if(msg == SET_BRIGHTNESS_PREVIOUS)
        {
            s32 previouslevel = 
                readPowerManagement(PM_DSLITE_REG) - 64 - 1;
            if(previouslevel < 0)
                previouslevel = 3;
            writePowerManagement(PM_DSLITE_REG, previouslevel);
            return;
        }
        else if(msg == GET_BACKLIGHTS)
        {
            u32 backlight_bits = readPowerManagement(PM_CONTROL_REG) & PM_BACKLIGHTS;
            if(backlight_bits == 12)
                REG_IPC_FIFO_TX = 2;
            else if(backlight_bits == 8 || backlight_bits == 4)
                REG_IPC_FIFO_TX = 1;
            else
                REG_IPC_FIFO_TX = 0;
            return;
        }
        else if(msg == SET_BACKLIGHTS_ON)
        {
            u32 reg_with_backlight = readPowerManagement(PM_CONTROL_REG) | PM_BACKLIGHTS;
            writePowerManagement(PM_CONTROL_REG, reg_with_backlight);
            return;
        }
        else if(msg == SET_BACKLIGHTS_OFF)
        {
            u32 reg_without_backlight = readPowerManagement(PM_CONTROL_REG) & ~PM_BACKLIGHTS;
            writePowerManagement(PM_CONTROL_REG, reg_without_backlight & 255);
            return;
        }
        else if(msg == SET_BACKLIGHTS_TOGGLE)
        {
            u32 oldreg = readPowerManagement(PM_CONTROL_REG);
            if(oldreg & PM_BACKLIGHTS) 
                oldreg &= ~PM_BACKLIGHTS;
            else
                oldreg |= PM_BACKLIGHTS;
            writePowerManagement(PM_CONTROL_REG, oldreg);
            return;
        }
    } 
    else // Is Phatty!
    {
        if(msg == GET_BRIGHTNESS || 
            msg == GET_BACKLIGHTS)
        {
            u32 backlight_bits = readPowerManagement(PM_CONTROL_REG) & PM_BACKLIGHTS;
            if(backlight_bits == 12)
                REG_IPC_FIFO_TX = 2;
            else if(backlight_bits == 8 || backlight_bits == 4)
                REG_IPC_FIFO_TX = 1;
            else
                REG_IPC_FIFO_TX = 0;
            return;
        }    
        else if(msg == SET_BRIGHTNESS_0)
        {
            u32 reg_without_backlight = readPowerManagement(PM_CONTROL_REG) & ~PM_BACKLIGHTS;
            writePowerManagement(PM_CONTROL_REG, reg_without_backlight);
            return;
        }
        else if(msg == SET_BRIGHTNESS_1 ||
            msg == SET_BRIGHTNESS_2 ||
            msg == SET_BRIGHTNESS_3)
        {
            u32 reg_with_backlight = readPowerManagement(PM_CONTROL_REG) | PM_BACKLIGHTS;
            writePowerManagement(PM_CONTROL_REG, reg_with_backlight);
            return;
        }
        else if(msg == SET_BRIGHTNESS_NEXT ||
            msg == SET_BRIGHTNESS_PREVIOUS ||
            msg == SET_BACKLIGHTS_TOGGLE)
        {
            u32 oldreg = readPowerManagement(PM_CONTROL_REG);
            if(oldreg & PM_BACKLIGHTS) 
                oldreg &= ~PM_BACKLIGHTS;
            else
                oldreg |= PM_BACKLIGHTS;
            writePowerManagement(PM_CONTROL_REG, oldreg);
            return;
        }
        else if(msg == SET_BACKLIGHTS_ON)
        {
            u32 reg_with_backlight = readPowerManagement(PM_CONTROL_REG) | PM_BACKLIGHTS;
            writePowerManagement(PM_CONTROL_REG, reg_with_backlight);
            return;
        }
        else if(msg == SET_BACKLIGHTS_OFF)
        {
            u32 reg_without_backlight = readPowerManagement(PM_CONTROL_REG) & ~PM_BACKLIGHTS;
            writePowerManagement(PM_CONTROL_REG, reg_without_backlight);
            return;
        }
    } 
}
*/









#ifdef __cplusplus
}
#endif

