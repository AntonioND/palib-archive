#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"


//////////////////////////////////////////////////////////////////////
// Interrupts
//////////////////////////////////////////////////////////////////////

//u16 Int_Flag;

//funcpointer interruptfunc[14];
funcpointer CustomVBL;

volatile u8 PA_vblok; // Passe à 1 quand VBL activé...

volatile u8 PA_Newframe;

void PA_UpdateSpriteAnims(void);

volatile s32 PA_TestVBLs;


// infos pour le tst CPU
u8 PA_nVBLs = 0;
s32 PA_VBLCounter[16];  // VBL counters available
bool PA_VBLCounterOn[16];  // On or off
u32 PA_CPU = 0; // Pourcentage CPU...
u32 PA_MaxCPU = 0; // Max CPU...
u32 PA_lines = 0; // Nombre de lignes, pour faire le calcul...
u8 PA_VBLCount = 0; // Compteur de vbl


void PA_VBLCountersReset(void){
	u8 i;
	for (i = 0; i < 16; i++){
		PA_VBLCounter[i] = 0; // Ready to start
		PA_VBLCounterOn[i] = 0;  // Not counting yet
	}
}

/*
void PA_ResetInterrupts(void) {
u8 i;

   for (i = 0; i < 14; i++) {
      //interruptfunc[i] = &PA_Nothing;
	  irqDisable(BIT(i));
   }
}*/



/*
void PA_StartInt(u8 inter, funcpointer interfunc) {
//disable interrupts
REG_IME = 0x00;
REG_IE |= BIT(inter); // Enable interrupt..
interruptfunc[inter] = interfunc;

// En plus pour les HBL, VBL, et VCOUNT
if (inter == INT_HBLANK) REG_DISPSTAT |= ENABLE_HBLANK;
if (inter == INT_VBLANK) {
	REG_DISPSTAT |= ENABLE_VBLANK;
	DISP_SR |= DISP_VBLANK_IRQ;
	//VBLANK_INTR_WAIT_FLAGS |= IRQ_VBLANK;
}
if (inter == INT_VCOUNT) REG_DISPSTAT |= ENABLE_VCOUNT;
if (inter == INT_TIMER0) PA_EnableTimer0();
if (inter == INT_TIMER1) PA_EnableTimer1();
if (inter == INT_TIMER2) PA_EnableTimer2();
if (inter == INT_TIMER3) PA_EnableTimer3();
if (inter == INT_KEY)    REG_KEYCNT = 49151; // Par défaut, il faut appuyer sur n'importe quelle touche

//enable interrupts
REG_IME = 0x01;
}*/



/*
void PA_StopInt(u8 inter) {
//disable interrupts
REG_IME = 0x00;
REG_IE &= (65535 - BIT(inter)); // Enable interrupt..
interruptfunc[inter] = &PA_Nothing;

// En plus pour les HBL, VBL, et VCOUNT
if (inter == INT_HBLANK) REG_DISPSTAT &= (65535 - ENABLE_HBLANK);
if (inter == INT_VBLANK) {
	REG_DISPSTAT &= (65535 - ENABLE_VBLANK);
	//VBLANK_INTR_WAIT_FLAGS &= ~IRQ_VBLANK;
}
if (inter == INT_VCOUNT) REG_DISPSTAT &= (65535 - ENABLE_VCOUNT);
if (inter == INT_TIMER0) PA_DisableTimer0();
if (inter == INT_TIMER1) PA_DisableTimer1();
if (inter == INT_TIMER2) PA_DisableTimer2();
if (inter == INT_TIMER3) PA_DisableTimer3();
if (inter == INT_KEY)    REG_KEYCNT = 0; // Par défaut, il faut appuyer sur n'importe quelle touche
//enable interrupts
REG_IME = 0x01;
}*/




/*
void interruptfunction(void) {

//disable interrupts
REG_IME = 0x00;
//backup the interrupt flags


Int_Flag = REG_IF;


if(Int_Flag & INTVBLANK)  {
	PA_vblok = 1; 
	VBLANK_INTR_WAIT_FLAGS |= IRQ_VBLANK; 
	interruptfunc[0](); 
}

if(Int_Flag & INTHBLANK)   interruptfunc[1]();
if(Int_Flag & INTVCOUNT)   interruptfunc[2]();
if(Int_Flag & INTTIMER0)   interruptfunc[3]();
if(Int_Flag & INTTIMER1)   interruptfunc[4]();
if(Int_Flag & INTTIMER2)   interruptfunc[5]();
if(Int_Flag & INTTIMER3)   interruptfunc[6]();
if(Int_Flag & INTCOM)      interruptfunc[7]();
if(Int_Flag & INTDMA0)     interruptfunc[8]();
if(Int_Flag & INTDMA1)     interruptfunc[9]();
if(Int_Flag & INTDMA2)     interruptfunc[10]();
if(Int_Flag & INTDMA3)     interruptfunc[11]();
if(Int_Flag & INTBUTTON)   interruptfunc[12]();
if(Int_Flag & INTCART)     interruptfunc[13]();

//restore the interrupt flags
REG_IF = Int_Flag;
//enable interrupts
REG_IME = 0x01;

}

*/

extern inline void PA_RunCounters(void){
u8 i;
	for (i = 0; i < 16; i++)	PA_VBLCounter[i] += PA_VBLCounterOn[i]; // Counter
}




void PA_vblFunc(void){
//PA_OutputText(0, 0, 0, "VBL Ok");
PA_UpdateOAM(); // Updates the Object on screen
PA_UpdatePad(); // Updates the Keypad...
PA_UpdateStylus(); // Updates the stylus input
PA_UpdateMoveSprite(); // Met à jour les infos sur les déplacements de sprites
PA_UpdateRTC(); // Mise à jour de l'horloge...
DC_FlushRange((void*)PA_IPC, 256);// Flush the cache...

PA_Newframe = 1; // Synch prog to screen

++PA_nVBLs;
++PA_TestVBLs;
// Counters
PA_RunCounters();
CustomVBL(); // runs the user's custom VBL function


PA_UpdateSpriteAnims(); // Update the sprite animations...


}


#ifdef __cplusplus
}
#endif
