#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"

PA_BgInfos PA_BgInfo[2][4];

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
//u8  tilesetchar[2][4];  // Emplacement mémoire de chaque tileset
u16 tempsize;

extern u16 *PA_DrawBg[2]; // Fond dessinable
u8 charsetstart[2] = {8, 8};

u8 rotbg_size[2][4]; // Background size of each possible rotbg


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
			PA_BgInfo[j][i].tilesetsize = 0; // Place utilisée pour chaque tileset
			PA_BgInfo[j][i].mapsize = 0; // Place utilisée pour chaque map
			PA_BgInfo[j][i].TileSetChar = 0;  // Emplacement mémoire de chaque tileset
			PA_BgInfo[j][i].mapchar = 0;  // Emplacement mémoire de chaque map
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



void PA_LoadBgTilesEx(u8 screen, u8 bg_select, void* bg_tiles, u32 size) {
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

	PA_BgInfo[screen][bg_select].TileSetChar = charset; // On place les tiles à un endroit précis...
	PA_BgInfo[screen][bg_select].tilesetsize = size;    // On mémorise aussi la taille que ca fait pour pouvoir effacer plus tard...

	DMA_Copy(bg_tiles, (void*)CharBaseBlock(screen, charset), size, DMA_16NOW);
	
	// Save tiles pointer and position in VRAM
	PA_BgInfo[screen][bg_select].Tiles = bg_tiles;
	PA_BgInfo[screen][bg_select].TilePos = (u32*)CharBaseBlock(screen, PA_BgInfo[screen][bg_select].TileSetChar); // used for tile swapping

	for (i = 0; i < blocksize; i++) charblocks[screen][(charset << 3) + i] = 1;  // Les blocs sont occupés
}


void PA_ReLoadBgTiles(u8 screen, u8 bg_select, void* bg_tiles) {
	s8 charset = PA_BgInfo[screen][bg_select].TileSetChar; 
	u32 size = PA_BgInfo[screen][bg_select].tilesetsize;    

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

	PA_BgInfo[screen][bg_select].mapchar = charset; // On place la map à un endroit précis...
	PA_BgInfo[screen][bg_select].mapsize = blocksize;

	DMA_Copy(bg_map, (void*)ScreenBaseBlock(screen, charset), rotbg_sizes[bg_size], DMA_16NOW);

	for (i = 0; i < blocksize; i++) charblocks[screen][charset + i] = 1;  // Les blocs sont occupés
}


void PA_EasyBgScrollX(u8 screen, u8 bg_number, s32 x){
	if(PA_BgInfo[screen][bg_number].BgMode == BG_TILEDBG) PA_BGScrollX(screen, bg_number, x&511);
	else PA_InfLargeScrollX(screen, bg_number, x);
}


void PA_EasyBgScrollY(u8 screen, u8 bg_number, s32 y){
	if(PA_BgInfo[screen][bg_number].BgMode == BG_TILEDBG) PA_BGScrollY(screen, bg_number, y&511);
	else PA_InfLargeScrollY(screen, bg_number, y);
}



void PA_FSBgLoad(u8 screen, u8 bg_number, u32 filenumber)  {  
u32 *PA_BGinfo = (u32*)PA_PAFSFile(filenumber);
PA_BgInfo[screen][bg_number].BgMode = PA_BGinfo[0];   
PA_LoadBgPal(screen, bg_number, (void*)(PA_PAFSFile(filenumber+2))); 
PA_DeleteBg(screen, bg_number);
if (PA_BgInfo[screen][bg_number].BgMode == BG_TILEDBG) {	
	PA_LoadBgTilesEx(screen, bg_number, PA_PAFSFile(filenumber+3), PA_FSFile[filenumber+3].Length);
	PA_LoadBgMap(screen, bg_number, PA_PAFSFile(filenumber+1), PA_GetPAGfxBgSize(PA_BGinfo[1], PA_BGinfo[2])); 
	PA_InitBg(screen, bg_number, PA_GetPAGfxBgSize(PA_BGinfo[1], PA_BGinfo[2]), 0, 1);
}
else{
	PA_BgInfo[screen][bg_number].NTiles = PA_FSFile[filenumber+3].Length>>5;
	if (PA_BgInfo[screen][bg_number].NTiles < MAX_TILES) { 
		PA_LoadBgTilesEx(screen, bg_number, PA_PAFSFile(filenumber+3), PA_FSFile[filenumber+3].Length);
	}
	else{
		PA_LoadBgTilesEx(screen, bg_number, (void*)Blank, (1008<<5));
	}
	PA_BgInfo[screen][bg_number].Tiles = PA_PAFSFile(filenumber+3);
	PA_LoadBgMap(screen, bg_number, Blank, BG_512X256); 
	PA_InitBg(screen, bg_number, BG_512X256, 0, 1);
	PA_InitLargeBg(screen, bg_number, PA_BGinfo[1]>> 3, PA_BGinfo[2]>> 3, PA_PAFSFile(filenumber+1));
}
PA_BGScrollXY(screen, bg_number, 0, 0);
}


void PA_FSBgNameLoad(u8 screen, u8 bg_number, char* bg_name){
	char completename[32];
	u8 i;
	for (i= 0; bg_name[i] != 0; i++) completename[i] = bg_name[i];
	completename[i] = '_'; completename[i+1] = 'I'; completename[i+2] = 'n'; completename[i+3] = 'f'; completename[i+4] = 'o';  completename[i+5] = 0;
	PA_FSBgLoad(screen, bg_number, PA_FSGetFile(0, completename, "bin"));
//		PA_OutputText(1, 0, 4+screen, completename);
}





#ifdef __cplusplus
}
#endif



