#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"


//////////////////////////////////////////////////////////////////////
// Interrupts
//////////////////////////////////////////////////////////////////////

//u16 Int_Flag;

//funcpointer interruptfunc[14];
funcpointer CustomVBL = PA_Nothing;
funcpointer MotionVBL = PA_Nothing;
funcpointer GHPadVBL = PA_Nothing;
funcpointer PaddleVBL = PA_Nothing;
funcpointer SpriteVBL = PA_Nothing;

volatile u8 PA_vblok; // Passe à 1 quand VBL activé...

volatile u8 PA_Newframe;

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

void PA_vblFunc(void)
{
	DC_FlushAll();
	PA_UpdatePad(); // Updates the Keypad...
	PA_UpdateStylus(); // Updates the stylus input
	
	(*SpriteVBL)();
	
	MotionVBL(); // Update DS Motion info
	GHPadVBL(); // Update Guitar Hero pad info
	PaddleVBL(); // Update Taito Paddle info

	PA_UpdateRTC(); // Mise à jour de l'horloge...
	
	PA_Newframe = 1; // Synch prog to screen
	
	++PA_nVBLs;
	++PA_TestVBLs;
	
	// Counters
	PA_RunCounters();
	if (CustomVBL)
		CustomVBL(); // runs the user's custom VBL function
}

#ifdef __cplusplus
}
#endif
