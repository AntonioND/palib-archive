
#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"

//#ifndef TEXT_ALLCHARACTERS
	#include "Text/PA_text.c" // Include the text !
	#include "Text/PA_text.pal.c" // Include the text !	
//#endif
/*#ifdef TEXT_ALLCHARACTERS
	#include "Text/PA_text2.c" // Include the text !
	#include "Text/PA_text2.pal.c" // Include the text !	
#endif*/


u8 PAbgtext[2];
u16 textcol[2];

u16 *PA_textmap[2]; //Pointeur vers la map et les tiles...
u8 *PA_texttiles[2];
u16 *PA_textpal[2]; // Palette !



u32 pa_printx[2];
u32 pa_printy[2];
s16 pa_printscroll[2];






s8 PA_font[2] = {};  // 0 pour normal, 1 pour dégradé, -1 pour custom

// Text system
//u16 TextPal[256];

 u16 PAtext_pal[2] = {0, 0}; // text colors...


textborders PA_TextBox[2];



void PA_CreateTextPal(u8 screen, u8 pal_number, u16 r, u16 g, u16 b) {
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


void PA_InitText(u8 screen, u8 bg_select) {
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
	
	pa_printx[screen] = 0;
	pa_printy[screen] = 0;
	pa_printscroll[screen] = 0;
}


/*
void PA_SetTextCol(u8 screen, u16 r, u16 g, u16 b) {
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


/*
void PA_TextAllPal(u8 screen)
{

}
*/




u16 PA_OutputSimpleText(u8 screen, u16 x, u16 y, const char *text) {
s16 j, i = 0;
u16 textcount = 0; // compte le nombre de lettres...
   
BG_PALETTE[255 + (screen * 512)] = textcol[screen]; // On remet la couleur au cas où on ait chargé du texte par-dessus...

	for (j = 0; text[j]; j++) {
	   while (text[j] == '\n')
	   {
	      y++;
	      j++;
	      i = j;
	   }
		PA_SetTileLetter(screen, x+j-i, y, text[j]);
		++textcount;
	}
return textcount; 
}




void PA_OutputText(u8 screen, u16 x, u16 y, char* text, ...) {
s16 j, i, k = 0;
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
  			   while (PAextext[i] == '\n')
				   {
				      y++;
				      i++;
				      ++textcount;
				      k = i;
				   }
					PA_SetTileLetter(screen, x + textcount - k, y, PAextext[i]);
               ++textcount;
            }
            j += 1;
         }
         else if ((text[j+1] == 'x')) {
			u32 test = va_arg(varg, u32);
			u8 temp;
			u8 started = 0;
			for(i = 0; i < 32; i+=4){
			   temp = (test>>(28-i))&15;
			   if(started || temp){
				   if(temp >= 10) PA_SetTileLetter(screen, x + textcount, y, 'A'+(temp-10));
				   else PA_SetTileLetter(screen, x + textcount, y, '0'+(temp));
					textcount++;
				   started = 1;
				}					   
			}   
			j++;
		}	
         else if ((text[j+1] == 'd') || (text[j+1] == 'f')) {
			double test = 0;
            if (text[j+1] == 'd') {
				PAtextnumber = va_arg(varg, s32);
				test = PAtextnumber;
			}
            else {  // On ne prend que la partie entière pour l'instant, on garde le reste pour plus tard :p
               tempdouble = va_arg(varg, double);
			   test = tempdouble;
			   PAtextnumber = (s32)tempdouble ;
               tempdouble -= PAtextnumber;
            }

            s8 neg = 0; // Si negatif, passe à 1...
            if (test < 0) {
               PAtextnumber = -PAtextnumber;
			   //tempdouble = -tempdouble;
               neg = 1;
            }

            for (i = 0; PAtextnumber || (!PAtextnumber && !i); i++) {  // tant que le nombre ne vaut pas 0 mais que i est positif, on continu à la convertir...
               PAtext[i] = '0' + (PAtextnumber%10);
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
			   if (tempdouble < 0) tempdouble = -tempdouble;
               while((i-- > 0)) { // Pas plus de 6 chiffres après la virgule...
                  tempdouble *= 10;
				  //if (
                  PAtextnumber = (s32)tempdouble;
                  tempdouble -= PAtextnumber;
                  PAtext[0] = '0' + PAtextnumber;
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


void PA_ClearTextBg(u8 screen){
	u8 i, j;
	for (i = 0; i < 32; i++) for (j = 0; j < 32; j++) PA_SetMapTileAll(screen, PAbgtext[screen], i, j, 0);
	pa_printx[screen] = 0;
	pa_printy[screen] = 0;
	pa_printscroll[screen] = 0;
	PA_BGScrollY(screen, PAbgtext[screen], 0);
}





#ifdef __cplusplus
}
#endif









