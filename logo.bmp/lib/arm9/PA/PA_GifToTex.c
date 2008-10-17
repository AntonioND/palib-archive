
#ifdef __cplusplus
extern "C" {
#endif

#include <PA9.h>


extern u16 pa_giftotexcolor;
const unsigned char gif_Texture[262144] __attribute__ ((aligned (4)));	//512x512 texture for the Gif.


u8* PA_GifToTex(void *gif, u16 *temppal){

	s16 i, j;
	
	u16 zero = 0;
	u16 width = PA_GetGifWidth(gif);
	u16 height = PA_GetGifHeight(gif);
	
	u8 *decodgif = (u8*)malloc(width*height);
	
	DMA_Force(zero, decodgif, (width*height)>>1, DMA_16NOW); // Blank out the gif
	DecodeGif((const u8*)gif, decodgif, temppal, 0, width);
	
	s16 magenta = 0;
	u32 pos = 0;
	while((magenta < 255) && (temppal[magenta] != pa_giftotexcolor)) {
	   magenta++;
	}   
	//Switch palette entry 0 with palette entry magenta.
	u16 tempcolor = temppal[magenta];
	temppal[magenta] = temppal[0];
	temppal[0] = tempcolor;
	
	if(magenta != 0){
		for(j = 0; j < height; j++){
		   pos = j*width;
		   for(i = 0; i < width; i++){
		      if(decodgif[pos] == 0){
		         decodgif[pos] = magenta;
		      }   
		      else if(decodgif[pos] == magenta){
		         decodgif[pos] = 0;
		      }   			      
		      pos++;
		   }
		}	
	}					   
		

	return (decodgif);

}



u16 PA_3DCreateTexFromGif(void *gif, u8 palette){
   vramSetBankF(VRAM_F_LCD);  
		u16 *pal = (u16*)(VRAM_F + (palette << 8));
	   u8 *giftemp = PA_GifToTex((void*)gif, pal);  // Load pal directly
		// Copy the Gif texture into an aligned texture.
		memcpy((void*)gif_Texture, giftemp, PA_GetGifWidth(gif) * PA_GetGifHeight(gif));	   
	vramSetBankF(VRAM_F_TEX_PALETTE);   
	
   u16 ntex = PA_3DCreateTex((void*)gif_Texture, PA_GetGifWidth(gif), PA_GetGifHeight(gif), TEX_256COL);	
   
   free(giftemp);
   
   return ntex;
}   







#ifdef __cplusplus
}
#endif



