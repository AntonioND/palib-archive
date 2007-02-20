
#ifdef __cplusplus
extern "C" {
#endif

#include <PA9.h>


void PA_Init3D(void){
   videoSetMode(  MODE_0_3D ); // Set 3d on...
	DISPLAY_CR |= 1 << 30; // Bg extended palette

	vramSetBankA(VRAM_A_TEXTURE); // Say good-bye to sprites ^^
	vramSetBankF(VRAM_F_TEX_PALETTE); // Palettes for the textures...

	//this should work the same as the normal gl call
	glViewPort(0,0,255,191);
	
	glClearDepth(0x7FFF);		
	*(u32*)0x04000350=0; // CLEAR_COLOR
}




void PA_Load3DSpritePal(u16 palette_number, void* palette){
	vramSetBankF(VRAM_F_LCD);  
	DMA_Copy(palette, VRAM_F + (palette_number << 9), 256, DMA_16NOW);
	vramSetBankF(VRAM_F_TEX_PALETTE);
}
  






#ifdef __cplusplus
}
#endif

