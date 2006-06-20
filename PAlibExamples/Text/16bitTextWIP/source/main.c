// This example shows how to use 8 bit backgrounds, 
// on which you can draw with the stylus or show text in different sizes, colors, or rotations

// Includes
#include <PA9.h>       // Include for PA_Lib

void PA_16c8X8Letter2(u8 screen, s16 x, s16 y, u32 *image, u16 color){  
   x += 8; y += 8;
   u16 temp = (x&7)<<2;
   u16 pos = PA_16cPos(x, y);
   PA_Draw1632[screen][pos] = color;
   PA_Draw1632[screen][pos+1] = color;
   PA_Draw1632[screen][pos+2] = color;
   PA_Draw1632[screen][pos+3] = color;
   PA_Draw1632[screen][pos+4] = color;   
   PA_Draw1632[screen][pos+5] = color;
   PA_Draw1632[screen][pos+6] = color;
   PA_Draw1632[screen][pos+7] = color;  
   pos += (26*8); // Next columns
   temp = 32-temp;
   PA_Draw1632[screen][pos] = color;
   PA_Draw1632[screen][pos+1] = color;
   PA_Draw1632[screen][pos+2] = color;
   PA_Draw1632[screen][pos+3] = color;
   PA_Draw1632[screen][pos+4] = color;   
   PA_Draw1632[screen][pos+5] = color;
   PA_Draw1632[screen][pos+6] = color;
   PA_Draw1632[screen][pos+7] = color;       
}  

extern inline void PA_16cLetter2(u8 screen, s16 x, s16 y, char letter, u8 size, u16 color){
PA_16c8X8Letter2(screen, x, y, (u32*)(c16_font[size]+(letter<<3)), color);
}

s16 PA_16cText2(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u8 color, u8 size, s32 limit){
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
						PA_16cLetter2(screen, x, y, letter, size, color);
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
					PA_16cLetter2(screen, x, y, letter, size, color);
					//letters[transp](size, screen, x, y, letter, color);				
					x += lx;
//				}
			}
			i+=wordletter-1;
		}

	}
}

length = i;



return length;

}

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL


    PA_SetBgPalCol(0, 1, PA_RGB(31, 0,0));
    
	//PA_Init16bitBg(0, 3);
	//PA_Init16bitBg(1, 3);
	
	PA_Init16cBgEx(0, 3, 1);
	//PA_Init16cBgEx(0,3, u8 npalette);
	PA_16c8X8Letter2(0, 0,0,(u32*)(c16_font[0]+(('a'-32)<<3)),PA_RGB(31, 0,0));
	// Infinite loop to keep the program running
	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
