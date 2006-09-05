

#ifdef __cplusplus
extern "C" {
#endif

#include "arm9/PA_Text.h"
#include "arm9/PA_Draw.h"



void Letter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color);
void TranspLetter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color);
void NoLetter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color);
void RotLetter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color);
void RotLetter2(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color);

letterfp letters[5] = {(letterfp)&Letter, (letterfp)&TranspLetter, (letterfp)&NoLetter, (letterfp)&RotLetter, (letterfp)&RotLetter2};
	

extern inline void normala(u8 screen, u16 i, s8 ly, s16 pos, u8 *data, u8 color){
u8 j;
s16 temp;

	for (j = 0; j < 8; j++){
		temp = (data[i + (j << 3)]) << 8;
			PA_DrawBg[screen][pos] &= 255;
		PA_DrawBg[screen][pos] |= temp*color;	
		pos+=128;		
	}
	ly -= 8;
	for (j = 0; j < ly; j++){
		temp = (data[128 + i + (j << 3)]) << 8;
			PA_DrawBg[screen][pos] &= 255;
		PA_DrawBg[screen][pos] |= temp*color;	
		pos+=128;		
	}			
}


extern inline void normalb(u8 screen, u16 i, s8 ly, s16 pos, u8 *data, u8 color){
u8 j;
s16 temp;

	for (j = 0; j < 8; j++){
		temp = data[i + (j << 3)];
		PA_DrawBg[screen][pos] &= ~255;
		PA_DrawBg[screen][pos] |= temp*color;
		pos+=128;	
	}
	ly -= 8;
	for (j = 0; j < ly; j++){
		temp = (data[128 + i + (j << 3)]);
		PA_DrawBg[screen][pos] &= ~255;
		PA_DrawBg[screen][pos] |= temp*color;
		pos+=128;		
	}	

}





void NoLetter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color) {

}




void RotLetter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color) {
u8 lx = policesize[size][(u8)letter];
u8 ly = policeheight[size];
u8 *data;
if (size > 1) data = (u8*)(textData[size] + (letter << 8)); // Grande police
else data = (u8*)(textData[size] + (letter << 6)); // Petite

s16 i, j;
//u16 pos;


s16 temp = x;
x = 255 - y;
y = temp; // On inverse X et Y...


if (lx > 8){
	for (i = 0; i < 8; i++){
		for (j = 0; j < 8; j++)
			PA_Put8bitPixel(screen, x - j, y + i, data[i + (j << 3)]*color);
		for (j = 0; j < ly-8; j++)
			PA_Put8bitPixel(screen, x - j - 8, y + i, data[128 + i + (j << 3)]*color);	
	}
	data = (u8*)(data+64);
	lx -= 8;
	y+= 8;
}

	for (i = 0; i < lx; i++){
		for (j = 0; j < 8; j++)
			PA_Put8bitPixel(screen, x - j, y + i, data[i + (j << 3)]*color);
		for (j = 0; j < ly-8; j++)
			PA_Put8bitPixel(screen, x - j - 8, y + i, data[128 + i + (j << 3)]*color);
	}


}


void RotLetter2(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color) {
u8 lx = policesize[size][(u8)letter];
u8 ly = policeheight[size];
u8 *data;
if (size > 1) data = (u8*)(textData[size] + (letter << 8)); // Grande police
else data = (u8*)(textData[size] + (letter << 6)); // Petite

s16 i, j;
//u16 pos;


s16 temp = x;
x = y;
y = 191 - temp; // On inverse X et Y...


if (lx > 8){
	for (i = 0; i < 8; i++){
		for (j = 0; j < 8; j++)
			PA_Put8bitPixel(screen, x + j, y - i, data[i + (j << 3)]*color);
		for (j = 0; j < ly-8; j++)
			PA_Put8bitPixel(screen, x + j + 8, y - i, data[128 + i + (j << 3)]*color);	
	}
	data = (u8*)(data+64);
	lx -= 8;
	y-= 8;
}

	for (i = 0; i < lx; i++){
		for (j = 0; j < 8; j++)
			PA_Put8bitPixel(screen, x + j, y - i, data[i + (j << 3)]*color);
		for (j = 0; j < ly-8; j++)
			PA_Put8bitPixel(screen, x + j + 8, y - i, data[128 + i + (j << 3)]*color);
	}


}





void Letter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color) {
u8 lx = policesize[size][(u8)letter];
u8 ly = policeheight[size];
u8 *data;
if (size > 1) data = (u8*)(textData[size] + (letter << 8)); // Grande police
else data = (u8*)(textData[size] + (letter << 6)); // Petite

u16 i;
u16 pos;


if (lx > 8) {
	for (i = 0; i < 8; i++){
		pos = ((i + x) >> 1) + (y << 7);
		if ((i + x)&1){
			normala(screen, i, ly, pos, data, color);	
		}
		else{
			normalb(screen, i, ly, pos, data, color);
		}
	}
	x += 8;
	lx -= 8;
	data = (u8*)(data+64);
}

	for (i = 0; i < lx; i++){
		pos = ((i + x) >> 1) + (y << 7);
		if ((i + x)&1){
			normala(screen, i, ly, pos, data, color);
		}
		else{
			normalb(screen, i, ly, pos, data, color);
		}
	}



}







extern inline void transpa(u8 screen, u16 i, s8 ly, s16 pos, u8 *data, u8 color){
u8 j;
s16 temp;

	for (j = 0; j < 8; j++){
		temp = (data[i + (j << 3)]) << 8;
		PA_DrawBg[screen][pos] &= ~(temp*255);
		PA_DrawBg[screen][pos] |= temp*color;	
		pos+=128;		
	}
	ly -= 8;
	for (j = 0; j < ly; j++){
		temp = (data[128 + i + (j << 3)]) << 8;
		PA_DrawBg[screen][pos] &= ~(temp*255);
		PA_DrawBg[screen][pos] |= temp*color;	
		pos+=128;		
	}			
}


extern inline void transpb(u8 screen, u16 i, s8 ly, s16 pos, u8 *data, u8 color){
u8 j;
s16 temp;

	for (j = 0; j < 8; j++){
		temp = data[i + (j << 3)];
		PA_DrawBg[screen][pos] &= ~(temp*255);
		PA_DrawBg[screen][pos] |= temp*color;
		pos+=128;	
	}
	ly -= 8;
	for (j = 0; j < ly; j++){
		temp = (data[128 + i + (j << 3)]);
		PA_DrawBg[screen][pos] &= ~(temp*255);
		PA_DrawBg[screen][pos] |= temp*color;
		pos+=128;		
	}	

}




void TranspLetter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color) {
u8 lx = policesize[size][(u8)letter];
u8 ly = policeheight[size];
u8 *data;
if (size > 1) data = (u8*)(textData[size] + (letter << 8)); // Grande police
else data = (u8*)(textData[size] + (letter << 6)); // Petite

u16 i;
u16 pos;


if (lx > 8) {
	for (i = 0; i < 8; i++){
		pos = ((i + x) >> 1) + (y << 7);
		if ((i + x)&1){
			transpa(screen, i, ly, pos, data, color);	
		}
		else{
			transpb(screen, i, ly, pos, data, color);
		}
	}
	x += 8;
	lx -= 8;
	data = (u8*)(data+64);
}

	for (i = 0; i < lx; i++){
		pos = ((i + x) >> 1) + (y << 7);
		if ((i + x)&1){
			transpa(screen, i, ly, pos, data, color);
		}
		else{
			transpb(screen, i, ly, pos, data, color);
		}
	}


}




s16 PA_SmartText(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u8 color, u8 size, u8 transp, s32 limit){
s16 i, j;
s16 x, y;
s16 lx, ly;
s16 letter; 

ly = policeheight[size];

x = basex;
y = basey;

s16 length = 0;
s16 ylimiy = maxy - ly;
s16 wordx = 0; // Longueur de mot
s16 wordletter = 0;

for (i = 0; (text[i] && y <= ylimiy && i < limit); i++) {
	if (text[i] == '\n'){
		if (transp == 0){
			while(x < maxx) { // On efface tout ce qui suit
				for (j = 0; j < ly; j++) PA_Put8bitPixel(screen, x, y + j, 0);
				x++;
			}
		}
		if (text[i+1] == ' ') i++; // On vire s'il y a un espace	
		x = basex;
		y += ly;	
	}
	else{
		wordletter = 1;
		wordx = 0;
		
		while(!((text[i+wordletter] <= 32) || (i + wordletter >= limit))) { // >= 32, donc si 0, '\n', on ' ' :)
			letter = text[i+wordletter] - 32;
			lx = policesize[size][letter];
			wordx += lx;
			wordletter++;
		}
		//wordletter = 4;
		//PA_OutputText(0, 0, 0, "%d, %d, %d", i, wordletter, wordx);
		
		if (x + wordx >= maxx ) {  // On dépasse en X...
//		if (x + lx >= maxx ) {  // On dépasse en X...
			if (transp == 0){
				while(x < maxx) { // On efface tout ce qui suit
					for (j = 0; j < ly; j++) PA_Put8bitPixel(screen, x, y + j, 0);
					x++;
				}
			}
		
			x = basex;
			y += ly;
		
			if(text[i] != ' ') { // On vire s'il y a un espace	
				if(y <= ylimiy) { // Si on n'a pas dépassé...
					for (j = i; j < (i + wordletter); j++) {
						letter = text[j] - 32;
						if (letter > 128) letter -= 96; // pour les accents...
						lx = policesize[size][letter];
						letters[transp](size, screen, x, y, letter, color);				
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
				lx = policesize[size][letter];
//				if (letter >= 0) {
					letters[transp](size, screen, x, y, letter, color);				
					x += lx;
//				}
			}
			i+=wordletter-1;
		}

	}
}

length = i;
if (transp == 0){ // Si pas de transparence, on efface tout le reste
	while(x < maxx) { // On efface tout ce qui suit
		for (i = 0; i < ly+5; i++) PA_Put8bitPixel(screen, x, y + i, 0);
		x++;
	}
	
	y += ly;
	basey = y;
	
	if (basex&1) {
		while(y < maxy) {
			PA_Put8bitPixel(screen, basex, y, 0);
			y++;
		}
		++basex;
	}
	
	for (x = basex; x < maxx; x++)
		for (y = basey; y < maxy; y++) PA_DrawBg[screen][(x >> 1) + (y << 7)] = 0;
}


return length;

}




// Use all sizes...
//#ifdef TEXT_ALLSIZES
	// Et pour le texte à taille variable...
	#include "8bitText/text0.c" // La police à taille variable...
	#include "8bitText/text1.c" // La police à taille variable...
	#include "8bitText/text2.c" // La police à taille variable...
	#include "8bitText/text3.c" // La police à taille variable...
	#include "8bitText/text4.c" // La police à taille variable...

	const u8 *textData[5] = {(u8*)(text0Data), (u8*)(text1Data), (u8*)(text2Data), (u8*)(text3Data), (u8*)(text4Data)};
	const u8 policeheight[5] = {6, 8, 9, 11, 14};
	const u8 policesize[5][256] = {
					  {2, 2, 4, 6, 6, 6, 0, 2, 3, 3, 2, 4, 3, 4, 2, 6, 5, 3, 5, 5, 5, 5, 5, 5, 5, 5, 2, 2, 4, 4, 4, 5,
					   6, 5, 5, 4, 5, 4, 4, 5, 5, 4, 5, 5, 4, 6, 5, 5, 5, 5, 5, 5, 4, 5, 5, 6, 5, 5, 4, 3, 6, 3, 4, 5,
				       2, 5, 5, 4, 5, 5, 4, 4, 5, 2, 3, 5, 2, 6, 5, 5, 4, 4, 4, 5, 4, 5, 5, 6, 4, 4, 5, 0, 0, 0, 0, 0,
				       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		
				       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		
				       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		
				       5, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0},		
					  {2, 2, 4, 7, 6, 9, 0, 2, 4, 4, 2, 6, 3, 4, 3, 4, 6, 4, 6, 6, 6, 6, 6, 6, 6, 6, 3, 3, 4, 6, 4, 5,
					   8, 6, 6, 6, 5, 5, 5, 6, 6, 4, 4, 6, 5, 6, 7, 7, 6, 7, 6, 6, 6, 6, 6, 8, 6, 6, 6, 4, 4, 4, 6, 6,
				       2, 5, 5, 5, 5, 5, 5, 6, 5, 2, 4, 5, 2, 8, 5, 5, 5, 5, 5, 5, 4, 5, 4, 6, 6, 4, 5, 0, 0, 0, 0, 0,	
				       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		
				       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		
				       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		
				       5, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0},						   
					  {2, 2, 4, 8, 6, 9, 0, 2, 4, 4, 2, 6, 3, 4, 3, 4, 6, 4, 7, 7, 7, 7, 7, 7, 7, 7, 3, 3, 6, 6, 6, 5,
					   9, 9, 7, 8, 8, 7, 7, 9, 10, 4, 6, 8, 7, 10, 9, 9, 7, 9, 8, 7, 8, 9, 9, 12, 9, 8, 7, 4, 4, 4, 8, 6,
				       2, 7, 7, 6, 7, 6, 6, 7, 8, 4, 4, 7, 4, 12, 8, 6, 7, 7, 6, 5, 5, 8, 7, 10, 7, 8, 5, 0, 0, 0, 0, 0,			
				       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		
				       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		
				       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		
				       7, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0},		
					  {2, 3, 6, 9, 8, 11, 0, 3, 6, 6, 3, 8, 3, 6, 3, 5, 9, 5, 8, 8, 8, 8, 8, 8, 8, 8, 3, 3, 7, 8, 7, 7,
					   13, 11, 10, 10, 11, 10, 9, 11, 12, 7, 8, 10, 11, 14, 11, 11, 9, 11, 10, 8, 11, 10, 11, 14, 10, 11, 9, 5, 7, 5, 8, 9,
				       3, 8, 9, 8, 9, 8, 6, 9, 10, 5, 5, 9, 5, 15, 10, 9, 9, 9, 7, 7, 6, 10, 9, 12, 9, 9, 7, 0, 0, 0, 0, 0,					   
				       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		
				       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		
				       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		
				       8, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0},		
					  {2, 4, 8, 12, 11, 15, 0, 4, 6, 6, 4, 10, 5, 6, 4, 6, 12, 8, 10, 10, 12, 10, 11, 10, 11, 11, 4, 5, 10, 10, 10, 9,
					   16, 15, 14, 13, 15, 13, 13, 16, 16, 8, 11, 15, 14, 16, 15, 14, 13, 14, 16, 11, 12, 15, 14, 16, 15, 16, 14, 5, 5, 5, 10, 9,
				       4, 10, 11, 10, 11, 9, 8, 10, 12, 6, 7, 11, 6, 16, 12, 10, 11, 11, 9, 8, 8, 12, 11, 16, 12, 11, 10, 0, 0, 0, 0, 0,
				       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		
				       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		
				       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		
				       10, 0, 0, 0, 0, 0, 0, 10, 9, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0},		
					   };



s16 PA_CenterSmartText(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u8 color, u8 size, u8 transp){
s16 i;
s16 x, y;
s16 lx, ly;
u8 letter; 

ly = policeheight[size];

x = basex;
y = basey;

s16 length = 0;


s16 nlines = 0; // Nombre de lignes
s16 xsize[30]; // Taille en largeur de chaque ligne

xsize[0] = 0;

for (i = 0; text[i]; i++){ // Calcul du centrage
	if (text[i] == '\n'){
		nlines++;
		xsize[nlines] = 0; // Rien, par défaut
	}
	else{
		letter = text[i] - 32;
		if (letter > 128) letter -= 96; // pour les accents...
		xsize[nlines] += policesize[size][letter];
	}
}


++nlines; // Si 0, ca fait 1 ligne
y = basey + (((maxy - basey) - (nlines*ly)) >> 1);
nlines = 0;
x = basex + (((maxx-basex) - xsize[nlines]) >> 1);
for (i = 0; text[i]; i++) {
	if (text[i] == '\n'){
		++nlines;
		x = basex + (((maxx-basex) - xsize[nlines]) >> 1);
		y += ly;	
	}
	else{
		letter = text[i] - 32;
		lx = policesize[size][letter];
		letters[transp](size, screen, x, y, letter, color);				
		x += lx;
	}

}

length = i;

return length;

}



#ifdef __cplusplus
}
#endif

