#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"






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


void PA_ResetBgSysScreen(u8 screen){
u8 i;

	PA_SetBgColor(screen, 0); 
	PA_SetVideoMode(screen, 0);

	for (i = 0; i < 4; i++){
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
	for(i = 0; i < blocksize; i++) {
//		if (charblocks[screen][(charset << 3) + i] == 0) { // Si on a de la place...
		if (charblocks[screen][(charset << 3) + i] == 0) { // Si on a de la place...
			--tempsize;
		}
	}
	charsetok = !tempsize;  // Si on a trouvé suffisament de blocs, on peut continuer
}

if (!charsetok) { // Si jamais on n'a pas trouvé de créneaux, on affiche un message d'erreur...
	u8 usedblocks[64];
	for(i = 0; i < 64; i++) usedblocks[i] = charblocks[screen][i];
	PA_ResetBgSys();

	PA_InitText(1, 0);
	BG_PALETTE_SUB[512] = 0; // On met le fond du haut au noir histoire que tout se voit...
	u8 j;
	for(j = 0; j < 8; j++)
		for(i = 0; i < 8; i++)
			PA_OutputText(1, j, i, "%d", usedblocks[i+j*8]);	
			
	PA_OutputText(1, 2, 10, "Needed %d free blocks", blocksize);	
	PA_WaitFor(Stylus.Newpress);	
	PA_OutputSimpleText(1, 0, 12, "Sorry, there just seems to not be enough place to put all your backgrounds... you could try loading them in a different order, sometimes it changes a little, but I would advise trying to optimise them by reducing the number of tile...");
	PA_OutputSimpleText(1, 0, 19, "Je suis désolé mon vieux, mais là je n'ai plus assez de place pour charger les tiles en mémoire ! Va falloir virer un fond ou alors réduire le nombre de tiles des autres fonds...");
	
	PA_WaitFor(Stylus.Newpress);

}


	PA_BgInfo[screen][bg_select].TileSetChar = charset; // On place les tiles à un endroit précis...
	PA_BgInfo[screen][bg_select].tilesetsize = size;    // On mémorise aussi la taille que ca fait pour pouvoir effacer plus tard...

	DMA_Copy(bg_tiles, (void*)CharBaseBlock(screen, charset), size, DMA_16NOW);
	
	// Save tiles pointer and position in VRAM
	PA_BgInfo[screen][bg_select].Tiles = bg_tiles;
	PA_BgInfo[screen][bg_select].TilePos = (u32*)CharBaseBlock(screen, PA_BgInfo[screen][bg_select].TileSetChar); // used for tile swapping

	for (i = 0; i < blocksize; i++) charblocks[screen][(charset << 3) + i] = 1;  // Les blocs sont occupés
	/*
	u8 j;
	for(j = 0; j < 8; j++)
		for(i = 0; i < 8; i++)
			PA_OutputText(1, j, i, "%d", charblocks[screen][i+j*8]);		
	*/
}


void PA_ReLoadBgTiles(u8 screen, u8 bg_select, void* bg_tiles) {
	s8 charset = PA_BgInfo[screen][bg_select].TileSetChar; 
	u32 size = PA_BgInfo[screen][bg_select].tilesetsize;    

	//DMA_Copy(bg_tiles, (void*)CharBaseBlock(screen, charset), size, DMA_16NOW);
	int i;
	for (i=0; i<size; i++){
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

while ((charset < 31 ) && (!charsetok)) {
	++charset;
	tempsize = blocksize;
	for(i = 0; i < blocksize; i++) {
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
	_REG16(REG_BGSCREEN(screen)) |= (0x100 << (bg_select));
	_REG16(REG_BGCNT(screen, bg_select)) = bg_select | (bg_size << 14) |(PA_BgInfo[screen][bg_select].mapchar << SCREEN_SHIFT) | (wraparound << 13) | (PA_BgInfo[screen][bg_select].TileSetChar << 2) | (color_mode << 7);
}





void PA_DeleteTiles(u8 screen, u8 bg_select) {

	if (PA_BgInfo[screen][bg_select].tilesetsize) { // Si y'a un truc, on efface
		u8 i;
		u8 charset = PA_BgInfo[screen][bg_select].TileSetChar;
		u16 size = PA_BgInfo[screen][bg_select].tilesetsize;
		u16 blocksize = (size + 1023) >> 10;

	// On efface les tiles en mémoire
		DMA_Copy((void*)Blank, (void*)CharBaseBlock(screen, charset), size, DMA_16NOW);

	// On remet le tableau à 0...
		for(i = 0; i < blocksize; i++) {
			charblocks[screen][(charset << 3) + i] = 0;
		}
	}
}







void PA_DeleteMap(u8 screen, u8 bg_select) {
	if (PA_BgInfo[screen][bg_select].mapsize) { // Si y'a un truc, on efface
		u8 i;
		u8 charset = PA_BgInfo[screen][bg_select].mapchar;
		u16 blocksize = PA_BgInfo[screen][bg_select].mapsize;

		// On efface les tiles en mémoire
		DMA_Copy((void*)Blank, (void*)CharBaseBlock(screen, charset), (blocksize << 10), DMA_16NOW);

		// On remet le tableau à 0...
		for(i = 0; i < blocksize; i++) {
			charblocks[screen][charset + i] = 0;
		}
	}
}



#ifdef __cplusplus
}
#endif



