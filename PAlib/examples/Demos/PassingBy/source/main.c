// Include any GIF file and output it on an 8bit or 16bit screen !

// Includes
#include <PA9.h>       // Include for PA_Lib

#include "Mollusk.h" // gif to include
#include "Zoom.h" // sound to include

u16 spritepal[256]; // our sprite palette


// Function: main()
int main(int argc, char ** argv){

	PA_Init();    // PA Init...
	PA_InitVBL();	// VBL Init...
	
	// Init the sound system
	AS_Init(AS_MODE_SURROUND | AS_MODE_16CH);
	AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND);
	
	PA_SetBgColor(0, PA_RGB(31, 31, 31)); // White background color

	// First, convert the gif to a sprite format...
	u8 *spritegfx = PA_GifToTiles((void*)Mollusk, // Gif file
											spritepal); // our palette
	
	// Next, load the sprite...
	PA_CreateSprite(0, 0, spritegfx, OBJ_SIZE_64X64, 1, 0, 64, 64);  // Just like plain sprites !
	PA_LoadSpritePal(0, 0, spritepal);  // Just like plain palettes !

	// spritegfx can be used over and over again to load other sprites... 
	// If you do not need it anymore, use free(spritegfx); to free the memory it uses !

	s32 spritex = -64;  // Sprite position
	s32 pan = 0;   // Audio panning
	
	PA_SetSpriteHflip(0, 0, 1); // Turn sprite around...	again...	
	
  while(1)  {  
  		int channel0 = PA_PlaySimpleSound(Zoom); // Play sound...

		for (spritex = -64; spritex < 256; spritex +=2){
			pan = spritex/2; 
			if (pan < 0) pan = 0; if (pan > 127) pan = 127; // Limit range...
			
			PA_SetSpriteX(0, 0, spritex);
			PA_SetSoundChannelPan(channel0, pan);
			PA_WaitForVBL();
		}
		
		PA_SetSpriteHflip(0, 0, 0); // Turn sprite around...
		
		int channel1 = PA_PlaySimpleSound(Zoom);
 
 		for (spritex = 256; spritex > -65; spritex -=2){
			pan = spritex/2; 
			if (pan < 0) pan = 0; if (pan > 127) pan = 127; // Limit range...
			
			PA_SetSpriteX(0, 0, spritex);
			PA_SetSoundChannelPan(channel1, pan);
			PA_WaitForVBL();
		} 
		
		PA_SetSpriteHflip(0, 0, 1); // Turn sprite around...	again...	
  
	}
		
	return 0;
} // End of main()




