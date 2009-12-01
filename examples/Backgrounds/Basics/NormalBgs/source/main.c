// This example just shows how the DS can be intelligent... Loading 4 different backgrounds, each with it's own palette...

// Includes
#include <PA9.h>

// Graphics converted using PAGfx
#include "all_gfx.h"

int main(){
	PA_Init();

	// Load the 4 Backgrounds
	PA_LoadBackground(0, 0, &bg0);	
	PA_LoadBackground(0, 1, &bg1);	
	PA_LoadBackground(0, 2, &bg2);	
	PA_LoadBackground(0, 3, &bg3);	

	// Infinite loop to keep the program running
	while(true){
		PA_WaitForVBL();
	}
}