#ifdef __cplusplus
extern "C" {
#endif



#include "PA_BgLargeMap.h"
#include "PA_Math.h"




void PA_LargeScrollXN(u8 screen, u8 bg_select, s32 x){
s32 i, j, lx, tilex, tempx, tempy;
lx = scrollpos[screen][bg_select].lx;

if (x < 0) x = 0;
if (x >= lx-256) x = lx-257;

lx = lx >> 3;

PA_BGScrollX(screen,bg_select,x&511);

tempx = scrollpos[screen][bg_select].scrollx >> 3;
tempy = scrollpos[screen][bg_select].scrolly >> 3;

//if (lx >= 64) { // Si moins de 64, pas besoin de faire du scrolling spécial, ca passe en hard !
// Dans un sens...
    while (x - scrollpos[screen][bg_select].scrollx >= 8) {  // Tant qu'on a du retour sur l'affichage de la carte, on fait afficher la ligne suivante
        scrollpos[screen][bg_select].scrollx += 8; // On a décalé de la taille d'un tile...
		tempx = scrollpos[screen][bg_select].scrollx >> 3;
		// On fait le decallage en copiant tout comme il faut
		i = (tempx+34)&63;
		tilex = tempx+34;	
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
//}
}




void PA_LargeScrollYN(u8 screen, u8 bg_select, s32 y){
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


//if (ly >= 32) { // Si moins de 32, pas besoin de faire du scrolling spécial, ca passe en hard !
// Verticalement
// Dans un sens...
    while (y - scrollpos[screen][bg_select].scrolly >= 8) {  // Tant qu'on a du retour sur l'affichage de la carte, on fait afficher la ligne suivante
        scrollpos[screen][bg_select].scrolly += 8; // On a décalé de la taille d'un tile...
		tempy = scrollpos[screen][bg_select].scrolly >> 3;
		// On fait le decallage en copiant tout comme il faut
		j = (tempy+25)&31;
		tiley = tempy+25;
		for (i = -2; i < 35; i++) PA_SetLargeMapTile(screen, bg_select, (tempx + i)&63, j, scrollpos[screen][bg_select].bg_map[(tempx + i) + (tiley * lx)]);
	}
// Et dans l'autre
    while (y - scrollpos[screen][bg_select].scrolly <= -8) {  // Tant qu'on a du retour sur l'affichage de la carte, on fait afficher la ligne suivante
        scrollpos[screen][bg_select].scrolly -= 8; // On a décalé de la taille d'un tile...
		tempy = scrollpos[screen][bg_select].scrolly >> 3;
		// On fait le decallage en copiant tout comme il faut
		j = (tempy-2)&31;
		tiley = tempy-2;
		for (i = -2; i < 35; i++) PA_SetLargeMapTile(screen, bg_select, (tempx + i)&63, j, scrollpos[screen][bg_select].bg_map[(tempx + i) + (tiley * lx)]);
	}
//}	

}





void PA_InfLargeScrollXN(u8 screen, u8 bg_select, s32 x){
s32 i, j, lx, ly, tilex, tempx, tempy;
lx = scrollpos[screen][bg_select].lx >> 3;
ly = scrollpos[screen][bg_select].ly >> 3;
//if (x < 0) x = 0;
//if (x >= lx-256) x = lx-257;

//lx = lx >> 3;


PA_BGScrollX(screen,bg_select,x&511);

tempx = scrollpos[screen][bg_select].scrollx >> 3;
tempy = scrollpos[screen][bg_select].scrolly >> 3;




//if (lx >= 64) { // Si moins de 64, pas besoin de faire du scrolling spécial, ca passe en hard !
// Dans un sens...
    while (x - scrollpos[screen][bg_select].scrollx >= 8) {  // Tant qu'on a du retour sur l'affichage de la carte, on fait afficher la ligne suivante
        scrollpos[screen][bg_select].scrollx += 8; // On a décalé de la taille d'un tile...
		tempx = scrollpos[screen][bg_select].scrollx >> 3;
		// On fait le decallage en copiant tout comme il faut
		i = (tempx+34)&63;
		tilex = PA_Modulo((tempx+34), lx);
		for (j = -2; j < 26; j++) PA_SetLargeMapTile(screen, bg_select, i, (tempy+j)&31, scrollpos[screen][bg_select].bg_map[tilex + (PA_Modulo((tempy+j),ly) * lx)]);
	}
// Et dans l'autre
    while (x - scrollpos[screen][bg_select].scrollx <= -8) {  // Tant qu'on a du retour sur l'affichage de la carte, on fait afficher la ligne suivante
        scrollpos[screen][bg_select].scrollx -= 8; // On a décalé de la taille d'un tile...
		tempx = scrollpos[screen][bg_select].scrollx >> 3;
		// On fait le decallage en copiant tout comme il faut
		i = (tempx-2)&63;
		tilex = PA_Modulo((tempx-2), lx);
		for (j = -2; j < 26; j++) PA_SetLargeMapTile(screen, bg_select, i, (tempy+j)&31, scrollpos[screen][bg_select].bg_map[tilex + (PA_Modulo((tempy+j),ly) * lx)]);
	}
//}
}



void PA_InfLargeScrollYN(u8 screen, u8 bg_select, s32 y){
s32 i, j, lx, ly, tiley, tempx, tempy;
lx = scrollpos[screen][bg_select].lx >> 3;
ly = scrollpos[screen][bg_select].ly >> 3;


PA_BGScrollY(screen,bg_select,y&255);

tempx = scrollpos[screen][bg_select].scrollx >> 3;
tempy = scrollpos[screen][bg_select].scrolly >> 3;


// Verticalement
// Dans un sens...
    while (y - scrollpos[screen][bg_select].scrolly >= 8) {  // Tant qu'on a du retour sur l'affichage de la carte, on fait afficher la ligne suivante
        scrollpos[screen][bg_select].scrolly += 8; // On a décalé de la taille d'un tile...
		tempy = scrollpos[screen][bg_select].scrolly >> 3;
		// On fait le decallage en copiant tout comme il faut
		j = (tempy+25)&31;
		tiley = PA_Modulo((tempy+25),ly);
		for (i = -2; i < 35; i++) PA_SetLargeMapTile(screen, bg_select, (tempx + i)&63, j, scrollpos[screen][bg_select].bg_map[PA_Modulo((tempx + i),lx) + (tiley * lx)]);
	}
// Et dans l'autre
    while (y - scrollpos[screen][bg_select].scrolly <= -8) {  // Tant qu'on a du retour sur l'affichage de la carte, on fait afficher la ligne suivante
        scrollpos[screen][bg_select].scrolly -= 8; // On a décalé de la taille d'un tile...
		tempy = scrollpos[screen][bg_select].scrolly >> 3;
		// On fait le decallage en copiant tout comme il faut
		j = (tempy-2)&31;
		tiley =PA_Modulo((tempy-2),ly);
		for (i = -2; i < 35; i++) PA_SetLargeMapTile(screen, bg_select, (tempx + i)&63, j, scrollpos[screen][bg_select].bg_map[PA_Modulo((tempx + i),lx) + (tiley * lx)]);
	}

}


void PA_UpdateTileEngineX(u8 screen, u8 bg_select, s32 x, s32 y){
s32 tilenumber;
s32 modulj;
s32 lx = scrollpos[screen][bg_select].lx>>3;
s32 ly = scrollpos[screen][bg_select].ly>>3;
s32 modulx = PA_Modulo(x, lx);

u32 *map = (u32*)scrollpos[screen][bg_select].bg_map;
s32 j;


	for (j = y-2; j < 26+y; j++){
		modulj = PA_Modulo(j, ly);
		tilenumber = PA_TileNumber(x, j);
//		PA_SetLargeMapTile(screen, bg_select, i&63, j&31, TEST_NUMBER(i, j)); // Tile numbers...
		PA_SetLargeMapTile(screen, bg_select, x&63, j&31, 
							(tilenumber
							|((TILE_HFLIP|TILE_VFLIP)&(map[modulx+(modulj*lx)]>>19))
							)						
							); // Tile numbers...		
		PA_UpdateBgTile(screen, bg_select, tilenumber, 
			(void*)(PA_BgInfo[screen][bg_select].Tiles+
							((map[modulx+(modulj*lx)]&INF_JUSTTILE)<<6)
			));
	}
}

void PA_UpdateTileEngineY(u8 screen, u8 bg_select, s32 x, s32 y){
s32 tilenumber;
s32 moduli;
s32 lx = scrollpos[screen][bg_select].lx>>3;
s32 ly = scrollpos[screen][bg_select].ly>>3;
s32 moduly = PA_Modulo(y, ly);

u32 *map = (u32*)scrollpos[screen][bg_select].bg_map;
s32 i;


	for (i = x-2; i < 34+x; i++){
		moduli = PA_Modulo(i, lx);
		tilenumber = PA_TileNumber(i, y);
//		PA_SetLargeMapTile(screen, bg_select, i&63, j&31, TEST_NUMBER(i, j)); // Tile numbers...
		PA_SetLargeMapTile(screen, bg_select, i&63, y&31, 
							(tilenumber
							|((TILE_HFLIP|TILE_VFLIP)&(map[moduli+(moduly*lx)]>>19))
							)						
							); // Tile numbers...		
		PA_UpdateBgTile(screen, bg_select, tilenumber, 
			(void*)(PA_BgInfo[screen][bg_select].Tiles+
							((map[moduli+(moduly*lx)]&INF_JUSTTILE)<<6)
			));
	}
}


void PA_InfTilesScrollX(u8 screen, u8 bg_select, s32 x){

PA_BGScrollX(screen,bg_select,x&511);	

s32 tempx = scrollpos[screen][bg_select].scrollx >> 3;
s32 tempy = scrollpos[screen][bg_select].scrolly >> 3;

scrollpos[screen][bg_select].scrollx = x; // updated
x = x>>3;

//PA_OutputText(1, 0, 10, "%d     - %d      ", x, tempx);
	while(x < tempx) {
		tempx--;
		PA_UpdateTileEngineX(screen, bg_select, tempx-2, tempy);
	}
	while(x > tempx){
		tempx++;
		PA_UpdateTileEngineX(screen, bg_select, 33+tempx, tempy);
	}
}

void PA_InfTilesScrollY(u8 screen, u8 bg_select, s32 y){

PA_BGScrollY(screen,bg_select,y&255);	

s32 tempx = scrollpos[screen][bg_select].scrollx >> 3;
s32 tempy = scrollpos[screen][bg_select].scrolly >> 3;

scrollpos[screen][bg_select].scrolly = y; // updated
y = y>>3;

//PA_OutputText(1, 0, 11, "%d     - %d      ", y, tempy);
	while(y < tempy) {
		tempy--;
		PA_UpdateTileEngineY(screen, bg_select, tempx, tempy-2);
	}
	while(y > tempy){
		tempy++;
		PA_UpdateTileEngineY(screen, bg_select, tempx, tempy+25);
	}
}





void PA_InitLargeBg(u8 screen, u8 bg_select, s32 lx, s32 ly, void* bg_map){
s32 i, j;


scrollpos[screen][bg_select].infscroll = 1; // On active le scrolling infini	
scrollpos[screen][bg_select].scrollx = 0;		
scrollpos[screen][bg_select].scrolly = 0;

scrollpos[screen][bg_select].lx = lx << 3; // Tailles en pixels...
scrollpos[screen][bg_select].ly = ly << 3;	





if (lx > 62) scrollpos[screen][bg_select].maxx = 62;
else {
	scrollpos[screen][bg_select].maxx = lx; // pas de scrolling infini en largeur necessaire...
	//if(scrollpos[screen][bg_select].maxx<42) scrollpos[screen][bg_select].maxx = 42;
}
scrollpos[screen][bg_select].bg_map = (u16*)bg_map;	


if (PA_BgInfo[screen][bg_select].NTiles < 884){ // Normal largemap
	u16 *map = scrollpos[screen][bg_select].bg_map;
	for (i = -2; i < scrollpos[screen][bg_select].maxx; i++)
		for (j = -2; j < 30; j++)
			PA_SetLargeMapTile(screen, bg_select, i&63, j&31, map[PA_Modulo(i, lx) + (PA_Modulo(j, ly) * lx)]);
}
else{ // Tile engine active
	s32 moduli, modulj;
	u32 *map = (u32*)scrollpos[screen][bg_select].bg_map;
	u16 tilenumber;
	for (i = -2; i < 34; i++){
		for (j = -2; j < 26; j++){
			tilenumber = PA_TileNumber(i, j);
			moduli = PA_Modulo(i, lx);
			modulj = PA_Modulo(j, ly);			
	//		PA_SetLargeMapTile(screen, bg_select, i&63, j&31, TEST_NUMBER(i, j)); // Tile numbers...
			PA_SetLargeMapTile(screen, bg_select, i&63, j&31, 
								(tilenumber
								|((TILE_HFLIP|TILE_VFLIP)&(map[moduli+((modulj)*lx)]>>19))
								)						
								); // Tile numbers...		
			PA_UpdateBgTile(screen, bg_select, tilenumber, 
				(void*)(PA_BgInfo[screen][bg_select].Tiles+
								((map[moduli+((modulj)*lx)]&INF_JUSTTILE)<<6)
				));
		}
	}
}


}







#ifdef __cplusplus
}
#endif

