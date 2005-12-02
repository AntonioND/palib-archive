#include "PA9.h"
#include <nds.h>


#include "PA/PA_API.c"
#include "PA/PA_Draw.c"
#include "PA/PA_Gif.c"
#include "PA/PA_Interrupt.c"
#include "PA/PA_Keys.c"
#include "PA/PA_FS.c"
#include "PA/PA_GBFS.c"
#include "PA/video.c"
#include "PA/PA_Math.c"
#include "PA/PA_Palette.c"
#include "PA/PA_Sprite.c"
#include "PA/PA_Sound.c"
#include "PA/PA_Text.c"
#include "PA/PA_TextIWRAM.c"
#include "PA/PA_Tile.c"
#include "PA/PA_Mode7.c"
#include "PA/PA_Keyboard.c"

#include "PA/PA_3D.c"
#include "PA/PA_IA.c"

#include "PA/PA_Micro.c"


#include "PA/bitmap.map.c"
#include "PA/jpeg.c"



#ifdef USE_PA_SPLASH
	// Pour afficher le splash screen PA
	#include "PA/splash.c"
	#include "PA/splash2.c"
	#include "PA/PApalet.c"
#endif

#ifdef USE_NEOSPLASH
	#include "PA/NeoSplash.c"  // Splash NeoFlash
#endif
#define VAR_IN_EXRAM __attribute__ ((section (".ewram")))
TransferSound snd;

u32 *sndMemPool;

u32 *Blank;

typedef struct {
   s16 x, y, oldx, oldy, vx, vy;  // Coordonnées
   bool stylus; // Si on déplace ou non
   bool moving; // Si la boule est en mouvement ou non
} positions;

#define LETTRES 6


u32 PA_bgmap[2][4]; // Pointeur vers les maps, 4 maps par screen
bool PA_Screen = 0;

s16 PA_ScreenSpace; // Espace entre les 2 écrans...+192



// Pour les fenetres
const s16 winfades[][4] = {
	{4, 3, -4, -3},
	{4, 0, -4, 0},	
	{8, 0, 0, 0},	
	{0, 0, -8, 0},
	{0, 6, -8, 0},	
	{8, 0, 0, -6},	
	{0, 0, -8, -6},	
	{8, 6, 0, 0},		
};


/*
typedef struct{
	u8 snd_action[16]; // 0 - rien, 1 - play, 2 - stop, 3 - pause
	u8 snd_action[16]; // 0 - rien, 1 - play, 2 - stop, 3 - pause	


}PA_IPCinfo;
PA_IPCinfo PA_IPC;*/


//////////////////////////////////////////////////////////////////////
// Général
//////////////////////////////////////////////////////////////////////


void PA_Init(void) {
// Turn on the screens and 2D cores and switch to mode 0
powerON(POWER_ALL);
  POWER_CR = POWER_ALL_2D;
  
 POWER_CR &= ~SWITCH_SCREENS; // on s'assure que l'écran est bien

//PA_SetScreenLight(0, 1); PA_SetScreenLight(1, 1); // Les 2 écrans sont allumés

//IPC->mailData

//sndMemPool = new u32[SND_MEM_POOL_SIZE/4]; 

  	VRAM_A_CR=VRAM_ENABLE|VRAM_A_MAIN_BG; 
	VRAM_B_CR=VRAM_ENABLE|VRAM_B_MAIN_SPRITE; 
	VRAM_C_CR=VRAM_ENABLE|VRAM_C_SUB_BG;
	VRAM_D_CR=VRAM_ENABLE|VRAM_D_SUB_SPRITE; 


  DISPLAY_CR = MODE_0_2D | DISPLAY_SPR_1D_LAYOUT | DISPLAY_SPR_ACTIVE|2<<20;  // 1 << 31 pour 256 couleurs avec palettes
  SUB_DISPLAY_CR = MODE_0_2D | DISPLAY_SPR_1D_LAYOUT | DISPLAY_SPR_ACTIVE|2<<20;

vramSetMainBanks(VRAM_A_MAIN_BG,VRAM_B_MAIN_SPRITE,VRAM_C_SUB_BG,VRAM_D_SUB_SPRITE);

u32 temp = 0;
free(Blank); // Free, just in case it's a ReInit...
Blank = (u32*)malloc(130000); // Malloc a big blank chunk
DMA_Force(temp, Blank, 32500, DMA_32NOW);


PA_ResetSpriteSys(); // Init's the sprite system
PA_InitSpriteExtPal(); // Init's sprite extended palettes

PA_ResetBgSys();
PA_InitBgExtPal(); // Init's bg extended palettes


WAIT_CR &= ~(1 << 7);

PA_UpdateRTC();
PA_SRand(35329 + PA_RTC.Minutes + PA_RTC.Seconds + PA_RTC.Hour + PA_RTC.Day);

PA_SetTextCol(0, 31, 31, 31);
PA_SetTextCol(1, 31, 31, 31);

Stylus.X = 128;
Stylus.Y = 96;

PA_ResetInterrupts();


PA_SetBrightness(0, 0); PA_SetBrightness(1, 0); // On affiche les écrans

	PA_font[0] = 0;
	PA_textmap[0] = (u16*)PA_font_Map;
	PA_texttiles[0] = (u16*)PA_font_Tiles;
	PA_font[1] = 0;
	PA_textmap[1] = (u16*)PA_font_Map;
	PA_texttiles[1] = (u16*)PA_font_Tiles;

PA_UpdateUserInfo();

PA_SetScreenSpace(48);

}




void PA_Splash(void* tiles0, void* map0, void* tiles1, void* map1, s16 color0, s16 color1, s16 time){

s32 i;
s8 sens = -1; 

// White
if (color0) {
	color0 = 31;
}
else {
	color0 = -31;
	sens = 1;
}




PA_SetBrightness(0, color0);
PA_SetBrightness(1, color0);


PA_LoadBg(0, 3, tiles0, 50000, map0, BG_256X256, 1, 1);
PA_LoadBg(1, 3, tiles1, 50000, map1, BG_256X256, 1, 1);


// Fade in...
for (i = color0; i != -sens; i+= sens) {
	PA_SetBrightness(0, i);
	PA_SetBrightness(1, i);
	PA_WaitForVBL();
}

sens = -sens;


while(time && !Pad.Newpress.Start && !Stylus.Newpress) {
	time--;
	PA_WaitForVBL();
}


sens = 1; 

// White
if (color1) {
	color1 = 31;
}
else {
	color1 = -31;
	sens = -1;
}


// Fade out...
for (i = 0; i != color1; i+= sens) {
	PA_SetBrightness(0, i);
	PA_SetBrightness(1, i);
	PA_WaitForVBL();
}

PA_ResetBgSys();

PA_SetBrightness(0, 0);
PA_SetBrightness(1, 0);



}





void PA_LoadSplash() {
#ifdef USE_PA_SPLASH

u16 i = 150;
u8 n_lettre; // lettre à laquelle on en est...
bool already = 0;


positions lettres[LETTRES];


	PA_LoadPal(PAL_BG0, splashPal);
	PA_LoadPal(PAL_BG1, splashPal);

	PA_LoadPal(PAL_SPRITE0, PApaletPal);
	PA_LoadPal(PAL_SPRITE1, PApaletPal);

PA_LoadSimpleBg(0, 3, splashData, bitmap, BG_256X256, 1, 1);
PA_LoadSimpleBg(1, 3, splash2Data, bitmap, BG_256X256, 1, 1);


	for (n_lettre = 0; n_lettre < LETTRES; n_lettre++) {
		lettres[n_lettre].stylus = 0;  // Par défaut on bouge pas...
		lettres[n_lettre].moving = 0; // Bouge pas
		lettres[n_lettre].vx = 0;
		lettres[n_lettre].vy = 0;
		lettres[n_lettre].x = 6 + (n_lettre *42);
		lettres[n_lettre].y = 192 + 46;
		PA_CreateSpriteEx(0, n_lettre, (void*)(PApaletData + (n_lettre << 7)), OBJ_SIZE_32X32, 0, 0, 0, 0, 0, 0, 0, 0, lettres[n_lettre].x, lettres[n_lettre].y);
		PA_CreateSpriteEx(1, n_lettre, (void*)(PApaletData + (n_lettre << 7)), OBJ_SIZE_32X32, 0, 0, 0, 0, 0, 0, 0, 0, 1, 192);

	}

for (n_lettre = 0; n_lettre < LETTRES; n_lettre++) {
	lettres[n_lettre].stylus = 0;  // Par défaut on bouge pas...
	lettres[n_lettre].moving = 0; // Bouge pas
	lettres[n_lettre].vx = 0;
	lettres[n_lettre].vy = 0;
	lettres[n_lettre].x = 6 + (n_lettre *42);
	lettres[n_lettre].y = 192 + 46;
}

while (!Pad.Held.Start) {
PA_UpdatePad();
PA_UpdateStylus();
	for (n_lettre = 0; n_lettre < LETTRES; n_lettre ++) {
		
		if (lettres[n_lettre].vx==0 && lettres[n_lettre].vy==0){
			PA_SetSpritePrio(0, n_lettre, 3);
		}
		else {
			PA_SetSpritePrio(0, n_lettre, 2);
		}
		
		if (Stylus.Released && lettres[n_lettre].stylus) { // Si on relache alors qu'avait on bougeait
			lettres[n_lettre].vx = lettres[n_lettre].x - lettres[n_lettre].oldx;
			lettres[n_lettre].vy = lettres[n_lettre].y - lettres[n_lettre].oldy;
			lettres[n_lettre].stylus = 0;  // Si on ne touche pas, on ne fait rien
			lettres[n_lettre].moving = 1;
			already = 0; // On ne déplace plus rien...
		}

		if (Stylus.Held & !already) { // Si nouvelle pression, on regarde si on touche ou pas le truc
			if (PA_Distance (Stylus.X, Stylus.Y, lettres[n_lettre].x + 16, lettres[n_lettre].y + 16 - 192) < 15*15){				
			//	((Stylus.X>=lettres[n_lettre].x) && (Stylus.X<=lettres[n_lettre].x+32)) && ((Stylus.Y>=lettres[n_lettre].y-192) && (Stylus.Y<=(lettres[n_lettre].y-192)+32))) {
				lettres[n_lettre].stylus = 1;
				already = 1; // On annonce qu'on déplace, faut pas en prendre d'autres...
				lettres[n_lettre].moving = 0;
			}
		}
		
		lettres[n_lettre].oldx = lettres[n_lettre].x;
		lettres[n_lettre].oldy = lettres[n_lettre].y;

		if (lettres[n_lettre].stylus) { // Si on peut le déplacer...
			PA_SetSpritePrio(0, n_lettre, 2);
			lettres[n_lettre].x=Stylus.X-16;
			lettres[n_lettre].y=Stylus.Y+192-16;
			if(lettres[n_lettre].x<0)lettres[n_lettre].x=0;
			if(lettres[n_lettre].x>256-32)lettres[n_lettre].x=256-32;
			if(lettres[n_lettre].y<0)lettres[n_lettre].y=0;
			if(lettres[n_lettre].y>384-32)lettres[n_lettre].y=384-32;
		}
		else { // Si on ne le déplace pas, on utilise vx et vy...
			lettres[n_lettre].x += lettres[n_lettre].vx;
			lettres[n_lettre].y += lettres[n_lettre].vy;
			if(lettres[n_lettre].x<0 && lettres[n_lettre].vx < 0)lettres[n_lettre].vx=-lettres[n_lettre].vx;
			if(lettres[n_lettre].x>256-32 && lettres[n_lettre].vx > 0)lettres[n_lettre].vx=-lettres[n_lettre].vx;
			if(lettres[n_lettre].y<0 && lettres[n_lettre].vy < 0)lettres[n_lettre].vy=-lettres[n_lettre].vy;
			if(lettres[n_lettre].y>384-32 && lettres[n_lettre].vy > 0)lettres[n_lettre].vy=-lettres[n_lettre].vy;


			if (lettres[n_lettre].moving) { // Si en mouvment...
				// Test de collision entre le palet et tous les palets qui suivent dans le tableau...
				for (i = n_lettre+1; i < LETTRES; i++) {
					if (lettres[i].moving) { // Si en mouvement aussi
						if (PA_Distance (lettres[n_lettre].x + 16, lettres[n_lettre].y + 16, lettres[i].x + 16, lettres[i].y + 16) < 30*30 && PA_GetSpritePrio(0, n_lettre)==2 && PA_GetSpritePrio(0, i)==2 ) { // Si collision entre 2 boules... On va les faire partir dans les directions opposées...
							lettres[n_lettre].vx = (lettres[n_lettre].x - lettres[i].x) >> 1;
							lettres[n_lettre].vy = (lettres[n_lettre].y - lettres[i].y) >> 1;
							lettres[i].vx = -lettres[n_lettre].vx;
							lettres[i].vy = -lettres[n_lettre].vy;
						}
					}
				}
			}

		}

		PA_SetSpriteY(0, n_lettre, 192); PA_SetSpriteY(1, n_lettre, 192);  // On cache par défaut
		if(lettres[n_lettre].y-192>=-32 && lettres[n_lettre].y-192<=224) PA_SetSpriteXY (0,n_lettre,lettres[n_lettre].x, lettres[n_lettre].y-192);		
		if(lettres[n_lettre].y>=-32 && lettres[n_lettre].y<=224) PA_SetSpriteXY (1,n_lettre,lettres[n_lettre].x, lettres[n_lettre].y); 
	}


	PA_WaitForVBL();



}



	for (i = 0; i < 32; i++) {
		PA_SetBrightness(0, i);
		PA_SetBrightness(1, -i);
		PA_WaitForVBL();
	}

	PA_ResetSpriteSys();
	PA_ResetBgSys();
	PA_LoadPal(PAL_BG0, Blank);
	PA_LoadPal(PAL_BG1, Blank);
	PA_SetBrightness(0, 0);
	PA_SetBrightness(1, 0);

	PA_MovedSprite.Time = 0;
	PA_MovedSprite.Moving = 0;

#endif

}


infos PA_UserInfo;

void PA_UpdateUserInfo(void) {

u8 i;

 PA_UserInfo.Color = INFO_COLOR;
 PA_UserInfo.BdayMonth = INFO_BDAY_MONTH;
 PA_UserInfo.BdayDay = INFO_BDAY_DAY;
 PA_UserInfo.BdayMonth = INFO_BDAY_MONTH;
 PA_UserInfo.AlarmHour = INFO_ALARM_HOUR;
 PA_UserInfo.AlarmMinute = INFO_ALARM_MINUTE;

 PA_UserInfo.NameLength = INFO_NAME_LENGTH;
 for (i = 0; i < PA_UserInfo.NameLength; i++) PA_UserInfo.Name[i] = *(u8*)(0x027FFC86 + (i << 1));
PA_UserInfo.Name[i] = 0; // Pour marquer la fin...

 PA_UserInfo.MessageLength = INFO_MESSAGE_LENGTH;
 for (i = 0; i < PA_UserInfo.MessageLength; i++) PA_UserInfo.Message[i] = *(u8*)(0x027FFC9C + (i << 1));
PA_UserInfo.Name[i] = 0; // Pour marquer la fin...
PA_UserInfo.Language = (INFO_LANGUAGE)&7;
}

RTC PA_RTC;  // Infos RTC...

void PA_UpdateRTC(void) {

u8 i;
u8 *temp;
temp = (u8*)&PA_RTC;
 for (i = 0; i < 8; i++) temp[i] = IPC->curtime[i];

 if (PA_RTC.Hour > 12) PA_RTC.Hour -= 40;
}







void PA_SetBrightness(bool screen, s8 bright) {
u16 mode = 1 << 14;

	if (bright < 0) {
		mode = 2 << 14;
		bright = -bright;
	}
	if (bright > 31) bright = 31;
	_REG16(REG_BRIGHT + (0x1000 * screen)) = bright + mode;
}




void PA_NeoSplash(void) {

#ifdef USE_NEOSPLASH

u16 i;


		PA_SetBrightness(0, 31);
		PA_SetBrightness(1, -31);

	PA_LoadPal(PAL_BG1, NeoSplashPal);
	PA_LoadPal(PAL_BG0, NeoSplashPal);

	PA_LoadTiledBitmap(0, NeoSplashData);
	PA_LoadTiledBitmap(1, NeoSplashData);

	i = 0;
	while (i < 32) {
		PA_SetBrightness(0, 31 - (i));
		PA_SetBrightness(1, (i) - 31);
		i++;
		PA_WaitForVBL();
	}

	i = 150;
	do {
		PA_WaitForVBL();
	}
//	while ((i--) && (!Pad.Released.Start) && (!Stylus.Held));
	while ((!Pad.Released.Start) && (!Stylus.Released));
	i = 0;


	while (i < 32) {
		PA_SetBrightness(0, -(i));
		PA_SetBrightness(1, (i ));
		i++;
		PA_WaitForVBL();
	}

	PA_ResetBgSys();
	PA_LoadPal(PAL_BG0, Blank);
	PA_LoadPal(PAL_BG1, Blank);
	PA_SetBrightness(0, 0);
	PA_SetBrightness(1, 0);

	

#endif
}


void PA_SetScreenLight(bool screen, bool light){
	if (light) IPC->aux |= BACKLIGHT(screen); // Allumé
	else IPC->aux &= ~(BACKLIGHT(screen)); // Eteint
}


void PA_Nothing(void){}














