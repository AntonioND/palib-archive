#include "PA9.h"

#ifndef TEXT_ALLCHARACTERS
	#include "Text/PA_text.c" // Include the text !
	#include "Text/PA_text.pal.c" // Include the text !	
#endif
#ifdef TEXT_ALLCHARACTERS
	#include "Text/PA_text2.c" // Include the text !
	#include "Text/PA_text2.pal.c" // Include the text !	
#endif


u8 PAbgtext[2];
u16 textcol[2];

u16 *PA_textmap[2]; //Pointeur vers la map et les tiles...
u8 *PA_texttiles[2];
u16 *PA_textpal[2]; // Palette !

s8 PA_font[2] = {};  // 0 pour normal, 1 pour dégradé, -1 pour custom

// Text system
//u16 TextPal[256];

 u16 PAtext_pal[2] = {0, 0}; // text colors...


textborders PA_TextBox[2];



void PA_CreateTextPal(bool screen, u8 pal_number, u16 r, u16 g, u16 b) {
	u16 baser, baseg, baseb;
	u16 i;
	
	for (i = 1; i < 7; i++) // Just do the first colors...
	{
		baser = 1+(PA_textpal[screen][i]&31)*r;
		baseg = 1+((PA_textpal[screen][i]>>5)&31)*g;
		baseb = 1+((PA_textpal[screen][i]>>10)&31)*b;
//		TextPal[i] = PA_RGB((baser>>5), (baseg>>5), (baseb>>5));
		PA_SetBgPalNCol(screen, PAbgtext[screen], pal_number, i, PA_RGB((baser>>5), (baseg>>5), (baseb>>5)));
	}	
}


void PA_InitText(bool screen, u8 bg_select) {
#ifndef TEXT_ALLCHARACTERS
	PA_InitCustomText(screen, bg_select, PA_text);
	PA_SetTextTileCol(screen, 0);
#endif
#ifdef TEXT_ALLCHARACTERS
	PA_InitCustomText(screen, bg_select, PA_text2);
	PA_SetTextTileCol(screen, 0);
#endif
	
	PA_CreateTextPal(screen, 0, 31, 31, 31);
	PA_CreateTextPal(screen, 1, 31, 0, 0);
	PA_CreateTextPal(screen, 2, 0, 31, 0);
	PA_CreateTextPal(screen, 3, 0, 0, 31);
	PA_CreateTextPal(screen, 4, 31, 0, 31);
	PA_CreateTextPal(screen, 5, 0, 31, 31);
	PA_CreateTextPal(screen, 6, 31, 31, 0);	
	PA_CreateTextPal(screen, 7, 25, 25, 25);
	PA_CreateTextPal(screen, 8, 20, 20, 20);
	PA_CreateTextPal(screen, 9, 0, 0, 0);	
}


/*
void PA_SetTextCol(bool screen, u16 r, u16 g, u16 b) {
	u16 baser, baseg, baseb;
	u16 i;
	
	
	
	for (i = 1; i < 256; i++)
	{
		baser = 1+(PA_textpal[screen][i]&31)*r;
		baseg = 1+((PA_textpal[screen][i]>>5)&31)*g;
		baseb = 1+((PA_textpal[screen][i]>>10)&31)*b;
		TextPal[i] = PA_RGB((baser>>5), (baseg>>5), (baseb>>5));
	}	
	PA_LoadBgPal(screen, PAbgtext[screen], (void*)TextPal);
}
*/



void PA_TextAllPal(bool screen)
{

}





void PA_EraseTextBox(bool screen){
s16 i, j;
	for (j = PA_TextBox[screen].y1+1; j < PA_TextBox[screen].y2; j++)
		for (i = PA_TextBox[screen].x1+1; i < PA_TextBox[screen].x2; i++)
			PA_SetTileLetter(screen, i, j, ' ');
}

void PA_InitTextBorders(bool screen, u8 x1, u8 y1, u8 x2, u8 y2){

	// Fill the text border info
	PA_TextBox[screen].x1 = x1;
	PA_TextBox[screen].x2 = x2;
	PA_TextBox[screen].y1 = y1;
	PA_TextBox[screen].y2 = y2;

	// Init the text with border and all...
	PA_SetMapTileEx(screen, PAbgtext[screen], x1, y1, 1, 0, 0, 0); // Top left corner
	PA_SetMapTileEx(screen, PAbgtext[screen], x2, y1, 1, 1, 0, 0); // Top right corner flipped
	PA_SetMapTileEx(screen, PAbgtext[screen], x1, y2, 1, 0, 1, 0); // Bottom left corner vflip
	PA_SetMapTileEx(screen, PAbgtext[screen], x2, y2, 1, 1, 1, 0); // Bottom right corner vflip+hflip

	s16 i, j; // Fill in the borders...
	for (i = x1+1; i < x2; i++) {
		PA_SetMapTileEx(screen, PAbgtext[screen], i, y1, 2, 0, 0, 0);
		PA_SetMapTileEx(screen, PAbgtext[screen], i, y2, 2, 0, 1, 0);
	}
	for (j = y1+1; j < y2; j++){
		PA_SetMapTileEx(screen, PAbgtext[screen], x1, j, 3, 0, 0, 0);
		PA_SetMapTileEx(screen, PAbgtext[screen], x2, j, 3, 1, 0, 0);	
	}
	
	PA_EraseTextBox(screen);
}














u16 PA_OutputSimpleText(bool screen, u16 x, u16 y, const char *text) {
s16 j;
u16 textcount = 0; // compte le nombre de lettres...
   
BG_PALETTE[255 + (screen * 512)] = textcol[screen]; // On remet la couleur au cas où on ait chargé du texte par-dessus...

	for (j = 0; text[j]; j++) {
		PA_SetTileLetter(screen, x+j, y, text[j]);
		++textcount;
	}
return textcount; 
}





u32 PA_BoxTextNoWrap(bool screen, u16 basex, u16 basey, u16 maxx, u16 maxy, const char *text, u32 limit) {
u16 x = basex;
u16 y = basey;
u16 j;
u16 textcount = 0; // compte le nombre de lettres...
bool loop = 1; // On continue...
   
BG_PALETTE[255 + (screen * 512)] = textcol[screen]; // On remet la couleur au cas où on ait chargé du texte par-dessus...

	for (j = 0; text[j]&&loop&&(j < limit); j++) {
		if (text[j] == '\n') {
			x = basex;
			y++;
			if (y > maxy) loop = 0;
		}
		//else if (text[j] == '\r')
		else{			
			PA_SetTileLetter(screen, x, y, text[j]);
			x++;
			if (x > maxx) {
				x = basex;
				y++;
				if (y > maxy) loop = 0;
			}
		}
		++textcount;
	}
return textcount; 
}


/*
void PA_OutputNLetters(bool screen, u16 x, u16 y, const char *text, u32 n){
s16 j;
u16 textcount = 0; // compte le nombre de lettres...
   
BG_PALETTE[255 + (screen * 512)] = textcol[screen]; // On remet la couleur au cas où on ait chargé du texte par-dessus...

	for (j = 0; text[j]&&(j < n); j++) {
			PA_SetMapTileAll(screen, PAbgtext[screen], x + j, y, PA_textmap[screen][(int)text[j]]);
			++textcount;
	}
}
*/





u32 PA_BoxText(bool screen, u16 basex, u16 basey, u16 maxx, u16 maxy, const char *text, u32 limit){
u16 i, j;
s16 x, y;

s16 letter; 


x = basex;
y = basey;
u16 ylimit = maxy;

s16 length = 0;

s16 wordx = 0; // Longueur de mot
s16 wordletter = 0;

//BG_PALETTE[255 + (screen * 512)] = textcol[screen]; // On remet la couleur au cas où on ait chargé du texte par-dessus...

//bool loop = 1; // On continue...
   u8 textcolor = PAtext_pal[screen]; // save the screen color
   
   
for (i = 0; (text[i] && (y <= maxy) && (i < limit)); i++) {
    if (text[i] == '%') {
		if (text[i+1] == 'c'){ // change color !
			PA_SetTextTileCol(screen, text[i+2]-'0');
			i+=2;
			limit+=3; // Don't count it in the limit
			length-=3; // Don't count them in the length
		}
	}
	else if (text[i] == '\n'){
		while(x < maxx) { // On efface tout ce qui suit
			PA_SetTileLetter(screen, x, y, ' ');
			x++;
		}
		if (text[i+1] == ' ') {
			i++; // On vire s'il y a un espace	
			limit++;
			length--;
		}
		x = basex;
		y ++;	
	}
	else{
		wordletter = 1;
		wordx = 0;
		
		while(!((text[i+wordletter] <= 32) || ((text[i+wordletter] == '%') && (text[i+wordletter+1] == 'c')))) { // >= 32, donc si 0, '\n', on ' ' :)
			letter = text[i+wordletter] - 32;
			wordx++;
			wordletter++;
		}
		
		//if (text[i+wordletter] == 0) loop = 0;

		if (x + wordx > maxx ) {  // On dépasse en X...
			while(x < maxx) { // On efface tout ce qui suit
				PA_SetTileLetter(screen, x, y, ' ');
				x++;
			}
			x = basex;
			y ++;
		
			if(text[i] != ' ') { // On vire s'il y a un espace	
				if(y <= ylimit) { // Si on n'a pas dépassé...
					for (j = i; (j < (i + wordletter))&&(j < limit); j++) {
						PA_SetTileLetter(screen, x, y, text[j]);
						x++;
						i++;
					}
					i--;
					//i+=wordletter-1;
				} // Sinon ca va s'arreter
				else i--; // On n'a pas affiché la derniere lettre...
			}
		}		
		else{ // Si pas de passage à la ligne auto
			s32 jmax = (i + wordletter);
			if (text[(i + wordletter-1)] < 32) jmax--; // On ne dessinera pas ce caractère
			
			for (j = i; (j < jmax)&&(j < limit); j++) {
				PA_SetTileLetter(screen, x, y, text[j]);		
				x++;
				i++;
			}
			i--;
		}

	}
}
length += i;
PA_SetTextTileCol(screen, textcolor);
return length;

}





void PA_OutputText(bool screen, u16 x, u16 y, char* text, ...) {
s16 j, i;
u16 textcount = 0; // compte le nombre de lettres...
u8 PAtext[32]; // tableau où l'on copie les nombres et tout...
u8 PAtextcount = 0; // Quand on a un nombre, il faut savoir sa longueur
u8 *PAextext; // Extra text
s32 PAtextnumber;
double tempdouble = 0;
     
//BG_PALETTE[255 + (screen * 512)] = textcol[screen]; // On remet la couleur au cas où on ait chargé du texte par-dessus...

va_list varg;           /* Variable identifiant le prochain paramètre. */

   va_start(varg, text);
   
   PAtextcount = 0;
   
   u8 textcolor = PAtext_pal[screen];

   
   
   
   for (j = 0; text[j]; j++) {
      if (text[j] == '%') {
		if (text[j+1] == 'c'){ // change color !
			PA_SetTextTileCol(screen, text[j+2]-'0');
			j+=2;
		}
        else if (text[j+1] == 's') {  // S'il y a %s, c'est une chaine de caractères...
            PAextext = (u8*)va_arg(varg, const u8 *);  // Pointeur vers la chaine de charactères...
            for (i = 0; PAextext[i]; i++) {
				PA_SetTileLetter(screen, x + textcount, y, PAextext[i]);
               ++textcount;
            }
            j += 1;
         }
         else if ((text[j+1] == 'd') || (text[j+1] == 'f')) {

            if (text[j+1] == 'd') PAtextnumber = va_arg(varg, s32);
            else {  // On ne prend que la partie entière pour l'instant, on garde le reste pour plus tard :p
               tempdouble = va_arg(varg, double);
               PAtextnumber = (s32)tempdouble ;
               tempdouble -= PAtextnumber;
            }

            s8 neg = 0; // Si negatif, passe à 1...
            if (PAtextnumber < 0) {
               PAtextnumber = -PAtextnumber;
               neg = 1;
            }

            for (i = 0; PAtextnumber || (!PAtextnumber && !i); i++) {  // tant que le nombre ne vaut pas 0 mais que i est positif, on continu à la convertir...
               PAtext[i] = 48 + (PAtextnumber%10);
               PAtextnumber = PAtextnumber / 10; // Nombre de départ...
            }
            if (neg) {
               PAtext[i] = '-';
               ++i;
            }
            for (i--; i > -1; i--) {
               	PA_SetTileLetter(screen, x + textcount, y, PAtext[i]);
               ++ textcount;
            }
            j += 1;
            
            if (text[j] == 'f') { // Il reste donc la virgule à traiter...
               PA_SetTileLetter(screen, x + textcount, y, '.');

               ++ textcount;
               i = text[j+1] - 48;  // Nombre de chiffres après la virgule...
               PAtext[1] = 0; // Permet de stopper si c'est vba...

               while((i-- > 0)) { // Pas plus de 6 chiffres après la virgule...
                  tempdouble *= 10;
                  PAtextnumber = (s32)tempdouble;
                  tempdouble -= PAtextnumber;
                  PAtext[0] = 48 + PAtextnumber;
                  PA_SetTileLetter(screen, x + textcount, y, PAtext[0]);
                  ++ textcount;
               }
            j += 1;
            }

          }
		  else if ((text[j+3] == 'd') && (text[j+1] != ' ')&& (text[j+2] != ' ')){ // C'est plus loin, donc on a genre %02d...
            PAtextnumber = va_arg(varg, s32);
            s8 neg = 0; // Si negatif, passe à 1...
			u8 amettre = text[j+1]; // Nombre à mettre pour remplir
			s8 nfois = text[j+2] - '0'; // Nombre de caractères à mettre
	
            if (PAtextnumber < 0) {
               PAtextnumber = -PAtextnumber;
               neg = 1;
            }

            for (i = 0; PAtextnumber || (!PAtextnumber && !i); i++) {  // tant que le nombre ne vaut pas 0 mais que i est positif, on continu à la convertir...
               PAtext[i] = 48 + (PAtextnumber%10);
               PAtextnumber = PAtextnumber / 10; // Nombre de départ...
			   nfois--; // Un nombre de moins à mettre...
            }
			
			while(nfois > 0) { // On complète de 0 est tout...
				PA_SetTileLetter(screen, x + textcount, y, amettre);
				++textcount;
				--nfois;
			}
			
            if (neg) {
               PAtext[i] = '-';
               ++i;
            }
            for (i--; i > -1; i--) {
               PA_SetTileLetter(screen, x + textcount, y, PAtext[i]);
               ++ textcount;
            }
            j += 3;
		  
		  }
          else { // Sinon c'est juste
             PA_SetTileLetter(screen, x + textcount, y, text[j]);
            ++textcount;
          }
      }
	  else if (text[j] == '\n'){
	     textcount = 0;
         ++y;
	  }
      else if ((text[j] == '/') && (text[j+1] == 'n')) {
         textcount = 0;
         ++y;
		 j++;
      }
      else{
           PA_SetTileLetter(screen, x + textcount, y, text[j]);
         ++textcount;
      }
   }


    va_end(varg);           /* Terminaison. */

   PA_SetTextTileCol(screen, textcolor); // put back the old color
	
	
}
char *selectchar(char *text,int begin, int end){
char *text2 = (char*)malloc(sizeof(char)*(end-begin));
int i;
for(i=begin;i<end;i++)text2[i-begin]=text[i];
text2[i-begin+1]='\0';
return text2;
}
//defilement
void PA_OutputTextSpecial0(bool screen, int x1, int y,char *text){
int i,k,x2=strlen(text)+x1+1;
for(i=x2;i>x1;i--){
PA_OutputSimpleText(screen, i, y, selectchar(text,0,x2-x1-1));
for(k=0;k<30;k++)PA_WaitForVBL();
PA_OutputSimpleText(screen, i+strlen(selectchar(text,0,x2-x1-1))-1, y, " ");
}
}
//separation
void PA_OutputTextSpecial1(bool screen, int x1, int y,char *text){
int i,k,x2=strlen(text)+x1+1;
for(i=x2;i>x1;i--){
PA_OutputSimpleText(screen, i+strlen(selectchar(text,0,i-x1-1)), y, " ");
PA_OutputSimpleText(screen, i, y, selectchar(text,0,i-x1-1));
for(k=0;k<30;k++)PA_WaitForVBL();
}
}
//etirement
void PA_OutputTextSpecial2(bool screen, int x1, int y,char *text){
int i,k,x2=strlen(text)+x1;
for(i=x2;i>x1;i--){
PA_OutputSimpleText(screen, i, y, selectchar(text,0,i-x1));
for(k=0;k<30;k++)PA_WaitForVBL();
}
}
//etirement + suppression de la fin
void PA_OutputTextSpecial3(bool screen, int x1, int y,char *text){
int i,k,x2=strlen(text)+x1+1;
for(i=x2;i>x1;i--){
PA_OutputSimpleText(screen, x2+strlen(selectchar(text,0,i-x1-1))+1, y, " ");
PA_OutputSimpleText(screen, i, y, selectchar(text,0,i-x1-1));
for(k=0;k<30;k++)PA_WaitForVBL();
}
}
//etirement + separation
void PA_OutputTextSpecial4(bool screen, int x1, int y,char *text){
int i,k,x2=strlen(text)+x1+1;
for(i=x2;i>x1;i--){
PA_OutputSimpleText(screen, i+strlen(selectchar(text,0,i-x1)), y, " ");
PA_OutputSimpleText(screen, i, y, selectchar(text,0,i-x1-1));
for(k=0;k<30;k++)PA_WaitForVBL();
}
}
//defilement + suppression
void PA_OutputTextSpecial5(bool screen, int x1, int y,char *text){
int i,k,x2=strlen(text)+x1+1;
for(i=x2;i>x1;i--){
PA_OutputSimpleText(screen, i, y, selectchar(text,0,i-x1-1));
for(k=0;k<30;k++)PA_WaitForVBL();
PA_OutputSimpleText(screen, i+strlen(selectchar(text,0,i-x1-1))-2, y, "   ");
}
}



















// Use all sizes...
#ifdef TEXT_ALLSIZES
	// Et pour le texte à taille variable...
	#include "text0.c" // La police à taille variable...
	#include "text1.c" // La police à taille variable...
	#include "text2.c" // La police à taille variable...
	#include "text3.c" // La police à taille variable...
	#include "text4.c" // La police à taille variable...

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



s16 PA_CenterSmartText(bool screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u8 color, u8 size, u8 transp){
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


#else
// If only one size, use size 1...
//#ifndef TEXT_ALLSIZES
	//#include "text1.c" // La police à taille variable...
	//const u8 *textData[5] = {(u8*)(text1Data), (u8*)(text1Data), (u8*)(text1Data), (u8*)(text1Data), (u8*)(text1Data)};
	//const u8 policeheight[5]// = {8, 8, 8, 8, 8};
	/*const u8 policesize[5][256] = {
					  {2, 2, 4, 7, 6, 9, 0, 2, 4, 4, 2, 6, 3, 4, 3, 4, 6, 4, 6, 6, 6, 6, 6, 6, 6, 6, 3, 3, 4, 6, 4, 5,
					   8, 6, 6, 6, 5, 5, 5, 6, 6, 4, 4, 6, 5, 6, 7, 7, 6, 7, 6, 6, 6, 6, 6, 8, 6, 6, 6, 4, 4, 4, 6, 6,
				       2, 5, 5, 5, 5, 5, 5, 6, 5, 2, 4, 5, 2, 8, 5, 5, 5, 5, 5, 5, 4, 5, 4, 6, 6, 4, 5, 0, 0, 0, 0, 0,	
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
					  {2, 2, 4, 7, 6, 9, 0, 2, 4, 4, 2, 6, 3, 4, 3, 4, 6, 4, 6, 6, 6, 6, 6, 6, 6, 6, 3, 3, 4, 6, 4, 5,
					   8, 6, 6, 6, 5, 5, 5, 6, 6, 4, 4, 6, 5, 6, 7, 7, 6, 7, 6, 6, 6, 6, 6, 8, 6, 6, 6, 4, 4, 4, 6, 6,
				       2, 5, 5, 5, 5, 5, 5, 6, 5, 2, 4, 5, 2, 8, 5, 5, 5, 5, 5, 5, 4, 5, 4, 6, 6, 4, 5, 0, 0, 0, 0, 0,	
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
					  {2, 2, 4, 7, 6, 9, 0, 2, 4, 4, 2, 6, 3, 4, 3, 4, 6, 4, 6, 6, 6, 6, 6, 6, 6, 6, 3, 3, 4, 6, 4, 5,
					   8, 6, 6, 6, 5, 5, 5, 6, 6, 4, 4, 6, 5, 6, 7, 7, 6, 7, 6, 6, 6, 6, 6, 8, 6, 6, 6, 4, 4, 4, 6, 6,
				       2, 5, 5, 5, 5, 5, 5, 6, 5, 2, 4, 5, 2, 8, 5, 5, 5, 5, 5, 5, 4, 5, 4, 6, 6, 4, 5, 0, 0, 0, 0, 0,	
				       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		
				       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		
				       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		
				       5, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0},	
					   };*/
#endif




