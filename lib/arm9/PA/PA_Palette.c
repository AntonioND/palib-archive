#include "PA9.h"

bool PA_ExtPal[2][2]; // 0 si pas étendu, 1 sinon


void PA_InitSpriteExtPal(void) {
	// Palettes étendus pour les 256 couleurs...
	vramSetBankG(VRAM_G_OBJ_EXT_PALETTE);
	vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);
	PA_ExtPal[0][0] = 1;
	PA_ExtPal[0][1] = 1;
	DISPLAY_CR |= 1 << 31;
	SUB_DISPLAY_CR |= 1 << 31;

	PA_LoadSpriteExtPal(0, 0, (void*)PAL_SPRITE0);
	PA_LoadSpriteExtPal(1, 0, (void*)PAL_SPRITE1);
}



void PA_LoadSpriteExtPal(bool screen, u16 palette_number, void* palette){
	if (screen == 0) {
		vramSetBankG(VRAM_G_LCD);  // On passe en mode LCD pour pouvoir ecrire dessus, on reviendre en palette apres
		DMA_Copy(palette, VRAM_G + (palette_number << 9), 256, DMA_16NOW);


		vramSetBankG(VRAM_G_OBJ_EXT_PALETTE);
	}
	else {
		vramSetBankI(VRAM_I_LCD);  // On passe en mode LCD pour pouvoir ecrire dessus, on reviendre en palette apres
		DMA_Copy(palette, VRAM_I + (palette_number << 9), 256, DMA_16NOW);
		vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);
	}
}


void PA_InitBgExtPal(void) {
	// Palettes étendus pour les 256 couleurs...
	vramSetBankF(VRAM_F_BG_EXT_PALETTE);
	vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
	PA_ExtPal[1][0] = 1;
	PA_ExtPal[1][1] = 1;
	DISPLAY_CR |= 1 << 30;
	SUB_DISPLAY_CR |= 1 << 30;

	PA_LoadBgExtPal(0, 0, (void*)PAL_BG0);
	PA_LoadBgExtPal(1, 0, (void*)PAL_BG1);
}



void PA_LoadBgExtPal(bool screen, u16 palette_number, void* palette){
	if (screen == 0) {
		vramSetBankF(VRAM_F_LCD);  // On passe en mode LCD pour pouvoir ecrire dessus, on reviendre en palette apres
		DMA_Copy(palette, VRAM_F + (palette_number << 9), 256, DMA_16NOW);

		vramSetBankF(VRAM_F_BG_EXT_PALETTE);
	}
	else {
		vramSetBankH(VRAM_H_LCD);  // On passe en mode LCD pour pouvoir ecrire dessus, on reviendre en palette apres
		DMA_Copy(palette, VRAM_H + (palette_number << 9), 256, DMA_16NOW);
		vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
	}
}

