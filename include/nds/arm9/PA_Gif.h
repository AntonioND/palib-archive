#ifndef _PA_Gif
#define _PA_Gif

#ifdef __cplusplus
extern "C" {
#endif

/*! \file PA_Gif.h
    \brief Gif, animations...

    Manages everything about gif files.
*/


#include <PA9.h>

#include "PA_Interrupt.h"
#include "PA_Palette.h"
#include "PA_Draw.h"
#include "gif/gif_lib.h"

extern GifFileType* gifinfo;
extern s16 gifwidth, gifheight; 
extern u8 PA_nBit[2]; // 8 or 16 bit Bg


typedef struct{
	float Speed;
	u8 Play; // 0 stop; 1 play; 2 pause;
} PA_GifInfos;
extern PA_GifInfos PA_GifInfo;







void PA_Load16bitGif(u8 screen, s16 x, s16 y, void *gif);

int DecodeGif(const u8 *userData, u8 *ScreenBuff, u16* Palette, u8 nBits, s16 SWidth);



/** @defgroup Gif Gifs...
 *  Manages everything about gif files.
 *  @{
 */






/*! \fn extern inline u16 PA_GetGifWidth(void* gif)
    \brief
         \~english Get a Gif's width in pixels
         \~french Récupérer la largeur d'un Gif en pixels
    \param gif
         \~english Gif image...
         \~french image au format Gif...	 
*/
extern inline u16 PA_GetGifWidth(void* gif){
	u16 *temp = (u16*)gif;
	temp += 3;
	return *temp;
}



/*! \fn extern inline u16 PA_GetGifHeight(void* gif)
    \brief
         \~english Get a Gif's height in pixels
         \~french Récupérer la hauteur d'un Gif en pixels
    \param gif
         \~english Gif image...
         \~french image au format Gif...	 
*/
extern inline u16 PA_GetGifHeight(void* gif){
	u16 *temp = (u16*)gif;
	temp += 4;
	return *temp;
}



/*! \fn extern inline void PA_LoadGif(u8 screen, void *gif)
    \brief
         \~english Load a Gif on a 16 bit background... Don't forget to Init the background !
         \~french Charger un Gif sur un fond de 16 bits... Faut pas oublier de charger ce fond avant !
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)		 
    \param gif
         \~english Gif image...
         \~french image au format Gif...	 
*/
extern inline void PA_LoadGif(u8 screen, void *gif){
	if (PA_nBit[screen]) DecodeGif((const u8*)gif, (u8*)PA_DrawBg[screen], (u16*)0x05000000, 1, 256);
	//PA_Load16bitGif(screen, 0, 0, gif); // 16 bit...
	else DecodeGif((const u8*)gif, (u8*)PA_DrawBg[screen], (u16*)(0x05000000+(0x400*screen)), 0, 256);
}




/*! \fn extern inline void PA_GifAnimSpeed(float speed)
    \brief
         \~english Set the gif's speed
         \~french Changer la vitesse d'un gif
    \param speed
         \~english 1 for normal, 2 for 2x, 0.5 for half speed...
         \~french 1 pour normal, 2 pour 2x, 0.5 pour la moitié... 
*/
extern inline void PA_GifAnimSpeed(float speed){
	PA_GifInfo.Speed = speed;
}


/*! \fn extern inline void PA_GifAnimStop(void)
    \brief
         \~english Stop a Gif animation
         \~french Arrêter l'animation d'un gif
*/
extern inline void PA_GifAnimStop(void){
	PA_GifInfo.Play = 0;
}




/*! \fn extern inline void PA_GifAnimPause(void)
    \brief
         \~english Pause a Gif animation
         \~french Mettre en pause l'animation d'un gif
*/
extern inline void PA_GifAnimPause(void){
	PA_GifInfo.Play = 2;
}


/*! \fn extern inline void PA_GifAnimStop(void)
    \brief
         \~english Unpause a Gif animation
         \~french Reprendre l'animation d'un gif
*/
extern inline void PA_GifAnimPlay(void){
	PA_GifInfo.Play = 1;
}



#ifdef __cplusplus
}
#endif








#endif


