
#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"





void PA_PrintYPlus(u8 screen){
	u8 i;
	for(i = pa_printx[screen]; i < 32; i++) PA_SetTileLetter(screen, pa_printx[screen], pa_printy[screen]&31, ' '); // Erase
   pa_printy[screen]++;
   pa_printx[screen] = 0;
	if(pa_printy[screen] > 24) {
		pa_printscroll[screen] += 8;
		PA_BGScrollY(screen, PAbgtext[screen], pa_printscroll[screen]);
	}     
}   


void PA_Print(u8 screen, char* text, ...) {
	s16 j, i;

	u8 PAtext[32]; // tableau où l'on copie les nombres et tout...
	u8 *PAextext; // Extra text
	s32 PAtextnumber;
	double tempdouble = 0;
	     
	//BG_PALETTE[255 + (screen * 512)] = textcol[screen]; // On remet la couleur au cas où on ait chargé du texte par-dessus...
	
	va_list varg;           /* Variable identifiant le prochain paramètre. */

   va_start(varg, text);

   
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
               PA_PrintLetter(screen, PAextext[i]);
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
				   if(temp >= 10) PA_PrintLetter(screen, 'A'+(temp-10));
				   else PA_PrintLetter(screen, '0'+(temp));
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
               PA_PrintLetter(screen, PAtext[i]);
            }
            j += 1;
            
            if (text[j] == 'f') { // Il reste donc la virgule à traiter...
               PA_PrintLetter(screen, '.');
               
               i = text[j+1] - 48;  // Nombre de chiffres après la virgule...
               PAtext[1] = 0; // Permet de stopper si c'est vba...
			   if (tempdouble < 0) tempdouble = -tempdouble;
               while((i-- > 0)) { // Pas plus de 6 chiffres après la virgule...
                  tempdouble *= 10;
				  //if (
                  PAtextnumber = (s32)tempdouble;
                  tempdouble -= PAtextnumber;
                  PAtext[0] = '0' + PAtextnumber;
                  PA_PrintLetter(screen, PAtext[0]);
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
				PA_PrintLetter(screen, amettre);
				--nfois;
			}
			
            if (neg) {
               PAtext[i] = '-';
               ++i;
            }
            for (i--; i > -1; i--) {
               PA_PrintLetter(screen, PAtext[i]);
            }
            j += 3;
		  
		  }
          else { // Sinon c'est juste
	          PA_PrintLetter(screen, text[j]);
          }
      }
	  else if (text[j] == '\n'){
         PA_PrintYPlus(screen);
	  }
      else if ((text[j] == '/') && (text[j+1] == 'n')) {
         PA_PrintYPlus(screen);
		 	j++;
      }
      else{
         PA_PrintLetter(screen, text[j]);
      }
   }

    va_end(varg);           /* Terminaison. */

   PA_SetTextTileCol(screen, textcolor); // put back the old color
}




#ifdef __cplusplus
}
#endif









