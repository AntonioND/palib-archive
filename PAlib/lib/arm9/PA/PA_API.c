

#ifdef __cplusplus
extern "C" {
#endif

#include "arm9/PA_API.h"
#include "arm9/PA_Keys.h"
#include "arm9/PA_Text.h"

#include "API/PAPI.c" // Toutes les images de l'PAPI...
#include "API/PAPI.h"




u8 *PAPI_Graphics[2];
void *PAPI_Pal[2];

PAPI_checks PAPI_check[2][32]; // 32 possibles par écran
PAPI_all PAPI_checkinfo[2]; // Nombre de Checks, premier, et dernier


PAPI_buttons PAPI_button[2][64]; //64 possibles par écran
PAPI_all PAPI_buttoninfo[2]; // Nombre de boutons, premier, et dernier

void PAPI_DrawButton(u8 screen, s16 n, u8 value);
u8 PAPI_buttonpressed[2] = {0, 0}; // Pas de bouton pressé par défaut


PAPI_sliders PAPI_slider[2][32];
PAPI_all PAPI_sliderinfo[2]; // Nombre de boutons, premier, et dernier






void PAPI_Init(u8 screen){
PAPI_SetImages(screen, (void*)PAPIData, (void*)PAPIPal);
PAPI_ReInit(screen);
}


void PAPI_SetImages(u8 screen, void *images, void *palette){
// Images générales
PAPI_Graphics[screen] = (u8*)images;

// Images pour les boutons
PAPI_buttoninfo[screen].image[0] = (u8*)(PAPI_Graphics[screen]);
PAPI_buttoninfo[screen].image[1] = (u8*)(PAPI_Graphics[screen] + 64);

// Images pour les checks
PAPI_checkinfo[screen].image[0] = (u8*)(PAPI_Graphics[screen] + 128);
PAPI_checkinfo[screen].image[1] = (u8*)(PAPI_Graphics[screen] + 192);

PAPI_Pal[screen] = palette;
}


void PAPI_ReInit(u8 screen){
u16 j;

PAPI_LoadPal(screen);

// Initialisation des Checks :
PAPI_checkinfo[screen].n = 0; // Par défaut aucun
PAPI_checkinfo[screen].first = 0; // Aucun...
PAPI_checkinfo[screen].last = 0; 
for (j = 0; j < 32; j++) PAPI_checkinfo[screen].free[j] = j; // On place les libres...

// Initialisation des boutons :
PAPI_buttonpressed[screen] = 0; // Pas de bouton pressé
PAPI_buttoninfo[screen].n = 0; // Par défaut aucun
PAPI_buttoninfo[screen].first = 0; // Aucun...
PAPI_buttoninfo[screen].last = 0; 
for (j = 0; j < 64; j++) PAPI_buttoninfo[screen].free[j] = j; // On place les libres...	

// Initialisation des sliders :
PAPI_sliderinfo[screen].n = 0; // Par défaut aucun
PAPI_sliderinfo[screen].first = 0; // Aucun...
PAPI_sliderinfo[screen].last = 0; 
for (j = 0; j < 32; j++) PAPI_sliderinfo[screen].free[j] = j; // On place les libres...	

}


void PAPI_LoadPal(u8 screen){
	PA_LoadPal16((PAL_BG0 + (512*screen)), 13, PAPI_Pal[screen]);
}




u8 PAPI_CreateButton(u8 screen, s16 x, s16 y, s16 lx, s16 ly, fp funct, char* text, u8 color, s8 textsize){
	u8 nbutton; // Numéro que l'on va avoir pour ce bouton...
	s16 i;
	
	nbutton = PAPI_buttoninfo[screen].free[PAPI_buttoninfo[screen].n]; // Premier des dispos
	
	if(PAPI_buttoninfo[screen].n == 0){ // Premier !
		PAPI_button[screen][nbutton].previous = -1; // Aucun avant lui...
		PAPI_buttoninfo[screen].first = nbutton; // Premier
	}
	else {
		PAPI_button[screen][nbutton].previous = PAPI_buttoninfo[screen].last;
		PAPI_button[screen][nbutton].next = -1; // Aucun apres...
		PAPI_button[screen][PAPI_buttoninfo[screen].last].next = nbutton;
	}
	
	
	PAPI_buttoninfo[screen].last = nbutton; // C'est le dernier pour l'instant
	
	PAPI_buttoninfo[screen].n++;
	
	PAPI_button[screen][nbutton].x = x; PAPI_button[screen][nbutton].y = y;
	PAPI_button[screen][nbutton].lx = lx; PAPI_button[screen][nbutton].ly = ly;
	PAPI_button[screen][nbutton].funct = funct;
	PAPI_button[screen][nbutton].color = color;
	PAPI_button[screen][nbutton].textsize = textsize;
	
	for (i = 0; text[i]; i++) PAPI_button[screen][nbutton].text[i] = text[i];
	PAPI_button[screen][nbutton].text[i] = 0; // Signal la fin
	
	PAPI_DrawButton(screen, nbutton, 0);
	
	return nbutton;

}





void PAPI_CheckButton(void){
	u8 i;
	s16 x, y, lx, ly;
	u8 screen = PA_Screen;
	u8 n; // N en cours
	
	if (PAPI_buttoninfo[screen].n) { // Uniquement si on a des boutons affichés
		if(Stylus.Newpress){ // Que si nouvelle pression
			n = PAPI_buttoninfo[screen].first;
			for (i = 0; i < PAPI_buttoninfo[screen].n; i++){
				// Si on touche...
				x = PAPI_button[screen][n].x; y = PAPI_button[screen][n].y;
				lx = PAPI_button[screen][n].lx; ly = PAPI_button[screen][n].ly;
					
				if (PA_StylusInZone(x, y, x + lx, y + ly)){
					PAPI_button[screen][n].funct();
					PAPI_DrawButton(screen, n, 1);
					PAPI_buttoninfo[screen].touched = n;
					PAPI_buttonpressed[screen] = 1;
				}
				
				//PA_OutputText(1, 25, i, "%d  ", n);
				n = PAPI_button[screen][n].next;
			}
		}
		else {
			n = PAPI_buttoninfo[screen].touched;
			
			if(Stylus.Released && PAPI_buttonpressed[screen]){ // Si on relève et 1 bouton appuyé
				x = PAPI_button[screen][n].x; y = PAPI_button[screen][n].y;
				lx = PAPI_button[screen][n].lx; ly = PAPI_button[screen][n].ly;
		
				PAPI_DrawButton(screen, n, 0);
				PAPI_buttoninfo[screen].touched = n;
				PAPI_buttonpressed[screen] = 0;			
			}
		}
	}


}






void PAPI_DrawButton(u8 screen, s16 n, u8 value){
	s16 i, j;
	u8* data = (u8*)(PAPI_buttoninfo[screen].image[value]);
	s16 tempx, tempy;
	s16 x, y, lx, ly;
	
	
	x = PAPI_button[screen][n].x; y = PAPI_button[screen][n].y;
	lx = PAPI_button[screen][n].lx; ly = PAPI_button[screen][n].ly;
	s16 color = PAPI_button[screen][n].color;
	tempx = x + lx - 3;
	tempy = y + ly - 3;
	
	for (i = 0; i < 3; i++){
		for (j = 0; j < 3; j++){
			PA_Put8bitPixel(screen, i + x, j + y, data[i + (j << 3)]);
			PA_Put8bitPixel(screen, i + tempx, j + y, data[i + 5 + (j << 3)]);		
			PA_Put8bitPixel(screen, i + x, j + tempy, data[i + ((j + 5) << 3)]);
			PA_Put8bitPixel(screen, i + tempx, j + tempy, data[i + 5 + ((j + 5) << 3)]);
		}
	}
	
	tempx = x + 3;
	for (i = 0; i < (lx - 6); i++) {
		PA_Put8bitPixel(screen, i + tempx, y, data[4]);
		PA_Put8bitPixel(screen, i + tempx, y+1, data[12]);	
		PA_Put8bitPixel(screen, i + tempx, y+2, data[20]);	
		PA_Put8bitPixel(screen, i + tempx, y+ly - 3, data[44]);
		PA_Put8bitPixel(screen, i + tempx, y+ly - 2, data[52]);	
		PA_Put8bitPixel(screen, i + tempx, y+ly - 1, data[60]);		
	}
	
	tempy = y + 3;
	for (j = 0; j < (ly - 6); j++) {
		PA_Put8bitPixel(screen, x, j + tempy, data[24]);
		PA_Put8bitPixel(screen, x+1, j + tempy, data[25]);	
		PA_Put8bitPixel(screen, x+2, j + tempy, data[26]);	
		PA_Put8bitPixel(screen, x+lx-3, j + tempy, data[29]);
		PA_Put8bitPixel(screen, x+lx-2, j + tempy, data[30]);	
		PA_Put8bitPixel(screen, x+lx-1, j + tempy, data[31]);		
	}
	
	i = 0;
	u16 colors = data[28] + (data[28] << 8);
	
	if ((i + tempx)&1){ // Premier pixel à mettre tout seul peut-etre
		for (j = 0; j < (ly - 6); j++) PA_Put8bitPixel(screen, i + tempx, j + tempy, data[28]);
		i++;
	}
	
	// Tant qu'il reste au moins 2 pixels à mettre, on fait 2 par 2
	while((i < (lx - 6)-1)){
		for (j = 0; j < (ly - 6); j++) {
			PA_Put2_8bitPixels(screen, i + tempx, j + tempy, colors);
		}
		i+=2;
	}
	
	if (!((i + tempx)&1)){ // Dernier pixel à mettre tout seul peut-etre
		for (j = 0; j < (ly - 6); j++) PA_Put8bitPixel(screen, i + tempx, j + tempy, data[28]);
		i++;
	}
	
	
	PA_CenterSmartText(screen, x, y, x+lx, y+ly, PAPI_button[screen][n].text, color, PAPI_button[screen][n].textsize, 1);


}



/*


u8 PAPI_CreateSlider(u8 screen, s16 x, s16 y, s16 lx, s32* var, s32 min, s32 max, u8 sens){
u8 nslider; // Numéro que l'on va avoir pour ce slider...


nslider = PAPI_sliderinfo[screen].free[PAPI_sliderinfo[screen].n]; // Premier des dispos

if(PAPI_sliderinfo[screen].n == 0){ // Premier !
	PAPI_slider[screen][nslider].previous = -1; // Aucun avant lui...
	PAPI_sliderinfo[screen].first = nslider; // Premier
}
else {
	PAPI_slider[screen][nslider].previous = PAPI_sliderinfo[screen].last;
	PAPI_slider[screen][nslider].next = -1; // Aucun apres...
	PAPI_slider[screen][PAPI_sliderinfo[screen].last].next = nslider;
}


PAPI_sliderinfo[screen].last = nslider; // C'est le dernier pour l'instant

PAPI_sliderinfo[screen].n++;

PAPI_slider[screen][nslider].lx = lx;
PAPI_slider[screen][nslider].var = var;
PAPI_slider[screen][nslider].min = min;  PAPI_slider[screen][nslider].max = max;
PAPI_slider[screen][nslider].sens = sens;





s16 i, j;

s16 paltemp = (PAPI_SPRITEPAL << 4);
u16 *palpt = (u16*)sliderPal;

	for (i = 0; i < 16; i++) BG_PALETTE[i + 256 + paltemp + (screen * 512)] = palpt[i];



u8* data;



if (sens == 0){ // Horizontal
	data = (u8*)sliderbarData;
	for (i = 0; i < 3; i++) for (j = 0; j < 5; j++) {
		PA_Put8bitPixel(screen, i + x, y + j, data[i + (j << 3)]);
		PA_Put8bitPixel(screen, i + x + lx - 3, y+j, data[i+5 + (j << 3)]);
	}
	
	
	for (i = 3; i < (lx - 3); i++){
		for (j = 0; j < 5; j++){
			PA_Put8bitPixel(screen, i + x, y + j, data[3 + (j << 3)]);
		}
	}
	
	PAPI_slider[screen][nslider].x = x; PAPI_slider[screen][nslider].y = y +2;
	// On fait le sprite
	PA_CreateSpriteFromGfx(screen, SLIDER_SPRITE + nslider, slidergfx[screen][sens][0], OBJ_SIZE_8X16, 0, PAPI_SPRITEPAL, x - 4, y - 6);	

}
else if (sens == 1){ // Horizontal
	data = (u8*)(sliderbarData + 16);
	for (j = 0; j < 3; j++) for (i = 0; i < 5; i++) {
		PA_Put8bitPixel(screen, i + x, y + j, data[i + (j << 3)]);
		PA_Put8bitPixel(screen, i + x, y+j + lx - 3, data[i + ((j+5) << 3)]);
	}
	
	for (j = 3; j < (lx - 3); j++){
		for (i = 0; i < 5; i++){
			PA_Put8bitPixel(screen, i + x, y + j, data[i + (3 << 3)]);
		}
	}

	PAPI_slider[screen][nslider].x = x+2; PAPI_slider[screen][nslider].y = y;	
	// On fait le sprite
	PA_CreateSpriteFromGfx(screen, SLIDER_SPRITE + nslider, slidergfx[screen][sens][0], OBJ_SIZE_16X8, 0, PAPI_SPRITEPAL, x - 6, y - 4);	
	
}



return nslider;

}


void ChangeSlider(u8 screen, s8 nslider, s32 min, s32 max){

PAPI_slider[screen][nslider].min = min;  PAPI_slider[screen][nslider].max = max;



}


void PAPI_CheckSlider(void){
u8 i;
u8 screen = PA_Screen;
u8 n; // N en cours
//s16 x, y;

u8 sens;

	n = PAPI_sliderinfo[screen].first;
	for (i = 0; i < PAPI_sliderinfo[screen].n; i++){
		sens = PAPI_slider[screen][n].sens;
		s16 x = PAPI_slider[screen][n].x; s16 y = PAPI_slider[screen][n].y; 
		s16 lx = PAPI_slider[screen][n].lx;
		s16 min = PAPI_slider[screen][n].min; s16 max = PAPI_slider[screen][n].max; 
		s16 value; // Valeur à afficher dans la variable
		
		if (PA_MoveSpriteEx(screen, SLIDER_SPRITE + n, 16, 16)) { // Si ce slider est bougé...
			

			if (sens == 0) { // Horiz
				// Y ne bouge pas...
				PA_SetSpriteY(screen, SLIDER_SPRITE + n, y - 8);
				PA_MovedSprite.Y = y;			
				if(PA_MovedSprite.X < x) {
					PA_SetSpriteX(screen, SLIDER_SPRITE + n, x - 4);
					PA_MovedSprite.X = x;
					value = min;
				}
				else if(PA_MovedSprite.X >= x + lx) {
					PA_SetSpriteX(screen, SLIDER_SPRITE + n, x + lx - 5);
					PA_MovedSprite.X = x + lx - 1;
					value = max;
				}	
				else {
					value = PA_MovedSprite.X - x; 
					value = ((value * (max - min))/ lx) + min;
				}
			}
			else  { // Verti
				// X ne bouge pas...
				PA_SetSpriteX(screen, SLIDER_SPRITE + n, x - 8);
				PA_MovedSprite.X = x;			
				if(PA_MovedSprite.Y < y - 4) {
					PA_SetSpriteY(screen, SLIDER_SPRITE + n, y - 4);
					PA_MovedSprite.Y = y;
					value = min;
				}
				else if(PA_MovedSprite.Y >= y + lx - 4) {
					PA_SetSpriteY(screen, SLIDER_SPRITE + n, y + lx - 5);
					PA_MovedSprite.Y = y + lx -5;
					value = max;
				}	
				else {
					value = PA_MovedSprite.Y - (y - 4);
					value = ((value * (max - min))/ lx) + min;
				}
			}		
			
			*PAPI_slider[screen][n].var = value; // Nouvelle valeur
			PA_SetSpriteGfx(screen, SLIDER_SPRITE + n, slidergfx[screen][sens][1]);
			
		}
		else PA_SetSpriteGfx(screen, SLIDER_SPRITE + n, slidergfx[screen][sens][0]);
		
	 // On ne le déplace pas, donc on le met à l'emplacement qui correspond à sa valeur...
		value =	*PAPI_slider[screen][n].var;
		value -= min;
		value *= lx;
		value /= (max - min);

		if (sens == 0) {
			value += x;			
			PA_SetSpriteX(screen, SLIDER_SPRITE + n, value - 4);
		}
		else if (sens == 1) {
			value += y;	
			PA_SetSpriteY(screen, SLIDER_SPRITE + n, value - 4);	
		}
		
		
		
		
		
		n = PAPI_slider[screen][n].next;
	}



}





*/















u8 PAPI_CreateCheck(u8 screen, s16 x, s16 y, u8 *var){
u8 ncheck; // Numéro que l'on va avoir pour ce Check...


PA_8bit8x8Image(screen, x, y, (u8*)PAPI_checkinfo[screen].image[*var]);

ncheck = PAPI_checkinfo[screen].free[PAPI_checkinfo[screen].n]; // Premier des dispos

if(PAPI_checkinfo[screen].n == 0){ // Premier !
	PAPI_check[screen][ncheck].previous = -1; // Aucun avant lui...
	PAPI_checkinfo[screen].first = ncheck; // Premier
}
else {
	PAPI_check[screen][ncheck].previous = PAPI_checkinfo[screen].last;
	PAPI_check[screen][ncheck].next = -1; // Aucun apres...
	PAPI_check[screen][PAPI_checkinfo[screen].last].next = ncheck;
}

PAPI_checkinfo[screen].last = ncheck; // C'est le dernier pour l'instant

PAPI_checkinfo[screen].n++;

PAPI_check[screen][ncheck].x = x; PAPI_check[screen][ncheck].y = y;
PAPI_check[screen][ncheck].var = var;

return ncheck;

}





void PAPI_CheckCheck(void){
u8 i;
u8 screen = PA_Screen;
u8 n; // N en cours
s16 x, y;


if(Stylus.Newpress){ // Que si nouvelle pression

	n = PAPI_checkinfo[screen].first;
	for (i = 0; i < PAPI_checkinfo[screen].n; i++){
		// Si on touche...
		x = PAPI_check[screen][n].x;
		y = PAPI_check[screen][n].y;
			
		if (PA_StylusInZone(x, y, x + 8, y + 8)){
			u8 temp = *PAPI_check[screen][n].var = !(*PAPI_check[screen][n].var);
			PA_8bit8x8Image(screen, x, y, (u8*)PAPI_checkinfo[screen].image[temp]);
		}
		
		n = PAPI_check[screen][n].next;
	}
}



}



#ifdef __cplusplus
}
#endif

