//copyright smealum http://jvteamds.free.fr
// Les Includes
#include <PA9.h>
#include "gfx/firstsprite.c"
#include "gfx/sprite0.pal.c"

//Fonction principale du code
int main(void)

{
//Initialision de PAlib
PA_Init();
PA_InitVBL();
PA_LoadPal(PAL_SPRITE0, sprite0_Pal);
PA_CreateSprite(0, 0,(void*)firstsprite_Sprite, OBJ_SIZE_32X32,1, 0, 50, 50);

while(1)
{

//La boucle infinie du programme
}
return 0;
}
