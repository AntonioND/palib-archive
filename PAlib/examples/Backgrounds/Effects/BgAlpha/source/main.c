// This example just shows how the DS can be intelligent... Loading 4 different backgrounds, each with it's own palette...

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
	
	PA_InitText(1, 0);
	PA_OutputText(1,3,2, "--- BG Alpha Example ---");
	PA_OutputText(1,3,4, "D-Pad Left :    Alpha1--");
	PA_OutputText(1,3,5, "D-Pad Right:    Alpha1++");
	PA_OutputText(1,3,6, "D-Pad Up   :    Alpha2--");
	PA_OutputText(1,3,7, "D-Pad Down :    Alpha2++");

	//turn on alpha....
	PA_EnableSpecialFx(0, SFX_ALPHA, SFX_BG0 | SFX_BG1 | SFX_BG2 | SFX_BG3 | SFX_OBJ | SFX_BD, SFX_BG0 | SFX_BG1 | SFX_BG2 | SFX_BG3 | SFX_OBJ | SFX_BD);

	// Load the Backgrounds
	PA_EasyBgLoad(0, 0, bg0);	
	PA_EasyBgLoad(0, 1, bg1);	
	
	int alpha1 = 15;
	int alpha2 = 15;
	PA_SetSFXAlpha(0, alpha1, alpha2);
	// Infinite loop to keep the program running
	while (1)
	{
      
		if (Pad.Newpress.Left && alpha1 > 0) alpha1--;
		else if (Pad.Newpress.Right && alpha1 < 31) alpha1++;

		if (Pad.Newpress.Down && alpha2 > 0) alpha2--;
		else if (Pad.Newpress.Up && alpha2 < 31) alpha2++;

		//update the alpha values
		PA_SetSFXAlpha(0, alpha1, alpha2);
		
		PA_OutputText(1,2,9, "Alpha 1: %d   ", alpha1);
		PA_OutputText(1,2,10, "Alpha 2: %d   ", alpha2);

		PA_WaitForVBL();
      
      
      
	}
	
	return 0;
} // End of main()
