
// Includes
#include <PA9.h>

extern const unsigned short master_Palette[256];
extern const unsigned char vaisseau_Bitmap[1024];

int main(void){

	PA_Init();
	
	PA_LoadDefaultText(1,0); // On the top screen

	PA_LoadSpritePal(0, 0, (void*)master_Palette);

	PA_CreateSprite(0, 0,(void*)vaisseau_Bitmap, OBJ_SIZE_32X32,1, 0, 128-16, 96-16);

	while(1)
	{
		
		PA_OutputText(1, 5, 10, "Angle : %d  ", PA_GetAngle(128, 96, Stylus.X, Stylus.Y));

		
		PA_WaitForVBL();

	}
	return 0;
}