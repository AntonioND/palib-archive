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

bool  charblocks[2][70];  // On met à 0 les emplacements utilisés... pour chaque écran...
u16  tilesetsize[2][4]; // Place utilisée pour chaque tileset
u16  mapsize[2][4]; // Place utilisée pour chaque map
u8  tilesetchar[2][4];  // Emplacement mémoire de chaque tileset
u8  mapchar[2][4];  // Emplacement mémoire de chaque map
u16 tempsize;

extern u16 *PA_DrawBg[2]; // Fond dessinable
u8 charsetstart[2] = {8, 8};



void PA_ResetBgSys(void) {
u8 i, j;
	for (i = 0; i < 4; i++){
		PA_DeleteBg(0, i);
		PA_DeleteBg(1, i);
	}
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 2; j++) {
		//	bool ;  // On met à 0 les emplacements utilisés... pour chaque écran...
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



void PA_LoadBgTilesEx(bool screen, u8 bg_select, void* bg_tiles, u16 size) {
u16 blocksize = (size + 1023) >> 10;
s8 charset = charsetstart[screen]; // On commence par le dernier... soit le 8ème, et on ira vers le bas
bool charsetok = 0;

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

}

	tilesetchar[screen][bg_select] = charset; // On place les tiles à un endroit précis...
	tilesetsize[screen][bg_select] = size;    // On mémorise aussi la taille que ca fait pour pouvoir effacer plus tard...

	DMA_Copy(bg_tiles, (void*)CharBaseBlock(screen, charset), size, DMA_16NOW);

	for (i = 0; i < blocksize; i++) charblocks[screen][(charset << 3) + i] = 1;  // Les blocs sont occupés
}


void PA_ReLoadBgTiles(bool screen, u8 bg_select, void* bg_tiles) {
	s8 charset = tilesetchar[screen][bg_select]; 
	u32 size = tilesetsize[screen][bg_select];    

	DMA_Copy(bg_tiles, (void*)CharBaseBlock(screen, charset), size, DMA_16NOW);
}







void PA_LoadBgMap(bool screen, u8 bg_select, void* bg_map, u8 bg_size) {
s16 charset = -1;
bool charsetok = 0;
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




void PA_LoadRotBgMap(bool screen, u8 bg_select, void* bg_map, u8 bg_size) {
s16 charset = -1;
bool charsetok = 0;
u8 blocksize = rotbg_place[bg_size];
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

	DMA_Copy(bg_map, (void*)ScreenBaseBlock(screen, charset), rotbg_sizes[bg_size], DMA_16NOW);

	for (i = 0; i < blocksize; i++) charblocks[screen][charset + i] = 1;  // Les blocs sont occupés
}





void PA_InitBg(bool screen, u8 bg_select, u8 bg_size, bool wraparound, bool color_mode) {	
	scrollpos[screen][bg_select].infscroll = 0; // Par défaut pas de scrolling infini...
	PA_bgmap[screen][bg_select] = ScreenBaseBlock(screen, mapchar[screen][bg_select]);
	_REG16(REG_BGSCREEN(screen)) |= (0x100 << (bg_select));
	_REG16(REG_BGCNT(screen, bg_select)) = bg_select | (bg_size << 14) |(mapchar[screen][bg_select] << SCREEN_SHIFT) | (wraparound << 13) | (tilesetchar[screen][bg_select] << 2) | (color_mode << 7);
}





void PA_DeleteTiles(bool screen, u8 bg_select) {

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







void PA_DeleteMap(bool screen, u8 bg_select) {
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





void PA_LargeScrollX(bool screen, u8 bg_select, s32 x){
s32 i, j, lx, tilex, tempx, tempy;
lx = scrollpos[screen][bg_select].lx;

if (x < 0) x = 0;
if (x >= lx-256) x = lx-257;

lx = lx >> 3;

PA_BGScrollX(screen,bg_select,x&511);

tempx = scrollpos[screen][bg_select].scrollx >> 3;
tempy = scrollpos[screen][bg_select].scrolly >> 3;

if (lx >= 64) { // Si moins de 64, pas besoin de faire du scrolling spécial, ca passe en hard !
// Dans un sens...
    while (x - scrollpos[screen][bg_select].scrollx >= 8) {  // Tant qu'on a du retour sur l'affichage de la carte, on fait afficher la ligne suivante
        scrollpos[screen][bg_select].scrollx += 8; // On a décalé de la taille d'un tile...
		tempx = scrollpos[screen][bg_select].scrollx >> 3;
		// On fait le decallage en copiant tout comme il faut
		i = (tempx+33)&63;
		tilex = tempx+33;	
		for (j = -2; j < 27; j++) PA_SetLargeMapTile(screen, bg_select, i, (tempy+j)&31, scrollpos[screen][bg_select].bg_map[tilex + ((tempy+j) * lx)]);
	}
// Et dans l'autre
    while (x - scrollpos[screen][bg_select].scrollx <= -8) {  // Tant qu'on a du retour sur l'affichage de la carte, on fait afficher la ligne suivante
        scrollpos[screen][bg_select].scrollx -= 8; // On a décalé de la taille d'un tile...
		tempx = scrollpos[screen][bg_select].scrollx >> 3;
		// On fait le decallage en copiant tout comme il faut
		i = (tempx-2)&63;
		tilex = tempx-2;
		for (j = -2; j < 27; j++) PA_SetLargeMapTile(screen, bg_select, i, (tempy+j)&31, scrollpos[screen][bg_select].bg_map[tilex + ((tempy+j) * lx)]);
	}
}
}




void PA_LargeScrollY(bool screen, u8 bg_select, s32 y){
s32 i, j, lx, ly, tiley, tempx, tempy;
lx = scrollpos[screen][bg_select].lx;
ly = scrollpos[screen][bg_select].ly;
if (y < 0) y = 0;
if (y >= ly-192) y = ly-193;

lx = lx >> 3;
ly = ly >> 3;

PA_BGScrollY(screen,bg_select,y&255);

tempx = scrollpos[screen][bg_select].scrollx >> 3;
tempy = scrollpos[screen][bg_select].scrolly >> 3;


if (ly >= 32) { // Si moins de 32, pas besoin de faire du scrolling spécial, ca passe en hard !
// Verticalement
// Dans un sens...
    while (y - scrollpos[screen][bg_select].scrolly >= 8) {  // Tant qu'on a du retour sur l'affichage de la carte, on fait afficher la ligne suivante
        scrollpos[screen][bg_select].scrolly += 8; // On a décalé de la taille d'un tile...
		tempy = scrollpos[screen][bg_select].scrolly >> 3;
		// On fait le decallage en copiant tout comme il faut
		j = (tempy+26)&31;
		tiley = tempy+26;
		for (i = -2; i < scrollpos[screen][bg_select].maxx-10; i++) PA_SetLargeMapTile(screen, bg_select, (tempx + i)&63, j, scrollpos[screen][bg_select].bg_map[(tempx + i) + (tiley * lx)]);
	}
// Et dans l'autre
    while (y - scrollpos[screen][bg_select].scrolly <= -8) {  // Tant qu'on a du retour sur l'affichage de la carte, on fait afficher la ligne suivante
        scrollpos[screen][bg_select].scrolly -= 8; // On a décalé de la taille d'un tile...
		tempy = scrollpos[screen][bg_select].scrolly >> 3;
		// On fait le decallage en copiant tout comme il faut
		j = (tempy-2)&31;
		tiley = tempy-2;
		for (i = -2; i < scrollpos[screen][bg_select].maxx-10; i++) PA_SetLargeMapTile(screen, bg_select, (tempx + i)&63, j, scrollpos[screen][bg_select].bg_map[(tempx + i) + (tiley * lx)]);
	}
}	

}





void PA_InfLargeScrollX(bool screen, u8 bg_select, s32 x){
s32 i, j, lx, ly, tilex, tempx, tempy;
lx = scrollpos[screen][bg_select].lx >> 3;
ly = scrollpos[screen][bg_select].ly >> 3;
//if (x < 0) x = 0;
//if (x >= lx-256) x = lx-257;

//lx = lx >> 3;


PA_BGScrollX(screen,bg_select,x&511);

tempx = scrollpos[screen][bg_select].scrollx >> 3;
tempy = scrollpos[screen][bg_select].scrolly >> 3;




if (lx >= 64) { // Si moins de 64, pas besoin de faire du scrolling spécial, ca passe en hard !
// Dans un sens...
    while (x - scrollpos[screen][bg_select].scrollx >= 8) {  // Tant qu'on a du retour sur l'affichage de la carte, on fait afficher la ligne suivante
        scrollpos[screen][bg_select].scrollx += 8; // On a décalé de la taille d'un tile...
		tempx = scrollpos[screen][bg_select].scrollx >> 3;
		// On fait le decallage en copiant tout comme il faut
		i = (tempx+33)&63;
		tilex = PA_Modulo((tempx+33), lx);
		for (j = -2; j < 27; j++) PA_SetLargeMapTile(screen, bg_select, i, (tempy+j)&31, scrollpos[screen][bg_select].bg_map[tilex + (PA_Modulo((tempy+j),ly) * lx)]);
	}
// Et dans l'autre
    while (x - scrollpos[screen][bg_select].scrollx <= -8) {  // Tant qu'on a du retour sur l'affichage de la carte, on fait afficher la ligne suivante
        scrollpos[screen][bg_select].scrollx -= 8; // On a décalé de la taille d'un tile...
		tempx = scrollpos[screen][bg_select].scrollx >> 3;
		// On fait le decallage en copiant tout comme il faut
		i = (tempx-2)&63;
		tilex = PA_Modulo((tempx-2), lx);
		for (j = -2; j < 27; j++) PA_SetLargeMapTile(screen, bg_select, i, (tempy+j)&31, scrollpos[screen][bg_select].bg_map[tilex + (PA_Modulo((tempy+j),ly) * lx)]);
	}
}
}



void PA_InfLargeScrollY(bool screen, u8 bg_select, s32 y){
s32 i, j, lx, ly, tiley, tempx, tempy;
lx = scrollpos[screen][bg_select].lx >> 3;
ly = scrollpos[screen][bg_select].ly >> 3;
//if (y < 0) y = 0;
//if (y >= ly-192) y = ly-193;

//lx = lx >> 3;
//ly = ly >> 3;

PA_BGScrollY(screen,bg_select,y&255);

tempx = scrollpos[screen][bg_select].scrollx >> 3;
tempy = scrollpos[screen][bg_select].scrolly >> 3;


if (ly >= 32) { // Si moins de 32, pas besoin de faire du scrolling spécial, ca passe en hard !
// Verticalement
// Dans un sens...
    while (y - scrollpos[screen][bg_select].scrolly >= 8) {  // Tant qu'on a du retour sur l'affichage de la carte, on fait afficher la ligne suivante
        scrollpos[screen][bg_select].scrolly += 8; // On a décalé de la taille d'un tile...
		tempy = scrollpos[screen][bg_select].scrolly >> 3;
		// On fait le decallage en copiant tout comme il faut
		j = (tempy+26)&31;
		tiley = PA_Modulo((tempy+26),ly);
		for (i = -2; i < scrollpos[screen][bg_select].maxx-10; i++) PA_SetLargeMapTile(screen, bg_select, (tempx + i)&63, j, scrollpos[screen][bg_select].bg_map[PA_Modulo((tempx + i),lx) + (tiley * lx)]);
	}
// Et dans l'autre
    while (y - scrollpos[screen][bg_select].scrolly <= -8) {  // Tant qu'on a du retour sur l'affichage de la carte, on fait afficher la ligne suivante
        scrollpos[screen][bg_select].scrolly -= 8; // On a décalé de la taille d'un tile...
		tempy = scrollpos[screen][bg_select].scrolly >> 3;
		// On fait le decallage en copiant tout comme il faut
		j = (tempy-2)&31;
		tiley =PA_Modulo((tempy-2),ly);
		for (i = -2; i < scrollpos[screen][bg_select].maxx-10; i++) PA_SetLargeMapTile(screen, bg_select, (tempx + i)&63, j, scrollpos[screen][bg_select].bg_map[PA_Modulo((tempx + i),lx) + (tiley * lx)]);
	}
}	

}





void PA_InitLargeBg(bool screen, u8 bg_select, s32 lx, s32 ly, void* bg_map){
s32 i, j;


scrollpos[screen][bg_select].infscroll = 1; // On active le scrolling infini	
scrollpos[screen][bg_select].scrollx = 0;		
scrollpos[screen][bg_select].scrolly = 0;

scrollpos[screen][bg_select].lx = lx << 3; // Tailles en pixels...
scrollpos[screen][bg_select].ly = ly << 3;	

if (lx > 62) scrollpos[screen][bg_select].maxx = 62;
else scrollpos[screen][bg_select].maxx = lx; // pas de scrolling infini en largeur necessaire...


scrollpos[screen][bg_select].bg_map = (u16*)bg_map;	

for (i = -2; i < scrollpos[screen][bg_select].maxx; i++)
	for (j = -2; j < 30; j++)
		PA_SetLargeMapTile(screen, bg_select, i&63, j&31, scrollpos[screen][bg_select].bg_map[PA_Modulo(i, lx) + (PA_Modulo(j, ly) * lx)]);


}




void PA_SetBgRot(bool screen, u8 bg_select, s32 x_scroll, s32 y_scroll, s32 x_rotcentre, s32 y_rotcentre, s16 bg_angle, s32 bg_zoom){

/*
s32 center_x = ((x_rotcentre) * (bg_zoom)) >> 8;
s32 center_y = ((y_rotcentre) * (bg_zoom)) >> 8;*/

s32 pa, pb, pc, pd;

pa = (PA_Cos(bg_angle) * (bg_zoom)) >> 8;
pb = (-PA_Sin(bg_angle) * (bg_zoom)) >> 8;
pc = (PA_Sin(bg_angle) * (bg_zoom)) >> 8;
pd = pa;

PA_BGXX(screen, bg_select) = (x_scroll << 8) - (x_rotcentre * pa + y_rotcentre * pb);
PA_BGXY(screen, bg_select) = (y_scroll << 8) - (x_rotcentre * pc + y_rotcentre * pd);

PA_BGXPA(screen, bg_select) = pa;
PA_BGXPB(screen, bg_select) = pb;
PA_BGXPC(screen, bg_select) = pc;
PA_BGXPD(screen, bg_select) = pd;


}






