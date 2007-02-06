#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"


EasyBgPixels PA_EasyBgPixel[6] = {PANoPixel, PANoPixel, PAEasyBgGetPixelTiled, PANoPixel, PAEasyBgGetPixelLarge, PAEasyBgGetPixelInf}; // Background types


u8 PAEasyBgGetPixelTiled(u8 screen, u8 bg_number, s32 x, s32 y){
	// Adjust X/Y values
	x += PA_BgInfo[screen][bg_number].ScrollX; x &= (PA_BgInfo[screen][bg_number].Infos.Width-1);
	y += PA_BgInfo[screen][bg_number].ScrollY; y %= (PA_BgInfo[screen][bg_number].Infos.Height);
	

		
	s32 mappos;
	
	if ((x <= 256)&&(y <= 256)) {// Normal default size
		mappos = (x>>3) + ((y>>3)*32); // Adjust position in map
	}
	else if ((x > 256)&&(y <= 256)){
		mappos = 32*32 + ((x&255)>>3) + ((y>>3)*32); // Adjust position in map
	}
	else if ((x <= 256)&&(y>256)){ // Tall
		mappos = 64*32 + (x>>3) + (((y-256)>>3)*32); // Adjust position in map	
	}
	else{
		mappos = 96*32 + ((x-256)>>3) + (((y-256)>>3)*32); // Adjust position in map			
	
	}
	
	
	 	
	u16 *Map = (u16*)PA_BgInfo[screen][bg_number].Infos.Map;
		
	s32 tilepix = (Map[mappos]&1023)<<6;
	u8 hflip = (Map[mappos]>>10)&1;
	u8 vflip = (Map[mappos]>>11)&1;
	
	x&=7; y&=7; // Adjust in tile...
	if (hflip) x = 7-x;   if (vflip) y = 7-y;   // Adjust flips...
	
	u8 *Tiles = (u8*)PA_BgInfo[screen][bg_number].Infos.Tiles;

	return Tiles[tilepix+x+(y<<3)];
}



u8 PAEasyBgGetPixelLarge(u8 screen, u8 bg_number, s32 x, s32 y){
	// Adjust X/Y values
	x += PA_BgInfo[screen][bg_number].ScrollX; x = PA_Modulo(x, PA_BgInfo[screen][bg_number].Infos.Width);
	y += PA_BgInfo[screen][bg_number].ScrollY; y = PA_Modulo(y, PA_BgInfo[screen][bg_number].Infos.Height);
	s32 mappos = (x>>3) + ((y>>3)*(PA_BgInfo[screen][bg_number].Infos.Width>>3)); // Adjust position in map			
	 	
	u16 *Map = (u16*)PA_BgInfo[screen][bg_number].Infos.Map;
		
	s32 tilepix = (Map[mappos]&1023)<<6;
	u8 hflip = (Map[mappos]>>10)&1;
	u8 vflip = (Map[mappos]>>11)&1;
	
	x&=7; y&=7; // Adjust in tile...
	if (hflip) x = 7-x;   if (vflip) y = 7-y;   // Adjust flips...
	
	u8 *Tiles = (u8*)PA_BgInfo[screen][bg_number].Infos.Tiles;

	return Tiles[tilepix+x+(y<<3)];
}



u8 PANoPixel(u8 screen, u8 bg_number, s32 x, s32 y){
	return 0;
}



u8 PAEasyBgGetPixelInf(u8 screen, u8 bg_number, s32 x, s32 y){
	// Adjust X/Y values
	x += PA_BgInfo[screen][bg_number].ScrollX; x = PA_Modulo(x, PA_BgInfo[screen][bg_number].Infos.Width);
	y += PA_BgInfo[screen][bg_number].ScrollY; y = PA_Modulo(y, PA_BgInfo[screen][bg_number].Infos.Height);
	
	s32 mappos = (x>>3) + ((y>>3)*(PA_BgInfo[screen][bg_number].Infos.Width>>3)); // Adjust position in map			
	 	
	u32 *Map = (u32*)PA_BgInfo[screen][bg_number].Infos.Map;
		
	s32 tilepix = (Map[mappos]&INF_JUSTTILE)<<6;
	u8 hflip = (Map[mappos]>>29)&1;
	u8 vflip = (Map[mappos]>>30)&1;
	
	x&=7; y&=7; // Adjust in tile...
	if (hflip) x = 7-x;   if (vflip) y = 7-y;   // Adjust flips...
	
	u8 *Tiles = (u8*)PA_BgInfo[screen][bg_number].Infos.Tiles;

	return Tiles[tilepix+x+(y<<3)];
}







#ifdef __cplusplus
}
#endif



