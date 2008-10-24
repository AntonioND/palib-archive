
#ifdef __cplusplus
extern "C" {
#endif



#include <PA9.h>
#include "PA_TextBits.h"
#include "16c/all_gfx.h"
	u32 *c16_tiles[10] = {(u32*)(void*)(c16_text0_Tiles), (u32*)(void*)c16_text1_Tiles, (u32*)(void*)c16_text2_Tiles, (u32*)(void*)c16_text3_Tiles, (u32*)(void*)c16_text4_Tiles};	
	


// Pointers...
u16 *PA_Draw16[2];
u32 *PA_Draw1632[2];

void PA_16c8X8Color(u8 screen, s16 x, s16 y, u32 *image, u8 color){  
   x += 8; y += 8;
   u16 temp = (x&7)<<2;
   u16 pos = PA_16cPos(x, y);
   PA_Draw1632[screen][pos] |= (image[0]<<temp)*color;
   PA_Draw1632[screen][pos+1] |= (image[1]<<temp)*color;
   PA_Draw1632[screen][pos+2] |= (image[2]<<temp)*color;
   PA_Draw1632[screen][pos+3] |= (image[3]<<temp)*color;
   PA_Draw1632[screen][pos+4] |= (image[4]<<temp)*color;   
   PA_Draw1632[screen][pos+5] |= (image[5]<<temp)*color;
   PA_Draw1632[screen][pos+6] |= (image[6]<<temp)*color;
   PA_Draw1632[screen][pos+7] |= (image[7]<<temp)*color;  
   pos += (26*8); // Next columns
   temp = 32-temp;
   PA_Draw1632[screen][pos] |= (image[0]>>temp)*color;
   PA_Draw1632[screen][pos+1] |= (image[1]>>temp)*color;
   PA_Draw1632[screen][pos+2] |= (image[2]>>temp)*color;
   PA_Draw1632[screen][pos+3] |= (image[3]>>temp)*color;
   PA_Draw1632[screen][pos+4] |= (image[4]>>temp)*color;   
   PA_Draw1632[screen][pos+5] |= (image[5]>>temp)*color;
   PA_Draw1632[screen][pos+6] |= (image[6]>>temp)*color;
   PA_Draw1632[screen][pos+7] |= (image[7]>>temp)*color;      
} 


void PA_Init16cBgEx(u8 screen, u8 bg, u8 npalette){
	PA_LoadBg(screen, bg, Blank, ((8*4*34*26)+8)>>1, Blank, BG_512X256, 0, 0);
	PA_BGScrollXY(screen, bg, 8, 8);

	s32 i, j;
	s32 k = 1;
	for (i = 0; i < 34; i++) for (j = 0; j < 26; j++){
	   PA_SetLargeMapTile(screen, bg, i, j, k + ((npalette) << 12));
	   k++;
	}
	
	PA_Draw16[screen] = (u16*)CharBaseBlock(screen, PA_BgInfo[screen][bg].TileSetChar);   
	PA_Draw1632[screen] = (u32*)CharBaseBlock(screen, PA_BgInfo[screen][bg].TileSetChar); 

//#ifdef USE_16cTEXT 
	PA_SetBgPalCol(screen, (npalette<<4)+1, PA_RGB(31, 31, 31));
	PA_SetBgPalCol(screen, (npalette<<4)+2, PA_RGB(31, 0, 0));
	PA_SetBgPalCol(screen, (npalette<<4)+3, PA_RGB(0, 31, 0));
	PA_SetBgPalCol(screen, (npalette<<4)+4, PA_RGB(0, 0, 31));
	PA_SetBgPalCol(screen, (npalette<<4)+5, PA_RGB(31, 0, 31));
	PA_SetBgPalCol(screen, (npalette<<4)+6, PA_RGB(0, 31, 31));
	PA_SetBgPalCol(screen, (npalette<<4)+7, PA_RGB(31, 31, 0));
	PA_SetBgPalCol(screen, (npalette<<4)+8, PA_RGB(25, 25, 25));
	PA_SetBgPalCol(screen, (npalette<<4)+9, PA_RGB(20, 20, 20));
	PA_SetBgPalCol(screen, (npalette<<4)+10, PA_RGB(0, 0, 0));
//#endif
}   




void pa_16cTextDecompress(u8 size){
	s16 i;
	
	if((size < 5)&&(((void*)bittext_tiles_blank[size]) == ((void*)c16_tiles[size]))){ // Using default font and still compressed
		u32 tilesize = pa_bittextsizes[size];
		c16_tiles[size] = (u32*)malloc(tilesize<<2);
		for(i = 0; i < tilesize; i++){
		   c16_tiles[size][i] = (bittext_tiles_blank[size][i]&1) | (((bittext_tiles_blank[size][i]>>1)&1)<<4)   | (((bittext_tiles_blank[size][i]>>2)&1)<<8) | (((bittext_tiles_blank[size][i]>>3)&1)<<12) |
								(((bittext_tiles_blank[size][i]>>4)&1)<<16)   | (((bittext_tiles_blank[size][i]>>5)&1)<<20) | (((bittext_tiles_blank[size][i]>>6)&1)<<24)  | (((bittext_tiles_blank[size][i]>>7)&1)<<28);
		}   				
	}
}




s16 PA_16cText(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u8 color, u8 size, s32 limit){
	s32 i;
	
	pa_16cTextDecompress(size);
		
	PA_DoLetterPos(basex, basey, maxx, maxy, text, color, size, limit);
	
	if(limit > PA_LetterPos.NLetters) limit = PA_LetterPos.NLetters; // Plus short que prévu
	
	for(i = 0; i < limit; i++) PA_16cLetter(screen, PA_LetterPos.Letter[i].X, PA_LetterPos.Letter[i].Y, 
										PA_LetterPos.Letter[i].Letter, PA_LetterPos.Letter[i].Size, PA_LetterPos.Letter[i].Color);

//	PA_WaitFor(Stylus.Newpress);
	return limit;
}





void PA_16cClearZone(u8 screen, s16 x1, s16 y1, s16 x2, s16 y2){  
	x1 += 8; y1 += 8; y2 += 8;
	u16 temp, pos, temp2, pos2;
	s16 i, j;
	s16 minx = x1&(0xffffffff-7);
	if (minx != x1) minx += 8;
	s16 maxx = x2&(0xffffffff-7);

	pos = PA_16cPos(x1, y1);		temp = (x1&7)<<2;
	pos2 = PA_16cPos(x2, y1);		temp2 = (x2&7)<<2;	
	for (j = 0; j <= y2-y1; j++){
		PA_Draw1632[screen][pos] &= (0xffffffff>>(32-temp)); 
		PA_Draw1632[screen][pos+(26*8)] &= (0xffffffff<<temp);	
		pos++;
		PA_Draw1632[screen][pos2] &= (0xffffffff>>(32-temp2)); 
		PA_Draw1632[screen][pos2+(26*8)] &= (0xffffffff<<temp2);
		pos2++;
	}
   
	for (i = minx; i <= maxx; i+=8){ // 8 by 8...
		pos = PA_16cPos(i, y1);		
		for (j = 0; j <= y2-y1; j++){
			PA_Draw1632[screen][pos] = 0;
			pos++;
		}
	}
	
}  





#ifdef __cplusplus
}
#endif



