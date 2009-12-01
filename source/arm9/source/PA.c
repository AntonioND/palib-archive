#include <PA9.h>

u8 pa_checklid;
u16 pa_giftotexcolor = 0;

volatile PA_IPCType PA_IPC; // lololololol this is deprecated

typedef struct{
	s16 x, y, oldx, oldy, vx, vy; // Coordonnées
	u8 stylus; // Si on déplace ou non
	u8 moving; // Si la boule est en mouvement ou non
}positions;

u32 PA_bgmap[2][4]; // Pointeur vers les maps, 4 maps par screen
u8 PA_Screen = 0;

s16 PA_ScreenSpace; // Espace entre les 2 écrans...+192

PA_GifInfos PA_GifInfo;

Pads Pad;
PA_Pad* PadPointer;

PA_Stylus Stylus;

PA_movingsprite PA_MovedSprite; // Pour les sprites que l'on bouge...

u8 PA_MoveSpriteType = 0;

void PA_SetBrightness(u8 screen, s8 bright){
	u16 mode = 1 << 14;
	if(bright < 0){
		mode = 2 << 14;
		bright = -bright;
	}
	if(bright > 31) bright = 31;

	u16 brightreg = mode | bright;
	if(screen) REG_MASTER_BRIGHT_SUB = brightreg;
	else REG_MASTER_BRIGHT = brightreg;
}

void PA_Nothing(){
}

u8 PA_CheckLid(){
	if(!PA_LidClosed()) return 0;

	// Power down the ARM7
	fifoSendValue32(FIFO_PM, PM_REQ_SLEEP);

	// Wait 100 milliseconds
	swiDelay(419000);

	// Disable interrupts - except VBlank (though with no handler)
	u32 backup_if = REG_IF;
	REG_IF = 0;
	irqSet(IRQ_VBLANK, 0);
	REG_IF = IRQ_VBLANK;

	// Power down all ARM9 devices
	u16 backup_power = REG_POWERCNT;
	REG_POWERCNT = 0;

	// Wait for the lid to be opened again
	for(;;){
		swiWaitForVBlank();

		if(!(keysCurrent() & KEY_LID))
			break;
	}

	// Return the power
	REG_POWERCNT = backup_power;

	// Wait 100 milliseconds
	swiDelay(419000);

	// Reinit interrupts
	irqSet(IRQ_VBLANK, PA_vblFunc);
	REG_IF = backup_if;
	return 1;
}
