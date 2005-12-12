#include "PA9.h"


//////////////////////////////////////////////////////////////////////
// Interrupts
//////////////////////////////////////////////////////////////////////

u16 Int_Flag;

funcpointer interruptfunc[14];


volatile bool PA_vblok; // Passe à 1 quand VBL activé...

volatile bool PA_Newframe;




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
if (inter == INT_KEY)    REG_KEYCNT = 49151; // Par défaut, il faut appuyer sur n'importe quelle touche

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
if (inter == INT_KEY)    REG_KEYCNT = 0; // Par défaut, il faut appuyer sur n'importe quelle touche
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

if(Int_Flag & INTVBLANK)  {PA_vblok = 1; VBLANK_INTR_WAIT_FLAGS |= IRQ_VBLANK; interruptfunc[0](); }
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




void PA_UpdateSpriteAnims(void)
{
u16 anims = nspriteanims;
u8 currentsprite = 0;
u8 screen = 0;
//	PA_SetSpriteAnimEx(screen, sprite, , PA_GetSpriteLy(screen, sprite), //PA_GetSpriteColors(screen, sprite), animframe);


while((anims > 0) && (currentsprite < 128))
{
	for (screen = 0; screen < 2; screen++){
		if (spriteanims[screen][currentsprite].play)
		{
			spriteanims[screen][currentsprite].time += spriteanims[screen][currentsprite].speed;
			if (spriteanims[screen][currentsprite].time >= 60) 
			{
				while (spriteanims[screen][currentsprite].time >= 60)
				{
					spriteanims[screen][currentsprite].time -= 60;
					spriteanims[screen][currentsprite].currentframe++;
					if (spriteanims[screen][currentsprite].currentframe > spriteanims[screen][currentsprite].lastframe)
						spriteanims[screen][currentsprite].currentframe = spriteanims[screen][currentsprite].firstframe;
				}
				
				PA_SetSpriteAnimEx(screen, currentsprite, spriteanims[screen][currentsprite].lx, spriteanims[screen][currentsprite].ly, spriteanims[screen][currentsprite].colors, spriteanims[screen][currentsprite].currentframe);
				
			}
			anims--; // Une de faite !
		}
	}
	currentsprite++; // next sprite...
}

}





void PA_vblFunc(void){

PA_UpdateOAM(); // Updates the Object on screen
PA_UpdatePad(); // Updates the Keypad...
PA_UpdateStylus(); // Updates the stylus input
PA_UpdateMoveSprite(); // Met à jour les infos sur les déplacements de sprites
PA_UpdateRTC(); // Mise à jour de l'horloge...
PA_Newframe = 1; // Synch prog to screen

PA_UpdateSpriteAnims(); // Update the sprite animations...

/*
++PA_nVBLs;
++PA_VBLCount;

if (PA_nVBLs == 64) { // Arrivé à 64, on calcule une moyenne de temps CPU...
	PA_CPU = ((100 * PA_lines) >> 6)/263;
	if (PA_CPU > PA_MaxCPU) PA_MaxCPU = PA_CPU;

	PA_nVBLs = 0;
	PA_lines = 0;
}*/
//test
//	PA_OutputText(1, 3, 3, "normal x : %d, y : %d   ", Stylus.X, Stylus.Y);
//	PA_OutputText(1, 3, 5, "autre  x : %d, y : %d   ", ((IPC->touchX - 0x0113) / 14), ((IPC->touchY - 0x00E0) / 19));

}



