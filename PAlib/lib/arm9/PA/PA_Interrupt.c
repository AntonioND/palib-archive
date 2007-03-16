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
funcpointer MotionVBL;

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





extern inline void PA_RunCounters(void){
u8 i;
	for (i = 0; i < 16; i++)	PA_VBLCounter[i] += PA_VBLCounterOn[i]; // Counter
}


u8 PA_oldVolume;

extern inline void PA_UpdateMoveSprite(void) {
	PA_MovedSprite.Time++;
	if ((PA_MovedSprite.Time > 2) || Stylus.Released) {
		PA_MovedSprite.Moving = 0;
		PA_MovedSprite.Time = 0;
	}
}

void PA_vblFunc(void){
	//PA_OutputText(0, 0, 0, "VBL Ok");
	DC_FlushAll();
	PA_UpdatePad(); // Updates the Keypad...
	PA_UpdateStylus(); // Updates the stylus input
	PA_UpdateMoveSprite(); // Met à jour les infos sur les déplacements de sprites
	PA_UpdateOAM(); // Updates the Object on screen
	MotionVBL(); // Update DS Motion info

	
	PA_UpdateRTC(); // Mise à jour de l'horloge...
	if((PA_oldVolume >= PA_IPC.Mic.Volume-1)&&(PA_oldVolume <= PA_IPC.Mic.Volume+1))	PA_IPC.Mic.Volume = PA_oldVolume;
	else PA_oldVolume = PA_IPC.Mic.Volume;
	
	PA_Newframe = 1; // Synch prog to screen
	
	++PA_nVBLs;
	++PA_TestVBLs;
	// Counters
	PA_RunCounters();
	CustomVBL(); // runs the user's custom VBL function
	
	
	PA_UpdateSpriteAnims(); // Update the sprite animations... done last because less important...


}


#ifdef __cplusplus
}
#endif
