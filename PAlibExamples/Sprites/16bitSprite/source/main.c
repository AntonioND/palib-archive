// Use 16 bit sprites ! No palettes needed for them, but they use twice as much memory...

#include <PA9.h>
#include "gfx/mollusk.raw.c" //gfx2gba line : gfx2gba -D -fsrc -c32k -align *.bmp
/* !!!!!!!Warning : a 16bit sprite MUST be 128 pixels wide, even if you sprite only takes up a small part on the left...!!!!*/


//Main...
int main(void){

	PA_Init(); //PAlib Init...
	PA_InitVBL(); //VBL Init...
	
	// No palettes !
	
	PA_Create16bitSprite(1, // Screen
						0,	// Sprite number
						(void*)mollusk_Bitmap, // image to load, 128 pixels wide !
						OBJ_SIZE_64X64, // sprite size
						0, // x
						50);	//y

	
	while(1){ // Infinite loop
		PA_WaitForVBL();	
	}
	return 0;
}
