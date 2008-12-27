#ifdef __cplusplus
extern "C" {
#endif

#include "PA9.h"
#include <nds.h>


#define VAR_IN_EXRAM __attribute__ ((section (".ewram")))
TransferSound snd;

u32 *sndMemPool;
u8 pa_checklid;

u16 pa_giftotexcolor= 0;




volatile PA_IPCType PA_IPC;


typedef struct {
   s16 x, y, oldx, oldy, vx, vy;  // Coordonnées
   u8 stylus; // Si on déplace ou non
   u8 moving; // Si la boule est en mouvement ou non
} positions;

//#define LETTRES 6


u32 PA_bgmap[2][4]; // Pointeur vers les maps, 4 maps par screen
u8 PA_Screen = 0;

s16 PA_ScreenSpace; // Espace entre les 2 écrans...+192

PA_GifInfos PA_GifInfo;




//Keypad stuff...
PA_Pad_Struct Pad;

PA_Stylus_Struct Stylus;



PA_movingsprite  PA_MovedSprite; // Pour les sprites que l'on bouge...

u8 PA_MoveSpriteType = 0;


/*
typedef struct{
	u8 snd_action[16]; // 0 - rien, 1 - play, 2 - stop, 3 - pause
	u8 snd_action[16]; // 0 - rien, 1 - play, 2 - stop, 3 - pause


}PA_IPCinfo;
PA_IPCinfo PA_IPC;*/


void PA_SetBrightness(u8 screen, s8 bright) {
u16 mode = 1 << 14;

	if (bright < 0) {
		mode = 2 << 14;
		bright = -bright;
	}
	if (bright > 31) bright = 31;
	_REG16(REG_BRIGHT + (0x1000 * screen)) = bright + mode;
}


void PA_Nothing(void)
{
}


#ifdef __cplusplus
}
#endif
