
#ifdef __cplusplus
extern "C" {
#endif

#include <PA9.h>



extern const int PA_text_Info[3]; // BgMode, Width, Height
extern const unsigned short PA_text_Map[768] __attribute__ ((aligned (4))) ;  // Pal : PA_text_Pal
extern const unsigned char PA_text_Tiles[6080] __attribute__ ((aligned (4))) ;  // Pal : PA_text_Pal
extern const unsigned short PA_text_Pal[5] __attribute__ ((aligned (4)));


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

void PA_UpdateSpriteVBL(void);
extern funcpointer SpriteVBL;

void PA_Init2D(void){
	// Turn on the screens and 2D cores and switch to mode 0
	powerON(POWER_ALL);
	//  POWER_CR = POWER_ALL_2D;
	
	REG_POWERCNT &= ~SWITCH_SCREENS; // on s'assure que l'écran est bien
/*	
	VRAM_A_CR=VRAM_ENABLE|VRAM_A_MAIN_BG; 
	VRAM_B_CR=VRAM_ENABLE|VRAM_B_MAIN_SPRITE; 
	VRAM_C_CR=VRAM_ENABLE|VRAM_C_SUB_BG;
	VRAM_D_CR=VRAM_ENABLE|VRAM_D_SUB_SPRITE; */
    videoSetMode(  MODE_0_2D | 
                   DISPLAY_SPR_ACTIVE |    //turn on sprites
                   DISPLAY_SPR_1D |        //this is used when in tile mode
				   DISPLAY_SPR_1D_SIZE_128|
                   DISPLAY_SPR_1D_BMP      //and this in bitmap mode
                    );
	videoSetModeSub(  MODE_0_2D | 
                   DISPLAY_SPR_ACTIVE |    //turn on sprites  
                   DISPLAY_SPR_1D |        //this is used when in tile mode
				   DISPLAY_SPR_1D_SIZE_128|
                   DISPLAY_SPR_1D_BMP      //and this in bitmap mode
                    );		
					
//	DISPLAY_CR = MODE_0_2D | DISPLAY_SPR_1D_LAYOUT | DISPLAY_SPR_ACTIVE|DISPLAY_SPR_1D_SIZE_128|DISPLAY_SPR_1D_BMP; 
//	SUB_DISPLAY_CR = MODE_0_2D | DISPLAY_SPR_1D_LAYOUT | DISPLAY_SPR_ACTIVE|DISPLAY_SPR_1D_SIZE_128|DISPLAY_SPR_1D_BMP;

	
	vramSetMainBanks(VRAM_A_MAIN_SPRITE,VRAM_B_MAIN_BG_0x06000000,VRAM_C_SUB_BG,VRAM_D_SUB_SPRITE);
	
	// Sprite inits...
	PA_ResetSpriteSys(); // Init's the sprite system
	PA_InitSpriteExtPal(); // Init's sprite extended palettes
	
	PA_ResetBgSys();
	PA_InitBgExtPal(); // Init's bg extended palettes
	
	
	// VBL Inits
	SpriteVBL = PA_UpdateSpriteVBL;

}



extern funcpointer MotionVBL;
extern funcpointer PaddleVBL;
extern funcpointer GHPadVBL;


void PA_Init(void) {
s32 i;
for (i = 0; i < 130000>>2; i++) Blank[i] = 0;

REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_SEND_CLEAR; // enable & clear FIFO

PA_Init2D();
pa_checklid = 1; // Autochecklid by default

//WAIT_CR &= ~(1 << 7);



PA_VBLCountersReset();

Stylus.X = 128;
Stylus.Y = 96;


PA_VBLFunctionReset();
irqInit();
//PA_ResetInterrupts();

IPC->mailData = (u32)(&PA_IPC);

for (i = 0; i < 2; i++){
	PA_SetBrightness(i, 0); // On affiche les écrans

	PA_font[i] = 0;
//	#ifndef TEXT_ALLCHARACTERS
		PA_textmap[i] = (u16*)PA_text_Map;
		PA_texttiles[i] = (u8*)PA_text_Tiles;
		PA_textpal[i] = (u16*)PA_text_Pal;	
//	#endif
/*	#ifdef TEXT_ALLCHARACTERS
		PA_textmap[i] = (u16*)PA_text2_Map;
		PA_texttiles[i] = (u8*)PA_text2_Tiles;
		PA_textpal[i] = (u16*)PA_text2_Pal;	
	#endif	*/
}
	PA_UpdateUserInfo();
	PA_ResetRecoSys(); // Reco system init
	PA_SetScreenSpace(48); // Default spacing
	
	PA_GifInfo.StartFrame = 0; // start from the beginning
	PA_GifInfo.EndFrame = 10000; // random high number
	
	MotionVBL = PA_Nothing;
	PaddleVBL = PA_Nothing;
	GHPadVBL = PA_Nothing;
	
	PA_UpdateRTC();
	//PA_SRand(35329 + PA_RTC.Minutes + PA_RTC.Seconds + PA_RTC.Hour + PA_RTC.Day);
	PA_InitRand();
}










#ifdef __cplusplus
}
#endif

