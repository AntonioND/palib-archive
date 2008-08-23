// Includes
#include <PA9.h>       // Include for PA_Lib

// PAGfx include
#include "gfx\all_gfx.h"
#include "gfx\all_gfx.c"

// Image includes
#include "photo.h"
#include "bmpimg.h"
#include "gifimg.h"
#include "gifanim.h"

#define PA_RandColor() (PA_RGB(PA_RandMinMax(0, 31), PA_RandMinMax(0, 31), PA_RandMinMax(0, 31)))
#define PA_HangUp() {while(true)PA_WaitForVBL();}

void DrawTest(u8 s)
{
	PA_LoadFake16bitBitmap(s, (void*)test_Bitmap);
	PA_PutFake16bitPixel(s, 0, 0, PA_RGB(31, 31, 31));
	PA_PutFake16bitPixel(s, 20, 40, PA_RGB(31, 0, 31));
	PA_DrawFake16bitLine(s, 1, 1, 20, 20, PA_RGB(0, 0, 0));
	PA_DrawFake16bitRect(s, 50, 50, 200, 191, PA_RGB(0, 15, 15));
}

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();
	PA_InitVBL();
	PA_InitFake16bitBg(0, 3);
	PA_InitFake16bitBg(1, 3);
	int oldx = 0, oldy = 0;
	while(true)
	{
		DrawTest(0);
		DrawTest(1);
		while(true)
		{
			if(Pad.Newpress.A)
				break;
			if(Stylus.Newpress)
			{
				// Draw!
				u16 c = PA_RandColor();
				PA_PutFake16bitPixel(0, Stylus.X, Stylus.Y, c);
				PA_PutFake16bitPixel(1, Stylus.X, Stylus.Y, c);
				oldx = Stylus.X;
				oldy = Stylus.Y;
			}else if(Stylus.Held)
			{
				u16 c = PA_RandColor();
				PA_DrawFake16bitLine(0, oldx, oldy, Stylus.X, Stylus.Y, c);
				PA_DrawFake16bitLine(1, oldx, oldy, Stylus.X, Stylus.Y, c);
				oldx = Stylus.X;
				oldy = Stylus.Y;
			}
			PA_WaitForVBL();
		}
		PA_ClearFake16bitBg(1);
		PA_Fake16bitLoadJpeg(1,(void*)photo);
		PA_WaitFor(Pad.Newpress.A);
		PA_ClearFake16bitBg(1);
		PA_Fake16bitLoadBmpEx(1,(void*)bmpimg, 40, 20);
		PA_Fake16bitLoadBmpEx(1,(void*)bmpimg, 80, 40);
		PA_WaitFor(Pad.Newpress.A);
		PA_ClearFake16bitBg(1);
		PA_Fake16bitLoadGifXY(1,gifimg, 50, 10);
		PA_Fake16bitLoadGifXY(1,gifimg, 70, 30);
		PA_WaitFor(Pad.Newpress.A);
		PA_ClearFake16bitBg(1);
		PA_Fake16bitLoadGifXY(1,gifanim, 10, 10);
		PA_WaitFor(Pad.Newpress.A);
		PA_ClearFake16bitBg(1);
		PA_WaitForVBL();
	}
} // End of main()
