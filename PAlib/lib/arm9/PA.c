#include "PA9.h"
#include <nds.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "PA/PA_Draw.c"
#include "PA/PA_Gif.c"
#include "PA/PA_Interrupt.c"
#include "PA/PA_Keys.c"
#include "PA/PA_FS.c"
#include "PA/PA_GBFS.c"
#include "PA/vidfile.c"
#include "PA/PA_Math.c"
#include "PA/PA_Palette.c"
#include "PA/PA_Sprite.c"
#include "PA/PA_Sound.c"
#include "PA/PA_Timer.c"
#include "PA/PA_Text.c"

#include "PA/PA_16c.c"

#ifdef TEXT_ALLSIZES
	#include "PA/PA_API.c"
	#include "PA/PA_TextIWRAM.c"
#endif

#include "PA/PA_Tile.c"
#include "PA/PA_Mode7.c"

#ifdef USE_KEYBOARD
	#include "PA/PA_Keyboard.c"
#endif


#ifdef USE_3D
	#include "PA/PA_3D.c"
#endif

#include "PA/PA_IA.c"
#include "PA/PA_Reco.c"

#include "PA/PA_Micro.c"

#include "PA/jpeg.c"

#include "PA/PA_FAT.c"

#define VAR_IN_EXRAM __attribute__ ((section (".ewram")))
TransferSound snd;

u32 *sndMemPool;

u32 Blank[130000>>2];

typedef struct {
   s16 x, y, oldx, oldy, vx, vy;  // Coordonnées
   u8 stylus; // Si on déplace ou non
   u8 moving; // Si la boule est en mouvement ou non
} positions;

#define LETTRES 6


u32 PA_bgmap[2][4]; // Pointeur vers les maps, 4 maps par screen
u8 PA_Screen = 0;

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


	//IPC->aux = 3; PA_SetScreenLight(0, 1); PA_SetScreenLight(1, 1);
	
s32 i;
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

//u32 temp = 0;
//if (Blank) free(Blank); // Free, just in case it's a ReInit...
//Blank = (u32*)malloc(130000); // Malloc a big blank chunk
for (i = 0; i < 130000>>2; i++) Blank[i] = 0;
 
// Sprite inits...
PA_ResetSpriteSys(); // Init's the sprite system
PA_InitSpriteExtPal(); // Init's sprite extended palettes


PA_ResetBgSys();
PA_InitBgExtPal(); // Init's bg extended palettes


WAIT_CR &= ~(1 << 7);

PA_UpdateRTC();
PA_SRand(35329 + PA_RTC.Minutes + PA_RTC.Seconds + PA_RTC.Hour + PA_RTC.Day);

Stylus.X = 128;
Stylus.Y = 96;

irqInit();
//PA_ResetInterrupts();

for (i = 0; i < 2; i++){
	PA_SetBrightness(i, 0); // On affiche les écrans

	PA_font[i] = 0;
	#ifndef TEXT_ALLCHARACTERS
		PA_textmap[i] = (u16*)PA_text_Map;
		PA_texttiles[i] = (u8*)PA_text_Tiles;
		PA_textpal[i] = (u16*)PA_text_Pal;	
	#endif
	#ifdef TEXT_ALLCHARACTERS
		PA_textmap[i] = (u16*)PA_text2_Map;
		PA_texttiles[i] = (u8*)PA_text2_Tiles;
		PA_textpal[i] = (u16*)PA_text2_Pal;	
	#endif	
}


PA_UpdateUserInfo();

PA_ResetRecoSys(); // Reco system init

PA_SetScreenSpace(48); // Default spacing
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







void PA_SetBrightness(u8 screen, s8 bright) {
u16 mode = 1 << 14;

	if (bright < 0) {
		mode = 2 << 14;
		bright = -bright;
	}
	if (bright > 31) bright = 31;
	_REG16(REG_BRIGHT + (0x1000 * screen)) = bright + mode;
}


/*
void PA_SetScreenLight(u8 screen, u8 light){
	IPC->aux &= ~(BACKLIGHT(screen))
	IPC->aux |= BACKLIGHT(screen)*light;
	//if (light) IPC->aux |= BACKLIGHT(screen); // Allumé
	//else IPC->aux &= ~(BACKLIGHT(screen)); // Eteint
}*/


void PA_Nothing(void){}




#ifdef __cplusplus
}
#endif







