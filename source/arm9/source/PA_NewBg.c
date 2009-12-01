#include <PA9.h>

void PA_LoadBackground(u8 screen, u8 bg_select, const PA_BgStruct* bg){
	int tempsize;

	switch(bg->BgType){
		case PA_BgNormal:
			PA_StoreEasyBgInfos(screen, bg_select, BG_TILEDBG, bg->width,
				bg->height, (void*) bg->BgTiles, bg->BgTiles_size >> 1,
				(void*) bg->BgMap, bg->BgMap_size >> 1, (void*) bg->BgPalette);

			PA_LoadBgPal(screen, bg_select, (void*) bg->BgPalette);

			tempsize = PA_GetPAGfxBgSize(bg->width, bg->height);

			PA_DeleteBg(screen, bg_select);
			PA_LoadBgTilesEx(screen, bg_select, (void*) bg->BgTiles, bg->BgTiles_size >> 1);
			PA_LoadBgMap(screen, bg_select, (void*) bg->BgMap, tempsize);

			PA_InitBg(screen, bg_select, tempsize, 0, 1);
			PA_BGScrollXY(screen, bg_select, 0, 0);
			break;
			
		case PA_BgLarge:
			PA_StoreEasyBgInfos(screen, bg_select, BG_LARGEMAP, bg->width,
				bg->height, (void*) bg->BgTiles, bg->BgTiles_size >> 1,
				(void*) bg->BgMap, bg->BgMap_size >> 1, (void*) bg->BgPalette);

			PA_LoadBgPal(screen, bg_select, (void*) bg->BgPalette);

			PA_BgInfo[screen][bg_select].NTiles = bg->BgTiles_size >> 6;

			PA_DeleteBg(screen, bg_select);
			PA_LoadBgTilesEx(screen, bg_select, (void*) bg->BgTiles, bg->BgTiles_size >> 1);
			PA_LoadBgMap(screen, bg_select, NULL, BG_512X256);

			PA_InitBg(screen, bg_select, BG_512X256, 0, 1);
			PA_BGScrollXY(screen, bg_select, 0, 0);

			PA_InitLargeBg(screen, bg_select, bg->width >> 3, bg->height >> 3, (void*) bg->BgMap);
			break;

		case PA_BgUnlimited:
			PA_StoreEasyBgInfos(screen, bg_select, BG_INFINITEMAP, bg->width,
				bg->height, (void*) bg->BgTiles, bg->BgTiles_size >> 1,
				(void*) bg->BgMap, bg->BgMap_size >> 1, (void*) bg->BgPalette);

			PA_LoadBgPal(screen, bg_select, (void*) bg->BgPalette);

			PA_BgInfo[screen][bg_select].NTiles = bg->BgTiles_size >> 6;

			PA_DeleteBg(screen, bg_select);
			PA_LoadBgTilesEx(screen, bg_select, (void*)NULL, (1008<<5));
			PA_BgInfo[screen][bg_select].Tiles = (void*) bg->BgTiles;
			PA_LoadBgMap(screen, bg_select, NULL, BG_512X256);
			PA_InitBg(screen, bg_select, BG_512X256, 0, 1);
			PA_BGScrollXY(screen, bg_select, 0, 0);

			PA_InitLargeBg(screen, bg_select, bg->width >> 3, bg->height >> 3, (void*) bg->BgMap);
			break;

		case PA_BgRot:
			PA_Assert(bg_select >= 2, "Invalid background");
			PA_Assert(PA_GetVideoMode(screen) != 0, "Invalid video mode");

			PA_Load8bitBgPal(screen, (void*) bg->BgPalette);

			tempsize = PA_GetPAGfxRotBgSize(bg->width);

			PA_DeleteBg(screen, bg_select);
			PA_LoadBgTilesEx(screen, bg_select, (void*) bg->BgTiles, bg->BgTiles_size >> 1);
			PA_LoadRotBgMap(screen, bg_select, (void*) bg->BgMap, tempsize);

			PA_InitBg(screen, bg_select, tempsize, 0, 1);
			PA_SetBgRot(screen, bg_select, 0, 0, 0, 0, 0, 256);
			break;
	}
}

void PA_LoadText(u8 screen, u8 bg_select, const PA_BgStruct* font){
	PA_Assert(font->BgType == PA_BgNormal, "Invalid font type");
	PAbgtext[screen] = bg_select;

	PA_LoadBgPal(screen, bg_select, (void*) font->BgPalette);

	PA_DeleteBg(screen, bg_select);
	PA_LoadBgTilesEx(screen, bg_select, (void*) font->BgTiles, font->BgTiles_size >> 1);
	PA_LoadBgMap(screen, bg_select, NULL, BG_256X256);

	PA_InitBg(screen, bg_select, BG_256X256, 0, 1);
	PA_BGScrollXY(screen, bg_select, 0, 0);

	PA_textmap[screen] = (u16*) font->BgMap;
	PA_textpal[screen] = (u16*) font->BgPalette;
	PAtext_pal[screen] = 0;
}

void PA_LoadKeyboard(u8 bg_select, const PA_BgStruct* keybd){
	PA_Assert(keybd->BgType == PA_BgLarge, "Invalid keyboard type");
	PA_LoadBgPal(keyb_screen, bg_select, (void*) keybd->BgPalette);

	PA_DeleteBg(keyb_screen, bg_select);
	PA_LoadBgTilesEx(keyb_screen, bg_select, (void*) keybd->BgTiles, keybd->BgTiles_size >> 1);
	PA_LoadBgMap(keyb_screen, bg_select, (void*) keybd->BgMap, BG_256X512);

	PA_InitBg(keyb_screen, bg_select, BG_256X512, 0, 1);
	PA_BGScrollXY(keyb_screen, bg_select, 0, 0);

	PA_Keyboard_Struct.Bg = bg_select;
	PA_Keyboard_Struct.Type = 0;
	PA_Keyboard_Struct.Repeat = 0;
	PA_Keyboard_Struct.Custom = 1;
	PA_BgInfo[keyb_screen][bg_select].Map = (u32) keybd->BgMap;
}

void PA_AddBitmapFont(int slot, const PA_BgStruct* font){
	PA_Assert(font->BgType == PA_Font8bit, "Invalid font type");

	bittext_maps[slot] = (u16*) font->BgMap;
	bit8_tiles[slot] = (u8*) font->BgTiles;
	pa_bittextdefaultsize[slot] = (u8*) font->FontSizes;
	pa_bittextpoliceheight[slot] = font->FontHeight;
}

void PA_Add16cFont(int slot, const PA_BgStruct* font){
	PA_Assert(font->BgType == PA_Font4bit, "Invalid font type");

	bittext_maps[slot] = (u16*) font->BgMap;
	c16_tiles[slot] = (u32*) font->BgTiles;
	pa_bittextdefaultsize[slot] = (u8*) font->FontSizes;
	pa_bittextpoliceheight[slot] = font->FontHeight;
}
