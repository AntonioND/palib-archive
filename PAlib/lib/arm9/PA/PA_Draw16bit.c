#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"


void PA_Default16bitInit(u8 screen, u8 bg_priority){
	PA_DeleteBg(screen, 3);
	PA_nBit[screen] = 1; // 16 bit
	
	PA_BGXPA(screen, 3) = 1 << 8;
	PA_BGXPB(screen, 3) = 0;
	PA_BGXPC(screen, 3) = 0;
	PA_BGXPD(screen, 3) = 1 << 8;
	PA_BGXX(screen, 3) = 0;
	PA_BGXY(screen, 3) = 0;	

	_REG16(REG_BGSCREEN(screen)) &= ~7;
	_REG16(REG_BGSCREEN(screen)) |= (0x100 << (3)) | MODE_3_2D;
}

void PA_Init16bitBg(u8 screen, u8 bg_priority){

	PA_Default16bitInit(screen, bg_priority);

	PA_DrawBg[screen] =  (u16*)(0x06000000 + (0x200000 *  screen) + 128 * 256);
	PA_DrawBg32[screen] = (u32*)PA_DrawBg[screen];
	DMA_Copy(Blank, (void*)PA_DrawBg[screen], 256*192, DMA_16NOW);
		
	//this is just used so we can write red color bits to one frame and green to the 
	//other
	
	charsetstart[screen] = 2; // On se réserve la moitié de la mémoire...
	charblocks[screen][16] = 1; // Block la mémoire

	_REG16(REG_BGCNT(screen, 3)) = bg_priority | BG_BMP16_256x256 | BG_BMP_BASE(2);

	PA_SetDrawSize(screen, 1);
}


u8 pa16bitbuffer[2];



void PA_Init16bitDblBuffer(u8 screen, u8 bg_priority){

	PA_Default16bitInit(screen, bg_priority);

	DMA_Copy(Blank, (0x06000000 + (0x200000 *  screen)), 256*384, DMA_16NOW);
	PA_DrawBg[screen] =  (u16*)(0x06000000 + (0x200000 *  screen) + 192 * 512);
	PA_DrawBg32[screen] = (u32*)PA_DrawBg[screen];
		
	pa16bitbuffer[screen] = 0; // First buffer...
	charsetstart[screen] = 0; // On se réserve la moitié de la mémoire...

	_REG16(REG_BGCNT(screen, 3)) = bg_priority | BG_BMP16_256x256 | BG_BMP_BASE(0);
	PA_SetDrawSize(screen, 1);
	vramSetMainBanks(VRAM_A_MAIN_BG_0x06000000,VRAM_B_MAIN_BG_0x06020000,VRAM_C_SUB_BG,VRAM_D_SUB_SPRITE);
}

void PA_16bitSwapBuffer(u8 screen){
	pa16bitbuffer[screen] = !pa16bitbuffer[screen];
	PA_DrawBg[screen] =  (u16*)(0x06000000 + (0x200000 *  screen) + (192-pa16bitbuffer[screen]*192) * 512);
	PA_DrawBg32[screen] = (u32*)PA_DrawBg[screen];
	_REG16(REG_BGCNT(screen, 3)) &= ~(BG_BMP_BASE(7));
	_REG16(REG_BGCNT(screen, 3)) |= BG_BMP_BASE(pa16bitbuffer[screen]*6);
}


#ifdef __cplusplus
}
#endif
