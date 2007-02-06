
#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"





void PA_EraseTextBox(u8 screen){
s16 i, j;
	for (j = PA_TextBox[screen].y1+1; j < PA_TextBox[screen].y2; j++)
		for (i = PA_TextBox[screen].x1+1; i < PA_TextBox[screen].x2; i++)
			PA_SetTileLetter(screen, i, j, ' ');
}

void PA_InitTextBorders(u8 screen, u8 x1, u8 y1, u8 x2, u8 y2){

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

















u32 PA_BoxTextNoWrap(u8 screen, u16 basex, u16 basey, u16 maxx, u16 maxy, const char *text, u32 limit) {
u16 x = basex;
u16 y = basey;
u16 j;
u16 textcount = 0; // compte le nombre de lettres...
u8 loop = 1; // On continue...
   
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







u32 PA_BoxText(u8 screen, u16 basex, u16 basey, u16 maxx, u16 maxy, const char *text, s32 limit){
s16 i, j;
s16 x, y;

s16 letter; 


x = basex;
y = basey;
u16 ylimit = maxy;

s16 length = 0;

s16 wordx = 0; // Longueur de mot
s16 wordletter = 0;

//BG_PALETTE[255 + (screen * 512)] = textcol[screen]; // On remet la couleur au cas où on ait chargé du texte par-dessus...

//u8 loop = 1; // On continue...
   u8 textcolor = PAtext_pal[screen]; // save the screen color
   
  // u8 goon = 1;
   u8 temp = 1;
   
for (i = 0; (text[i] && (y <= maxy) && (i < limit)&&temp); i++) {
//PA_OutputText(0, x*4, y*4, "%d ", text[i]);
    if (text[i] == '%') {
		if (text[i+1] == 'c'){ // change color !
			PA_SetTextTileCol(screen, text[i+2]-'0');
			i+=2;
			limit+=3; // Don't count it in the limit
			length-=3; // Don't count them in the length
			//PA_OutputText(0, 0, 0, "%d  ", length);
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
		//PA_OutputText(0, 0, 0, "%d ", temp);
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
					for (j = i; (j < (i + wordletter))&&(j < limit)&&(text[j]!=0); j++) {
						PA_SetTileLetter(screen, x, y, text[j]);
						/*goon = goon && text[j];
						if (goon == 0) temp = 0;
						PA_OutputText(0, x*4, y*4, "%d ", goon);*/
						
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
			
			for (j = i; (j < jmax)&&(j < limit)&&(text[j]!=0); j++) {
				PA_SetTileLetter(screen, x, y, text[j]);
						/*goon = goon && text[j];
						if (goon == 0) temp = 0;
						PA_OutputText(0, x*4, y*4, "%d ", goon);	*/
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







char *selectchar(char *text,int begin, int end){
char *text2 = (char*)malloc(sizeof(char)*(end-begin));
int i;
for(i=begin;i<end;i++)text2[i-begin]=text[i];
text2[i-begin+1]='\0';
return text2;
}
//defilement
void PA_OutputTextSpecial0(u8 screen, int x1, int y,char *text){
int i,k,x2=strlen(text)+x1+1;
for(i=x2;i>x1;i--){
PA_OutputSimpleText(screen, i, y, selectchar(text,0,x2-x1-1));
for(k=0;k<30;k++)PA_WaitForVBL();
PA_OutputSimpleText(screen, i+strlen(selectchar(text,0,x2-x1-1))-1, y, " ");
}
}
//separation
void PA_OutputTextSpecial1(u8 screen, int x1, int y,char *text){
int i,k,x2=strlen(text)+x1+1;
for(i=x2;i>x1;i--){
PA_OutputSimpleText(screen, i+strlen(selectchar(text,0,i-x1-1)), y, " ");
PA_OutputSimpleText(screen, i, y, selectchar(text,0,i-x1-1));
for(k=0;k<30;k++)PA_WaitForVBL();
}
}
//etirement
void PA_OutputTextSpecial2(u8 screen, int x1, int y,char *text){
int i,k,x2=strlen(text)+x1;
for(i=x2;i>x1;i--){
PA_OutputSimpleText(screen, i, y, selectchar(text,0,i-x1));
for(k=0;k<30;k++)PA_WaitForVBL();
}
}
//etirement + suppression de la fin
void PA_OutputTextSpecial3(u8 screen, int x1, int y,char *text){
int i,k,x2=strlen(text)+x1+1;
for(i=x2;i>x1;i--){
PA_OutputSimpleText(screen, x2+strlen(selectchar(text,0,i-x1-1))+1, y, " ");
PA_OutputSimpleText(screen, i, y, selectchar(text,0,i-x1-1));
for(k=0;k<30;k++)PA_WaitForVBL();
}
}
//etirement + separation
void PA_OutputTextSpecial4(u8 screen, int x1, int y,char *text){
int i,k,x2=strlen(text)+x1+1;
for(i=x2;i>x1;i--){
PA_OutputSimpleText(screen, i+strlen(selectchar(text,0,i-x1)), y, " ");
PA_OutputSimpleText(screen, i, y, selectchar(text,0,i-x1-1));
for(k=0;k<30;k++)PA_WaitForVBL();
}
}
//defilement + suppression
void PA_OutputTextSpecial5(u8 screen, int x1, int y,char *text){
int i,k,x2=strlen(text)+x1+1;
for(i=x2;i>x1;i--){
PA_OutputSimpleText(screen, i, y, selectchar(text,0,i-x1-1));
for(k=0;k<30;k++)PA_WaitForVBL();
PA_OutputSimpleText(screen, i+strlen(selectchar(text,0,i-x1-1))-2, y, "   ");
}
}







#ifdef __cplusplus
}
#endif









