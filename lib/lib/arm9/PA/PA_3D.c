
#ifdef __cplusplus
extern "C" {
#endif

#include <PA9.h>

// Added because libnds changed them to something Im too lazy to search for
#define DISPLAY_CR       (*(vuint32*)0x04000000)
#define SUB_DISPLAY_CR   (*(vuint32*)0x04001000)

extern u8 	pa_3Dscreen;
extern u8 pa_3Ddual;

void PADefault3DInit(void){
	//this should work the same as the normal gl call
	glInit();
	glViewport(0,0,255,191);
	
	glClearDepth(0x7FFF);		
	*(u32*)0x04000350=0; // CLEAR_COLOR
	
	if(pa_3Dscreen) PA_SwitchScreens();
	pa_3Dscreen = 0;
	pa_3Ddual = 0;
}

void PA_Init3D(void){
   videoSetMode(  MODE_0_3D ); // Set 3d on...
	DISPLAY_CR |= DISPLAY_BG_EXT_PALETTE; // Bg extended palette

	vramSetBankA(VRAM_A_TEXTURE); // Say good-bye to sprites ^^
	vramSetBankF(VRAM_F_TEX_PALETTE); // Palettes for the textures...
	
	PA_BGScrollXY(0, 0, 0, 0);


	PADefault3DInit();
}


void PA_Init3D2Banks(void){
   videoSetMode(  MODE_0_3D ); // Set 3d on...
	DISPLAY_CR |= DISPLAY_BG_EXT_PALETTE; // Bg extended palette

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
  






#ifdef __cplusplus
}
#endif

