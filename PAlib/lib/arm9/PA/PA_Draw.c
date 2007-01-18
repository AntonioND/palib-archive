#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"


//////////////////////////////////////////////////////////////////////
// Buffer mode
//////////////////////////////////////////////////////////////////////


u16 PA_oldx[2], PA_oldy[2];
u8 PA_drawsize[2];
u16 *PA_DrawBg[2];
u32 *PA_DrawBg32[2];
u8 PA_nBit[2]; // 8 or 16 on each screen...

u16 PA_temppal[256];


u16 tempvar;



void PA_LoadBmpToBuffer(u16 *Buffer, s16 x, s16 y, void *bmp, s16 SWidth){

	u8 *temp = (u8*)bmp;
	BMP_Headers *Bmpinfo = (BMP_Headers*)(temp+14);
	
	u8 *gfx = temp+54;
	u16 *gfx2 = (u16*)(temp+54); // Pour le mode 16 bit...
	s32 r, g, b;  s16 tempx, tempy;
	s16 lx = Bmpinfo->Width;   s16 ly = Bmpinfo->Height;
	u16 Bits = Bmpinfo->BitsperPixel;
	//Buffer = (u16*)(Buffer + ((x + (y*SWidth)) << 1)); // Position de départ
	
	s32 i = 0;
	if (Bits > 16){ // Pour 24 et 32 bits
		for (tempy = ly-1; tempy > -1; tempy--){
			for (tempx = 0; tempx < lx; tempx++){
				b = (gfx[i] >> 3)&31;	i++;
				g = (gfx[i] >> 3)&31;	i++;
				r = (gfx[i] >> 3)&31;	i++;	
				if (Bits == 32) i++; // On passe le bit alpha
				//PA_Put16bitPixel(screen, x + tempx, y + tempy, PA_RGB(r, g, b));
				Buffer[x + tempx + ((y + tempy) * SWidth)] = PA_RGB(r, g, b);
			}
			while(i&3) i++; // Padding....
		}
	}
	if (Bits == 16){
		for (tempy = ly-1; tempy > -1; tempy--){
			for (tempx = 0; tempx < lx; tempx++){
				b = *gfx2&31;
				g = (*gfx2>>5)&31;
				r = (*gfx2>>10)&31;		
				//PA_Put16bitPixel(screen, x + tempx, y + tempy, PA_RGB(r, g, b));
				Buffer[x + tempx + ((y + tempy) * SWidth)] = PA_RGB(r, g, b);
				gfx2++; // On passe au pixel suivant
			}
			s32 temp = (s32)gfx2;
			while(temp&3) temp++; // Padding....
			gfx2 = (u16*)temp;
		}	
	}
}




#ifdef __cplusplus
}
#endif
