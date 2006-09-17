
#ifdef __cplusplus
extern "C" {
#endif



#include <PA9.h>

	#include "16c/c16_text0.c"
	#include "16c/c16_text1.c"
	#include "16c/c16_text2.c"
	#include "16c/c16_text3.c"
	#include "16c/c16_text4.c"
	
	const u8 c16_policeheight[5] = {6, 8, 9, 11, 14};
	extern const u8 c16_policesize[5][96];
	u32 *c16_font[5] = {(u32*)(void*)(c16_text0Tiles), (u32*)(void*)c16_text1Tiles, (u32*)(void*)c16_text2Tiles, (u32*)(void*)c16_text3Tiles, (u32*)(void*)c16_text4Tiles};	


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
}   





s16 PA_16cText(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u8 color, u8 size, s32 limit){
s16 i, j;
s16 x, y;
s16 lx, ly;
s16 letter; 

ly = c16_policeheight[size];

x = basex;
y = basey;

s16 length = 0;
s16 ylimiy = maxy - ly;
s16 wordx = 0; // Longueur de mot
s16 wordletter = 0;

for (i = 0; (text[i] && y <= ylimiy && i < limit); i++) {
	if (text[i] == '\n'){
		/*if (transp == 0){
			while(x < maxx) { // On efface tout ce qui suit
				//for (j = 0; j < ly; j++) PA_Put8bitPixel(screen, x, y + j, 0);
				x++;
			}
		}*/
		if (text[i+1] == ' ') i++; // On vire s'il y a un espace	
		x = basex;
		y += ly;	
	}
	else{
		wordletter = 1;
		wordx = 0;
		
		while(!((text[i+wordletter] <= 32) || (i + wordletter >= limit))) { // >= 32, donc si 0, '\n', on ' ' :)
			letter = text[i+wordletter] - 32;
			lx = c16_policesize[size][letter];
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
						letter = text[j] - 32;
						if (letter > 128) letter -= 96; // pour les accents...
						lx = c16_policesize[size][letter];
						PA_16cLetter(screen, x, y, letter, size, color);
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
				letter = text[j] - 32;
				lx = c16_policesize[size][letter];
//				if (letter >= 0) {
					PA_16cLetter(screen, x, y, letter, size, color);
					//letters[transp](size, screen, x, y, letter, color);				
					x += lx;
//				}
			}
			i+=wordletter-1;
		}

	}
}
PA_16cLetterPos.Letter[PA_16cLetterPos.NLetters].X = x;
PA_16cLetterPos.Letter[PA_16cLetterPos.NLetters].Y = y;
length = i;



return length;

}








	const u8 c16_policesize[5][96] = {
					  {2, 2, 4, 6, 6, 6, 0, 2, 3, 3, 2, 4, 3, 4, 2, 6, 5, 3, 5, 5, 5, 5, 5, 5, 5, 5, 2, 2, 4, 4, 4, 5,
					   6, 5, 5, 4, 5, 4, 4, 5, 5, 4, 5, 5, 4, 6, 5, 5, 5, 5, 5, 5, 4, 5, 5, 6, 5, 5, 4, 3, 6, 3, 4, 5,
				       2, 5, 5, 4, 5, 5, 4, 4, 5, 2, 3, 5, 2, 6, 5, 5, 4, 4, 4, 5, 4, 5, 5, 6, 4, 4, 5, 0, 0, 0, 0, 0},		
					  {2, 2, 4, 7, 6, 9, 0, 2, 4, 4, 2, 6, 3, 4, 3, 4, 6, 4, 6, 6, 6, 6, 6, 6, 6, 6, 3, 3, 4, 6, 4, 5,
					   8, 6, 6, 6, 5, 5, 5, 6, 6, 4, 4, 6, 5, 6, 7, 7, 6, 7, 6, 6, 6, 6, 6, 8, 6, 6, 6, 4, 4, 4, 6, 6,
				       2, 5, 5, 5, 5, 5, 5, 6, 5, 2, 4, 5, 2, 8, 5, 5, 5, 5, 5, 5, 4, 5, 4, 6, 6, 4, 5, 0, 0, 0, 0, 0},						   
					  {2, 2, 4, 8, 6, 9, 0, 2, 4, 4, 2, 6, 3, 4, 3, 4, 6, 4, 7, 7, 7, 7, 7, 7, 7, 7, 3, 3, 6, 6, 6, 5,
					   9, 9, 7, 8, 8, 7, 7, 9, 10, 4, 6, 8, 7, 10, 9, 9, 7, 9, 8, 7, 8, 9, 9, 12, 9, 8, 7, 4, 4, 4, 8, 6,
				       2, 7, 7, 6, 7, 6, 6, 7, 8, 4, 4, 7, 4, 12, 8, 6, 7, 7, 6, 5, 5, 8, 7, 10, 7, 8, 5, 0, 0, 0, 0, 0},		
					  {2, 3, 6, 9, 8, 11, 0, 3, 6, 6, 3, 8, 3, 6, 3, 5, 9, 5, 8, 8, 8, 8, 8, 8, 8, 8, 3, 3, 7, 8, 7, 7,
					   13, 11, 10, 10, 11, 10, 9, 11, 12, 7, 8, 10, 11, 14, 11, 11, 9, 11, 10, 8, 11, 10, 11, 14, 10, 11, 9, 5, 7, 5, 8, 9,
				       3, 8, 9, 8, 9, 8, 6, 9, 10, 5, 5, 9, 5, 15, 10, 9, 9, 9, 7, 7, 6, 10, 9, 12, 9, 9, 7, 0, 0, 0, 0, 0},		
					  {2, 4, 8, 12, 11, 15, 0, 4, 6, 6, 4, 10, 5, 6, 4, 6, 12, 8, 10, 10, 12, 10, 11, 10, 11, 11, 4, 5, 10, 10, 10, 9,
					   16, 15, 14, 13, 15, 13, 13, 16, 16, 8, 11, 15, 14, 16, 15, 14, 13, 14, 16, 11, 12, 15, 14, 16, 15, 16, 14, 5, 5, 5, 10, 9,
				       4, 10, 11, 10, 11, 9, 8, 10, 12, 6, 7, 11, 6, 16, 12, 10, 11, 11, 9, 8, 8, 12, 11, 16, 12, 11, 10, 0, 0, 0, 0, 0},		
					   };



#ifdef __cplusplus
}
#endif



