

#ifdef __cplusplus
extern "C" {
#endif

#include "arm9/PA_Text.h"
#include "arm9/PA_Draw.h"
#include "arm9/PA_TextBits.h"

#include "16c/all_gfx.h"




void Letter16bit(u8 size, u8 screen, u16 x, u16 y, char letter, u16 color);
void TranspLetter16bit(u8 size, u8 screen, u16 x, u16 y, char letter, u16 color);
void NoLetter16bit(u8 size, u8 screen, u16 x, u16 y, char letter, u16 color);
void RotLetter16bit(u8 size, u8 screen, u16 x, u16 y, char letter,u16 color);
void RotLetter216bit(u8 size, u8 screen, u16 x, u16 y, char letter,u16 color);

letterfp16bit letters16bit[5] = {(letterfp16bit)&Letter16bit, (letterfp16bit)&TranspLetter16bit, (letterfp16bit)&NoLetter16bit, (letterfp16bit)&RotLetter16bit, (letterfp16bit)&RotLetter216bit};



extern inline void PA_16bitDrawTile(u8 screen, u16 x, u16 y, s8 lx, s8 ly, u8 *data, u16 color){
	u16 i, j;
	for (j = 0; j < ly; j++){
		for (i = 0; i < lx; i++){
			PA_Put16bitPixel(screen, x+i, y+j, data[i+(j<<3)]*color);
		}
	}
}	

extern inline void PA_16bitDrawTileTransp(u8 screen, u16 x, u16 y, s8 lx, s8 ly, u8 *data, u16 color){
	u16 i, j;
	for (j = 0; j < ly; j++){
		for (i = 0; i < lx; i++){
			if(data[i+(j<<3)]) PA_Put16bitPixel(screen, x+i, y+j, data[i+(j<<3)]*color);
		}
	}
}	

	

extern inline void PA_16bitDrawTileRot(u8 screen, u16 x, u16 y, s8 lx, s8 ly, u8 *data, u16 color){
	u16 i, j;
	s16 temp = x;
	x = 255- y;
	y = temp;
	
	for (j = 0; j < ly; j++){
		for (i = 0; i < lx; i++){
			PA_Put16bitPixel(screen, x-j, y+i, data[i+(j<<3)]*color);
		}
	}
}	

extern inline void PA_16bitDrawTileRot2(u8 screen, u16 x, u16 y, s8 lx, s8 ly, u8 *data, u16 color){
	u16 i, j;
	s16 temp = 192 - x;
	x = y;
	y = temp;
	
	for (j = 0; j < ly; j++){
		for (i = 0; i < lx; i++){
			PA_Put16bitPixel(screen, x+j, y-i, data[i+(j<<3)]*color);
		}
	}
}	



void NoLetter16bit(u8 size, u8 screen, u16 x, u16 y, char letter, u16 color) {

}




void RotLetter16bit(u8 size, u8 screen, u16 x, u16 y, char letter, u16 color) {
u8 lx = pa_bittextdefaultsize[size][(u8)letter];
u8 ly = pa_bittextpoliceheight[size];
u8 *data;

if (ly > 8) {
	ly--;
	u16 firstpos = ((letter&31)<<1)+((letter >> 5)<<7);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos]<<6));
	PA_16bitDrawTileRot(screen, x, y, 8, 8, data, color);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos+64]<<6));
	PA_16bitDrawTileRot(screen, x, y+8, 8, ly-8, data, color);	
	firstpos ++;
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos]<<6));
	PA_16bitDrawTileRot(screen, x+8, y, lx-8, 8, data, color);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos+64]<<6));
	PA_16bitDrawTileRot(screen, x+8, y+8, lx-8, ly-8, data, color);			
}
else {
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][(u8)letter]<<6));
	PA_16bitDrawTileRot(screen, x, y, lx, ly, data, color);
}

}


void RotLetter216bit(u8 size, u8 screen, u16 x, u16 y, char letter, u16 color) {
u8 lx = pa_bittextdefaultsize[size][(u8)letter];
u8 ly = pa_bittextpoliceheight[size];
u8 *data;

if (ly > 8) {
	ly--;
	u16 firstpos = ((letter&31)<<1)+((letter >> 5)<<7);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos]<<6));
	PA_16bitDrawTileRot2(screen, x, y, 8, 8, data, color);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos+64]<<6));
	PA_16bitDrawTileRot2(screen, x, y+8, 8, ly-8, data, color);	
	firstpos ++;
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos]<<6));
	PA_16bitDrawTileRot2(screen, x+8, y, lx-8, 8, data, color);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos+64]<<6));
	PA_16bitDrawTileRot2(screen, x+8, y+8, lx-8, ly-8, data, color);			
}
else {
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][(u8)letter]<<6));
	PA_16bitDrawTileRot2(screen, x, y, lx, ly, data, color);
}

}





void Letter16bit(u8 size, u8 screen, u16 x, u16 y, char letter, u16 color) {
u8 lx = pa_bittextdefaultsize[size][(u8)letter];
u8 ly = pa_bittextpoliceheight[size];
u8 *data;

if (ly > 8) {
	u16 firstpos = ((letter&31)<<1)+((letter >> 5)<<7);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos]<<6));
	PA_16bitDrawTile(screen, x, y, 8, 8, data, color);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos+64]<<6));
	PA_16bitDrawTile(screen, x, y+8, 8, ly-8, data, color);	
	firstpos ++;
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos]<<6));
	PA_16bitDrawTile(screen, x+8, y, lx-8, 8, data, color);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos+64]<<6));
	PA_16bitDrawTile(screen, x+8, y+8, lx-8, ly-8, data, color);			
}
else {
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][(u8)letter]<<6));
	PA_16bitDrawTile(screen, x, y, lx, ly, data, color);
}
}









void TranspLetter16bit(u8 size, u8 screen, u16 x, u16 y, char letter, u16 color) {
u8 lx = pa_bittextdefaultsize[size][(u8)letter];
u8 ly = pa_bittextpoliceheight[size];
u8 *data;

if (ly > 8) {
	u16 firstpos = ((letter&31)<<1)+((letter >> 5)<<7);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos]<<6));
	PA_16bitDrawTileTransp(screen, x, y, 8, 8, data, color);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos+64]<<6));
	PA_16bitDrawTileTransp(screen, x, y+8, 8, ly-8, data, color);	
	firstpos ++;
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos]<<6));
	PA_16bitDrawTileTransp(screen, x+8, y, lx-8, 8, data, color);
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][firstpos+64]<<6));
	PA_16bitDrawTileTransp(screen, x+8, y+8, lx-8, ly-8, data, color);			
}
else {
	data = (u8*)(bit8_tiles[size]+(bittext_maps[size][(u8)letter]<<6));
	PA_16bitDrawTileTransp(screen, x, y, lx, ly, data, color);
}


}




void pa_16bitTextDecompress(u8 size){
	s16 i;
	
	if((size < 5)&&(((void*)bittext_tiles_blank[size]) == ((void*)bit8_tiles[size]))){ // Using default font and still compressed
		u32 tilesize = pa_bittextsizes[size]<<3;
		bit8_tiles[size] = (u8*)malloc(tilesize);
		for(i = 0; i < tilesize; i++){
		   bit8_tiles[size][i] = (bittext_tiles_blank[size][i>>3]>>(i&7))&1;
		}   				
	}
}


s16 PA_16bitText(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u16 color, u8 size, u8 transp, s32 limit){
	s32 i;
	
	pa_16bitTextDecompress(size);
		
	PA_DoLetterPos(basex, basey, maxx, maxy, text, color, size, limit);
	
	if(limit > PA_LetterPos.NLetters) limit = PA_LetterPos.NLetters; // Plus short que prévu
	
	for(i = 0; i < limit; i++) letters16bit[transp](PA_LetterPos.Letter[i].Size, screen, PA_LetterPos.Letter[i].X, PA_LetterPos.Letter[i].Y, 
										PA_LetterPos.Letter[i].Letter, PA_LetterPos.Letter[i].Color);

//	PA_WaitFor(Stylus.Newpress);
	return limit;

}






#ifdef __cplusplus
}
#endif

