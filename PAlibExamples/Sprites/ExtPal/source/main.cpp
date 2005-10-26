//copyright birslip <http://birslip.pizz.biz>
// Includes
#include <PA9.h>       // Include for PA_Lib


// *** Sprites ***
// gfx2gba -D -fsrc -pSprites.pal -t8 Cloud.bmp
#include "gfx/ds.pal.c"
#include "gfx/ds.raw.c"

#include "gfx/giz.pal.c"
#include "gfx/giz.raw.c"

#include "gfx/gp.pal.c"
#include "gfx/gp.raw.c"

#include "gfx/psp.pal.c"
#include "gfx/psp.raw.c"


// Function: main()
int main(int argc, char ** argv)
{
	u8 i;
	
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	//PA_LoadSplash();  // PA_Lib splash screen
	
	// Init Extended Palette
	PA_InitSpriteExtPal();
	
	// Load 8bit palettes
	PA_LoadSpriteExtPal(0,0,(void*)ds_Palette);

	PA_LoadSpriteExtPal(0,1,(void*)giz_Palette);
	
	PA_LoadSpriteExtPal(0,2,(void*)gp_Palette);
	
	PA_LoadSpriteExtPal(0,3,(void*)psp_Palette);
	
	// Create sprites
	PA_CreateSprite(0,0,(void*)ds_Bitmap,OBJ_SIZE_64X64,1,0,PA_Rand()%192,PA_Rand()%128);
	
	PA_CreateSprite(0,1,(void*)giz_Bitmap,OBJ_SIZE_64X64,1,1,PA_Rand()%192,PA_Rand()%128);
	
	PA_CreateSprite(0,2,(void*)gp_Bitmap,OBJ_SIZE_64X64,1,2,PA_Rand()%192,PA_Rand()%128);
	
	PA_CreateSprite(0,3,(void*)psp_Bitmap,OBJ_SIZE_64X64,1,3,PA_Rand()%192,PA_Rand()%128);
	
	//Set the main palette
	BG_PALETTE[0] = PA_RGB(27,27,27); 
	// Infinite loop to keep the program running
	while (1)
	{
		PA_WaitForVBL();

		for(i=0;i<4;i++) PA_MoveSprite(i);
		
	}
	
	return 0;
} // End of main()