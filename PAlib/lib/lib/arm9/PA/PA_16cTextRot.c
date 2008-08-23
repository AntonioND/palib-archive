
#ifdef __cplusplus
extern "C" {
#endif



#include <PA9.h>
#include "PA_TextBits.h"
#include "16c/all_gfx.h"

	u32 buffer16c[8];
	
void pa_16cTextDecompress(u8 size);
	

void PA_16cRotate8x8(u32 *data, u32 *buffer){
   u8 i;
   
   for(i = 0; i < 8; i++){
      buffer[i] = ((data[7]>>(i*4))&15) | (((data[6]>>(i*4))&15)<<4)  | (((data[5]>>(i*4))&15)<<8)  | (((data[4]>>(i*4))&15)<<12) |
      				(((data[3]>>(i*4))&15)<<16) | (((data[2]>>(i*4))&15)<<20)  | (((data[1]>>(i*4))&15)<<24)  | (((data[0]>>(i*4))&15)<<28);
   }   	
} 

void PA_16c16X16LetterRot(u8 screen, s16 x, s16 y, u8 letter, u8 size, u8 color)
{  
	u16 firstpos = ((letter&31)<<1)+((letter >> 5)<<7);
	PA_16cRotate8x8((c16_tiles[size]+(bittext_maps[size][firstpos]<<3)), buffer16c);
	PA_16c8X8Color(screen, 247-y, x, buffer16c, color);
	PA_16cRotate8x8((c16_tiles[size]+(bittext_maps[size][firstpos+64]<<3)), buffer16c);
	PA_16c8X8Color(screen, 239-y, x, buffer16c, color);
	firstpos ++;
	PA_16cRotate8x8((c16_tiles[size]+(bittext_maps[size][firstpos]<<3)), buffer16c);
	PA_16c8X8Color(screen, 247-y, x+8, buffer16c, color);
	PA_16cRotate8x8((c16_tiles[size]+(bittext_maps[size][firstpos+64]<<3)), buffer16c);
	PA_16c8X8Color(screen, 239-y, x+8, buffer16c, color);
}  

void PA_16c8X8LetterRot(u8 screen, s16 x, s16 y, u8 letter, u8 size, u8 color){  
   PA_16cRotate8x8((c16_tiles[size]+(bittext_maps[size][letter]<<3)), buffer16c);
	PA_16c8X8Color(screen, 247-y, x, buffer16c, color);
}  

void PA_16cLetterRot(u8 screen, s16 x, s16 y, char letter, u8 size, u8 color){
	if (pa_bittextpoliceheight[size]<=8) PA_16c8X8LetterRot(screen, x, y, letter, size, color);
	else PA_16c16X16LetterRot(screen, x, y, letter, size, color);
}

void PA_16cRotate8x8b(u32 *data, u32 *buffer){
   u8 i;
   
   for(i = 0; i < 8; i++){
      buffer[7-i] = ((data[0]>>(i*4))&15) | (((data[1]>>(i*4))&15)<<4)  | (((data[2]>>(i*4))&15)<<8)  | (((data[3]>>(i*4))&15)<<12) |
      				(((data[4]>>(i*4))&15)<<16) | (((data[5]>>(i*4))&15)<<20)  | (((data[6]>>(i*4))&15)<<24)  | (((data[7]>>(i*4))&15)<<28);
   }   	
}   


void PA_16c16X16LetterRot2(u8 screen, s16 x, s16 y, u8 letter, u8 size, u8 color)
{  
	u16 firstpos = ((letter&31)<<1)+((letter >> 5)<<7);
   PA_16cRotate8x8b((c16_tiles[size]+(bittext_maps[size][firstpos]<<3)), buffer16c);
	PA_16c8X8Color(screen, y, 183-x, buffer16c, color);
   PA_16cRotate8x8b((c16_tiles[size]+(bittext_maps[size][firstpos+64]<<3)), buffer16c);	
	PA_16c8X8Color(screen, y+8, 183-x, buffer16c, color);
	firstpos ++;
   PA_16cRotate8x8b((c16_tiles[size]+(bittext_maps[size][firstpos]<<3)), buffer16c);
	PA_16c8X8Color(screen, y, 175-x, buffer16c, color);
   PA_16cRotate8x8b((c16_tiles[size]+(bittext_maps[size][firstpos+64]<<3)), buffer16c);	
	PA_16c8X8Color(screen, y+8, 175-x, buffer16c, color);	
}  

void PA_16c8X8LetterRot2(u8 screen, s16 x, s16 y, u8 letter, u8 size, u8 color){  
   PA_16cRotate8x8b((c16_tiles[size]+(bittext_maps[size][letter]<<3)), buffer16c);
	PA_16c8X8Color(screen, y, 183-x, buffer16c, color);
}  

void PA_16cLetterRot2(u8 screen, s16 x, s16 y, char letter, u8 size, u8 color){
	if (pa_bittextpoliceheight[size]<=8) PA_16c8X8LetterRot2(screen, x, y, letter, size, color);
	else PA_16c16X16LetterRot2(screen, x, y, letter, size, color);
}




s16 PA_16cTextRot(u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u8 color, u8 size, s32 limit){
	s32 i;
	
	pa_16cTextDecompress(size);
		
	PA_DoLetterPos(basex, basey, maxx, maxy, text, color, size, limit);
	
	if(limit > PA_LetterPos.NLetters) limit = PA_LetterPos.NLetters; // Plus short que prévu
		
	if(textinfo.rot == 1) for(i = 0; i < limit; i++) PA_16cLetterRot(screen, PA_LetterPos.Letter[i].X, PA_LetterPos.Letter[i].Y, 
										PA_LetterPos.Letter[i].Letter, PA_LetterPos.Letter[i].Size, PA_LetterPos.Letter[i].Color);																	
	else for(i = 0; i < limit; i++) PA_16cLetterRot2(screen, PA_LetterPos.Letter[i].X, PA_LetterPos.Letter[i].Y, 
										PA_LetterPos.Letter[i].Letter, PA_LetterPos.Letter[i].Size, PA_LetterPos.Letter[i].Color);
	
//	PA_WaitFor(Stylus.Newpress);
	return limit;
}





#ifdef __cplusplus
}
#endif



