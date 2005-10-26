#ifndef _PA_GIF
#define _PA_GIF

#include <PA9.h>
#include "PA_Draw.h"


#include "gif/gif_lib.h"

extern GifFileType* gifinfo;
extern s16 gifwidth, gifheight; 
extern u8 PA_nBit[2]; // 8 or 16 bit Bg

void PA_Load16bitGif(bool screen, s16 x, s16 y, void *gif);
int DecodeGif(const u8 *userData, u8 ScreenBuff[192][256], u16* Palette, s16 *WidthP, s16 *HeightP);

extern inline void PA_LoadGif(bool screen, void *gif){
	if (PA_nBit[screen]) PA_Load16bitGif(screen, 0, 0, gif); // 16 bit...
	else DecodeGif(gif, (void*)PA_DrawBg[screen], (u16*)(0x05000000+(0x400*screen)), &gifwidth, &gifheight);
}
/*
extern inline u16 PA_GetGifWidth(void *gif){
gifinfo = (GifFileType*)gif;
	return gifinfo->Image.Width;
}
extern inline u16 PA_GetGifHeight(void *gif){
gifinfo = (GifFileType*)gif;
	return gifinfo->Image.Height;
}*/



/*
int readFunc(GifFileType* GifFile, GifByteType* buf, int count);
void CopyLine(void* dst, void* src, int count);
int DGifGetLineByte(GifFileType *GifFile, GifPixelType *Line, int LineLen);
*/
#endif

