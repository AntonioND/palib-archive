#include <PA9.h>

extern u8 pa_3Dscreen;
extern u8 pa_3Ddual;

void PADefault3DInit(){
	// this should do the same as the normal gl call
	glInit();
	glViewport(0, 0, 255, 191);
	
	glClearDepth(0x7FFF);		
	GFX_CLEAR_COLOR = 0;
	
	if(pa_3Dscreen) PA_SwitchScreens();
	pa_3Dscreen = 0;
	pa_3Ddual = 0;
}

void PA_Init3D(){
	videoSetMode(MODE_0_3D); // Set 3D on...
	REG_DISPCNT |= DISPLAY_BG_EXT_PALETTE; // BG extended palettes

	vramSetBankA(VRAM_A_TEXTURE); // Say good-bye to sprites ^^
	vramSetBankF(VRAM_F_TEX_PALETTE); // Palettes for the textures...
	
	PA_BGScrollXY(0, 0, 0, 0);

	PADefault3DInit();
}

void PA_Init3D2Banks(){
	videoSetMode(MODE_0_3D); // Set 3D on...
	REG_DISPCNT |= DISPLAY_BG_EXT_PALETTE; // BG extended palettes

	vramSetBankA(VRAM_A_TEXTURE); // Say good-bye to sprites ^^
	vramSetBankB(VRAM_B_TEXTURE); // Say good-bye to sprites ^^	
	vramSetBankF(VRAM_F_TEX_PALETTE); // Palettes for the textures...

	PADefault3DInit();
}

void PA_Load3DSpritePal(u16 palette_number, void* palette){
	vramSetBankF(VRAM_F_LCD);  
	DMA_Copy(palette, VRAM_F + (palette_number << 9), 256, DMA_16NOW);
	vramSetBankF(VRAM_F_TEX_PALETTE);
}
