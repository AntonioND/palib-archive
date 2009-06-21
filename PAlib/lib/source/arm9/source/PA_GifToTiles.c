
#ifdef __cplusplus
extern "C" {
#endif

#include <PA9.h>

extern u16 pa_giftotexcolor;


u8* PA_GifToTiles(void *gif, u16 *temppal){

	u8 zero = 0;
	u16 width = PA_GetGifWidth(gif);
	u16 height = PA_GetGifHeight(gif);
	
	u8 *decodgif = (u8*)malloc(width*height);
	u8 *newtiles = (u8*)malloc(width*height);
	
	DMA_Force(zero, decodgif, (width*height)>>1, DMA_16NOW); // Clear out the gif
	DecodeGif((const u8*)gif, decodgif, temppal, 0, width);
	
	s32 i, j, tile;
	tile = 0; 
	s32 tempx, tempy;
	tempx = 0; tempy = 0;
	u16 temp;
	
	u16 tilemax = (width*height)>>6;
	
	for (tile = 0; tile < tilemax; tile++){
		//tileexists[0][tile] = 0;
		for (i = 0; i < 8; i++){ // put the right transp color	
			for (j = 0; j < 8; j++) {
				temp = decodgif[tempx + i + ((tempy+j)*width)];
				/*if (temp == 0) temp = pal0;
				else if (temp == pal0) temp = 0;*/
				newtiles[(tile<<6) + i + (j<<3)] = temp;
				//tileexists[0][tile] |= temp;
			}
		}
		tempx += 8; 
		if (tempx >= width) {
			tempy += 8;
			tempx = 0;
		}
	}
	
	// Scan palette for magenta...
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
		      if(newtiles[pos] == 0){
		         newtiles[pos] = magenta;
		      }   
		      else if(newtiles[pos] == magenta){
		         newtiles[pos] = 0;
		      }   			      
		      pos++;
		   }
		}	
	}					   
	
	free(decodgif); // free the malloc
	
	
	return (newtiles);

}



#ifdef __cplusplus
}
#endif



