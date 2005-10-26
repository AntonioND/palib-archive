#ifndef _PA_Buffer
#define _PA_Buffer



/*! \file PA_Draw.h
    \brief Bitmap mode, for drawing, loading images in 8 or 16 bit mode...

    Draw on screen, either a pixel or a line, or anything ! Load a Bitmap, a Jpeg...
*/

#include "PA_Gif.h"
#include "PA9.h"

#include "PA_Interrupt.h"


#define PA_RGB8(r,g,b)	((((b)>>3)<<10)|(((g)>>3)<<5)|((r)>>3)|(1 << 15))


typedef struct{
	u16 Id; // ?
	u32 Length;
	u16 Nothing1, Nothing2; // ?
	u32 ImageStart; // Offset of start of image
} BMPHeader0;

typedef struct{
	u32 SizeofHeader; // 40
	u32 Width, Height;
	u16 Colorplanes; // Usually 1
	u16 BitsperPixel; //1, 2, 4, 8, 16, 24, 32
	u32 Compression;  // 0 for none, 1...
	u32 SizeofData; // Not reliable
	u32 WidthperMeter, HeightperMeter; // Don't care
	u32 NColors, ImportantColors; // Number of colors used, important colors ?
} BMP_Headers;


extern u16 PA_oldx[2];
extern u16 PA_oldy[2];
extern u8 PA_drawsize[2];
extern u16 *PA_DrawBg[2];
extern u8 PA_nBit[2]; // 8 or 16 bit Bg
//extern PA_SCreen

/** @defgroup Bitmap Bitmap mode, for any screen...
 *  Draw on screen, either a pixel or a line, or anything ! Load a Bitmap, a Jpeg...
 *  @{
 */


/*! \fn void PA_Init8bitBg(bool screen, u8 bg_priority)
    \brief
         \~english Initialise 8 bit draw mode (palette mode)... Chose the screen and the background priority (0-3). This drawable background will replace Background 3, and must be loaded before all other backgrounds. Takes about 3/8 of the VRAM.
         \~french Initialise le mode de dessin 8 bit (avec palette). Il suffit de choisir l'�cran et la priorit� de ce font (de 0 � 3). Ce fond sera plac� sur le fond 3 (le remplacant), et doit etre charg� avant tout autre fond ! Prend environ 3/8 de la VRAM
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param bg_priority
         \~english Background priority (0-3)
         \~english Background priority (0-3) 
*/
void PA_Init8bitBg(bool screen, u8 bg_priority);


/*! \fn void PA_Init16bitBg(bool screen, u8 bg_priority)
    \brief
         \~english Initialise 16 bit draw mode (no palette mode, true colors)... Chose the screen and the background priority (0-3). This drawable background will replace Background 3, and must be loaded before all other backgrounds. Takes about 6/8 of the VRAM, so almost all the space !
         \~french Initialise le mode de dessin 16 bit (sans palettes, couleurs RGB). Il suffit de choisir l'�cran et la priorit� de ce font (de 0 � 3). Ce fond sera plac� sur le fond 3 (le remplacant), et doit etre charg� avant tout autre fond ! Prend environ 3/8 de la VRAM
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param bg_priority
         \~english Background priority (0-3)
         \~english Background priority (0-3) 
*/
void PA_Init16bitBg(bool screen, u8 bg_priority);



/*! \fn extern inline void PA_Put8bitPixel(bool screen, s16 x, s16 y, u8 color) 
    \brief
         \~english Draw a pixel on screen, on an 8 bit background
         \~french Dessine un pixel � l'�cran, sur un fond de 8 bits
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param x
         \~english X position (0-255)
         \~french Position X (0-255)
    \param y
         \~english Y position (0-191)
         \~french Position Y (0-191)	
    \param color
         \~english Color in the background palette (0-255)
         \~french Couleur de la palette du fond (0-255) 
*/
extern inline void PA_Put8bitPixel(bool screen, s16 x, s16 y, u8 color) {
	u16 pos = 	(x >> 1) + (y << 7);
	if (x&1){
		PA_DrawBg[screen][pos] &= 255;
		PA_DrawBg[screen][pos] |= color << 8;			
	}
	else {
		PA_DrawBg[screen][pos] &= 255 << 8;
		PA_DrawBg[screen][pos] |= color;
	}
}


/*! \fn extern inline void PA_Put2_8bitPixels(bool screen, s16 x, s16 y, u16 colors)
    \brief
         \~english Draw 2 pixels on screen, on an 8 bit background. These pixels are next to another, and the first pixel must be with a pair X. WAY faster than drawing both pixels separately
         \~french Dessine deux pixels � l'�cran, sur un fond de 8 bits. Ces pixels sont contigus, et le premier doit avoir une position X pair. Beaucoup plus rapide que de dessiner les 2 pixels s�paremment
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1) 
   \param x
         \~english X position (0-254), must be PAIR
         \~french Position X (0-254), doit etre PAIR
    \param y
         \~english Y position (0-191)
         \~french Position Y (0-191)	
    \param colors
         \~english Colors of the first and second pixels (*256 for the second)
         \~french Couleurs des premier et deuxi�me pixels (*256 pour le deuxi�me)
	 
*/
extern inline void PA_Put2_8bitPixels(bool screen, s16 x, s16 y, u16 colors) {		PA_DrawBg[screen][(x >> 1) + (y << 7)] = colors;
}

/*! \fn extern inline void PA_PutDouble8bitPixels(bool screen, s16 x, s16 y, u8 color1, u8 color2)
    \brief
         \~english Draw 2 pixels on screen, on an 8 bit background. These pixels are next to another, and the first pixel must be with a pair X. WAY faster than drawing both pixels separately
         \~french Dessine deux pixels � l'�cran, sur un fond de 8 bits. Ces pixels sont contigus, et le premier doit avoir une position X pair. Beaucoup plus rapide que de dessiner les 2 pixels s�paremment
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1) 
   \param x
         \~english X position (0-254), must be PAIR
         \~french Position X (0-254), doit etre PAIR
    \param y
         \~english Y position (0-191)
         \~french Position Y (0-191)	
    \param color1
         \~english Color of the first pixel, in the background palette (0-255)
         \~french Couleur de la palette du fond (0-255) pour le premier pixel
    \param color2
         \~english Color of the second pixel, in the background palette (0-255)
         \~french Couleur de la palette du fond (0-255) pour le deuxi�me pixel		 
*/
extern inline void PA_PutDouble8bitPixels(bool screen, s16 x, s16 y, u8 color1, u8 color2) {
	PA_Put2_8bitPixels(screen, x, y, color1 + (color2 << 8));		
}



/*! \fn extern inline void PA_Put4_8bitPixels(bool screen, s16 x, s16 y, u32 colors)
    \brief
         \~english Draw 4 pixels on screen, on an 8 bit background. These pixels are next to another, and the first pixel must be with a pair X. Fastest way to draw on the screen...
         \~french Dessine 4 pixels � l'�cran, sur un fond de 8 bits. Ces pixels sont contigus, et le premier doit avoir une position X pair. Fa�on la plus rapide de dessiner � l'�cran
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1) 
   \param x
         \~english X position (0-254), must be PAIR
         \~french Position X (0-254), doit etre PAIR
    \param y
         \~english Y position (0-191)
         \~french Position Y (0-191)	
    \param colors
         \~english Colors of the 4 pixels
         \~french Couleurs des 4 pixels
	 
*/
extern inline void PA_Put4_8bitPixels(bool screen, s16 x, s16 y, u32 colors) {
/*
	(*u32*)(PA_DrawBg[screen] + (x >> 1) + (y << 7)) = colors;*/
}


/*! \fn extern inline u8 PA_Get8bitPixel(bool screen, u8 x, u8 y)
    \brief
         \~english Get the pixel's color in 8 bit Draw mode... 
         \~french R�cup�rer la couleur d'un pixel, en mode dessin 8 bit
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1) 
    \param x
         \~english X position. Be carefull, if X is not between 0 and 255, it'll give unwanted results
         \~french Position X. Attention, si X n'est pas compris entre 0 et 255, le r�sultat ne sera pas celui escompt�
    \param y
         \~english Y position. Be carefull, if Y is not between 0 and 191, it'll give unwanted results
         \~french Position Y. Attention, si Y n'est pas compris entre 0 et 191, le r�sultat ne sera pas celui escompt�
 */
extern inline u8 PA_Get8bitPixel(bool screen, u8 x, u8 y) {
	u16 pos = 	(x >> 1) + (y << 7);
	if (x&1){
		return (PA_DrawBg[screen][pos] >> 8);
	}
	else {
		return (PA_DrawBg[screen][pos]);
	}
}




/*! \fn extern inline void PA_Put16bitPixel(bool screen, s16 x, s16 y, u16 color)
    \brief
         \~english Draw a pixel on screen, on an 16 bit background
         \~french Dessine un pixel � l'�cran, sur un fond de 16 bits
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param x
         \~english X position (0-255)
         \~french Position X (0-255)
    \param y
         \~english Y position (0-191)
         \~french Position Y (0-191)	
    \param color
         \~english 16 bit color, obtained using PA_RGB(red, green, blue)
         \~french Couleur de 16 bits, obtenue avec PA_RGB(rouge, vert, bleu)
*/
extern inline void PA_Put16bitPixel(bool screen, s16 x, s16 y, u16 color) {
	PA_DrawBg[screen][x + (y << 8)] = color;
}


/*! \def PA_Get16bitPixel(x, y)
    \brief
         \~english Get the pixel's color in 16 bit Draw mode... 
         \~french R�cup�rer la couleur d'un pixel, en mode dessin 16 bit
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1) 
    \param x
         \~english X position. Be carefull, if X is not between 0 and 255, it'll give unwanted results
         \~french Position X. Attention, si X n'est pas compris entre 0 et 255, le r�sultat ne sera pas celui escompt�
    \param y
         \~english Y position. Be carefull, if Y is not between 0 and 191, it'll give unwanted results
         \~french Position Y. Attention, si Y n'est pas compris entre 0 et 191, le r�sultat ne sera pas celui escompt�
 */
#define PA_Get16bitPixel(screen, x, y) PA_DrawBg[screen][x + (y << 8)]






/*! \fn void PA_Draw8bitLine(bool screen, u16 x1, u16 y1, u16 x2, u16 y2, u8 color)
    \brief
         \~english Draw a line in Draw mode... for 8 bit drawable background
         \~french Dessiner une ligne en mode dessin... pour le mode dessin 8 bit
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1) 
    \param x1
         \~english X position of the first point. Be carefull, if X is not between 0 and 255, it'll give unwanted results
         \~french Position X du premier point. Attention, si X n'est pas compris entre 0 et 255, le r�sultat ne sera pas celui escompt�
    \param y1
         \~english Y position of the first point. Be carefull, if Y is not between 0 and 191, it'll give unwanted results
         \~french Position Y du premier point. Attention, si Y n'est pas compris entre 0 et 191, le r�sultat ne sera pas celui escompt�
    \param x2
         \~english X position of the second point. Be carefull, if X is not between 0 and 255, it'll give unwanted results
         \~french Position X du deuxi�me point. Attention, si X n'est pas compris entre 0 et 255, le r�sultat ne sera pas celui escompt�
    \param y2
         \~english Y position of the second point. Be carefull, if Y is not between 0 and 191, it'll give unwanted results
         \~french Position Y du deuxi�me point. Attention, si Y n'est pas compris entre 0 et 191, le r�sultat ne sera pas celui escompt�
    \param color
         \~english Color in the background palette (0-255)
         \~french Couleur de la palette du fond (0-255) 
 */
void PA_Draw8bitLine(bool screen, u16 x1, u16 y1, u16 x2, u16 y2, u8 color);





/*! \fn void PA_Draw16bitLine(bool screen, u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
    \brief
         \~english Draw a line in Draw mode... for 16 bit drawable background
         \~french Dessiner une ligne en mode dessin... pour le mode dessin 16 bit
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1) 
    \param x1
         \~english X position of the first point. Be carefull, if X is not between 0 and 255, it'll give unwanted results
         \~french Position X du premier point. Attention, si X n'est pas compris entre 0 et 255, le r�sultat ne sera pas celui escompt�
    \param y1
         \~english Y position of the first point. Be carefull, if Y is not between 0 and 191, it'll give unwanted results
         \~french Position Y du premier point. Attention, si Y n'est pas compris entre 0 et 191, le r�sultat ne sera pas celui escompt�
    \param x2
         \~english X position of the second point. Be carefull, if X is not between 0 and 255, it'll give unwanted results
         \~french Position X du deuxi�me point. Attention, si X n'est pas compris entre 0 et 255, le r�sultat ne sera pas celui escompt�
    \param y2
         \~english Y position of the second point. Be carefull, if Y is not between 0 and 191, it'll give unwanted results
         \~french Position Y du deuxi�me point. Attention, si Y n'est pas compris entre 0 et 191, le r�sultat ne sera pas celui escompt�
    \param color
         \~english 15 bits color. You can use the PA_RGB macro to set the RGB values...
         \~french Couleur de 15 bits.On peut utiliser la macro PA_RGB pour entrer les valeurs RGB...
 */
void PA_Draw16bitLine(bool screen, u16 x1, u16 y1, u16 x2, u16 y2, u16 color);




/*! \fn PA_8bitDraw(bool screen, u16 color)
    \brief
         \~english For 8 bit background : Nice little function that draws on screen ! All you need to do is chose the color, it'll do the rest. If the PA VBL isn't initialised, don't forget to update the stylus position every frame... Juste execute PA_Draw every cycle...
         \~french Pour 8 bit : Jolie petite fonction qui dessine � l'�cran ! Tout ce qu'il reste � faire, c'est de choisir la couleur. Si le VBL PA n'est pas initialiser, ne pas oublier de rafraichir le Stylet � chaque cycle (et non, pas avec des glacons !). Il suffit d'executer PA_Draw � chaque cycle pour dessiner...
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1) 
    \param color
         \~english 15 bits color. You can use the PA_RGB macro to set the RGB values...
         \~french Couleur de 15 bits.On peut utiliser la macro PA_RGB pour entrer les valeurs RGB...
 */
void PA_8bitDraw(bool screen, u8 color);


/*! \fn PA_16bitDraw(bool screen, u16 color)
    \brief
         \~english For 16 bit : Nice little function that draws on screen ! All you need to do is chose the color, it'll do the rest. If the PA VBL isn't initialised, don't forget to update the stylus position every frame... Juste execute PA_Draw every cycle...
         \~french Pour 16 bit : Jolie petite fonction qui dessine � l'�cran ! Tout ce qu'il reste � faire, c'est de choisir la couleur. Si le VBL PA n'est pas initialiser, ne pas oublier de rafraichir le Stylet � chaque cycle (et non, pas avec des glacons !). Il suffit d'executer PA_Draw � chaque cycle pour dessiner...
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1) 
    \param color
         \~english 15 bits color. You can use the PA_RGB macro to set the RGB values...
         \~french Couleur de 15 bits.On peut utiliser la macro PA_RGB pour entrer les valeurs RGB...
 */
void PA_16bitDraw(bool screen, u16 color);


/*! \def PA_SetDrawSize(screen, draw_size)
    \brief
         \~english Set the size of the pen when drawing.
         \~french Regler la taille du stylo quand on dessine.
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1) 		 
    \param draw_size
         \~english Size...
         \~french Taille...
*/
#define PA_SetDrawSize(screen, draw_size) PA_drawsize[screen] = draw_size;





/*! \def PA_Load8bitBitmap(screen, bitmap)
    \brief
         \~englishLoad a bitmap on the screen for an 8 bit drawable background
         \~french Charger une image � l'�cran... pour une fond dessinable de 8 bits
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)	
    \param bitmap
         \~english Bitmap name
         \~french Nom du bitmap
*/
#define PA_Load8bitBitmap(screen, bitmap) DMA_Copy(bitmap, (void*)PA_DrawBg[screen], 256*96, DMA_16NOW);

/*! \def PA_Load16bitBitmap(screen, bitmap)
    \brief
         \~englishLoad a bitmap on the screen for an 16 bit drawable background
         \~french Charger une image � l'�cran... pour une fond dessinable de 16 bits
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)	
    \param bitmap
         \~english Bitmap name
         \~french Nom du bitmap
*/
#define PA_Load16bitBitmap(screen, bitmap) {u32 PA_temp; \
for (PA_temp = 0; PA_temp < 256*192; PA_temp++)\
PA_DrawBg[screen][PA_temp] = bitmap[PA_temp] + (1 << 15);}


/*! \def PA_Clear8bitBg(screen)
    \brief
         \~english Clears the screen... for an 8 bit drawable background
         \~french Efface l'�cran... pour une fond dessinable de 8 bits
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)		 
*/
#define PA_Clear8bitBg(screen) DMA_Copy(Blank, (void*)PA_DrawBg[screen], 256*96, DMA_16NOW);

/*! \def PA_Clear16bitBg(screen)
    \brief
         \~english Clears the screen... for an 16 bit drawable background
         \~french Efface l'�cran... pour une fond dessinable de 16 bits
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)		 
*/
#define PA_Clear16bitBg(screen) DMA_Copy(Blank, (void*)PA_DrawBg[screen], 256*192, DMA_16NOW)


/*! \fn extern inline void PA_LoadJpeg(bool screen, void *jpeg)
    \brief
         \~english Load a jpeg on a 16 bit background... Don't forget to Init the background !
         \~french Charger un jpeg sur un fond de 16 bits... Faut pas oublier de charger ce fond avant !
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)	
    \param jpeg
         \~english jpeg image...
         \~french image au format jpeg...	 
*/
extern inline void PA_LoadJpeg(bool screen, void *jpeg) {
	REG_IME = 0x00;
	JPEG_DecompressImage((u8*)jpeg, PA_DrawBg[screen], 256, 192);
	REG_IME = 0x01;	
}


void PA_LoadBmpEx(bool screen, s16 x, s16 y, void *bmp);


extern inline void PA_LoadBmp(bool screen, void *bmp){
	PA_LoadBmpEx(screen, 0, 0, bmp);
}



/* // Les diff�rentes fonctions images...
extern inline void PA_LoadJpeg(bool screen, void *jpeg)
void PA_LoadBmp(bool screen, s16 x, s16 y, void *bmp);*/

extern inline void PA_LoadGBFSImage(bool screen, s16 GBFSImage){
	if (PA_CompareText(PA_GBFSfile[GBFSImage].Ext, "bmp")){
		PA_LoadBmp(screen, PA_GBFSfile[GBFSImage].File);
	}
	if (PA_CompareText(PA_GBFSfile[GBFSImage].Ext, "jpg")){ 
		PA_LoadJpeg(screen, PA_GBFSfile[GBFSImage].File);
	}	
	if (PA_CompareText(PA_GBFSfile[GBFSImage].Ext, "gif")){ 
		PA_LoadGif(screen, PA_GBFSfile[GBFSImage].File);
	}		
}




void PA_Draw16bitLineEx(bool screen, s16 basex, s16 basey, s16 endx, s16 endy, u16 color, s8 size);

void PA_Draw16bitRect(bool screen, s16 basex, s16 basey, s16 endx, s16 endy, u16 color);


extern inline u16 PA_GetBmpWidth(void *bmp){
	u8 *temp = (u8*)bmp;
	BMP_Headers *Bmpinfo = (BMP_Headers*)(temp+14);
	return Bmpinfo->Width;
}
extern inline u16 PA_GetBmpHeight(void *bmp){
	u8 *temp = (u8*)bmp;
	BMP_Headers *Bmpinfo = (BMP_Headers*)(temp+14);
	return Bmpinfo->Height;
}
/** @} */ // end of Draw


extern inline void PA_8bit8x8Image(bool screen, u16 basex, u16 basey, u8* data){
s16 x, y;

for (x = 0; x < 8; x++)
	for (y = 0; y < 8; y++)
		PA_Put8bitPixel(screen, x + basex, y + basey, data[x + (y << 3)]);
}

/*
extern inline void PA_DrawImage(bool screen, s16 x, s16 y, s16 lx, s16 ly, u8* data){
s16 i, j;
u16* data2 = (u16*)data;

if (x&1){ // Premier pixel � mettre tout seul peut-etre
	for (j = 0; j < ly; j++) PA_Put8bitPixel(screen, x, j + y, data[0]);
	x++;
	data2 = (u16*)(data + 1);
}

i = 0;
// Tant qu'il reste au moins 2 pixels � mettre, on fait 2 par 2
while(i < (lx-2)){
	for (j = 0; j < ly; j++) {
		PA_PutDouble8bitPixels(screen, i + x, j + y, data2[(i >> 1) + (j*lx)]);
	}
	i+=2;
}

if (!((i + x)&1)){ // Dernier pixel � mettre tout seul peut-etre
	for (j = 0; j < ly; j++) PA_Put8bitPixel(screen, i + x, j + y, data2[(i >> 1) + (j*lx)]&255);
}

}
*/




















#endif


