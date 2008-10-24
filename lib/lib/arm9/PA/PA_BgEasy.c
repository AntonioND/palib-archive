#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"







void PA_EasyBgScrollX(u8 screen, u8 bg_number, s32 x){
	if((PA_BgInfo[screen][bg_number].BgMode == BG_TILEDBG)||(PA_BgInfo[screen][bg_number].BgMode == 0)) 
		PA_BGScrollX(screen, bg_number, x&511);
	else PA_InfLargeScrollX(screen, bg_number, x);
}


void PA_EasyBgScrollY(u8 screen, u8 bg_number, s32 y){
	if((PA_BgInfo[screen][bg_number].BgMode == BG_TILEDBG)||(PA_BgInfo[screen][bg_number].BgMode == 0)) 
		PA_BGScrollY(screen, bg_number, y&511);
	else PA_InfLargeScrollY(screen, bg_number, y);
}








void PA_StoreEasyBgInfos(u8 screen, u8 bg_number, u32 Type, u32 Width, u32 Height, void *Tiles, u32 TileSize, void *Map, u32 MapSize, void *Palette){
	PA_BgInfo[screen][bg_number].Infos.Type = Type;
	PA_BgInfo[screen][bg_number].Infos.Width = Width;	
	PA_BgInfo[screen][bg_number].Infos.Height = Height;	
	//PA_OutputText(1, 0,5, "%d - %d   ", PA_BgInfo[screen][bg_number].Infos.Width, PA_BgInfo[screen][bg_number].Infos.Height);
	PA_BgInfo[screen][bg_number].Infos.Tiles = Tiles;
	PA_BgInfo[screen][bg_number].Infos.TileSize = TileSize;
	PA_BgInfo[screen][bg_number].Infos.Map = Map;
	PA_BgInfo[screen][bg_number].Infos.MapSize = MapSize;	
	PA_BgInfo[screen][bg_number].Infos.Palette = Palette;
	
	PA_BgInfo[screen][bg_number].BgMode = Type;
}


	

void PA_EasyBgLoadEx(u8 screen, u8 bg_number, u32 *Infos, void *Tiles, u32 TileSize, void *Map, u32 MapSize, void *Palette)  {  
	PA_StoreEasyBgInfos(screen, bg_number, Infos[0], Infos[1], Infos[2], Tiles, TileSize, Map, MapSize, Palette);
//	PA_BgInfo[screen][bg_number].BgMode = Infos[0];   
	PA_LoadBgPal(screen, bg_number, Palette); 
	PA_DeleteBg(screen, bg_number);
	if (PA_BgInfo[screen][bg_number].BgMode == BG_TILEDBG) {	
		PA_LoadBgTilesEx(screen, bg_number, Tiles, TileSize>>1);
		PA_LoadBgMap(screen, bg_number, Map, PA_GetPAGfxBgSize(Infos[1], Infos[2])); 
		PA_InitBg(screen, bg_number, PA_GetPAGfxBgSize(Infos[1], Infos[2]), 0, 1);
	}
	else{
		PA_BgInfo[screen][bg_number].NTiles = TileSize>>5;
		if (PA_BgInfo[screen][bg_number].BgMode == BG_LARGEMAP) { 
			PA_LoadBgTilesEx(screen, bg_number, Tiles, TileSize>>1);
		}
		else{
			PA_LoadBgTilesEx(screen, bg_number, (void*)Blank, (1008<<5));
		}
		PA_BgInfo[screen][bg_number].Tiles = Tiles;
		PA_LoadBgMap(screen, bg_number, Blank, BG_512X256); 
		PA_InitBg(screen, bg_number, BG_512X256, 0, 1);
		PA_InitLargeBg(screen, bg_number, Infos[1]>> 3, Infos[2]>> 3, Map);
	}
	PA_BGScrollXY(screen, bg_number, 0, 0);
}










#ifdef __cplusplus
}
#endif



