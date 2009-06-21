

#ifdef __cplusplus
extern "C" {
#endif

#include "arm9/PA_Text.h"
#include "arm9/PA_Draw.h"
#include "arm9/PA_TextBits.h"

#include "16c/all_gfx.h"

	u8 *bit8_tiles[10] = {(u8*)(void*)(c16_text0_Tiles), (u8*)(void*)c16_text1_Tiles, (u8*)(void*)c16_text2_Tiles, (u8*)(void*)c16_text3_Tiles, (u8*)(void*)c16_text4_Tiles};	
	


void Letter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color);
void TranspLetter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color);
void NoLetter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color);
void RotLetter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color);
void RotLetter2(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color);

letterfp letters[5] = {(letterfp)&Letter, (letterfp)&TranspLetter, (letterfp)&NoLetter, (letterfp)&RotLetter, (letterfp)&RotLetter2};



extern inline void PA_8bitDrawTile(u8 screen, u16 x, u16 y, s8 lx, s8 ly, u8 *data, u8 color){
	u16 i, j;
	for (j = 0; j < ly; j++){
		for (i = 0; i < lx; i++){
			PA_Put8bitPixel(screen, x+i, y+j, data[i+(j<<3)]*color);
		}
	}
}	

extern inline void PA_8bitDrawTileTransp(u8 screen, u16 x, u16 y, s8 lx, s8 ly, u8 *data, u8 color){
	u16 i, j;
	for (j = 0; j < ly; j++){
		for (i = 0; i < lx; i++){
			u8 decal = (((x+i)&1)<<3);
			u16 pixel = (data[i+(j<<3)]*color)<<decal;
			if(pixel){
				PA_DrawBg[screen][((y+j) <<7) + ((x+i)>>1)] &= 255<<(8-decal);
				PA_DrawBg[screen][((y+j) <<7) + ((x+i)>>1)] |= pixel;
			}
		}
	}
}	

	

extern inline void PA_8bitDrawTileRot(u8 screen, u16 x, u16 y, s8 lx, s8 ly, u8 *data, u8 color){
	u16 i, j;
	s16 temp = x;
	x = 255- y;
	y = temp;
	
	for (j = 0; j < ly; j++){
		for (i = 0; i < lx; i++){
			PA_Put8bitPixel(screen, x-j, y+i, data[i+(j<<3)]*color);
		}
	}
}	

extern inline void PA_8bitDrawTileRot2(u8 screen, u16 x, u16 y, s8 lx, s8 ly, u8 *data, u8 color){
	u16 i, j;
	s16 temp = 192 - x;
	x = y;
	y = temp;
	
	for (j = 0; j < ly; j++){
		for (i = 0; i < lx; i++){
			PA_Put8bitPixel(screen, x+j, y-i, data[i+(j<<3)]*color);
		}
	}
}	



void NoLetter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color) {

}




void RotLetter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color) {
u8 lx = pa_bittextdefaultsize[size][(u8)letter];
u8 ly = pa_bittextpoliceheight[size];
u8 *data;

if (ly > 8) {
	ly--;
	u16 firstpos = ((letter&31)<<1)+((letter >> 5)<<7);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos]<<6));
	PA_8bitDrawTileRot(screen, x, y, 8, 8, data, color);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos+64]<<6));
	PA_8bitDrawTileRot(screen, x, y+8, 8, ly-8, data, color);	
	firstpos ++;
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos]<<6));
	PA_8bitDrawTileRot(screen, x+8, y, lx-8, 8, data, color);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos+64]<<6));
	PA_8bitDrawTileRot(screen, x+8, y+8, lx-8, ly-8, data, color);			
}
else {
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][(u8)letter]<<6));
	PA_8bitDrawTileRot(screen, x, y, lx, ly, data, color);
}

}


void RotLetter2(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color) {
u8 lx = pa_bittextdefaultsize[size][(u8)letter];
u8 ly = pa_bittextpoliceheight[size];
u8 *data;

if (ly > 8) {
	ly--;
	u16 firstpos = ((letter&31)<<1)+((letter >> 5)<<7);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos]<<6));
	PA_8bitDrawTileRot2(screen, x, y, 8, 8, data, color);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos+64]<<6));
	PA_8bitDrawTileRot2(screen, x, y+8, 8, ly-8, data, color);	
	firstpos ++;
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos]<<6));
	PA_8bitDrawTileRot2(screen, x+8, y, lx-8, 8, data, color);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos+64]<<6));
	PA_8bitDrawTileRot2(screen, x+8, y+8, lx-8, ly-8, data, color);			
}
else {
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][(u8)letter]<<6));
	PA_8bitDrawTileRot2(screen, x, y, lx, ly, data, color);
}

}





void Letter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color) {
u8 lx = pa_bittextdefaultsize[size][(u8)letter];
u8 ly = pa_bittextpoliceheight[size];
u8 *data;

if (ly > 8) {
	u16 firstpos = ((letter&31)<<1)+((letter >> 5)<<7);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos]<<6));
	PA_8bitDrawTile(screen, x, y, 8, 8, data, color);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos+64]<<6));
	PA_8bitDrawTile(screen, x, y+8, 8, ly-8, data, color);	
	firstpos ++;
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos]<<6));
	PA_8bitDrawTile(screen, x+8, y, lx-8, 8, data, color);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos+64]<<6));
	PA_8bitDrawTile(screen, x+8, y+8, lx-8, ly-8, data, color);			
}
else {
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][(u8)letter]<<6));
	PA_8bitDrawTile(screen, x, y, lx, ly, data, color);
}
}









void TranspLetter(u8 size, u8 screen, u16 x, u16 y, char letter, u8 color) {
u8 lx = pa_bittextdefaultsize[size][(u8)letter];
u8 ly = pa_bittextpoliceheight[size];
u8 *data;

if (ly > 8) {
	u16 firstpos = ((letter&31)<<1)+((letter >> 5)<<7);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos]<<6));
	PA_8bitDrawTileTransp(screen, x, y, 8, 8, data, color);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos+64]<<6));
	PA_8bitDrawTileTransp(screen, x, y+8, 8, ly-8, data, color);	
	firstpos ++;
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos]<<6));
	PA_8bitDrawTileTransp(screen, x+8, y, lx-8, 8, data, color);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos+64]<<6));
	PA_8bitDrawTileTransp(screen, x+8, y+8, lx-8, ly-8, data, color);			
}
else {
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][(u8)letter]<<6));
	PA_8bitDrawTileTransp(screen, x, y, lx, ly, data, color);
}


}




void pa_8bitTextDecompress(u8 size){
	s16 i;
	
	if((size < 5)&&(((void*)bittext_tiles_blank[size]) == ((void*)bit8_tiles[size]))){ // Using default font and still compressed
		u32 tilesize = pa_bittextsizes[size]<<3;
		bit8_tiles[size] = (u8*)malloc(tilesize);
		for(i = 0; i < tilesize; i++){
		   bit8_tiles[size][i] = (bittext_tiles_blank[size][i>>3]>>(i&7))&1;
		}   				
	}
}


s16 PA_8bitText(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u8 color, u8 size, u8 transp, s32 limit){
	s32 i;
	
	pa_8bitTextDecompress(size);
		
	PA_DoLetterPos(basex, basey, maxx, maxy, text, color, size, limit);
	
	if(limit > PA_LetterPos.NLetters) limit = PA_LetterPos.NLetters; // Plus short que prévu
	
	for(i = 0; i < limit; i++) letters[transp](PA_LetterPos.Letter[i].Size, screen, PA_LetterPos.Letter[i].X, PA_LetterPos.Letter[i].Y, 
										PA_LetterPos.Letter[i].Letter, PA_LetterPos.Letter[i].Color);

//	PA_WaitFor(Stylus.Newpress);
	return limit;

}


s16 PA_CenterSmartText(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u8 color, u8 size, u8 transp){

	u8 align = textinfo.align;
	textinfo.align = ALIGN_CENTER;

	u16 temp = PA_8bitText(screen, basex, basey, maxx, maxy, text, color, size, transp, 100000);
	textinfo.align = align; 
	
	return temp;
	
}




#ifdef __cplusplus
}
#endif

