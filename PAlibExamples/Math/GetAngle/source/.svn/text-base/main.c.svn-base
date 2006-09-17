
// Includes
#include <PA9.h>
#include "gfx/vaisseau.raw.c"
#include "gfx/master.pal.c"

int main(void){

	PA_Init();
	PA_InitVBL();
	
	PA_InitText(1,0); // On the top screen

	PA_LoadSpritePal(0, 0, (void*)master_Palette);

	PA_CreateSprite(0, 0,(void*)vaisseau_Bitmap, OBJ_SIZE_32X32,1, 0, 128-16, 96-16);

	while(1)
	{
		
		PA_OutputText(1, 5, 10, "Angle : %d  ", PA_GetAngle(128, 96, Stylus.X, Stylus.Y));

		
		PA_WaitForVBL();

	}
	return 0;
}
