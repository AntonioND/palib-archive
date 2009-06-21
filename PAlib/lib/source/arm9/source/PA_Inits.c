#ifdef __cplusplus
extern "C" {
#endif

#include <PA9.h>

extern const int PA_text_Info[3]; // BgMode, Width, Height
extern const unsigned short PA_text_Map[768] __attribute__ ((aligned (4))) ;  // Pal : PA_text_Pal
extern const unsigned char PA_text_Tiles[6080] __attribute__ ((aligned (4))) ;  // Pal : PA_text_Pal
extern const unsigned short PA_text_Pal[5] __attribute__ ((aligned (4)));

// For the window fade system
const s16 winfades[][4] = {
	{4, 3, -4, -3},
	{4, 0, -4,  0},
	{8, 0,  0,  0},
	{0, 0, -8,  0},
	{0, 6, -8,  0},
	{8, 0,  0, -6},
	{0, 0, -8, -6},
	{8, 6,  0,  0},
};

void PA_UpdateSpriteVBL(void);
extern funcpointer SpriteVBL;

void PA_Init2D(void){
	// Turn on the screens and 2D cores and switch to mode 0
	// WTF, no idea

	REG_POWERCNT &= ~SWITCH_SCREENS; // ensure that the screens are OK
	powerOn(POWER_2D_A | POWER_2D_B); // may this help???

	videoSetMode(MODE_0_2D |
					DISPLAY_SPR_ACTIVE |		// turn on sprites
					DISPLAY_SPR_1D |			// this is used when in tile mode
					DISPLAY_SPR_1D_SIZE_128 |	// no idea
					DISPLAY_SPR_1D_BMP);		// and this in bitmap mode
	videoSetModeSub(MODE_0_2D |
					DISPLAY_SPR_ACTIVE |		// turn on sprites
					DISPLAY_SPR_1D |			// this is used when in tile mode
					DISPLAY_SPR_1D_SIZE_128 |	// no idea
					DISPLAY_SPR_1D_BMP);		//and this in bitmap mode

	vramSetMainBanks(VRAM_A_MAIN_SPRITE, VRAM_B_MAIN_BG_0x06000000, VRAM_C_SUB_BG, VRAM_D_SUB_SPRITE);

	// Sprite inits...
	PA_ResetSpriteSys();		// Initializes the sprite system
	PA_InitSpriteExtPal();		// Initializes sprite extended palettes

	PA_ResetBgSys();
	PA_InitBgExtPal();	// Initializes bg extended palettes

	PA_SetVideoMode(0, 0);
	PA_SetVideoMode(1, 0);

	// VBL Inits
	SpriteVBL = PA_UpdateSpriteVBL;
}

extern funcpointer MotionVBL;
extern funcpointer PaddleVBL;
extern funcpointer GHPadVBL;

void PA_Init(void) {
	s32 i;
	
	PA_Init2D();
	pa_checklid = 1; // Autochecklid by default

	PA_VBLCountersReset();

	Stylus.X = 128;
	Stylus.Y = 96;

	PA_VBLFunctionReset();
	
	PA_IPC_compat->mailData = (u32)(&PA_IPC);
	for(i = 0; i < 2; i++){
		PA_SetBrightness(i, 0);
		PA_font[i] = 0;
		PA_textmap[i] = (u16*)PA_text_Map;
		PA_texttiles[i] = (u8*)PA_text_Tiles;
		PA_textpal[i] = (u16*)PA_text_Pal;	
	}

	PA_UpdateUserInfo();
	PA_ResetRecoSys();				// Reco system init
	PA_SetScreenSpace(48);			// Default spacing

	PA_GifInfo.StartFrame = 0;		// start from the beginning
	PA_GifInfo.EndFrame = 10000;	// random high number

	MotionVBL = PA_Nothing;
	PaddleVBL = PA_Nothing;
	GHPadVBL  = PA_Nothing;
	
	PA_UpdateRTC();
	PA_InitRand();
}

#ifdef __cplusplus
}
#endif
