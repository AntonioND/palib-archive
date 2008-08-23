// Include any GIF file and output it on an 8bit or 16bit screen !

// Includes
#include <PA9.h>       // Include for PA_Lib

#include "Mollusk.h" // gif to include

u16 spritepal[256]; // our sprite palette



// Function: main()
int main(int argc, char ** argv){

	PA_Init();    // PA Init...
	PA_InitVBL();	// VBL Init...
	
	PA_SetBgColor(0, PA_RGB(31, 31, 31)); // White background color
	
	PA_GifToTexTransp(PA_RGB(31, 0, 31)); // Set transparency color for GIFs (magenta in this case)

	// First, convert the gif to a sprite format...
	u8 *spritegfx = PA_GifToTiles((void*)Mollusk, // Gif file
											spritepal); // our palette
											
	// Next, load the sprite...
	PA_CreateSprite(0, 0, spritegfx, OBJ_SIZE_64X64, 1, 0, 64, 64);  // Just like plain sprites !
	PA_LoadSpritePal(0, 0, spritepal);  // Just like plain palettes !


	// spritegfx can be used over and over again to load other sprites... 
	// If you do not need it anymore, use free(spritegfx); to free the memory it uses !

  while(1)  {
		PA_WaitForVBL();
	}
		
	return 0;
} // End of main()




