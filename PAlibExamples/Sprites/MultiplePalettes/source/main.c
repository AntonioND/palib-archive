//copyright birslip <http://birslip.pizz.biz>
// Includes
#include <PA9.h>       // Include for PA_Lib



// Converted using PAGfxConverter
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

// Function: main()
int main(int argc, char ** argv)
{
	u8 i;
	
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	
	// Load 8bit palettes for sprites...
	PA_LoadSpritePal(0,0,(void*)ds_Pal);
	PA_LoadSpritePal(0,1,(void*)giz_Pal);
	PA_LoadSpritePal(0,2,(void*)gp_Pal);
	PA_LoadSpritePal(0,3,(void*)psp_Pal);
	
	// Create sprites
	PA_CreateSprite(0,0,(void*)ds_Sprite,OBJ_SIZE_64X64,1,0,PA_Rand()%192,PA_Rand()%128);
	
	PA_CreateSprite(0,1,(void*)giz_Sprite,OBJ_SIZE_64X64,1,1,PA_Rand()%192,PA_Rand()%128);
	
	PA_CreateSprite(0,2,(void*)gp_Sprite,OBJ_SIZE_64X64,1,2,PA_Rand()%192,PA_Rand()%128);
	
	PA_CreateSprite(0,3,(void*)psp_Sprite,OBJ_SIZE_64X64,1,3,PA_Rand()%192,PA_Rand()%128);
	
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
