#ifndef _PA_GENERAL
#define _PA_GENERAL

#ifdef __cplusplus
extern "C" {
#endif

#include <PA_IPC.h>

#include <arm9/as_lib9.h>
#include <arm9/PA_Sound.h>

#include <nds.h>
#include <nds/memory.h>
#include <nds/bios.h>
#include <malloc.h>

#include <stdarg.h> // Pour le système de text
#include <string.h>


extern u8 PA_Screen;

typedef struct {
   u8 NoIdea; 
   u8 Year;
   u8 Month;
   u8 Day;
   u8 NoIdea2;
   u8 Hour;
   u8 Minutes;
   u8 Seconds;
   u8 FPS, Frames, OldSeconds; // To test the FPS...
} RTC;
extern RTC PA_RTC;

extern inline void PA_WaitForVBL(void);

extern s16 PA_ScreenSpace; // Espace entre les 2 écrans...+192

extern inline void PAWaitForVBL(void){
	PA_RTC.Frames++; // For the FPS counter
	swiWaitForVBlank();
}
extern inline void PA_PowerOff()
{
	IPC->aux |= BIT(6); // libnds arm7: #define PM_POWER_DOWN BIT(6)
}

extern u8 pa_checklid;


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

#define DMA_CopyEx(type, source, dest, count, mode) {DMA_SRC(type) = (u32)source; DMA_DEST(type) = (u32)dest; DMA_CR(type) = (count) | (mode);}


// Commandes pour la lumière des écrans
#define BACKLIGHT(screen)	BIT(2+screen)

#define ALWAYSINLINE __attribute__((always_inline)) static inline








#define _REG8 *(volatile u16 *)
#define _REG16 *(volatile u16 *)
#define _REG32 *(volatile u32 *)


#define SWITCH_SCREENS  (1<<15)



#define N_HBLS 263



/*! \file PA_General.h
    \brief Contains prototypes and macros... for the arm9

    Contains prototypes and macros... for the arm9
*/



//extern u32 *Blank;
extern u32 Blank[130000>>2];

extern volatile PA_IPCType PA_IPC;




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


/*! \fn void PA_Init2D(void)
    \brief
         \~english Resets to 2D state after using 3D functions
         \~french Remet en mode 2D après avoir utilisé la 3D
*/
void PA_Init2D(void);



/*! \fn extern inline void PA_SetVideoMode(u8 screen, u8 mode)
    \brief
         \~english Change the video mode... Use this with caution
         \~french Changer de mode video... A utiliser avec précaution
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param mode
         \~english Mode 0 for normal, 1 for 1 rotating backgrounds, 2 for 2
         \~french Mode 0 pour normal, 1 pour 1 fond rotatif, 2 pour 2
*/
extern inline void PA_SetVideoMode(u8 screen, u8 mode) {
	(*(vuint32*)(0x04000000 + (0x1000 * screen))) &= ~7;
	(*(vuint32*)(0x04000000 + (0x1000 * screen))) |= mode;
}



/*! \fn void PA_UpdateUserInfo(void)
    \brief
         \~english Updates the user info. This is automatically done in PA_Init. You can then get any info with the following variables : PA_UserInfo.Color (favorite color), .BdayDay, .BdayMonth, .AlarmHour, .AlarmMinute, .Name, .NameLength, .Message, .MessageLength, .Language
         \~french Met à jour les infos sur l'utilisateur... C'est fait automatiquement dans PA_Init. On peut ensuite récupérer toutes les infos avec PA_UserInfo.Color (couleur favorite), .BdayDay, .BdayMonth, .AlarmHour, .AlarmMinute, .Name, .NameLength, .Message, .MessageLength, .Language
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




/*! \fn extern inline void PA_SwitchScreens(void)
    \brief
         \~english Switch the bottom and top screens...
         \~french Echange les écrans du haut et du bas
*/
extern inline void PA_SwitchScreens(void) {
	REG_POWERCNT ^= SWITCH_SCREENS; 
	PA_Screen = !PA_Screen;
}


/*! \fn extern inline void PA_InitCPUMeter()
    \brief
         \~english BROKEN ----- Initialises the CPU Meter. The CPU value is saved in PA_CPU (and PA_MaxCPU for max usage)
         \~french BROKEN ----- Initialise le compteur CPU. La valeur (en %) est sauvée dans PA_CPU (et le plus gros dans PA_MaxCPU)
*//*
extern inline void PA_InitCPUMeter() {
	PA_CPU = 0; 
	PA_MaxCPU = 0; 
	PA_lines = 0; 
	PA_VBLCount = 0;
	PA_nVBLs = 0;
}*/





/*! \def PA_LidClosed()
    \brief
         \~english Check if the DS is closed. Returns 0 if open, 1 if closed
         \~french Vérifie si la DS est fermée. Renvoie 0 si ouverte, 1 si fermée
*/
#define PA_LidClosed() (IPC->buttons>>7)


/*! \fn extern inline void PA_SetAutoCheckLid(u8 on)
    \brief
         \~english Automatically check if the DS is closed in PA_WaitForVBL
         \~french Vérifie automatiquement si la DS est fermée dans PA_WaitForVBL
    \~\param on
         \~english 1 for on, 0 for off
         \~french 1 pour activer, 0 pour désactiver
*/
extern inline void PA_SetAutoCheckLid(u8 on){
	pa_checklid = on;
}

/*! \fn extern inline u8 PA_CheckLid(void)
    \brief
         \~english Check if the DS is closed. If closed, it pauses the DS, and returns 1.
         \~french Vérifie si la DS est fermée. Si fermée, ca met en pause la DS et renvoie 1.
*/
extern inline u8 PA_CheckLid(void) {

	if (!PA_LidClosed()) return 0;
	else {
		u16 power_cr = REG_POWERCNT; // backup the power...
		REG_POWERCNT = 0; // Shutdown everything :p
		
		// Wait for the lid to be opened again...
		while(PA_LidClosed()) PAWaitForVBL();
		
		// Return the power !
		REG_POWERCNT = power_cr;
		return 1;
	}

}


/*! \fn extern inline void PA_WaitForVBL(void)
    \brief
         \~english Wait for the VBlank to occur
         \~french Attendre le vbl...
*/
extern inline void PA_WaitForVBL(void){
	if(pa_checklid) PA_CheckLid();
	PAWaitForVBL();
}



/*! \def PA_CloseLidSound(close_sound)
    \brief
         \~english Check if the DS is closed. If closed, it pauses the DS, and plays a sound
         \~french Vérifie si la DS est fermée. Si fermée, ca met en pause la DS et joue un son
    \~\param close_sound
         \~english Sound to play, check the sounds doc if you're not sure what to do here
         \~french Son à jouer, regarder la doc son si pas certain de quoi mettre... 
*/
#define PA_CloseLidSound(close_sound){\
			if(PA_LidClosed()){\
				PA_PlaySimpleSound(close_sound);\
				PA_CheckLid(); \
			}}


/*! \def PA_CloseLidSound2(close_sound, open_sound)
    \brief
         \~english Check if the DS is closed. If closed, it pauses the DS, and plays a sound. The sound system must be initialized before.
         \~french Vérifie si la DS est fermée. Si fermée, ca met en pause la DS et joue un son
    \~\param close_sound
         \~english Sound to play when closes, check the sounds doc if you're not sure what to do here
         \~french Son à jouer quand se ferme, regarder la doc son si pas certain de quoi mettre...
    \~\param open_sound
         \~english Sound to play when opens, check the sounds doc if you're not sure what to do here
         \~french Son à jouer quand s'ouvre, regarder la doc son si pas certain de quoi mettre... 
*/
#define PA_CloseLidSound2(close_sound, open_sound){\
			if(PA_LidClosed()){\
				PA_PlaySimpleSound(close_sound);\
				PA_CheckLid(); \
				PA_PlaySimpleSound(open_sound); \
			}}





/*! \fn void PA_SetScreenLight(u8 screen, u8 light)
    \brief
         \~english Set on or off the screen's light
         \~french Allumer ou eteindre la lumière d'un écran
    \~\param screen
         \~english Screen...
         \~french Ecran...
    \~\param light
         \~english Light, 1 for on, 0 for off
         \~french Lumière, 1 pour allumé, 0 pour éteint
*/
extern inline void PA_SetScreenLight(u8 screen, u8 light){
	IPC->aux &= ~(1<<(2+screen));
	IPC->aux |= light<<(2+screen);
}


/*! \fn extern inline void PA_SetLedBlink(u8 blink, u8 speed)
    \brief
         \~english Set teh DS Led blinking
         \~french Régler le clignotement de la led
    \~\param blink
         \~english 1 for blinking, 0 for always on
         \~french 1 pour clignoter, 0 pour toujours allumé
    \~\param speed
         \~english Speed : 0 for slow, 1 for fast
         \~french Vitesse : 0 pour lent, 1 pour rapide
*/
extern inline void PA_SetLedBlink(u8 blink, u8 speed){
	IPC->aux &= ~(3<<4);
	IPC->aux |= ((blink&1) + ((speed&1)<<1))<<4;
}


/*! \def PA_WaitFor(something)
    \brief
         \~english Wait for a specific thing to happen...
         \~french Attendre un événement précis...
    \~\param something
         \~english Thing to wait for, like Pad.Newpress.A, or Stylus.Newpress, etc...
         \~french Evénement à attendre, comme Pad.Newpress.A, ou Stylus.Newpress, etc...
*/

#define PA_WaitFor(something) {PA_WaitForVBL(); while(!(something)){PA_WaitForVBL(); }}


/*! \fn extern inline void PA_SetDSLBrightness(u8 level)
    \brief
         \~english Set the DS Lite Light level...
         \~french Régler le niveau de lumière de la DS Lite
    \~\param level
         \~english Light level (0-3)
         \~french Niveau de la lumière (0-3)
*/
extern inline void PA_SetDSLBrightness(u8 level){
	PA_IPC.Sound[16].Busy = (1<<7) | level;  // Brightness level, 0-3
}

/*! \fn bool PA_Locate(char *start, char *target, bool isDir, int depth, char *result)
    \brief
        Find a directory in the file system within a given depth
    \param start
        from which directory to start, use "/" to search from the root
	\param target
		what to look for: the name of a file or directory
	\param isDir
		look for a directory or a file?
	\param depth
		how much depth level (in number of directories) to traverse; limiting this speeds up the search on crowded cards. A reasonable value is, for example, 3.
	\param result
		pointer to a buffer where the result will be stored
	\return
		true if the target was found
*/
bool PA_Locate(char *start, char *target, bool isDir, int depth, char *result);



/** @} */ // end of General


void PA_Nothing(void);


#ifdef __cplusplus
}
#endif
#endif



