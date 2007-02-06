
#ifdef __cplusplus
extern "C" {
#endif

#include "PA9.h"


void PA_CreatePalBright(u16 *pal, u16 *newpal, s8 bright){
u16 i;
s8 r, g, b;
for (i = 0; i < 256; i++){
	r = pal[i]&31; PA_AdjustCol(r, bright);
	g = (pal[i]>>5)&31; PA_AdjustCol(g, bright);
	b = (pal[i]>>10)&31; PA_AdjustCol(b, bright);
	newpal[i] = PA_RGB(r, g, b);
}

}


void PA_CreatePalTransition(u16 *pal, u16 *newpal, s8 level, u8 destr, u8 destg, u8 destb){
u16 i;
s16 r, g, b;
for (i = 0; i < 256; i++){
	r = ((pal[i]&31)*(1-level) + level*destr)>>5;
	g = (((pal[i]>>5)&31)*(1-level) + level*destr)>>5;
	b = (((pal[i]>>10)&31)*(1-level) + level*destr)>>5;
	newpal[i] = PA_RGB(r, g, b);
}

}



void PA_SetBgPalNCol(u8 screen, u8 bg_number, u8 pal_number, u8 color_number, u16 color)
{
	u16 *palcolor;
	if (screen == 0) {
		vramSetBankE(VRAM_E_LCD);  // On passe en mode LCD pour pouvoir ecrire dessus, on reviendre en palette apres
		palcolor = (u16*)(VRAM_E + (bg_number << 12) + (pal_number << 8));
		palcolor[color_number] = color;
		vramSetBankE(VRAM_E_BG_EXT_PALETTE);
	}
	else {
		vramSetBankH(VRAM_H_LCD);  // On passe en mode LCD pour pouvoir ecrire dessus, on reviendre en palette apres
		palcolor = (u16*)(VRAM_H + (bg_number << 12) + (pal_number << 8));
		palcolor[color_number] = color;
		vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
	}
}


void PA_SetSpritePalCol(u8 screen, u8 pal_number, u8 color_number, u16 color)
{
	u16 *palcolor;
	if (screen == 0) {
		vramSetBankE(VRAM_G_LCD);  // On passe en mode LCD pour pouvoir ecrire dessus, on reviendre en palette apres
		palcolor = (u16*)(VRAM_G + (pal_number << 8));
		palcolor[color_number] = color;
		vramSetBankE(VRAM_G_OBJ_EXT_PALETTE);
	}
	else {
		vramSetBankH(VRAM_I_LCD);  // On passe en mode LCD pour pouvoir ecrire dessus, on reviendre en palette apres
		palcolor = (u16*)(VRAM_I + (pal_number << 8));
		palcolor[color_number] = color;
		vramSetBankH(VRAM_I_SUB_SPRITE_EXT_PALETTE);
	}
}



#ifdef __cplusplus
}
#endif
