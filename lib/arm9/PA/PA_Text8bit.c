

#ifdef __cplusplus
extern "C" {
#endif

#include "arm9/PA_Text.h"
#include "arm9/PA_Draw.h"

#include "8bitText/all_gfx.c"

	u8 *text_tiles[10] = {(u8*)(text0_Tiles), (u8*)(text1_Tiles), (u8*)(text2_Tiles), (u8*)(text3_Tiles), (u8*)(text4_Tiles)};
	u16 *text_maps[10] = {(u16*)(void*)(text0_Map), (u16*)(void*)text1_Map, (u16*)(void*)text2_Map, (u16*)(void*)text3_Map, (u16*)(void*)text4_Map};		

	u8 *pa8bitdefaultsize[10] = {(u8*)text0_Sizes, (u8*)text1_Sizes, (u8*)text2_Sizes, (u8*)text3_Sizes, (u8*)text4_Sizes};
	u8 police8bitheight[10];



void Letter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color);
void TranspLetter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color);
void NoLetter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color);
void RotLetter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color);
void RotLetter2(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color);

letterfp letters[5] = {(letterfp)&Letter, (letterfp)&TranspLetter, (letterfp)&NoLetter, (letterfp)&RotLetter, (letterfp)&RotLetter2};



extern inline void PA_8bitDrawTile(u8 screen, u16 x, u16 y, s8 lx, s8 ly, u8 *data, u8 color){
	u16 i, j;
	for (j = 0; j < ly; j++){
		for (i = 0; i < lx; i++){
			PA_Put8bitPixel(screen, x+i, y+j, data[i+(j<<3)]*color);
		}
	}
}	

extern inline void PA_8bitDrawTileTransp(u8 screen, u16 x, u16 y, s8 lx, s8 ly, u8 *data, u8 color){
	u16 i, j;
	for (j = 0; j < ly; j++){
		for (i = 0; i < lx; i++){
			PA_DrawBg[screen][((y+j) <<7) + ((x+i)>>1)] |= (data[i+(j<<3)]*color)<<(((x+i)&1)<<3);
		}
	}
}	

	

extern inline void PA_8bitDrawTileRot(u8 screen, u16 x, u16 y, s8 lx, s8 ly, u8 *data, u8 color){
	u16 i, j;
	s16 temp = x;
	x = 255- y;
	y = temp;
	
	for (j = 0; j < ly; j++){
		for (i = 0; i < lx; i++){
			PA_Put8bitPixel(screen, x-j, y+i, data[i+(j<<3)]*color);
		}
	}
}	

extern inline void PA_8bitDrawTileRot2(u8 screen, u16 x, u16 y, s8 lx, s8 ly, u8 *data, u8 color){
	u16 i, j;
	s16 temp = x;
	x = 255- y;
	y = temp;
	
	for (j = 0; j < ly; j++){
		for (i = 0; i < lx; i++){
			PA_Put8bitPixel(screen, x+j, y-i, data[i+(j<<3)]*color);
		}
	}
}	



void NoLetter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color) {

}




void RotLetter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color) {
u8 lx = pa8bitdefaultsize[size][(u8)letter];
u8 ly = police8bitheight[size];
u8 *data;

if (ly > 8) {
	u16 firstpos = ((letter&31)<<1)+((letter >> 5)<<7);
	data = (u8*)(text_tiles[size]+(text_maps[size][firstpos]<<6));
	PA_8bitDrawTileRot(screen, x, y, 8, 8, data, color);
	data = (u8*)(text_tiles[size]+(text_maps[size][firstpos+64]<<6));
	PA_8bitDrawTileRot(screen, x, y+8, 8, ly-8, data, color);	
	firstpos ++;
	data = (u8*)(text_tiles[size]+(text_maps[size][firstpos]<<6));
	PA_8bitDrawTileRot(screen, x+8, y, lx-8, 8, data, color);
	data = (u8*)(text_tiles[size]+(text_maps[size][firstpos+64]<<6));
	PA_8bitDrawTileRot(screen, x+8, y+8, lx-8, ly-8, data, color);			
}
else {
	data = (u8*)(text_tiles[size]+(text_maps[size][(u8)letter]<<6));
	PA_8bitDrawTileRot(screen, x, y, lx, ly, data, color);
}

}


void RotLetter2(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color) {
u8 lx = pa8bitdefaultsize[size][(u8)letter];
u8 ly = police8bitheight[size];
u8 *data;

if (ly > 8) {
	u16 firstpos = ((letter&31)<<1)+((letter >> 5)<<7);
	data = (u8*)(text_tiles[size]+(text_maps[size][firstpos]<<6));
	PA_8bitDrawTileRot2(screen, x, y, 8, 8, data, color);
	data = (u8*)(text_tiles[size]+(text_maps[size][firstpos+64]<<6));
	PA_8bitDrawTileRot2(screen, x, y+8, 8, ly-8, data, color);	
	firstpos ++;
	data = (u8*)(text_tiles[size]+(text_maps[size][firstpos]<<6));
	PA_8bitDrawTileRot2(screen, x+8, y, lx-8, 8, data, color);
	data = (u8*)(text_tiles[size]+(text_maps[size][firstpos+64]<<6));
	PA_8bitDrawTileRot2(screen, x+8, y+8, lx-8, ly-8, data, color);			
}
else {
	data = (u8*)(text_tiles[size]+(text_maps[size][(u8)letter]<<6));
	PA_8bitDrawTileRot2(screen, x, y, lx, ly, data, color);
}

}





void Letter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color) {
u8 lx = pa8bitdefaultsize[size][(u8)letter];
u8 ly = police8bitheight[size];
u8 *data;

if (ly > 8) {
	u16 firstpos = ((letter&31)<<1)+((letter >> 5)<<7);
	data = (u8*)(text_tiles[size]+(text_maps[size][firstpos]<<6));
	PA_8bitDrawTile(screen, x, y, 8, 8, data, color);
	data = (u8*)(text_tiles[size]+(text_maps[size][firstpos+64]<<6));
	PA_8bitDrawTile(screen, x, y+8, 8, ly-8, data, color);	
	firstpos ++;
	data = (u8*)(text_tiles[size]+(text_maps[size][firstpos]<<6));
	PA_8bitDrawTile(screen, x+8, y, lx-8, 8, data, color);
	data = (u8*)(text_tiles[size]+(text_maps[size][firstpos+64]<<6));
	PA_8bitDrawTile(screen, x+8, y+8, lx-8, ly-8, data, color);			
}
else {
	data = (u8*)(text_tiles[size]+(text_maps[size][(u8)letter]<<6));
	PA_8bitDrawTile(screen, x, y, lx, ly, data, color);
}
}









void TranspLetter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color) {
u8 lx = pa8bitdefaultsize[size][(u8)letter];
u8 ly = police8bitheight[size];
u8 *data;

if (ly > 8) {
	u16 firstpos = ((letter&31)<<1)+((letter >> 5)<<7);
	data = (u8*)(text_tiles[size]+(text_maps[size][firstpos]<<6));
	PA_8bitDrawTileTransp(screen, x, y, 8, 8, data, color);
	data = (u8*)(text_tiles[size]+(text_maps[size][firstpos+64]<<6));
	PA_8bitDrawTileTransp(screen, x, y+8, 8, ly-8, data, color);	
	firstpos ++;
	data = (u8*)(text_tiles[size]+(text_maps[size][firstpos]<<6));
	PA_8bitDrawTileTransp(screen, x+8, y, lx-8, 8, data, color);
	data = (u8*)(text_tiles[size]+(text_maps[size][firstpos+64]<<6));
	PA_8bitDrawTileTransp(screen, x+8, y+8, lx-8, ly-8, data, color);			
}
else {
	data = (u8*)(text_tiles[size]+(text_maps[size][(u8)letter]<<6));
	PA_8bitDrawTileTransp(screen, x, y, lx, ly, data, color);
}


}

void PA_8bitTextInit(void){
	police8bitheight[0] = text0_Height;
	police8bitheight[1] = text1_Height;
	police8bitheight[2] = text2_Height;
	police8bitheight[3] = text3_Height;
	police8bitheight[4] = text4_Height;
}


s16 PA_SmartText(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u8 color, u8 size, u8 transp, s32 limit){

PA_8bitTextInit();


s16 i, j;
s16 x, y;
s16 lx, ly;
s16 letter; 

ly = police8bitheight[size];

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
			letter = text[i+wordletter];
			lx = pa8bitdefaultsize[size][letter];
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
						letter = text[j];
						//if (letter > 128) letter -= 96; // pour les accents...
						lx = pa8bitdefaultsize[size][letter];
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
				letter = text[j];
				lx = pa8bitdefaultsize[size][letter];
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





s16 PA_CenterSmartText(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u8 color, u8 size, u8 transp){
PA_8bitTextInit();
s16 i;
s16 x, y;
s16 lx, ly;
u8 letter; 

ly = police8bitheight[size];

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
		letter = text[i];
		//if (letter > 128) letter -= 96; // pour les accents...
		xsize[nlines] += pa8bitdefaultsize[size][letter];
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
		letter = text[i];
		lx = pa8bitdefaultsize[size][letter];
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

