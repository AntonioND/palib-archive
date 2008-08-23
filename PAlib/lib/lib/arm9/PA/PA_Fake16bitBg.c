// Fake 16 bit Background mode

#include "PA_Fake16bit.h"

u16* PA_DrawFake16[2];
bool PA_HasFake16bitBg[2];

// Function to get the current scanline
inline int PA_GetScanline(){
	int __temp = PA_GetVcount()+1;
	if(__temp == 192) __temp = 0; // Adjust value: if we got 192, it's 0 really
	return __temp;
}

void fake16bit_hbl(){
	// Fake a 16 bit background

	// Get the scanline the hardware is about to draw
	int vc = PA_GetScanline();

	if(vc < 192){ // If scanline is a valid scanline
		// Load pallette based on the line the hardware is about to draw
		// for screen 0
		if(PA_HasFake16bitBg[0])
			PA_Load8bitBgPal(0, (void*) &PA_DrawFake16[0][256 * vc]);
		// for screen 1
		if(PA_HasFake16bitBg[1])
			PA_Load8bitBgPal(1, (void*) &PA_DrawFake16[1][256 * vc]);
	}
}

void PA_InitFake16bitBg(u8 screen, u8 prio){
	// Function to make a fake 16 bit background!
	// Caution: fake 16 bit bgs and mode 7 DON'T MIX
	bool hadbg = false;

	// Disable fake 16 bit backgrounds
	if(PA_HasFake16bitBg[screen]){
		PA_HasFake16bitBg[screen] = false;
		free(PA_DrawFake16[screen]); // Free the memory
		hadbg = true;
	}
	// Allocate memory for a 16 bit background
	PA_DrawFake16[screen] = calloc(256*192, sizeof(u16));
	if(!PA_DrawFake16[screen]){
		// ERROR!!
		// Just hang up
		while(true)
			PA_WaitForVBL();
	}
	// Initialize an 8 bit background on bg 3 at the specified screen
	if(!hadbg) // Only initialize if that screen never had a fake 16 bit background before
		PA_Init8bitBg(screen, prio);
	// Make each scanline display the entire pallette
	int x, y;
	for(y = 0; y < 192; y ++)
		for(x = 0; x < 256; x ++)
			PA_Put8bitPixel(screen, x, y, x);
	// Set the horizontal blank handler
	irqSet(IRQ_HBLANK, fake16bit_hbl);
	irqEnable(IRQ_HBLANK);
	// Confirm that we have a fake 16 bit background
	PA_ClearFake16bitBg(screen); // Make sure the background is empty
	PA_HasFake16bitBg[screen] = true;
}

void PA_DrawFake16bitLine(u8 screen, u16 x1, u16 y1, u16 x2, u16 y2, u16 color){
	// Draws a line on a fake 16 bit background

	// Copypasted (and shorted) from the real 16 bit background function
	int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;

	dx=x2-x1, dy=y2-y1, dxabs = dx, sdx = 1;
	if(dx < 0) dxabs = -dx, sdx = -1;
	dyabs = dy, sdy = 1;
	if (dy < 0) dyabs = -dy, sdy = -1;
	x=dyabs>>1, y=dxabs>>1, px=x1, py=y1;
	PA_DrawFake16[screen][px + 256 * py] = color;
	if (dxabs>=dyabs) {
		for(i=0;i<dxabs;i++)  {
			y+=dyabs;
			if (y>=dxabs) y-=dxabs, py+=sdy;
			px+=sdx;
			PA_DrawFake16[screen][px + 256 * py] = color;
		}
	}else{
		for(i=0;i<dyabs;i++) {
			x+=dxabs;
			if (x>=dyabs) x-=dyabs, px+=sdx;
			py+=sdy;
			PA_DrawFake16[screen][px + 256 * py] = color;
		}
	}
}
