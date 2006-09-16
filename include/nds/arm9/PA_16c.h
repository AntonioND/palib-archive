#ifndef _PA_16C
#define _PA_16C

#ifdef __cplusplus
extern "C" {
#endif

/*! \file PA_16c.h
    \brief 16color pseudo-bitmap mode. 

    Special 16color background on which you can paste images. Usefull to show shots in SHMUP !
*/

#include "PA9.h"

#include "PA_Palette.h"


// Pointers...
extern u16 *PA_Draw16[2];
extern u32 *PA_Draw1632[2];

extern u32 Blank[130000>>2];


#define PA_LoadPal16c(palette, source)   DMA_Copy(source, (void*)palette, 16, DMA_16NOW);
#define PA_16cPos(x, y) (((x>>3)*26*8) + y + 8)

	extern const u8 c16_policeheight[5];
	extern const u8 c16_policesize[5][96];
	extern u32 *c16_font[5];

ALWAYSINLINE void PA_Plot8Pixels(u8 screen, u16 pos, u16 temp, u32 color){
   PA_Draw1632[screen][pos] |= (color<<temp);
   PA_Draw1632[screen][pos+(26*8)] |= (color>>(32-temp));   
}  

ALWAYSINLINE void PA_16c16X16Letter(u8 screen, s16 x, s16 y, u32 *image, u8 color){  
   
   x += 8; y += 8;

   u16 temp = (x&7)<<2;
   u16 pos = PA_16cPos(x, y);
   u8 i;
   for (i = 0; i < 2; i++){
	   PA_Plot8Pixels(screen, pos, temp, image[0]*color);
	   PA_Plot8Pixels(screen, pos+1, temp, image[1]*color);
	   PA_Plot8Pixels(screen, pos+2, temp, image[2]*color);
	   PA_Plot8Pixels(screen, pos+3, temp, image[3]*color);
	   PA_Plot8Pixels(screen, pos+4, temp, image[4]*color);
	   PA_Plot8Pixels(screen, pos+5, temp, image[5]*color);
	   PA_Plot8Pixels(screen, pos+6, temp, image[6]*color); 
	   PA_Plot8Pixels(screen, pos+7, temp, image[7]*color);
	   PA_Plot8Pixels(screen, pos+8, temp, image[16]*color);
	   PA_Plot8Pixels(screen, pos+9, temp, image[17]*color); 
	   PA_Plot8Pixels(screen, pos+10, temp, image[18]*color);  
	   PA_Plot8Pixels(screen, pos+11, temp, image[19]*color);
	   PA_Plot8Pixels(screen, pos+12, temp, image[20]*color);
	   PA_Plot8Pixels(screen, pos+13, temp, image[21]*color);
	   PA_Plot8Pixels(screen, pos+14, temp, image[22]*color);
	   PA_Plot8Pixels(screen, pos+15, temp, image[23]*color);		   
	   pos+=26*8;
	   image+=8;	   
	}
}  
ALWAYSINLINE void PA_16c8X8Letter(u8 screen, s16 x, s16 y, u32 *image, u8 color){  
   x += 8; y += 8;
   u16 temp = (x&7)<<2;
   u16 pos = PA_16cPos(x, y);
   PA_Draw1632[screen][pos] |= (image[0]<<temp)*color;
   PA_Draw1632[screen][pos+1] |= (image[1]<<temp)*color;
   PA_Draw1632[screen][pos+2] |= (image[2]<<temp)*color;
   PA_Draw1632[screen][pos+3] |= (image[3]<<temp)*color;
   PA_Draw1632[screen][pos+4] |= (image[4]<<temp)*color;   
   PA_Draw1632[screen][pos+5] |= (image[5]<<temp)*color;
   PA_Draw1632[screen][pos+6] |= (image[6]<<temp)*color;
   PA_Draw1632[screen][pos+7] |= (image[7]<<temp)*color;  
   pos += (26*8); // Next columns
   temp = 32-temp;
   PA_Draw1632[screen][pos] |= (image[0]>>temp)*color;
   PA_Draw1632[screen][pos+1] |= (image[1]>>temp)*color;
   PA_Draw1632[screen][pos+2] |= (image[2]>>temp)*color;
   PA_Draw1632[screen][pos+3] |= (image[3]>>temp)*color;
   PA_Draw1632[screen][pos+4] |= (image[4]>>temp)*color;   
   PA_Draw1632[screen][pos+5] |= (image[5]>>temp)*color;
   PA_Draw1632[screen][pos+6] |= (image[6]>>temp)*color;
   PA_Draw1632[screen][pos+7] |= (image[7]>>temp)*color;      
}  


/** @defgroup 16c 16color pseudo-bitmap mode. 
 *  Special 16color background on which you can paste images. Usefull to show shots in SHMUP !
 *  @{
 */



/*! \fn void PA_Init16cBg(u8 screen, u8 bg)
    \brief
         \~english Initialise 16color background on which you can paste images...
         \~french Initialise le mode de dessin 16 couleurs, sur lequel on peut coller de petites images...
    \param screen
         \~english Choose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg
         \~english Background number (0-3)
         \~english Background number (0-3) 
*/
void PA_Init16cBgEx(u8 screen, u8 bg, u8 npalette);



/*! \fn extern inline void PA_Init16cBg(u8 screen, u8 bg)
    \brief
         \~english Initialise 16color background on which you can paste images... Using palette 0
         \~french Initialise le mode de dessin 16 couleurs, sur lequel on peut coller de petites images... Utilisant la palette 0
    \param screen
         \~english Choose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg
         \~english Background number (0-3)
         \~english Background number (0-3) 
*/
extern inline void PA_Init16cBg(u8 screen, u8 bg){
	PA_Init16cBgEx(screen, bg, 0);
}





/*! \fn extern inline void PA_16cErase(u8 screen)
    \brief
         \~english Erase the 16color background. Must be used right after PA_WaitForVBL to avoid glitches
         \~french Effacer un écran de 16 couleurs. Doit être utilisé juste après PA_WaitForVBL pour éviter des erreurs d'affichage
    \param screen
         \~english Choose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
*/
extern inline void PA_16cErase(u8 screen){
	DMA_Copy(Blank, PA_Draw1632[screen], 26*8*34, DMA_32NOW)
}



/*! \fn extern inline void PA_Dual16cErase(void)
    \brief
         \~english Erase the 16color background on both screens. Must be used right after PA_WaitForVBL to avoid glitches
         \~french Effacer un écran de 16 couleurs sur les 2 écrans. Doit être utilisé juste après PA_WaitForVBL pour éviter des erreurs d'affichage
*/
extern inline void PA_Dual16cErase(void){
	PA_16cErase(0); PA_16cErase(1);
}


/*! \fn extern inline void PA_InitComplete16c(u8 bg, void *Palette)
    \brief
         \~english Initialise a 16color background on each screen and give them a given palette
         \~french Initialise le mode de dessin 16 couleurs sur les 2 écrans avec une palette donnée
    \param bg
         \~english Background number
         \~french Numéro du fond
    \param Palette
         \~english 16 color palette...
         \~french Palette de 16 couleurs 
*/
extern inline void PA_InitComplete16c(u8 bg, void *Palette){
	PA_LoadPal16c(PAL_BG0, Palette);
	PA_LoadPal16c(PAL_BG1, Palette);	
	PA_Init16cBg(0, bg); PA_Init16cBg(1, bg);
}





s16 PA_16cText(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u8 color, u8 size, s32 limit);





/*! \fn ALWAYSINLINE void PA_16c8X4(u8 screen, s16 x, s16 y, u32 *image)
    \brief
         \~english Load an 8x4 pixels image at a given position. Fastest of all pasting functions
         \~french Afficher une image de 8x4 pixels à un endroit donné, fonction la plus rapide de copie...
    \param screen
         \~english Screen...
         \~french Ecran...
    \param x
         \~english X position in pixels of the top left corner. Note that it ranges from -8 to 255, in order to allow half-way offscreen images. NEVER DRAW BEYOND THESE LIMITS, or else you'll get major background glitches
         \~french Position X en pixels du coin supérieur gauche. A noter que celle-ci va de -8 à 255, afin de permettre des images à moitié sorties... NE JAMAIS DEPASSER DU CADRE, sous peine de gros bugs graphiques...
    \param y
         \~english y position in pixels of the top left corner. Note that it ranges from -8 to 191, in order to allow half-way offscreen images. NEVER DRAW BEYOND THESE LIMITS, or else you'll get major background glitches
         \~french Position y en pixels du coin supérieur gauche. A noter que celle-ci va de -8 à 191, afin de permettre des images à moitié sorties... NE JAMAIS DEPASSER DU CADRE, sous peine de gros bugs graphiques...
    \param image
         \~english 16 color image to load. Use (u32*)ImageName if you get an error...
         \~french Image en 16 couleurs à charger. Utiliser (u32*)NomImage en cas d'erreur de compilation
*/
ALWAYSINLINE void PA_16c8X4(u8 screen, s16 x, s16 y, u32 *image){  
   x += 8; y += 8;
   u16 temp = (x&7)<<2;
   u16 pos = PA_16cPos(x, y);
   PA_Draw1632[screen][pos] |= (image[0]<<temp);
   PA_Draw1632[screen][pos+1] |= (image[1]<<temp);
   PA_Draw1632[screen][pos+2] |= (image[2]<<temp);
   PA_Draw1632[screen][pos+3] |= (image[3]<<temp);
   pos += (26*8); // Next columns
   temp = 32-temp;
   PA_Draw1632[screen][pos] |= (image[0]>>temp);
   PA_Draw1632[screen][pos+1] |= (image[1]>>temp);
   PA_Draw1632[screen][pos+2] |= (image[2]>>temp);
   PA_Draw1632[screen][pos+3] |= (image[3]>>temp);
}  


/*! \fn ALWAYSINLINE void PA_16c8X6(u8 screen, s16 x, s16 y, u32 *image)
    \brief
         \~english Load an 8x6 pixels image at a given position. Second fastest of all pasting functions
         \~french Afficher une image de 8x6 pixels à un endroit donné, deuxième fonction la plus rapide de copie...
    \param screen
         \~english Screen...
         \~french Ecran...
    \param x
         \~english X position in pixels of the top left corner. Note that it ranges from -8 to 255, in order to allow half-way offscreen images. NEVER DRAW BEYOND THESE LIMITS, or else you'll get major background glitches
         \~french Position X en pixels du coin supérieur gauche. A noter que celle-ci va de -8 à 255, afin de permettre des images à moitié sorties... NE JAMAIS DEPASSER DU CADRE, sous peine de gros bugs graphiques...
    \param y
         \~english y position in pixels of the top left corner. Note that it ranges from -8 to 191, in order to allow half-way offscreen images. NEVER DRAW BEYOND THESE LIMITS, or else you'll get major background glitches
         \~french Position y en pixels du coin supérieur gauche. A noter que celle-ci va de -8 à 191, afin de permettre des images à moitié sorties... NE JAMAIS DEPASSER DU CADRE, sous peine de gros bugs graphiques...
    \param image
         \~english 16 color image to load. Use (u32*)ImageName if you get an error...
         \~french Image en 16 couleurs à charger. Utiliser (u32*)NomImage en cas d'erreur de compilation
*/
ALWAYSINLINE void PA_16c8X6(u8 screen, s16 x, s16 y, u32 *image){  
   
   x += 8; y += 8;
   u16 temp = (x&7)<<2;
   u16 pos = PA_16cPos(x, y);
   PA_Plot8Pixels(screen, pos, temp, image[0]);
   PA_Plot8Pixels(screen, pos+1, temp, image[1]);  
   PA_Plot8Pixels(screen, pos+2, temp, image[2]);   
   PA_Plot8Pixels(screen, pos+3, temp, image[3]); 
   PA_Plot8Pixels(screen, pos+4, temp, image[4]);
   PA_Plot8Pixels(screen, pos+5, temp, image[5]);  
}  


/*! \fn ALWAYSINLINE void PA_16c8X8(u8 screen, s16 x, s16 y, u32 *image)
    \brief
         \~english Load an 8x8 pixels image at a given position. 
         \~french Afficher une image de 8x8 pixels à un endroit donné.
    \param screen
         \~english Screen...
         \~french Ecran...
    \param x
         \~english X position in pixels of the top left corner. Note that it ranges from -8 to 255, in order to allow half-way offscreen images. NEVER DRAW BEYOND THESE LIMITS, or else you'll get major background glitches
         \~french Position X en pixels du coin supérieur gauche. A noter que celle-ci va de -8 à 255, afin de permettre des images à moitié sorties... NE JAMAIS DEPASSER DU CADRE, sous peine de gros bugs graphiques...
    \param y
         \~english y position in pixels of the top left corner. Note that it ranges from -8 to 191, in order to allow half-way offscreen images. NEVER DRAW BEYOND THESE LIMITS, or else you'll get major background glitches
         \~french Position y en pixels du coin supérieur gauche. A noter que celle-ci va de -8 à 191, afin de permettre des images à moitié sorties... NE JAMAIS DEPASSER DU CADRE, sous peine de gros bugs graphiques...
    \param image
         \~english 16 color image to load. Use (u32*)ImageName if you get an error...
         \~french Image en 16 couleurs à charger. Utiliser (u32*)NomImage en cas d'erreur de compilation
*/
ALWAYSINLINE void PA_16c8X8(u8 screen, s16 x, s16 y, u32 *image){  
   x += 8; y += 8;
   u16 temp = (x&7)<<2;
   u16 pos = PA_16cPos(x, y);
   PA_Draw1632[screen][pos] |= (image[0]<<temp);
   PA_Draw1632[screen][pos+1] |= (image[1]<<temp);
   PA_Draw1632[screen][pos+2] |= (image[2]<<temp);
   PA_Draw1632[screen][pos+3] |= (image[3]<<temp);
   PA_Draw1632[screen][pos+4] |= (image[4]<<temp);   
   PA_Draw1632[screen][pos+5] |= (image[5]<<temp);
   PA_Draw1632[screen][pos+6] |= (image[6]<<temp);
   PA_Draw1632[screen][pos+7] |= (image[7]<<temp);  
   pos += (26*8); // Next columns
   temp = 32-temp;
   PA_Draw1632[screen][pos] |= (image[0]>>temp);
   PA_Draw1632[screen][pos+1] |= (image[1]>>temp);
   PA_Draw1632[screen][pos+2] |= (image[2]>>temp);
   PA_Draw1632[screen][pos+3] |= (image[3]>>temp);
   PA_Draw1632[screen][pos+4] |= (image[4]>>temp);   
   PA_Draw1632[screen][pos+5] |= (image[5]>>temp);
   PA_Draw1632[screen][pos+6] |= (image[6]>>temp);
   PA_Draw1632[screen][pos+7] |= (image[7]>>temp);      
}  



ALWAYSINLINE void PA_16c16X8(u8 screen, s16 x, s16 y, u32 *image){  
   x += 8; y += 8;
   u16 temp = (x&7)<<2;
   u16 pos = PA_16cPos(x, y);
   u8 i;
   for (i = 0; i < 2; i++){
	   PA_Plot8Pixels(screen, pos, temp, image[0]);
	   PA_Plot8Pixels(screen, pos+1, temp, image[1]);  
	   PA_Plot8Pixels(screen, pos+2, temp, image[2]);   
	   PA_Plot8Pixels(screen, pos+3, temp, image[3]); 
	   PA_Plot8Pixels(screen, pos+4, temp, image[4]);
	   PA_Plot8Pixels(screen, pos+5, temp, image[5]);  
	   PA_Plot8Pixels(screen, pos+6, temp, image[6]);   
	   PA_Plot8Pixels(screen, pos+7, temp, image[7]); 	
	   pos+=26*8;
	   image+=8;	   
	}
} 



ALWAYSINLINE void PA_16c16X12(u8 screen, s16 x, s16 y, u32 *image){  
   
   x += 8; y += 8;
   u16 temp = (x&7)<<2;
   u16 pos = PA_16cPos(x, y);
   u8 i;
   for (i = 0; i < 2; i++){
	   PA_Plot8Pixels(screen, pos, temp, image[0]);
	   PA_Plot8Pixels(screen, pos+1, temp, image[1]);  
	   PA_Plot8Pixels(screen, pos+2, temp, image[2]);   
	   PA_Plot8Pixels(screen, pos+3, temp, image[3]); 
	   PA_Plot8Pixels(screen, pos+4, temp, image[4]);
	   PA_Plot8Pixels(screen, pos+5, temp, image[5]);  
	   PA_Plot8Pixels(screen, pos+6, temp, image[6]);   
	   PA_Plot8Pixels(screen, pos+7, temp, image[7]); 	
	   PA_Plot8Pixels(screen, pos+8, temp, image[16]);
	   PA_Plot8Pixels(screen, pos+9, temp, image[17]);  
	   PA_Plot8Pixels(screen, pos+10, temp, image[18]);   
	   PA_Plot8Pixels(screen, pos+11, temp, image[19]); 	   
	   pos+=26*8;
	   image+=8;	   
	}
} 

ALWAYSINLINE void PA_16c16X16(u8 screen, s16 x, s16 y, u32 *image){  
   
   x += 8; y += 8;

   u16 temp = (x&7)<<2;
   u16 pos = PA_16cPos(x, y);
   u8 i;
   for (i = 0; i < 2; i++){
	   PA_Plot8Pixels(screen, pos, temp, image[0]);
	   PA_Plot8Pixels(screen, pos+1, temp, image[1]);  
	   PA_Plot8Pixels(screen, pos+2, temp, image[2]);   
	   PA_Plot8Pixels(screen, pos+3, temp, image[3]); 
	   PA_Plot8Pixels(screen, pos+4, temp, image[4]);
	   PA_Plot8Pixels(screen, pos+5, temp, image[5]);  
	   PA_Plot8Pixels(screen, pos+6, temp, image[6]);   
	   PA_Plot8Pixels(screen, pos+7, temp, image[7]); 	
	   PA_Plot8Pixels(screen, pos+8, temp, image[16]);
	   PA_Plot8Pixels(screen, pos+9, temp, image[17]);  
	   PA_Plot8Pixels(screen, pos+10, temp, image[18]);   
	   PA_Plot8Pixels(screen, pos+11, temp, image[19]); 
	   PA_Plot8Pixels(screen, pos+12, temp, image[20]);
	   PA_Plot8Pixels(screen, pos+13, temp, image[21]);  
	   PA_Plot8Pixels(screen, pos+14, temp, image[22]);   
	   PA_Plot8Pixels(screen, pos+15, temp, image[23]); 		   
	   pos+=26*8;
	   image+=8;	   
	}
}  



extern inline void PA_16cLetter(u8 screen, s16 x, s16 y, char letter, u8 size, u8 color){
	if (c16_policeheight[size]<=8) PA_16c8X8Letter(screen, x, y, (u32*)(c16_font[size]+(letter<<3)), color);
	else PA_16c16X16Letter(screen, x, y, (u32*)(c16_font[size]+(letter<<5)), color);
}



/** @} */ // end of 16c




#ifdef __cplusplus
}
#endif








#endif




