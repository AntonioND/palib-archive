// Welcome to the magic of large and very large maps !

// Includes
#include <PA9.h>       // Include for PA_Lib



// Converted using PAGfxConverter
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"



void PA_Test(bool screen, u8 bg_select, s32 y);


// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	// Load on both screens at once...
	PA_DualLoadPAGfxLargeBg(3, //background number (0-3)
						zelda); // Name

	// Next we'll scroll, here are the variables...
	s32 scrollx = 0; 
	s32 scrolly = 0;

	
	
	// Infinite loop to keep the program running
while (1)
{

	// We need to change the scroll according to the held keys...
	scrollx += (Pad.Held.Right - Pad.Held.Left)*4; // scroll 4 pixels at a time
	scrolly += (Pad.Held.Down - Pad.Held.Up)*4; // Same thing
	
	PA_DualLargeScrollXY(3, // background number
						scrollx, // X scroll
						scrolly); // and Y scroll

	PA_WaitForVBL();
}
	
	return 0;
} // End of main()







/*
void PA_Test(bool screen, u8 bg_select, s32 y){
s32 i, j, lx, ly, tiley, tempx, tempy;
lx = scrollpos[screen][bg_select].lx;
ly = scrollpos[screen][bg_select].ly;
if (y < 0) y = 0;
if (y >= ly-192) y = ly-192;

lx = lx >> 3;
ly = ly >> 3;

PA_BGScrollY(screen,bg_select,y&255);

tempx = scrollpos[screen][bg_select].scrollx >> 3;
tempy = scrollpos[screen][bg_select].scrolly >> 3;


if (ly >= 32) { // Si moins de 32, pas besoin de faire du scrolling spécial, ca passe en hard !
// Verticalement
// Dans un sens...
    while (y - scrollpos[screen][bg_select].scrolly >= 8) {  // Tant qu'on a du retour sur l'affichage de la carte, on fait afficher la ligne suivante
        scrollpos[screen][bg_select].scrolly += 8; // On a décalé de la taille d'un tile...
		tempy = scrollpos[screen][bg_select].scrolly >> 3;
		// On fait le decallage en copiant tout comme il faut
		j = (tempy+26)&31;
		tiley = tempy+26;
		for (i = -2; i < scrollpos[screen][bg_select].maxx-10; i++) PA_SetLargeMapTile(screen, bg_select, (tempx + i)&63, j, scrollpos[screen][bg_select].bg_map[(tempx + i) + (tiley * lx)]);
	}
// Et dans l'autre
    while (y - scrollpos[screen][bg_select].scrolly <= -8) {  // Tant qu'on a du retour sur l'affichage de la carte, on fait afficher la ligne suivante
        scrollpos[screen][bg_select].scrolly -= 8; // On a décalé de la taille d'un tile...
		tempy = scrollpos[screen][bg_select].scrolly >> 3;
		// On fait le decallage en copiant tout comme il faut
		j = (tempy-2)&31;
		tiley = tempy-2;
		for (i = -2; i < scrollpos[screen][bg_select].maxx-10; i++) PA_SetLargeMapTile(screen, bg_select, (tempx + i)&63, j, scrollpos[screen][bg_select].bg_map[(tempx + i) + (tiley * lx)]);
	}
}	

}

*/



