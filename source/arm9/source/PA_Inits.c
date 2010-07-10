#include <PA9.h>
#include <locale.h>

// Text stuff
extern const unsigned short PA_TiledFont_Map[];
extern const unsigned char PA_TiledFont_Tiles[];
extern const unsigned short PA_TiledFont_Pal[];

#include "BitmapFont/all_gfx.h"

extern const u32 PA_BmpFont0_Tiles_size;
extern const u32 PA_BmpFont1_Tiles_size;
extern const u32 PA_BmpFont2_Tiles_size;
extern const u32 PA_BmpFont3_Tiles_size;
extern const u32 PA_BmpFont4_Tiles_size;

static void PAFillBitTextSizes(){
	pa_bittextsizes[0] = PA_BmpFont0_Tiles_size;
	pa_bittextsizes[1] = PA_BmpFont1_Tiles_size;
	pa_bittextsizes[2] = PA_BmpFont2_Tiles_size;
	pa_bittextsizes[3] = PA_BmpFont3_Tiles_size;
	pa_bittextsizes[4] = PA_BmpFont4_Tiles_size;

	pa_bittextpoliceheight[0] = PA_BmpFont0.FontHeight;
	pa_bittextpoliceheight[1] = PA_BmpFont1.FontHeight;
	pa_bittextpoliceheight[2] = PA_BmpFont2.FontHeight;
	pa_bittextpoliceheight[3] = PA_BmpFont3.FontHeight;
	pa_bittextpoliceheight[4] = PA_BmpFont4.FontHeight;
}

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

void PA_Init2D(){
	// Turn on the 2D cores and unset the switch screens flag
	REG_POWERCNT &= ~SWITCH_SCREENS;
	powerOn(POWER_2D_A | POWER_2D_B);

	// Set up the 2D cores
	videoSetMode   (MODE_0_2D |               // mode 0 (3D/text - text - text - text)
	                DISPLAY_SPR_ACTIVE |      // enable sprites
	                DISPLAY_SPR_1D |          // sprite VRAM 1D mapping
	                DISPLAY_SPR_1D_SIZE_128 | // sprite VRAM boundary 128
	                DISPLAY_SPR_1D_BMP);      // bitmap sprite VRAM 1D mapping

	videoSetModeSub(MODE_0_2D |               // mode 0 (3D/text - text - text - text)
	                DISPLAY_SPR_ACTIVE |      // enable sprites
	                DISPLAY_SPR_1D |          // sprite VRAM 1D mapping
	                DISPLAY_SPR_1D_SIZE_128 | // sprite VRAM boundary 128
	                DISPLAY_SPR_1D_BMP);      // bitmap sprite VRAM 1D mapping

	// Prepare some VRAM banks for graphics
	vramSetMainBanks(VRAM_A_MAIN_SPRITE, VRAM_B_MAIN_BG_0x06000000, VRAM_C_SUB_BG, VRAM_D_SUB_SPRITE);

	// Initialize extended palettes
	PA_InitBgExtPal();
	PA_InitSpriteExtPal();

	// Initialize the background and sprite system
	PA_ResetBgSys();
	PA_ResetSpriteSys();

	// VBL Inits
	SpriteVBL = PA_UpdateSpriteVBL;
}

extern funcpointer MotionVBL;
extern funcpointer PaddleVBL;
extern funcpointer GHPadVBL;

void PA_Init(){
	register int i;

	// Wait a couple frames to provide a sensible starting point
	swiWaitForVBlank();
	swiWaitForVBlank();

	PA_InitFifo();
	PA_Init2D();
	pa_checklid = 1; // Autochecklid by default
	_PA_LidDown = 0;
	setlocale(LC_ALL, "C"); // Disable UTF-8 in newlib
	PA_VBLCountersReset();
	PA_VBLFunctionReset();

	PAFillBitTextSizes();

	for(i = 0; i < 2; i ++){
		PA_SetBrightness(i, 0);
		PA_font[i]      = 0;
		PA_textmap[i]   = (u16*) PA_TiledFont_Map;
		PA_texttiles[i] = (u8*)  PA_TiledFont_Tiles;
		PA_textpal[i]   = (u16*) PA_TiledFont_Pal;
	}

	PA_UpdateUserInfo();
	PA_ResetRecoSys();           // Reco system init
	PA_SetScreenSpace(48);       // Default spacing
	PA_GifInfo.StartFrame = 0;   // start from the beginning
	PA_GifInfo.EndFrame = 10000; // random high number
	MotionVBL = PA_Nothing;
	PaddleVBL = PA_Nothing;
	GHPadVBL  = PA_Nothing;

	PA_UpdateRTC();
	PA_InitRand();

	// Initialize the VBlank function
	irqSet(IRQ_VBLANK, PA_vblFunc);
	//irqEnable(IRQ_VBLANK);
}
