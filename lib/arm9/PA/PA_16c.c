
#ifdef __cplusplus
extern "C" {
#endif



#include <PA9.h>

#include "16c/all_gfx.c"
/*
	#include "16c/c16_text0.c"
	#include "16c/c16_text1.c"
	#include "16c/c16_text2.c"
	#include "16c/c16_text3.c"
	#include "16c/c16_text4.c"*/
	
	u16 *c16_maps[10] = {(u16*)(void*)(c16_text0_Map), (u16*)(void*)c16_text1_Map, (u16*)(void*)c16_text2_Map, (u16*)(void*)c16_text3_Map, (u16*)(void*)c16_text4_Map};		
	u32 *c16_tiles[10] = {(u32*)(void*)(c16_text0_Tiles), (u32*)(void*)c16_text1_Tiles, (u32*)(void*)c16_text2_Tiles, (u32*)(void*)c16_text3_Tiles, (u32*)(void*)c16_text4_Tiles};	
	u8 *pa16cdefaultsize[10] = {(u8*)c16_text0_Sizes, (u8*)c16_text1_Sizes, (u8*)c16_text2_Sizes, (u8*)c16_text3_Sizes, (u8*)c16_text4_Sizes};
	
	u8 c16policeheight[10];

	

// Pointers...
u16 *PA_Draw16[2];
u32 *PA_Draw1632[2];


LetterPos16c PA_16cLetterPos; // Letter positions






void PA_Init16cBgEx(u8 screen, u8 bg, u8 npalette){
	PA_LoadBg(screen, bg, Blank, (8*4*34*26)+4, Blank, BG_512X256, 0, 0);
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

	c16policeheight[0] = c16_text0_Height;
	c16policeheight[1] = c16_text1_Height;
	c16policeheight[2] = c16_text2_Height;
	c16policeheight[3] = c16_text3_Height;
	c16policeheight[4] = c16_text4_Height;	

}   


extern inline void PA_AddLetterPos(s16 Letter, s16 x, s16 y){
	PA_16cLetterPos.Letter[Letter].X = x;
	PA_16cLetterPos.Letter[Letter].Y = y;
}


s16 PA_16cText(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u8 color, u8 size, s32 limit){
s16 i, j;
s16 x, y;
s16 lx, ly;
s16 letter; 

ly = c16policeheight[size];

x = basex;
y = basey;

s16 length = 0;
s16 ylimiy = maxy - ly;
s16 wordx = 0; // Longueur de mot
s16 wordletter = 0;

PA_16cLetterPos.NLetters = 0; // 0 letters now...

for (i = 0; (text[i] && y <= ylimiy && i < limit); i++) {
	if (text[i] == '\n'){
		PA_AddLetterPos(i, x, y);
		/*if (transp == 0){
			while(x < maxx) { // On efface tout ce qui suit
				//for (j = 0; j < ly; j++) PA_Put8bitPixel(screen, x, y + j, 0);
				x++;
			}
		}*/
		x = basex;
		y += ly;	
		if (text[i+1] == ' ') {
			PA_AddLetterPos(i+1, x, y);
			i++; // On vire s'il y a un espace	
		}		
	}
	else{
		wordletter = 1;
		wordx = 0;
		
		while(!((text[i+wordletter] <= 32) || (i + wordletter >= limit))) { // >= 32, donc si 0, '\n', on ' ' :)
			letter = text[i+wordletter];
			lx = pa16cdefaultsize[size][letter];
			wordx += lx;
			wordletter++;
		}
		
		if (x + wordx >= maxx ) {  // On dépasse en X...
		/*	if (transp == 0){
				while(x < maxx) { // On efface tout ce qui suit
					//for (j = 0; j < ly; j++) PA_Put8bitPixel(screen, x, y + j, 0);
					x++;
				}
			}*/
		
			x = basex;
			y += ly;
		
			if(text[i] != ' ') { // On vire s'il y a un espace	
				if(y <= ylimiy) { // Si on n'a pas dépassé...
					for (j = i; j < (i + wordletter); j++) {
						letter = text[j];
						//if (letter > 128) letter -= 96; // pour les accents...
						lx = pa16cdefaultsize[size][letter];
						PA_16cLetter(screen, x, y, letter, size, color);
						PA_AddLetterPos(j, x, y);
						x += lx;
					}
					i+=wordletter-1;
				} // Sinon ca va s'arreter
				else i--; // On n'a pas affiché la derniere lettre...
			}
		}		
		else{ // Si pas de passage à la ligne auto
			s32 jmax = (i + wordletter);
			if (text[(i + wordletter-1)] < 32) jmax--; // On ne dessinera pas ce caractère
			
			for (j = i; j < jmax; j++) {
				letter = text[j];
				lx = pa16cdefaultsize[size][letter];
//				if (letter >= 0) {
					PA_16cLetter(screen, x, y, letter, size, color);
					PA_AddLetterPos(j, x, y);
					//letters[transp](size, screen, x, y, letter, color);				
					x += lx;
//				}
			}
			i+=wordletter-1;
		}

	}
}
PA_AddLetterPos(i, x, y);
length = i;
PA_16cLetterPos.NLetters = length;


return length;

}



void PA_16cClearZone(u8 screen, s16 x1, s16 y1, s16 x2, s16 y2){  
	x1 += 8; y1 += 8; y2 += 8;
	u16 temp, pos;
	s16 i, j;
	s16 minx = x1&(0xffffffff-7);
	if (minx != x1) minx += 8;
	s16 maxx = x2&(0xffffffff-7);

	pos = PA_16cPos(x1, y1);	
	temp = (x1&7)<<2;
	for (j = 0; j <= y2-y1; j++){
		PA_Draw1632[screen][pos] &= (0xffffffff>>(32-temp)); 
		PA_Draw1632[screen][pos+(26*8)] &= (0xffffffff<<temp);	
		pos++;
	}
   
	for (i = minx; i <= maxx; i+=8){ // 8 by 8...
		pos = PA_16cPos(i, y1);		
		for (j = 0; j <= y2-y1; j++){
			PA_Draw1632[screen][pos] = 0;
			pos++;
		}
	}
	pos = PA_16cPos(x2, y1);	
	temp = (x2&7)<<2;
	for (j = 0; j <= y2-y1; j++){
		PA_Draw1632[screen][pos] &= (0xffffffff>>(32-temp)); 
		PA_Draw1632[screen][pos+(26*8)] &= (0xffffffff<<temp);
		pos++;
	}	
}  








#ifdef __cplusplus
}
#endif



