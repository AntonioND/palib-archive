//copyright smealum http://jvteamds.free.fr
// Les Includes
#include <PA9.h>
#include "gfx/vaisseau.raw.c"
#include "gfx/master.pal.c"

//Fonction principale du code
int main(void)

{
//Initialision de PAlib
PA_Init();
PA_InitText(0,0);
PA_InitVBL();
PA_LoadPal(PAL_SPRITE0, master_Palette);
PA_CreateSprite(0, 0,(void*)vaisseau_Bitmap, OBJ_SIZE_32X32,1, 0, 50, 50);
int x = 0,y = 0;
while(1)
{
x = x + Pad.Held.Right - Pad.Held.Left;
y = y + Pad.Held.Up - Pad.Held.Down;
PA_SetSpriteXY(0,0,x,y);
PA_WaitForVBL();
//La boucle infinie du programme
}
return 0;
}
