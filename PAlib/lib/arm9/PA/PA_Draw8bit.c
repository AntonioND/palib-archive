#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"



void PA_Default8bitInit(u8 screen, u8 bg_priority){
	PA_DeleteBg(screen, 3);
	PA_nBit[screen] = 0; // 8 bit
	
	PA_BGXPA(screen, 3) = 1 << 8;
	PA_BGXPB(screen, 3) = 0;
	PA_BGXPC(screen, 3) = 0;
	PA_BGXPD(screen, 3) = 1 << 8;
	PA_BGXX(screen, 3) = 0;
	PA_BGXY(screen, 3) = 0;	

	_REG16(REG_BGSCREEN(screen)) &= ~7;
	_REG16(REG_BGSCREEN(screen)) |= (0x100 << (3)) | MODE_3_2D;
}


void PA_Init8bitBg(u8 screen, u8 bg_priority){

	PA_Default8bitInit(screen, bg_priority);

	PA_DrawBg[screen] =  (u16*)(0x06000000 + (0x200000 *  screen) + 320 * 256);
	PA_DrawBg32[screen] = (u32*)PA_DrawBg[screen];
	DMA_Copy(Blank, (void*)PA_DrawBg[screen], 256*96, DMA_16NOW);
		
	charsetstart[screen] = 5; // On se réserve la moitié de la mémoire...
	u8 i; for(i = 40; i < 64; i++)	charblocks[screen][i] = 1; // Block la mémoire

	_REG16(REG_BGCNT(screen, 3)) = bg_priority | BG_BMP8_256x256 | BG_BMP_BASE(5);
PA_SetDrawSize(screen, 1);
}



void PA_InitBig8bitBg(u8 screen, u8 bg_priority){

PA_Default8bitInit(screen, bg_priority); 

   PA_DrawBg[screen] =  (u16*)(0x06000000 + (0x200000 *  screen) + 256 * 256);
   PA_DrawBg32[screen] = (u32*)PA_DrawBg[screen];
   DMA_Copy(Blank, (void*)PA_DrawBg[screen], 256*128, DMA_16NOW);
   
	charsetstart[screen] = 4; // On se réserve la moitié de la mémoire...
	u8 i; for(i = 32; i < 64; i++)	charblocks[screen][i] = 1; // Block la mémoire

   _REG16(REG_BGCNT(screen, 3)) = bg_priority | BG_BMP8_256x256 | BG_BMP_BASE(4);
PA_SetDrawSize(screen, 1);
}



u8 pa8bitbuffer[2];

void PA_8bitSwapBuffer(u8 screen){
	pa8bitbuffer[screen] = !pa8bitbuffer[screen];
	PA_DrawBg[screen] =  (u16*)(0x06000000 + (0x200000 *  screen) + (320-pa8bitbuffer[screen]*192) * 256);
	PA_DrawBg32[screen] = (u32*)PA_DrawBg[screen];
	_REG16(REG_BGCNT(screen, 3)) &= ~(BG_BMP_BASE(7));
	_REG16(REG_BGCNT(screen, 3)) |= BG_BMP_BASE(2+pa8bitbuffer[screen]*3);
}

void PA_Init8bitDblBuffer(u8 screen, u8 bg_priority){

	PA_Default8bitInit(screen, bg_priority);

	DMA_Copy(Blank, (0x06000000 + (0x200000 *  screen) + 128 * 256), 256*192, DMA_16NOW);
	PA_DrawBg[screen] =  (u16*)(0x06000000 + (0x200000 *  screen) + 320 * 256);
	PA_DrawBg32[screen] = (u32*)PA_DrawBg[screen];
	
	
	pa8bitbuffer[screen] = 0; // First buffer...
	
		
	charsetstart[screen] = 2; // On se réserve la moitié de la mémoire...
	u8 i; for(i = 8; i < 64; i++)	charblocks[screen][i] = 1; // Block la mémoire

	_REG16(REG_BGCNT(screen, 3)) = bg_priority | BG_BMP8_256x256 | BG_BMP_BASE(2);
	PA_SetDrawSize(screen, 1);
}



#ifdef __cplusplus
}
#endif
