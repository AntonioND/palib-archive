// Includes
#include <PA9.h>       // Include for PA_Lib
#include "gfx/all_gfx.h"
#include "gfx/all_gfx.c"


// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL

	PA_Init16cBg(0, 2);  //Initializes a 16 color bg
	PA_LoadPal16c(PAL_BG0, ropes_Pal); //And loads its palette
	PA_16cErase(0);	//Clear bg before start drawing

	u8 i=0;	// position of our rope, from bottom
	u8 j=0;  // step of our animation
	u8 frames=0; //frames rendered, usefull to do animation timing

	// Infinite loop to keep the program running
	while (1)
	{	
		i+=1; //Move the rope up 1 pixels
		if (i>192) i=0; //If the rope has hit the top of the screen, start again from bottom

		if (++frames==20) //If we have render 20 consecutive frames
		{
			j=j+64;			//Change animation to next frame
			// ropes_Sprite has three 8x16 pixels frames, so each frame is (8x16)/2 = 64 bytes (since each pixels is 4bits)
			// so our stride on the animation image is 64.
			if (j>128) j=0;//Check frame limit (only 3 frames on this animation)
			frames=0;		//Reset frame counter
		}

		PA_WaitForVBL(); //Typical wait for vertical blank...
		PA_16cErase(0);  //Erase background (if there is only one row like in this example, you should erase only the space ocupped by the row).
		PA_16c8Xi(0,125,192-i,(u32*)(ropes_Sprite+j),i); //Draw the row.
	}

	return 0;
} // End of main()
