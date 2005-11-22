// Use 16 bit sprites ! No palettes needed for them, but they use twice as much memory...

#include <PA9.h>
#include "gfx/ship.c" 


//Main...
int main(void){

	PA_Init(); //PAlib Init...
	PA_InitVBL(); //VBL Init...
	
	// No palettes !
	
	PA_Create16bitSprite(1, // Screen
						0,	// Sprite number
						(void*)ship_Sprite, // image to load, 128 pixels wide !
						OBJ_SIZE_64X64, // sprite size
						0, // x
						50);	//y

	
	while(1){ // Infinite loop
		PA_WaitForVBL();	
	}
	return 0;
}
