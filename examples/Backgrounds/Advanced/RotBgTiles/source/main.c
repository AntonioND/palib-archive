#include <PA9.h>

#include "all_gfx.h"

int main(){
	PA_Init();
	
	PA_SetVideoMode(0, 2);  //screen, mode

	PA_LoadBackground(0, //screen
	                  3, // background number
	                  &Rot); // background name in PAGfx
	
	PA_SetBgWrap(0, 3, 1);
	PA_LoadDefaultText(1, 0);
	
	PA_OutputSimpleText(1, 0, 10, "Stylus to change a given RotTile");
	PA_OutputSimpleText(1, 0, 12, "Up/Down to change tile number: ");	
	
	int tilenumber = 0;

	while(true){
		// Change tile number according to keys
		if (Pad.Newpress.Up && (tilenumber < 15)) tilenumber++;  // limited number of tiles on this background, just for test :)
		if (Pad.Newpress.Down && (tilenumber >0)) tilenumber--;	
			
		PA_OutputText(1, 2, 13, "Tile : %02d", tilenumber); // Display tile number
		
		if(Stylus.Held)
			PA_SetRotMapTile(0, // screen
			                 3, //bg_select
			                 Stylus.X >> 3,  // X in tiles, each tile being 8x8 you divide by 8 (equivalent to >> 3)
			                 Stylus.Y >> 3, // Same as for X, but for Y...
			                 tilenumber); // Tile Number, 0-255

		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()