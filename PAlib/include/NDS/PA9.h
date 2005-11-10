#ifndef _PA_Main
#define _PA_Main

#ifdef __cplusplus
extern "C" {
#endif



#define USE_PA_SPLASH

// Comment out this line to limit the text sizes to only a single size, size 2... 
// This will make you gain 200-300ko
//#define TEXT_ALLSIZES


#include <nds.h>
#include <nds/memory.h>
#include <nds/bios.h>
#include <malloc.h>

extern bool PA_Screen;

typedef struct {
   u8 NoIdea; 
   u8 Year;
   u8 Month;
   u8 Day;
   u8 NoIdea2;
   u8 Hour;
   u8 Minutes;
   u8 Seconds;
} RTC;
extern RTC PA_RTC;

extern inline void PA_WaitForVBL(void);

extern s16 PA_ScreenSpace; // Espace entre les 2 écrans...+192

#define BG_GFX1			0x6000000
#define BG_GFX2			0x6200000
#define SPRITE_GFX1		0x6400000
#define SPRITE_GFX2		0x6600000


//Pour le DMA Copy...
#define REG_DMA3SRC *(volatile u32*)0x040000D4
#define REG_DMA3DST *(volatile u32*)0x040000D8
#define REG_DMA3CNT *(volatile u32*)0x040000DC
#define DMA_ON 0x80000000
#define DMA_NOW 0x00000000
#define DMA_16 0x00000000
#define DMA_32 0x04000000
#define DMA_16NOW (DMA_ON | DMA_NOW | DMA_16)
#define DMA_32NOW (DMA_ON | DMA_NOW | DMA_32)
#define DMA_Copy(source, dest, count, mode) {REG_DMA3SRC = (u32)source; REG_DMA3DST = (u32)dest; REG_DMA3CNT = (count) | (mode);}

#define DMA_Clear(dest, count, mode) {REG_DMA3SRC = (u32)Blank; REG_DMA3DST = (u32)dest; REG_DMA3CNT = (count) | (mode);}

#define DMA_Force(ulVal,dest, count, mode) {REG_DMA3SRC=(u32)&ulVal; REG_DMA3DST = (u32)dest; REG_DMA3CNT = (count) |(mode) | DMA_SRC_FIX;}

// Commandes pour la lumière des écrans
#define BACKLIGHT(screen)	BIT(2+screen)







#include <stdarg.h> // Pour le système de text
#include <string.h>

#include "gbfs.h"
#include "gba-jpeg-decode.h"
#include "Sound9.h"


#define _REG8 *(volatile u16 *)
#define _REG16 *(volatile u16 *)
#define _REG32 *(volatile u32 *)

#include "arm9/video.h"
#include "arm9/PA_Sprite.h"
#include "arm9/PA_SpriteDual.h"
#include "arm9/PA_Sound.h"
#include "arm9/PA_Micro.h"
#include "arm9/PA_Draw.h"
#include "arm9/PA_Interrupt.h"
#include "arm9/PA_Keys.h"
#include "arm9/PA_API.h"
#include "arm9/PA_Math.h"
#include "arm9/PA_Palette.h"
#include "arm9/PA_PaletteDual.h"
#include "arm9/PA_Save.h"
#include "arm9/PA_GBFS.h"
#include "arm9/PA_SpecialFx.h"

#include "arm9/PA_Tile.h"
#include "arm9/PA_TileDual.h"
#include "arm9/PA_Keyboard.h"

#include "arm9/PA_Text.h"
#include "arm9/PA_Window.h"

#include "arm9/PA_Mode7.h"

#include "arm9/PA_3D.h"
#include "arm9/PA_IA.h"

#include "arm9/splash.h"
#include "arm9/splash2.h"
#include "arm9/PApalet.h"
extern const unsigned short bitmap[768];


#include "arm9/NeoSplash.h"  // Splash NeoFlash


#define SWITCH_SCREENS  (1<<15)



#define N_HBLS 263



/*! \file PA9.h
    \brief Contains prototypes and macros... for the arm9

    Contains prototypes and macros... for the arm9
*/



extern u32 *Blank;







//////////////////////////////////////////////////////////////////////
// Général
//////////////////////////////////////////////////////////////////////

#define SIZEOF_8BIT(x)          (sizeof(x))
#define SIZEOF_16BIT(x)         (sizeof(x)>>1)
#define SIZEOF_32BIT(x)         (sizeof(x)>>2)




#define CODE_IN_IWRAM __attribute__ ((section (".iwram"), long_call))
#define IN_IWRAM __attribute__ ((section (".iwram")))
#define CODE_IN_EWRAM __attribute__ ((section (".ewram"), long_call))
#define IN_EWRAM __attribute__ ((section (".ewram")))
#define PACKED __attribute__ ((packed))







// Jpeg functions :
void PA_jpeg_GetImageSize(char* name, int* width, int* height);
void PA_jpeg_BltImage(char* name, u16* vram, int output_width, int output_height);

extern const s16 PA_SIN[512];



typedef struct {
   u8 Color;  // Favorite Color
   u8 BdayMonth; // Mois d'anniversaire
   u8 BdayDay;  // Jour de naissance
   u8 AlarmHour;
   u8 AlarmMinute;
   u8 Name[21];
   u8 NameLength;
   u8 Message[53];
   u8 MessageLength;
   u8 Language; // Langue... 0 pour Jap, 1 pour anglais, 2 pour francais...
} infos;
extern infos PA_UserInfo;

#define INFO_COLOR *(u8*)0x027FFC82
#define INFO_BDAY_MONTH *(u8*)0x027FFC83
#define INFO_BDAY_DAY *(u8*)0x027FFC84
#define INFO_ALARM_HOUR *(u8*)0x027FFCD2
#define INFO_ALARM_MINUTE *(u8*)0x027FFCD3
#define INFO_NAME *(u8*)0x027FFC86
#define INFO_NAME_LENGTH *(u8*)0x027FFC9A
#define INFO_MESSAGE *(u8*)0x027FFC9C
#define INFO_MESSAGE_LENGTH *(u8*)0x027FFCD0
#define INFO_LANGUAGE  *(u8*)(0x027FFCE4)


/** @defgroup General General Functions
 *  Initialise the lib, and other general functions...
 *  @{
 */


/*! \fn void PA_Init(void)
    \brief
         \~english Initialise the library. Must be used at the beginning or main()
         \~french Initialise la lib... Doit etre placé au début de main()
*/
void PA_Init(void);



/*! \fn extern inline void PA_SetVideoMode(bool screen, u8 mode)
    \brief
         \~english Change the video mode... Use this with caution
         \~french Changer de mode video... A utiliser avec précaution
    \param screen
         \~english Screen...
         \~french Ecran...
    \param mode
         \~english Mode 0 for normal, 1 for 1 rotating backgrounds, 2 for 2
         \~french Mode 0 pour normal, 1 pour 1 fond rotatif, 2 pour 2
*/
extern inline void PA_SetVideoMode(bool screen, u8 mode) {
	(*(vuint32*)(0x04000000 + (0x1000 * screen))) &= ~7;
	(*(vuint32*)(0x04000000 + (0x1000 * screen))) |= mode;
}



/*! \fn void PA_UpdateUserInfo(void)
    \brief
         \~english Updates the user info. This is automatically done in PA_Init. You can then get any info with the following variables : PA_UserInfo.Color (favorite color), .BdayDay, .BdayMonth, .AlarmHour, .AlarmMinute, .Name, .NameLength, .Message, .MessageLength
         \~french Met à jour les infos sur l'utilisateur... C'est fait automatiquement dans PA_Init. On peut ensuite récupérer toutes les infos avec PA_UserInfo.Color (couleur favorite), .BdayDay, .BdayMonth, .AlarmHour, .AlarmMinute, .Name, .NameLength, .Message, .MessageLength
*/
void PA_UpdateUserInfo(void);


/*! \fn void PA_UpdateRTC(void)
    \brief
         \~english Updates the Real Time Clock, with info on the current date and hour. Automatically updated in the PA VBL... Get the info with PA_RTC.Minutes, .Hour, .Seconds, .Day, .Month, and .Year
         \~french Met à jour les infos sur l'heure et la date. C'est automatiquement mis à jour dans le VBL PA... On récupère les infos avec PA_RTC.Minutes, .Hour, .Seconds, .Day, .Month, et .Year
*/
void PA_UpdateRTC(void);


/*! \fn void PA_LoadSplash(void)
    \brief
         \~english Load a PlayerAdvance splash screen... It's always nice to give some credit ;)
         \~french Affiche un splash screen PlayerAdvance... C'est toujours sympa de remercier ;)


*/
void PA_LoadSplash(void);


/*! \fn void PA_LoadSplash()
    \brief
         \~english Load a NeoFlash splash screen...
         \~french Affiche un splash screen NeoFlash
*/
void PA_NeoSplash(void);



/*! \fn extern inline void PA_WaitForVBL(void)
    \brief
         \~english Wait for the VBlank to occur
         \~french Attendre le vbl...
*/
extern inline void PA_WaitForVBL(void){
swiWaitForVBlank();
}



/*! \def PA_SwitchScreens()
    \brief
         \~english Switch the bottom and top screens...
         \~french Echange les écrans du haut et du bas
*/
extern inline void PA_SwitchScreens(void) {
POWER_CR ^= SWITCH_SCREENS; 
PA_Screen = !PA_Screen;
}


/*! \fn extern inline void PA_InitCPUMeter()
    \brief
         \~english BROKEN ----- Initialises the CPU Meter. The CPU value is saved in PA_CPU (and PA_MaxCPU for max usage)
         \~french BROKEN ----- Initialise le compteur CPU. La valeur (en %) est sauvée dans PA_CPU (et le plus gros dans PA_MaxCPU)
*/
extern inline void PA_InitCPUMeter() {
	PA_CPU = 0; 
	PA_MaxCPU = 0; 
	PA_lines = 0; 
	PA_VBLCount = 0;
	PA_nVBLs = 0;
}





/*! \def PA_LidClosed()
    \brief
         \~english Check if the DS is closed. Returns 0 if open, 1 if closed
         \~french Vérifie si la DS est fermée. Renvoie 0 si ouverte, 1 si fermée
*/
#define PA_LidClosed() (IPC->buttons>>7)


/*! \fn extern inline bool PA_CheckLid(void)
    \brief
         \~english Check if the DS is closed. If closed, it pauses the DS, and returns 1.
         \~french Vérifie si la DS est fermée. Si fermée, ca met en pause la DS et renvoie 1.
*/
extern inline bool PA_CheckLid(void) {

if (!PA_LidClosed()) return 0;
else {
	u16 power_cr = POWER_CR; // backup the power...
	POWER_CR = 0; // Shutdown everything :p
	
	// Wait for the lid to be opened again...
	while(PA_LidClosed()) PA_WaitForVBL();
	
	// Return the power !
	POWER_CR = power_cr;
	return 1;
}

}





/*! \fn void PA_Splash(void* tiles0, void* map0, void* tiles1, void* map1, s16 color0, s16 color1, s16 time)
    \brief
         \~english Splash screen function ! Load your own splash screen...
         \~french Fonction Splash screen ! Permet de charger son propre splash screen...
    \param tiles0
         \~english Tiles of the splash to put on the bottom screen
         \~french Tiles du splash à afficher sur l'écran du bas
    \param map0
         \~english Map of the splash to put on the bottom screen
         \~french Map du splash à afficher sur l'écran du bas
    \param tiles1
         \~english Tiles of the splash to put on the top screen
         \~french Tiles du splash à afficher sur l'écran du haut
    \param map1
         \~english Map of the splash to put on the top screen
         \~french Map du splash à afficher sur l'écran du haut
    \param color0
         \~english Fade in color, 0 for balck, 1 for white.
         \~french Couleur du fondu d'entrée : 0 pour noir, 1 pour blanc.
    \param color1
         \~english Fade out color, 0 for balck, 1 for white.
         \~french Couleur du fondu de sortie : 0 pour noir, 1 pour blanc.
    \param time
         \~english Time to stay on the splash if you do net press Start or use the Stylus, in VBLs -> 60 for 1 second, 300 for 5 seconds...
         \~french Le temps que le splash doit rester, si on n'appuye pas sur Start ou le Stylet, en VBLs -> 60 pour 1 seconde, 300 pour 5 secondes... 

*/
void PA_Splash(void* tiles0, void* map0, void* tiles1, void* map1, s16 color0, s16 color1, s16 time);


/*! \fn void PA_SetScreenLight(bool screen, bool light)
    \brief
         \~english Set on or off the screen's light
         \~french Allumer ou eteindre la lumière d'un écran
    \param screen
         \~english Screen...
         \~french Ecran...
    \param light
         \~english Light, 1 for on, 0 for off
         \~french Lumière, 1 pour allumé, 0 pour éteint
*/
void PA_SetScreenLight(bool screen, bool light);


/** @} */ // end of General


void PA_Nothing(void);

#ifdef __cplusplus
}
#endif
#endif



