
// Les Includes
#include <PA9.h>
#include "gfx/vaisseau.raw.c"
#include "gfx/master.pal.c"

//Fonction principale du code
int main(void)

{
//Initialision de PAlib
PA_Init();
PA_InitVBL();

PA_InitText(1,0); // On the top screen

// Load de sprite palette
PA_LoadPal(PAL_SPRITE0, master_Palette);

// Load a sprite... centerd
PA_CreateSprite(0, 0,(void*)vaisseau_Bitmap, OBJ_SIZE_32X32,1, 0, 128-16, 96-16);


// Now we'll get the angle between the sprite, the stlyus, and a horizontal line... This can be useful in games !
while(1)
{

PA_OutputText(1, 5, 10, "Angle : %d  ", PA_GetAngle(128, 96, Stylus.X, Stylus.Y));
// To get the angle, just give the coordinates of 2 points (here, the sprite's center and the stlyus)
// Detail : the angle is counterclock wise, and goes from 0 to 511... 


PA_WaitForVBL();
//La boucle infinie du programme
}
return 0;
}
