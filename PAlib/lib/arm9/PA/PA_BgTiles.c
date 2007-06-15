#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"






PA_BgInfos PA_BgInfo[2][4];

// Quantit� de donn�es � charger en fonction de la taille de la map...
u16  bg_sizes[4] = {1024, 2048, 2048, 4096};
u8  bg_place[4] = {1, 2, 2, 4};
u8  rotbg_place[4] = {1, 1, 2, 8};
u16  rotbg_sizes[4] = {1024, 1024, 2048, 8192};

// Pour le parallax scrolling...
s32  PA_parallaxX[2][4];
s32  PA_parallaxY[2][4];

scrollpositions  scrollpos[2][4]; // Pour chaque �cran et pour chaque fond :)

u8  charblocks[2][70];  // On met � 0 les emplacements libres... pour chaque �cran...
//u8  tilesetchar[2][4];  // Emplacement m�moire de chaque tileset
u16 tempsize;

extern u16 *PA_DrawBg[2]; // Fond dessinable
u8 charsetstart[2] = {8, 8};

u8 rotbg_size[2][4]; // Background size of each possible rotbg


void PA_ResetBgSysScreen(u8 screen){
u8 i;

	PA_SetBgColor(screen, 0); 
	PA_SetVideoMode(screen, 0);

	for (i = 0; i < 4; i++){
		PA_DeleteBg(screen, i);
	}
	for (i = 0; i < 4; i++) {
	//	u8 ;  // On met � 0 les emplacements utilis�s... pour chaque �cran...
		PA_BgInfo[screen][i].tilesetsize = 0; // Place utilis�e pour chaque tileset
		PA_BgInfo[screen][i].mapsize = 0; // Place utilis�e pour chaque map
		PA_BgInfo[screen][i].TileSetChar = 0;  // Emplacement m�moire de chaque tileset
		PA_BgInfo[screen][i].mapchar = 0;  // Emplacement m�moire de chaque map
		tempsize = 0;
		PA_parallaxX[screen][i] = 0;
		PA_parallaxY[screen][i] = 0;
		scrollpos[screen][i].infscroll = 0;
	}

	for (i = 0; i < 64; i++) 
		charblocks[screen][i] = 0;

	// Emplacements pris � la suite, faut pas d�border...
	for (i = 64; i < 70; i++) 
		charblocks[screen][i] = 1;

charsetstart[screen] = 8; // Par d�faut � 8, pour dire de tout utiliser

}

void PA_ResetBgSys(void) {
	PA_ResetBgSysScreen(0);
	PA_ResetBgSysScreen(1);	
}



void PA_LoadBgTilesEx(u8 screen, u8 bg_select, void* bg_tiles, u32 size) {
u32 blocksize = (size + 1023) >> 10;

s8 charset = charsetstart[screen]; // On commence par le dernier... soit le 8�me, et on ira vers le bas
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
	charsetok = !tempsize;  // Si on a trouv� suffisament de blocs, on peut continuer
}

if (!charsetok) { // Si jamais on n'a pas trouv� de cr�neaux, on affiche un message d'erreur...
	u8 usedblocks[64];
	for(i = 0; i < 64; i++) usedblocks[i] = charblocks[screen][i];
	PA_ResetBgSys();

	PA_InitText(1, 0);
	PALETTE[512] = 0; // On met le fond du haut au noir histoire que tout se voit...
	u8 j;
	for(j = 0; j < 8; j++)
		for(i = 0; i < 8; i++)
			PA_OutputText(1, j, i, "%d", usedblocks[i+j*8]);	
			
	PA_OutputText(1, 2, 10, "Needed %d free blocks", blocksize);	
	PA_WaitFor(Stylus.Newpress);	
	PA_OutputSimpleText(1, 0, 12, "Sorry, there just seems to not be enough place to put all your backgrounds... you could try loading them in a different order, sometimes it changes a little, but I would advise trying to optimise them by reducing the number of tile...");
	PA_OutputSimpleText(1, 0, 19, "Je suis d�sol� mon vieux, mais l� je n'ai plus assez de place pour charger les tiles en m�moire ! Va falloir virer un fond ou alors r�duire le nombre de tiles des autres fonds...");
	
	PA_WaitFor(Stylus.Newpress);

}


	PA_BgInfo[screen][bg_select].TileSetChar = charset; // On place les tiles � un endroit pr�cis...
	PA_BgInfo[screen][bg_select].tilesetsize = size;    // On m�morise aussi la taille que ca fait pour pouvoir effacer plus tard...

	DMA_Copy(bg_tiles, (void*)CharBaseBlock(screen, charset), size, DMA_16NOW);
	
	// Save tiles pointer and position in VRAM
	PA_BgInfo[screen][bg_select].Tiles = bg_tiles;
	PA_BgInfo[screen][bg_select].TilePos = (u32*)CharBaseBlock(screen, PA_BgInfo[screen][bg_select].TileSetChar); // used for tile swapping

	for (i = 0; i < blocksize; i++) charblocks[screen][(charset << 3) + i] = 1;  // Les blocs sont occup�s
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
	charsetok = !tempsize;  // Si on a trouv� suffisament de blocs, on peut continuer
}

	PA_BgInfo[screen][bg_select].mapchar = charset; // On place la map � un endroit pr�cis...
	PA_BgInfo[screen][bg_select].mapsize = blocksize;

	DMA_Copy(bg_map, (void*)ScreenBaseBlock(screen, charset), bg_sizes[bg_size], DMA_16NOW);

	for (i = 0; i < blocksize; i++) charblocks[screen][charset + i] = 1;  // Les blocs sont occup�s
}




void PA_InitBg(u8 screen, u8 bg_select, u8 bg_size, u8 wraparound, u8 color_mode) {	
	scrollpos[screen][bg_select].infscroll = 0; // Par d�faut pas de scrolling infini...
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

	// On efface les tiles en m�moire
		DMA_Copy((void*)Blank, (void*)CharBaseBlock(screen, charset), size, DMA_16NOW);

	// On remet le tableau � 0...
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

		// On efface les tiles en m�moire
		DMA_Copy((void*)Blank, (void*)CharBaseBlock(screen, charset), (blocksize << 10), DMA_16NOW);

		// On remet le tableau � 0...
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
	charsetok = !tempsize;  // Si on a trouv� suffisament de blocs, on peut continuer
}

	PA_BgInfo[screen][bg_select].mapchar = charset; // On place la map � un endroit pr�cis...
	PA_BgInfo[screen][bg_select].mapsize = blocksize;

	DMA_Copy(bg_map, (void*)ScreenBaseBlock(screen, charset), rotbg_sizes[bg_size], DMA_16NOW);

	for (i = 0; i < blocksize; i++) charblocks[screen][charset + i] = 1;  // Les blocs sont occup�s
}


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




/*
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
}*/






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



