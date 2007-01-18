#include "PA9.h"

void Letter(u8 size, bool screen, u16 x, u16 y, char letter, u8 color);
void TranspLetter(u8 size, bool screen, u16 x, u16 y, char letter, u8 color);
void NoLetter(u8 size, bool screen, u16 x, u16 y, char letter, u8 color);
void RotLetter(u8 size, bool screen, u16 x, u16 y, char letter, u8 color);
void RotLetter2(u8 size, bool screen, u16 x, u16 y, char letter, u8 color);

letterfp letters[5] = {(letterfp)&Letter, (letterfp)&TranspLetter, (letterfp)&NoLetter, (letterfp)&RotLetter, (letterfp)&RotLetter2};



extern inline void normala(bool screen, u16 i, s8 ly, s16 pos, u8 *data, u8 color){
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


extern inline void normalb(bool screen, u16 i, s8 ly, s16 pos, u8 *data, u8 color){
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





void NoLetter(u8 size, bool screen, u16 x, u16 y, char letter, u8 color) {

}




void RotLetter(u8 size, bool screen, u16 x, u16 y, char letter, u8 color) {
u8 lx = PA_LETTER_SIZE(size, (u8)letter);
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


void RotLetter2(u8 size, bool screen, u16 x, u16 y, char letter, u8 color) {
u8 lx = PA_LETTER_SIZE(size, (u8)letter);
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





void Letter(u8 size, bool screen, u16 x, u16 y, char letter, u8 color) {
u8 lx = PA_LETTER_SIZE(size, (u8)letter);
u8 ly = policeheight[size];
s16 policex = policewidth[size];
u8 *data;
data = (u8*)(textData[size]);
s32 datax = policepos[size][(u8)letter];
s32 datax2;

//data = &data[policepos[size][(u8)letter]]; // on se place au bon endroit

u16 i, j;


for (i = 0; i < lx; i++){
	datax2 = datax;
	for (j = 0; j < ly; j++){
		PA_Put8bitPixel(screen, x + i, y + j, data[datax2]*color);
		datax2 += policex;
	}
	datax++;
}

}
/*	
	if ((i+x)&1){
		for (j = 0; j < ly; j++){
			PA_DrawBg[screen][pos] &= 255;
			PA_DrawBg[screen][pos] |= temp*color;	
			pos+=128;	
			tempdata = (u8*)(data + policex);
		}
	}
	else {
		for (j = 0; j < ly; j++){
			PA_DrawBg[screen][pos] &= 255 << 8;
			PA_DrawBg[screen][pos] |= temp*color;	
			pos+=128;	
			tempdata = (u8*)(data + policex);
		}
	}
	*/		

/*
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

*/








extern inline void transpa(bool screen, u16 i, s8 ly, s16 pos, u8 *data, u8 color){
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


extern inline void transpb(bool screen, u16 i, s8 ly, s16 pos, u8 *data, u8 color){
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




void TranspLetter(u8 size, bool screen, u16 x, u16 y, char letter, u8 color) {
u8 lx = PA_LETTER_SIZE(size, (u8)letter);
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




s16 PA_SmartText(bool screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u8 color, u8 size, u8 transp, s32 limit){
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
			lx = PA_LETTER_SIZE(size, letter);
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
						//if (letter > 128) letter -= 96; // pour les accents...
						lx = PA_LETTER_SIZE(size, letter);
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
				lx = PA_LETTER_SIZE(size, letter);
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
