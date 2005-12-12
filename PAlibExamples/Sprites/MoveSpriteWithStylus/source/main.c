//copyright smealum http://jvteamds.free.fr
// Les Includes
#include <PA9.h>
#include "gfx/vaisseau.raw.c"
#include "gfx/master.pal.c"

//Fonction principale du code
int main(void)	{
	//Initialision de PAlib
	PA_Init();
	PA_InitVBL();
	
	PA_InitText(0,0);	
	
	PA_LoadPal(PAL_SPRITE0, master_Palette);
	PA_CreateSprite(0, 0,(void*)vaisseau_Bitmap, OBJ_SIZE_32X32,1, 0, 50, 50);
	
	while(1)
	{
		PA_OutputText(0,0,0,"Stylus X : %d \nStylus Y : %d   ",Stylus.X,Stylus.Y);
		PA_SetSpriteXY(0,0,Stylus.X,Stylus.Y);
		PA_WaitForVBL();
	}
	return 0;
}
