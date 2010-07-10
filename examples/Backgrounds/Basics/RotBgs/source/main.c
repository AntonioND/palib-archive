// Rotational backgrounds example

// Includes
#include <PA9.h>

#include "all_gfx.h"

int main(){
	PA_Init();

	PA_LoadDefaultText(1, 0);
	
	PA_SetVideoMode(0, 2);  //screen, mode

	PA_LoadBackground(0, //screen
					  3, // background number
					  &Rot); // background name in PAGfx

	// Make the background wraparound
	PA_SetBgWrap(0, 3, 1);

	s32 scrollx = 0;
	s32 scrolly = 0;
	s32 rotcenterx = 0;
	s32 rotcentery = 0;
	s16 angle = 0;
	s32 zoom = 256;
	
	PA_OutputSimpleText(1, 2, 2, "Zoom       : Start/Select");
	PA_OutputSimpleText(1, 2, 3, "ScrollX    : Left/Right");
	PA_OutputSimpleText(1, 2, 4, "Scrolly    : Up/Down");
	PA_OutputSimpleText(1, 2, 5, "RotCenterX : A/Y");
	PA_OutputSimpleText(1, 2, 6, "RotCenterY : B/X");
	PA_OutputSimpleText(1, 2, 7, "Angle      : R/L");

	while(true){
		zoom += Pad.Held.Start - Pad.Held.Select;
		scrollx += Pad.Held.Right - Pad.Held.Left;
		scrolly += Pad.Held.Down - Pad.Held.Up;
		rotcenterx += Pad.Held.A - Pad.Held.Y;
		rotcentery += Pad.Held.B - Pad.Held.X;	
		angle += Pad.Held.R - Pad.Held.L;
		
		PA_SetBgRot(0, 3, scrollx, scrolly, rotcenterx, rotcentery, angle, zoom);

		PA_WaitForVBL();
	}

}