
#ifdef __cplusplus
extern "C" {
#endif



#include "PA_TextBits.h"

#include "16c/all_gfx.c"

	u16 *bittext_maps[10] = {(u16*)(void*)(c16_text0_Map), (u16*)(void*)c16_text1_Map, (u16*)(void*)c16_text2_Map, (u16*)(void*)c16_text3_Map, (u16*)(void*)c16_text4_Map};		
	u8 *bittext_tiles_blank[10] = {(u8*)(void*)(c16_text0_Tiles), (u8*)(void*)c16_text1_Tiles, (u8*)(void*)c16_text2_Tiles, (u8*)(void*)c16_text3_Tiles, (u8*)(void*)c16_text4_Tiles};		

	u8 *pa_bittextdefaultsize[10] = {(u8*)c16_text0_Sizes, (u8*)c16_text1_Sizes, (u8*)c16_text2_Sizes, (u8*)c16_text3_Sizes, (u8*)c16_text4_Sizes};
	u16 pa_bittextsizes[5] = {sizeof(c16_text0_Tiles), sizeof(c16_text1_Tiles), sizeof(c16_text2_Tiles), sizeof(c16_text3_Tiles), sizeof(c16_text4_Tiles)};
	
	u8 pa_bittextpoliceheight[10] = {c16_text0_Height, c16_text1_Height, c16_text2_Height, c16_text3_Height, c16_text4_Height};

LetterPos PA_LetterPos;
textinfo_type textinfo = {1, 0, ALIGN_LEFT, 0};

extern inline void PA_AddLetterPos(s16 Letter, s16 x, s16 y, u8 size, u16 color){
   PA_LetterPos.Letter[PA_LetterPos.NLetters].Letter = Letter;
	PA_LetterPos.Letter[PA_LetterPos.NLetters].X = x;
	PA_LetterPos.Letter[PA_LetterPos.NLetters].Y = y;
	PA_LetterPos.Letter[PA_LetterPos.NLetters].Size = size;
	PA_LetterPos.Letter[PA_LetterPos.NLetters].Color = color;
	PA_LetterPos.NLetters++;	
}


void PA_DoAlign(u16 start, s16 x, s16 maxx, u8 justify){
   s16 i;
   s16 width = (maxx+1)-x;
//   PA_Print(0, "%d   \n", width);
   
	if(textinfo.align == ALIGN_RIGHT){ // Cas simple, on rajoute la différence de largeur à toutes les lettres...
		width++;
		for(i = start; i < PA_LetterPos.NLetters; i++) PA_LetterPos.Letter[i].X += width;
	}     
	else if(textinfo.align == ALIGN_CENTER){ // Cas simple, on rajoute la moitié de la largeur à toutes les lettres...
		width = (width+1)>>1;
		for(i = start; i < PA_LetterPos.NLetters; i++) PA_LetterPos.Letter[i].X += width;
	}    
	else if(justify && (textinfo.align == ALIGN_JUSTIFY)){ // Cas relou ^^
		//u8 nletters = PA_LetterPos.NLetters-start; // Nombre de lettres
		u8 justify = 0; // Décallage à faire au début...
		u8 spaces = 0;
		u8 change = 0;
		u8 quickadd = 0;
		
		u8 nletters = PA_LetterPos.NLetters-1 - start;
		if(nletters < width){ // Less letters than the width to add, add 1 pixel/letter...
			quickadd = 1;
			width -= nletters;
		}
		// Compter le nombre d'espaces
		for(i = start; i < PA_LetterPos.NLetters-1; i++) if(PA_LetterPos.Letter[i].Letter == ' ') spaces++;
		
		for(i = start; i < PA_LetterPos.NLetters; i++) {
			justify+= quickadd;
		   PA_LetterPos.Letter[i].X += justify;
		   if((PA_LetterPos.Letter[i].Letter == ' ')&&width){
		      change = (width+(spaces>>1))/spaces;
				justify+=change;		 
				width-=change; // Moins à corriger par la suite...  
				spaces--;   
		   }   
		}   
	}    
		
}   


void PA_DoLetterPos(s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u16 color, u8 size, s32 limit){
	s16 x = basex; s16 y = basey;
	s16 width, letterstart;
	u8 lx, letter = ' ';
	s16 i;
	
	PA_LetterPos.NLetters = 0;
	letterstart = 0;
	
	if((textinfo.align == ALIGN_JUSTIFY)||(textinfo.align == ALIGN_LEFT)) limit += 100; // Pour bien caler la ligne
	
	for(i = 0; (i < limit+100)&&(text[i]); i++){
	   letter = text[i]; 
	   lx = pa_bittextdefaultsize[size][letter]+textinfo.letterspacing;
	   
	   if((letter == '\n') || ((x+lx > maxx)&&(letter != ' '))){ // retour ligne, ou on dépasse...
	      	
			
			width = x - basex; // LARGEUR !

      if(letter == '\n')PA_AddLetterPos(letter, x, y, size, color); // Retour ligne à mettre dans le tableau
      else if(letter == ' ') PA_AddLetterPos(letter, x, y, size, color); // Si espace, on met à la suite
      else {
         // Sinon, on revient en arrière pour pas couper un mot !
         while(text[i-1] != ' ') { i--; x -= pa_bittextdefaultsize[size][(u8)text[i]]+textinfo.letterspacing; PA_LetterPos.NLetters--;} // Retirer les lettres et la largeur
		   letter = text[i]; 
		   lx = pa_bittextdefaultsize[size][letter]+textinfo.letterspacing;	 
			x--; // Moins de largeur        
      }  
			  
  	   if(y+pa_bittextpoliceheight[size] * 2 > maxy) {
		        break; // EXIT !	
			}   
	 
	      // Calculer différence de largeur...
			if(text[i-1] == ' ') x-= pa_bittextdefaultsize[size][(u8)(' ')]; // Retirer la largeur de l'espace	  
		
				 	      
	      // Retour ligne ! Voir si besoin d'aligne le texte d'un côté ou de l'autre, centrer, etc...
			PA_DoAlign(letterstart, x, maxx, (letter != '\n')); // Autorise le justify que si pas '\'n
     
	      x = basex;
	      y += pa_bittextpoliceheight[size]+textinfo.linespacing;
	      
	      letterstart = PA_LetterPos.NLetters; // Première lettre de cette ligne !
	      if(letter > 32) {
			   PA_AddLetterPos(letter, x, y, size, color);
			   x += lx;   	         
	      }   	      

		   
	   }   
	   else{
		   PA_AddLetterPos(letter, x, y, size, color);
		   x += lx;   
		}		
	}   
	if(text[i-1] == ' ') x-= pa_bittextdefaultsize[size][(u8)(' ')]; // Retirer la largeur de l'espace	 
	PA_DoAlign(letterstart, x, maxx, (letter != '\n') && (text[i] != '\0'));

	// End Position
	PA_LetterPos.Letter[PA_LetterPos.NLetters].X = x;
	PA_LetterPos.Letter[PA_LetterPos.NLetters].Y = y;

}











#ifdef __cplusplus
}
#endif



