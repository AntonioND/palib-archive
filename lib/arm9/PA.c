#include "PA9.h"
#include <nds.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VAR_IN_EXRAM __attribute__ ((section (".ewram")))
TransferSound snd;

u32 *sndMemPool;

u32 Blank[130000>>2];


infos PA_UserInfo;
RTC PA_RTC;  // Infos RTC...

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




//Keypad stuff...
Pads Pad;
PA_Pad* PadPointer;

PA_Stylus Stylus;


PA_movingsprite  PA_MovedSprite; // Pour les sprites que l'on bouge...

u8 PA_MoveSpriteType = 0;


/*
typedef struct{
	u8 snd_action[16]; // 0 - rien, 1 - play, 2 - stop, 3 - pause
	u8 snd_action[16]; // 0 - rien, 1 - play, 2 - stop, 3 - pause	


}PA_IPCinfo;
PA_IPCinfo PA_IPC;*/


//////////////////////////////////////////////////////////////////////
// Général
//////////////////////////////////////////////////////////////////////

void PA_Init2D(void){
// Turn on the screens and 2D cores and switch to mode 0
powerON(POWER_ALL);
//  POWER_CR = POWER_ALL_2D;
  
 POWER_CR &= ~SWITCH_SCREENS; // on s'assure que l'écran est bien

  	VRAM_A_CR=VRAM_ENABLE|VRAM_A_MAIN_BG; 
	VRAM_B_CR=VRAM_ENABLE|VRAM_B_MAIN_SPRITE; 
	VRAM_C_CR=VRAM_ENABLE|VRAM_C_SUB_BG;
	VRAM_D_CR=VRAM_ENABLE|VRAM_D_SUB_SPRITE; 

  DISPLAY_CR = MODE_0_2D | DISPLAY_SPR_1D_LAYOUT | DISPLAY_SPR_ACTIVE|2<<20;  // 1 << 31 pour 256 couleurs avec palettes
  SUB_DISPLAY_CR = MODE_0_2D | DISPLAY_SPR_1D_LAYOUT | DISPLAY_SPR_ACTIVE|2<<20;

vramSetMainBanks(VRAM_A_MAIN_BG,VRAM_B_MAIN_SPRITE,VRAM_C_SUB_BG,VRAM_D_SUB_SPRITE);

// Sprite inits...
PA_ResetSpriteSys(); // Init's the sprite system
PA_InitSpriteExtPal(); // Init's sprite extended palettes

PA_ResetBgSys();
PA_InitBgExtPal(); // Init's bg extended palettes

}






void PA_Init(void) {
s32 i;
for (i = 0; i < 130000>>2; i++) Blank[i] = 0;

PA_Init2D();

WAIT_CR &= ~(1 << 7);

PA_UpdateRTC();
PA_SRand(35329 + PA_RTC.Minutes + PA_RTC.Seconds + PA_RTC.Hour + PA_RTC.Day);

PA_VBLCountersReset();

Stylus.X = 128;
Stylus.Y = 96;


PA_VBLFunctionReset();
irqInit();
//PA_ResetInterrupts();
PA_IPC.Stylus = &Stylus;  	// Gives access to the stylus data
PA_IPC.Pad = &Pad;	      	// Gives access to the pad data
IPC->mailData = (u32)(&PA_IPC);

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
	
	PA_GifInfo.StartFrame = 0; // start from the beginning
	PA_GifInfo.EndFrame = 10000; // random high number
}



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
PA_UserInfo.Message[i] = 0; // Pour marquer la fin...
PA_UserInfo.Language = (INFO_LANGUAGE)&7;
}





void PA_UpdateRTC(void) {

u8 i;
u8 *temp;
temp = (u8*)&PA_RTC;
	for (i = 0; i < 8; i++) temp[i] = IPC->curtime[i];

	if (PA_RTC.Hour > 12) PA_RTC.Hour -= 40;
 
	if (PA_RTC.OldSeconds != PA_RTC.Seconds){
		PA_RTC.FPS = PA_RTC.Frames;
		PA_RTC.Frames = 0;
		PA_RTC.OldSeconds = PA_RTC.Seconds;
	}

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



void PA_Nothing(void){}




#ifdef __cplusplus
}
#endif







