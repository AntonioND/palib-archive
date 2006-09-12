#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"

// Quantité de données à charger en fonction de la taille de la map...
u16  bg_sizes[4] = {1024, 2048, 2048, 4096};
u8  bg_place[4] = {1, 2, 2, 4};
u8  rotbg_place[4] = {1, 1, 2, 8};
u16  rotbg_sizes[4] = {1024, 1024, 2048, 8192};

// Pour le parallax scrolling...
s32  PA_parallaxX[2][4];
s32  PA_parallaxY[2][4];

scrollpositions  scrollpos[2][4]; // Pour chaque écran et pour chaque fond :)

u8  charblocks[2][70];  // On met à 0 les emplacements utilisés... pour chaque écran...
u16  tilesetsize[2][4]; // Place utilisée pour chaque tileset
u16  mapsize[2][4]; // Place utilisée pour chaque map
u8  tilesetchar[2][4];  // Emplacement mémoire de chaque tileset
u8  mapchar[2][4];  // Emplacement mémoire de chaque map
u16 tempsize;

extern u16 *PA_DrawBg[2]; // Fond dessinable
u8 charsetstart[2] = {8, 8};

u8 rotbg_size[2][4]; // Background size of each possible rotbg

PA_LoargeMaps PA_LargeMap[2][4];


void PA_ResetBgSys(void) {
u8 i, j;

	PA_SetBgColor(0, 0); PA_SetBgColor(1, 0);  // Black color by default
	PA_SetVideoMode(0, 0);	PA_SetVideoMode(1, 0);

	for (i = 0; i < 4; i++){
		PA_DeleteBg(0, i);
		PA_DeleteBg(1, i);
	}
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 2; j++) {
		//	u8 ;  // On met à 0 les emplacements utilisés... pour chaque écran...
			tilesetsize[j][i] = 0; // Place utilisée pour chaque tileset
			mapsize[j][i] = 0; // Place utilisée pour chaque map
			tilesetchar[j][i] = 0;  // Emplacement mémoire de chaque tileset
			mapchar[j][i] = 0;  // Emplacement mémoire de chaque map
			tempsize = 0;
			PA_parallaxX[j][i] = 0;
			PA_parallaxY[j][i] = 0;
			scrollpos[j][i].infscroll = 0;
			
		}
	}

	for (i = 0; i < 64; i++) 
		for (j = 0; j < 2; j++) 
			charblocks[j][i] = 0;

	// Emplacements pris à la suite, faut pas déborder...
	for (i = 64; i < 70; i++) 
		for (j = 0; j < 2; j++) 
			charblocks[j][i] = 1;

charsetstart[0] = 8; // Par défaut à 8, pour dire de tout utiliser
charsetstart[1] = 8; // Par défaut à 8, pour dire de tout utiliser
}



void PA_LoadBgTilesEx(u8 screen, u8 bg_select, void* bg_tiles, u16 size) {
u16 blocksize = (size + 1023) >> 10;
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
		if (charblocks[screen][(charset << 3) + i] == 0) { // Si on a de la place...
			--tempsize;
		}
	}
	charsetok = !tempsize;  // Si on a trouvé suffisament de blocs, on peut continuer
}

if (!charsetok) { // Si jamais on n'a pas trouvé de créneaux, on affiche un message d'erreur...
	PA_ResetBgSys();

	PA_InitText(1, 0);
	PALETTE[512] = 0; // On met le fond du haut au noir histoire que tout se voit...
	PA_OutputSimpleText(1, 0, 3, "Sorry, there just seems to not be enough place to put all your backgrounds... ou could try loading them in a different order, sometimes it changes a little, but I would advise trying to optimise them by reducing the number of tile...");
	PA_OutputSimpleText(1, 0, 10, "Je suis désolé mon vieux, mais là je n'ai plus assez de place pour charger les tiles en mémoire ! Va falloir virer un fond ou alors réduire le nombre de tiles des autres fonds... Je ne peux rien faire d'autre pour toi");
	
	PA_WaitForVBL();
	while(!Stylus.Newpress) PA_WaitForVBL();

}

	tilesetchar[screen][bg_select] = charset; // On place les tiles à un endroit précis...
	tilesetsize[screen][bg_select] = size;    // On mémorise aussi la taille que ca fait pour pouvoir effacer plus tard...

	DMA_Copy(bg_tiles, (void*)CharBaseBlock(screen, charset), size, DMA_16NOW);
	
	// Save tiles pointer and position in VRAM
	PA_LargeMap[screen][bg_select].Tiles = bg_tiles;
	PA_LargeMap[screen][bg_select].TilePos = (u32*)CharBaseBlock(screen, tilesetchar[screen][bg_select]); // used for tile swapping

	for (i = 0; i < blocksize; i++) charblocks[screen][(charset << 3) + i] = 1;  // Les blocs sont occupés
}


void PA_ReLoadBgTiles(u8 screen, u8 bg_select, void* bg_tiles) {
	s8 charset = tilesetchar[screen][bg_select]; 
	u32 size = tilesetsize[screen][bg_select];    

	DMA_Copy(bg_tiles, (void*)CharBaseBlock(screen, charset), size, DMA_16NOW);
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

	mapchar[screen][bg_select] = charset; // On place la map à un endroit précis...
	mapsize[screen][bg_select] = blocksize;

	DMA_Copy(bg_map, (void*)ScreenBaseBlock(screen, charset), bg_sizes[bg_size], DMA_16NOW);

	for (i = 0; i < blocksize; i++) charblocks[screen][charset + i] = 1;  // Les blocs sont occupés
}




void PA_InitBg(u8 screen, u8 bg_select, u8 bg_size, u8 wraparound, u8 color_mode) {	
	scrollpos[screen][bg_select].infscroll = 0; // Par défaut pas de scrolling infini...
	PA_bgmap[screen][bg_select] = ScreenBaseBlock(screen, mapchar[screen][bg_select]);
	_REG16(REG_BGSCREEN(screen)) |= (0x100 << (bg_select));
	_REG16(REG_BGCNT(screen, bg_select)) = bg_select | (bg_size << 14) |(mapchar[screen][bg_select] << SCREEN_SHIFT) | (wraparound << 13) | (tilesetchar[screen][bg_select] << 2) | (color_mode << 7);
}





void PA_DeleteTiles(u8 screen, u8 bg_select) {

	if (tilesetsize[screen][bg_select]) { // Si y'a un truc, on efface
		u8 i;
		u8 charset = tilesetchar[screen][bg_select];
		u16 size = tilesetsize[screen][bg_select];
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
	if (mapsize[screen][bg_select]) { // Si y'a un truc, on efface
		u8 i;
		u8 charset = mapchar[screen][bg_select];
		u16 blocksize = mapsize[screen][bg_select];

		// On efface les tiles en mémoire
		DMA_Copy((void*)Blank, (void*)CharBaseBlock(screen, charset), (blocksize << 10), DMA_16NOW);

		// On remet le tableau à 0...
		for(i = 0; i < blocksize; i++) {
			charblocks[screen][charset + i] = 0;
		}
	}
}




void PA_LoadRotBgMap(u8 screen, u8 bg_select, void* bg_map, u8 bg_size) {
s16 charset = -1;
u8 charsetok = 0;
u8 blocksize = rotbg_place[bg_size];
u8 i;
u8 tempsize;
rotbg_size[screen][bg_select] = bg_size;  // Save rot bg size (values 0-3 for 128, 256, 512 and 1024)

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

	mapchar[screen][bg_select] = charset; // On place la map à un endroit précis...
	mapsize[screen][bg_select] = blocksize;

	DMA_Copy(bg_map, (void*)ScreenBaseBlock(screen, charset), rotbg_sizes[bg_size], DMA_16NOW);

	for (i = 0; i < blocksize; i++) charblocks[screen][charset + i] = 1;  // Les blocs sont occupés
}









#ifdef __cplusplus
}
#endif



