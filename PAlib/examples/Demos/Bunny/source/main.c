
// Les Includes
#include <PA9.h>
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"



//Fonction principale du code
int main(void)	{

	//Initialision de PAlib
	PA_Init();
	PA_InitVBL();
	
	PA_LoadSpritePal(0, 0, (void*)bunny_Pal);	// Load the sprite palette
	
	PA_SetBgColor(0, PA_RGB(31, 31, 31));
	
	// Load a sprite...
	u8 i;
	PA_CreateSprite(0, 0,(void*)bunny_Sprite, OBJ_SIZE_64X64,1, 0, 128-32, 96-32);  //Create a sprites
	for (i = 0; i < 6; i++) {
	   PA_CreateSprite(0, i+1,(void*)bunnyrip_Sprite, OBJ_SIZE_64X64,1, 0, 128-32, 96-32);
	   PA_SetSpriteAnim(0, i+1, i);
}	
	
	
	
	PA_InitAllSpriteDraw();  // Inits the sprite draw mode, necessary for the pixel-perfect touch detection

	bool moved[6];
	for (i = 0; i < 6; i++) moved[i] = 0;


	while(1)
	{
	   for (i = 0; i < 6; i++) {
			bool temp = PA_MoveSpritePix(i+1);
			if ((temp == 1) && (moved[i] == 0) && (PA_Distance(PA_MovedSprite.X, 0, 128-32, 0) >= 20*20)) {
				PA_StartSpriteAnimEx(0, 0, 0, 5, 20, ANIM_UPDOWN, 2);
				moved[i] = 1;
			}	
			else if 	((temp == 1) && (moved[i] == 0) && (PA_Distance(PA_MovedSprite.X, 0, 128-32, 0) < 20*20))	{
			   PA_SetSpriteXY(0, i+1, 128-32 + ((PA_MovedSprite.X - (128-32))>>2), 96-32 + ((PA_MovedSprite.Y - (96-32))>>3));
			}   
		}
		u8 var = 0;
		for (i = 0; i < 6; i++) var+= moved[i];
		if (var == 6) PA_SetSpriteAnim(0, 0, 5);
					

		PA_WaitForVBL();
	}
	
	return 0;
}
