#include <PA9.h>

PA_BgInfos PA_BgInfo[2][4];

// Quantité de données à charger en fonction de la taille de la map...
u16  bg_sizes[4] = {1024, 2048, 2048, 4096};
u8  bg_place[4] = {1, 2, 2, 4};

// Pour le parallax scrolling...
s32  PA_parallaxX[2][4];
s32  PA_parallaxY[2][4];

scrollpositions  scrollpos[2][4]; // Pour chaque écran et pour chaque fond :)

u8  charblocks[2][70];  // On met à 0 les emplacements libres... pour chaque écran...
//u8  tilesetchar[2][4];  // Emplacement mémoire de chaque tileset
u16 tempsize;

extern u16 *PA_DrawBg[2]; // Fond dessinable
u8 charsetstart[2] = {8, 8};

void PA_ResetBgSysScreen(u8 screen) {
	u8 i;
	PA_SetBgColor(screen, 0);
	PA_SetVideoMode(screen, 0);

	for (i = 0; i < 4; i++) {
		PA_DeleteBg(screen, i);
	}

	for (i = 0; i < 64; i++)
		charblocks[screen][i] = 0;

	// Emplacements pris à la suite, faut pas déborder...
	for (i = 64; i < 70; i++)
		charblocks[screen][i] = 1;

	charsetstart[screen] = 8; // Par défaut à 8, pour dire de tout utiliser
}

void PA_ResetBgSys(void) {
	PA_ResetBgSysScreen(0);
	PA_ResetBgSysScreen(1);
}

void PA_LoadBgTilesEx(u8 screen, u8 bg_select, void* bg_tiles, u32 size) {
	u32 blocksize = (size + 1023) >> 10;
	s8 charset = charsetstart[screen]; // On commence par le dernier... soit le 8ème, et on ira vers le bas
	u8 charsetok = 0;
	u8 i;
	u8 tempsize;
	PA_DeleteTiles(screen, bg_select);

	// On va chercher dans le tableau des emplacements suffisament grand pour mettre les tiles
	while ((charset >= 0) && (!charsetok)) {
		--charset;
		tempsize = blocksize;

		for (i = 0; i < blocksize; i++) {
			//		if (charblocks[screen][(charset << 3) + i] == 0) { // Si on a de la place...
			if (charblocks[screen][(charset << 3) + i] == 0) { // Si on a de la place...
				--tempsize;
			}
		}

		charsetok = !tempsize;  // Si on a trouvé suffisament de blocs, on peut continuer
	}

	if (!charsetok) {
		u8 j;
		u8 usedblocks[64];

		for (i = 0; i < 64; i++) usedblocks[i] = charblocks[screen][i];

		PA_Error(
		    "\n"
		    "   Sorry, but there's not enough\n"
		    "           VRAM to load all your\n"
		    "backgrounds! You must reduce the\n"
		    "         number of tiles of your\n"
		    "backgrounds to fix this problem.\n"
		    "\n"
		    "   Désolé, mais là il n'y a plus\n"
		    "  assez de VRAM pour charger ton\n"
		    " arrière-plans ! Tu dois réduire\n"
		    "       le nombre de tiles de ton\n"
		    "  arrière-plans pour corriger ce\n"
		    "                       problème."
		);
		PA_InitText(0, 0);

		// Display debug info
		for (j = 0; j < 8; j ++)
			for (i = 0; i < 8; i ++)
				PA_OutputText(0, i, j, "%d", usedblocks[i + (j << 3)]);

		PA_OutputText(0, 1, 10, "%d free blocks were needed", blocksize);
		// Hang
		irqDisable(IRQ_ALL);
		irqSet(IRQ_VBLANK, 0);
		irqEnable(IRQ_VBLANK);

		for (;;) swiWaitForVBlank();
	}

	PA_BgInfo[screen][bg_select].TileSetChar = charset; // On place les tiles à un endroit précis...
	PA_BgInfo[screen][bg_select].tilesetsize = size;    // On mémorise aussi la taille que ca fait pour pouvoir effacer plus tard...
	DC_FlushRange((void*)CharBaseBlock(screen, charset), size);
	DMA_Copy(bg_tiles, (void*)CharBaseBlock(screen, charset), size, DMA_16NOW);
	// Save tiles pointer and position in VRAM
	PA_BgInfo[screen][bg_select].Tiles = bg_tiles;
	PA_BgInfo[screen][bg_select].TilePos = (u32*)CharBaseBlock(screen, PA_BgInfo[screen][bg_select].TileSetChar); // used for tile swapping

	for (i = 0; i < blocksize; i++) charblocks[screen][(charset << 3) + i] = 1;  // Les blocs sont occupés
}


void PA_ReLoadBgTiles(u8 screen, u8 bg_select, void* bg_tiles) {
	s8 charset = PA_BgInfo[screen][bg_select].TileSetChar;
	u32 size = PA_BgInfo[screen][bg_select].tilesetsize;
	int i;

	for (i = 0; i < size; i++) {
		((uint16*)CharBaseBlock(screen, charset))[i] = ((uint16*)bg_tiles)[i];
	}
}

void PA_LoadBgMap(u8 screen, u8 bg_select, void* bg_map, u8 bg_size) {
	s16 charset = -1;
	u8 charsetok = 0;
	u8 blocksize = bg_place[bg_size];
	u8 i;
	u8 tempsize;
	PA_DeleteMap(screen, bg_select);

	while ((charset < 31) && (!charsetok)) {
		++charset;
		tempsize = blocksize;

		for (i = 0; i < blocksize; i++) {
			if (charblocks[screen][charset + i] == 0) { // Si on a de la place...
				--tempsize;
			}
		}

		charsetok = !tempsize;  // Si on a trouvé suffisament de blocs, on peut continuer
	}

	PA_BgInfo[screen][bg_select].mapchar = charset; // On place la map à un endroit précis...
	PA_BgInfo[screen][bg_select].mapsize = blocksize;
	DMA_Copy(bg_map, (void*)ScreenBaseBlock(screen, charset), bg_sizes[bg_size], DMA_16NOW);

	for (i = 0; i < blocksize; i++) charblocks[screen][charset + i] = 1;  // Les blocs sont occupés
}

void PA_InitBg(u8 screen, u8 bg_select, u8 bg_size, u8 wraparound, u8 color_mode) {
	scrollpos[screen][bg_select].infscroll = 0; // Par défaut pas de scrolling infini...
	PA_BgInfo[screen][bg_select].Map = ScreenBaseBlock(screen, PA_BgInfo[screen][bg_select].mapchar);

	if (!screen) {
		videoBgEnable(bg_select);
		BGCTRL[bg_select] = bg_select |		// Priority
		                    (bg_size << 14) |				// Size
		                    BG_MAP_BASE(PA_BgInfo[screen][bg_select].mapchar) | // Map base
		                    (wraparound << 13) |			// Wraparound
		                    BG_TILE_BASE(PA_BgInfo[screen][bg_select].TileSetChar) | // Tile base
		                    (color_mode << 7);				// Color mode?
	} else {
		videoBgEnableSub(bg_select);
		BGCTRL_SUB[bg_select] = bg_select |		// Priority
		                        (bg_size << 14) |				// Size
		                        BG_MAP_BASE(PA_BgInfo[screen][bg_select].mapchar) | // Map base
		                        (wraparound << 13) |			// Wraparound
		                        BG_TILE_BASE(PA_BgInfo[screen][bg_select].TileSetChar) | // Tile base
		                        (color_mode << 7);				// Color mode?
	}
}

void PA_DeleteTiles(u8 screen, u8 bg_select) {
	if (PA_BgInfo[screen][bg_select].tilesetsize) { // Si y'a un truc, on efface
		u8 i;
		u8 charset = PA_BgInfo[screen][bg_select].TileSetChar;
		u16 size = PA_BgInfo[screen][bg_select].tilesetsize;
		u16 blocksize = (size + 1023) >> 10;
		dmaFillWords(0, (void*)CharBaseBlock(screen, charset), size*2);

		for (i = 0; i < blocksize; i++) {
			charblocks[screen][(charset << 3) + i] = 0;
		}
	}
}

void PA_DeleteMap(u8 screen, u8 bg_select) {
	if (PA_BgInfo[screen][bg_select].mapsize) { // Si y'a un truc, on efface
		u8 i;
		u8 charset = PA_BgInfo[screen][bg_select].mapchar;
		u16 blocksize = PA_BgInfo[screen][bg_select].mapsize;
		// On efface le carte en mémoire
		dmaFillWords(0, (void*)ScreenBaseBlock(screen, charset), (blocksize << 10)*2);

		// On remet le tableau à 0...
		for (i = 0; i < blocksize; i++) {
			charblocks[screen][charset + i] = 0;
		}
	}
}
