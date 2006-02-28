#include "PA9.h"


//////////////////////////////////////////////////////////////////////
// Interrupts
//////////////////////////////////////////////////////////////////////

u16 Int_Flag;

funcpointer interruptfunc[14];


volatile bool PA_vblok; // Passe � 1 quand VBL activ�...

volatile bool PA_Newframe;

void PA_UpdateSpriteAnims(void);


// infos pour le tst CPU
u8 PA_nVBLs = 0;
u32 PA_CPU = 0; // Pourcentage CPU...
u32 PA_MaxCPU = 0; // Max CPU...
u32 PA_lines = 0; // Nombre de lignes, pour faire le calcul...
u8 PA_VBLCount = 0; // Compteur de vbl





void PA_ResetInterrupts(void) {
u8 i;
REG_IME = 0x00;
REG_IE = 0;
REG_DISPSTAT = ENABLE_VBLANK; // Only interrupt to set by default...
REG_INTERRUPT = &interruptfunction;


   for (i = 0; i < 14; i++) {
      interruptfunc[i] = &PA_Nothing;
   }
}




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
if (inter == INT_KEY)    REG_KEYCNT = 49151; // Par d�faut, il faut appuyer sur n'importe quelle touche

//enable interrupts
REG_IME = 0x01;
}




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
if (inter == INT_KEY)    REG_KEYCNT = 0; // Par d�faut, il faut appuyer sur n'importe quelle touche
//enable interrupts
REG_IME = 0x01;
}





void interruptfunction(void) {

//disable interrupts
REG_IME = 0x00;
//backup the interrupt flags


Int_Flag = REG_IF;

// Check for all possible interrupts...
/*PA_OutputText(PA2 * 3, PAtest, "%d", Int_Flag);
++PAtest;
if (PAtest > 19) {PAtest = 0; ++PA2;}
*/

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






void PA_vblFunc(void){
//PA_OutputText(0, 0, 0, "VBL Ok");
PA_UpdateOAM(); // Updates the Object on screen
PA_UpdatePad(); // Updates the Keypad...
PA_UpdateStylus(); // Updates the stylus input
PA_UpdateMoveSprite(); // Met � jour les infos sur les d�placements de sprites
PA_UpdateRTC(); // Mise � jour de l'horloge...
PA_Newframe = 1; // Synch prog to screen

PA_UpdateSpriteAnims(); // Update the sprite animations...
++PA_nVBLs;
//PA_OutputText(0, 0, 10, "%d", PA_nVBLs);
}



