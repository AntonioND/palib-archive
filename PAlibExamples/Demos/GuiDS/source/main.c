//-------------------------//
// © http://www.ndsdev.de  //
//-------------------------//



// Includes
#include <PA9.h>       // Include for PA_Lib


// Graphics converted using PAGfxConverter
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	PA_InitText(0, 2); // Initializes the text system on the touchscreen

	bool mnuopened; mnuopened = false;
	PA_LoadTiledBg(0, 3, bg0);	// Loads the background


	void startbutton(void);
	
	PA_SetTextCol  	(0, 0, 0, 0); // Sets color of the text on touchscreen

	PA_LoadSpritePal(0, 0, (void*)sp6_Pal); // Our mousepointer
	PA_CreateSprite(0, 0, (void*)sp6_Sprite, OBJ_SIZE_16X16, 1, 0, 128, 96); // Our mousepointer


	while (1)
	{

		PA_SetSpriteXY(0 , 0, Stylus.X, (Stylus.Y + 4)); // Position for our mousepointer

		PA_OutputText(0, 27, 1, "%02d:%02d", PA_RTC.Hour, PA_RTC.Minutes); // Displays the time

// Needed, or the mnuopened counter would change from true to false continuously
		if  ((Stylus.Newpress) && PA_StylusInZone(0, 0, 40, 20) && // Defines the zone where the stylus should be pressed
		(mnuopened == false)) // Counter (needed to open the menu) 
		
	{		PA_LoadSpritePal(0,	1, (void*)sp0_Pal);
			PA_CreateSprite(0, 1, (void*)sp0_Sprite, OBJ_SIZE_64X64, 1, 1,	0, 0);
		
			PA_LoadSpritePal(0,	2, (void*)sp1_Pal);
			PA_CreateSprite(0, 2, (void*)sp1_Sprite, OBJ_SIZE_64X64, 1, 2,	64, 0);
		
			PA_LoadSpritePal(0,	3, (void*)sp2_Pal);
			PA_CreateSprite(0, 3, (void*)sp2_Sprite, OBJ_SIZE_64X64, 1, 3,	0, 64);
		
			PA_LoadSpritePal(0,	4, (void*)sp3_Pal);
			PA_CreateSprite(0, 4, (void*)sp3_Sprite, OBJ_SIZE_64X64, 1, 4,	64, 64);
		
			PA_LoadSpritePal(0,	5, (void*)sp4_Pal);
			PA_CreateSprite(0, 5, (void*)sp4_Sprite, OBJ_SIZE_64X64, 1, 5,	0, 128);
		
			PA_LoadSpritePal(0,	6, (void*)sp5_Pal);
			PA_CreateSprite(0, 6, (void*)sp5_Sprite, OBJ_SIZE_64X64, 1, 6,	64, 128);

			//creates the menu

			mnuopened = true; // sets back the counter

	}

	else if  ((Stylus.Newpress) && // Needed, or the mnuopened counter would change from true to false continuously
		PA_StylusInZone(0, 0, 40, 20) && // Defines the zone where the stylus should be pressed
		(mnuopened == true)) // Counter (needed to close the menu) 
			
	{		PA_DeleteSprite( 0, 1);
			PA_DeleteSprite( 0, 2);
			PA_DeleteSprite( 0, 3);
			PA_DeleteSprite( 0, 4);
			PA_DeleteSprite( 0, 5);
			PA_DeleteSprite( 0, 6);
			
			// deletes the menu

			mnuopened = false; // sets back the counter
	}

			// for every item in the start menu you have to create another bool,
			// but be careful: you have to define the zone where the stylus should be
			// pressed again and there has to be another condition
			// ----------------------------------------
			// example : if  ((Stylus.Newpress) &&
			// PA_StylusInZone(XX, XX, XX, XX)
			// (mnuopened == XXXXX)) &&
			// (settingsopened == XXXXX))
			// ----------------------------------------
			// of course you can add some buttons to the background
	
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()

