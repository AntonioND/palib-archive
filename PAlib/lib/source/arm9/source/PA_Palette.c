
#ifdef __cplusplus
extern "C" {
#endif

#include "PA9.h"

u8 PA_ExtPal[2][2]; // 0 si pas �tendu, 1 sinon


void PA_InitSpriteExtPal(void) {
	// Palettes �tendus pour les 256 couleurs...
	vramSetBankG(VRAM_G_SPRITE_EXT_PALETTE);
	vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);
	PA_ExtPal[0][0] = 1;
	PA_ExtPal[0][1] = 1;
	//DISPLAY_CR |= DISPLAY_SPR_EXT_PALETTE;
	//SUB_DISPLAY_CR |= DISPLAY_SPR_EXT_PALETTE;
	REG_DISPCNT |= DISPLAY_SPR_EXT_PALETTE;
	REG_DISPCNT_SUB |= DISPLAY_SPR_EXT_PALETTE;

	PA_LoadSpritePal(0, 0, (void*)PAL_SPRITE0);
	PA_LoadSpritePal(1, 0, (void*)PAL_SPRITE1);
}





void PA_LoadSpriteExtPal(u8 screen, u16 palette_number, void* palette){
	if (screen == 0) {
		vramSetBankG(VRAM_G_LCD);  // On passe en mode LCD pour pouvoir ecrire dessus, on reviendre en palette apres
		//DC_FlushRange(VRAM_G + (palette_number << 9), 256);
		DMA_Copy(palette, VRAM_G + (palette_number << 9), 256, DMA_16NOW);
		vramSetBankG(VRAM_G_SPRITE_EXT_PALETTE);
	}
	else {
		vramSetBankI(VRAM_I_LCD);  // On passe en mode LCD pour pouvoir ecrire dessus, on reviendre en palette apres
		//DC_FlushRange(VRAM_I + (palette_number << 9), 256);
		DMA_Copy(palette, VRAM_I + (palette_number << 9), 256, DMA_16NOW);
		vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);
	}
}


/*
void PA_LoadSpriteExtPal(u8 screen, u16 palette_number, void* palette){
	if(screen == 0){
		vramSetBankG(VRAM_G_LCD);
		dmaCopyHalfWords(3, palette, VRAM_G + palette_number * 256, 512);
		vramSetBankG(VRAM_G_OBJ_EXT_PALETTE);
	}
	if(screen == 1){
		vramSetBankI(VRAM_I_LCD);
		dmaCopyHalfWords(3, palette, VRAM_I + palette_number * 256, 512);
		vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);
	}
}
*/


void PA_InitBgExtPal(void) {
	// Palettes �tendus pour les 256 couleurs...
	vramSetBankE(VRAM_E_BG_EXT_PALETTE);
	vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
	PA_ExtPal[1][0] = 1;
	PA_ExtPal[1][1] = 1;
	//DISPLAY_CR |= 1 << 30;
	//SUB_DISPLAY_CR |= 1 << 30;
	REG_DISPCNT |= DISPLAY_BG_EXT_PALETTE;
	REG_DISPCNT_SUB |= DISPLAY_BG_EXT_PALETTE;

	u8 i;
	
	for (i = 0; i < 4; i++){ // On copie la palette partout
		PA_LoadBgExtPal(0, i, (void*)PAL_BG0);
		PA_LoadBgExtPal(1, i, (void*)PAL_BG0);	
	}

}

/*
void _PA_PrivateCopyPal(u16* palette, u16* destination){
	// This is a private copy function
	u32 i;
	for(i = 0; i < 256; i ++)
		destination[i] = palette[i];
	//DC_FlushRange(destination, 256); // ensure we copied the data
}
*/


void PA_LoadBgPalN(u8 screen, u8 bg_number, u8 pal_number, void* palette)
{
	if (screen == 0) {
		vramSetBankE(VRAM_E_LCD);  // On passe en mode LCD pour pouvoir ecrire dessus, on reviendre en palette apres
		//DC_FlushRange(palette, 256);
		//DC_FlushAll();
		DMA_Copy(palette, VRAM_E + (bg_number << 13) + (pal_number << 9), 256, DMA_16NOW);
		//u32 i;
		//for(i = 0; i < 16384; i ++)
		//	VRAM_E[i] = RGB15(15, 15, 15) & (~0x8000);
		//DC_FlushAll();
		//_PA_PrivateCopyPal((u16*)palette, (u16*)((u8*)(VRAM_E) + (bg_number << 13) + (pal_number << 9)));
		//DMA_Copy(palette, (void*) VRAM_E_EXT_PALETTE[pal_number+bg_number*4], 256, DMA_16NOW);
		//while(DMA_CR(3) & DMA_BUSY);
		vramSetBankE(VRAM_E_BG_EXT_PALETTE);
	}
	else {
		vramSetBankH(VRAM_H_LCD);  // On passe en mode LCD pour pouvoir ecrire dessus, on reviendre en palette apres
		//DC_FlushRange(palette, 256);
		//DC_FlushAll();
		DMA_Copy(palette, VRAM_H + (bg_number << 13) + (pal_number << 9), 256, DMA_16NOW);
		//u32 i;
		//for(i = 0; i < 16384; i ++)
		//	VRAM_H[i] = RGB15(15, 15, 15) | 0x8000; //& (~0x8000);

		//DC_FlushAll();
		//_PA_PrivateCopyPal((u16*)palette, (u16*)((u8*)(VRAM_H) + (bg_number << 13) + (pal_number << 9)));
		//DMA_Copy(palette, (void*) VRAM_H_EXT_PALETTE[pal_number+bg_number*4], 256, DMA_16NOW);
		//while(DMA_CR(3) & DMA_BUSY);
		vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
	}
}


#ifdef __cplusplus
}
#endif
