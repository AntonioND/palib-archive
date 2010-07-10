// This example shows how to use 8 bit backgrounds, 
// on which you can draw with the stylus or show text in different sizes, colors, or rotations

// Includes
#include <PA9.h>

// Include the newly converted fonts...
#include "all_gfx.h"

int main(){
	PA_Init();
	
	// Let's put some colors... in the background palettes
	PA_SetBgPalCol(0, 1, PA_RGB(31, 31, 31));
	PA_SetBgPalCol(0, 2, PA_RGB(31, 0, 0));	
	PA_SetBgPalCol(0, 3, PA_RGB(0, 0, 31));
	PA_SetBgPalCol(0, 4, PA_RGB(0, 31, 0));
	PA_SetBgPalCol(1, 1, PA_RGB(31, 31, 31));
	PA_SetBgPalCol(1, 2, PA_RGB(31, 0, 0));	
	PA_SetBgPalCol(1, 3, PA_RGB(0, 0, 31));
	PA_SetBgPalCol(1, 4, PA_RGB(0, 31, 0));
	
	// Let's not load any backgrounds :p
	
	// This will initialise an  bit background on each screen. This must be loaded before any other background.
	// If you need to load this after a backgrounds, you'll have to use PA_ResetBgSys, PA_Init8bit, then reload
	// your backgrounds...
	PA_Init8bitBg(0, 3);
	PA_Init8bitBg(1, 3);

	// Slots 0-4 are already used by default PAlib fonts, so we'll load on slots 5 and 6. 
	// Available slots : 5-9. You can overwrite a PAlib font though if you want to use slot 0 for example...
	PA_AddBitmapFont(5, &smallfont);
	PA_AddBitmapFont(6, &bigfont);	

	PA_8bitText(1, 0, 20, 255, 40, "Small custom font...", 1, 5, 1, 100);
	PA_8bitText(1, 0, 40, 255, 60, "Big custom font...", 1, 6, 1, 100);

	// Different colors on the bottom sreen...
	int i;
	for(i = 0; i < 10; i++)
		PA_8bitText(0, 10, i*20, 255, i*20 + 20, "Hello World", i+1, 6, 1, 100);	

	// Infinite loop to keep the program running
	while(true){
		PA_WaitForVBL();
	}
}