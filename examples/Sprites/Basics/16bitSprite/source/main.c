// Use 16 bit sprites ! No palettes needed for them, but they use twice as much memory...

#include <PA9.h>
#include "gfx/all_gfx.h"
#include "gfx/all_gfx.c"



//Main...
int main(void){

	PA_Init(); //PAlib Init...
	PA_InitVBL(); //VBL Init...
	
	
	// No palettes !
	
	PA_Create16bitSprite(0, // Screen
						0,	// Sprite number
						(void*)ship_Sprite, // image to load, 128 pixels wide !
						OBJ_SIZE_32X32, // sprite size
						0, // x
						50);	//y
													
	while(1){ // Infinite loop
		PA_WaitForVBL();	
	}
	return 0;
}
