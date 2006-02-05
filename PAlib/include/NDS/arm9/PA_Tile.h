#ifndef _PA_Tile
#define _PA_Tile



// Extracts the size from PAGfx convertions
extern inline u8 PA_GetPAGfxBgSize(u16 width, u16 height)
{
	u8 bg_size = 0;
	if (height == 512)	bg_size = 2;
	if (width == 512)	bg_size ++;	
	return bg_size;
}

// Extracts the size from PAGfx convertions
extern inline u8 PA_GetPAGfxRotBgSize(u16 width)
{
	u8 bg_size = 1; //256x256 by default
	if (width <= 128)	bg_size = 0;	
	else if (width <= 256)	bg_size = 1;	
	else if (width <= 512)	bg_size = 2;		
	else if (width <= 1024) bg_size = 3;		
	
	return bg_size;
}

/*! \file PA_Tile.h
    \brief Everything concerning the Bg Tile modes

    This file contains all the macros and variables regarding Tile modes (0-2), loading tiles and Bg, etc...
*/

//#include "../PA.h"



//////////////////////////////////////////////////////////////////////
// Système de Tiles
//////////////////////////////////////////////////////////////////////


//background memory offset macros
#define CharBaseBlock(screen, n) (((n)*0x4000) + 0x6000000 + (0x200000 *  screen))
#define ScreenBaseBlock(screen, n) (((n)*0x800) + 0x6000000 + (0x200000 *  screen))
extern u32 PA_bgmap[2][4]; // Pointeur vers les maps, 4 maps par screen
extern u8 tilesetchar[2][4];

// Quantité de données à charger en fonction de la taille de la map...
extern u16 bg_sizes[4];
extern u8 bg_place[4];

extern u16 *PA_DrawBg[2]; // Fond dessinable


extern bool charblocks[2][70];
extern u16 tilesetsize[2][4]; // Place utilisée pour chaque tileset
extern u16 mapsize[2][4]; // Place utilisée pour chaque map
extern u8 tilesetchar[2][4];  // Emplacement mémoire de chaque tileset
extern u8 mapchar[2][4];  // Emplacement mémoire de chaque map

extern u8 charsetstart[2];





extern s32 PA_parallaxX[2][4];
extern s32 PA_parallaxY[2][4];




typedef struct {
	bool infscroll; // Passe à 1 si scrolling infini activé...
	u16* bg_map; // Map
//	s32 x, y;
	s32 lx, ly;
	s32 scrollx, scrolly;
	s32 maxx; // 32 ou 64, en fonction de la largeur...
//	s32 startx, starty;
}scrollpositions;

extern scrollpositions scrollpos[2][4]; // Pour chaque écran et pour chaque fond :)






// Mode de couleur
#define BG_COLOR16 0x00
#define BG_COLOR256 0x80


#define CHAR_SHIFT 2
#define SCREEN_SHIFT 8
#define WRAPAROUND 0x1

#define SCREEN_TILES 24576  // Taille à convertir pour avoir un écran complet...


//scrolling registers for background 0


// Registre pour les controles de chaque écran

#define REG_BGSCREEN0  0x04000000
#define REG_BGSCREEN1  0x04001000
#define REG_BGSCREEN(screen) (0x04000000 + (screen * 0x1000))

#define REG_BGCNT(screen, bg_number) (0x4000008 + (screen * 0x1000) + (bg_number << 1))

// Premier registre pour les scroll. On en déduit les autres en ajoutant pour le screen et le numéro du Bg
#define REG_BGSCROLLX 0x4000010
#define REG_BGSCROLLY 0x4000012


// Tailles pour les Bg :
#define BG_256X256 0
#define BG_512X256 1
#define BG_256X512 2
#define BG_512X512 3

#define BG_ROT_128X128 0
#define BG_ROT_256X256 1
#define BG_ROT_512X512 2
#define BG_ROT_1024X1024 3

void PA_LoadRotBgMap(bool screen, u8 bg_select, void* bg_map, u8 bg_size);


//ALL_BUT() pour faire tout sauf...
#define TILE_N 1023
#define TILE_PAL 61440
#define TILE_HFLIP 1024
#define TILE_VFLIP 2048

#define PA_BGXX(screen, bg) _REG32(0x4000008 + (0x1000 * screen) + (bg << 4))
#define PA_BGXY(screen, bg) _REG32(0x400000C + (0x1000 * screen) + (bg << 4))
#define PA_BGXPA(screen, bg) _REG16(0x4000000 + (0x1000 * screen) + (bg << 4))
#define PA_BGXPB(screen, bg) _REG16(0x4000002 + (0x1000 * screen) + (bg << 4))
#define PA_BGXPC(screen, bg) _REG16(0x4000004 + (0x1000 * screen) + (bg << 4))
#define PA_BGXPD(screen, bg) _REG16(0x4000006 + (0x1000 * screen) + (bg << 4))





/** @defgroup Tile Bg Modes 0-2
 *  Load tiles, a map, scroll it...
 *  @{
 */

/*!
    \fn void PA_ResetBgSys(void)
    \brief
      \~english Reset the background system
      \~french Reinitialise le systeme de fonds
*/
void PA_ResetBgSys(void);

/*!
    \fn void PA_InitBg(bool screen, u8 bg_select, u8 bg_size, bool wraparound, bool color_mode)
    \brief
      \~english Initialise a given background. Do this only after having loaded a tileset and a map.
      \~french Initialise un fond. A faire uniquement après avoir chargé un tileset et une map.
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \param bg_size
      \~english Background size. This is important, because it also determines whether the Bg is rotatable or not. To use a normal background, use the macros BG_256X256, BG_256X512, etc... For a rotatable Bg, use the macros BG_ROT_128X128...
      \~french Taille du fond. Ceci est très important, car ça détermine aussi si le Bg est rotatif ou non. Pour un fond normal, on utilise les macros BG_256X256, BG_256X512, etc... Pour un fond rotatif, il suffit d'utiliser BG_ROT_128X128...
    \param wraparound
      \~english If the background wraps around or not. More important for rotating backgrounds.
      \~french Si le fond boucle ou non. C'est plus important pour les fonds rotatifs...
    \param color_mode
      \~english Color mode : 0 for 16 color mode, 1 for 256...
      \~french Nombre de couleurs : 0 pour 16 couleurs, 1 pour 256

*/
void PA_InitBg(bool screen, u8 bg_select, u8 bg_size, bool wraparound, bool color_mode);
//#define PA_InitBg(screen, bg_select, bg_size, wraparound, color_mode) {	PA_bgmap[screen][bg_select] = ScreenBaseBlock(screen, ((bg_select + 1) << 3) - bg_place[bg_size]);_REG16(REG_BGSCREEN(screen)) |= (0x100 << (bg_select)); _REG16(REG_BGCNT(screen, bg_select)) = ((bg_select) | (bg_size << 14) |((((bg_select + 1) << 3) - bg_place[bg_size]) << SCREEN_SHIFT) | (wraparound << 13) | ((bg_select) << 2)) + (color_mode << 7);}


/*!
    \def PA_HideBg(screen, bg_select)
    \brief
      \~english Hide a screen's background.
      \~french Cacher un fond.
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
*/
#define PA_HideBg(screen, bg_select) _REG16(REG_BGSCREEN(screen)) &= ~(0x100 << (bg_select))


/*!
    \def PA_ShowBg(screen, bg_select)
    \brief
      \~english Show a hidden background.
      \~french Afficher un fond auparavant caché.
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
*/
#define PA_ShowBg(screen, bg_select) _REG16(REG_BGSCREEN(screen)) |= (0x100 << (bg_select))


/*!
    \def PA_ResetBg(screen)
    \brief
      \~english Reinitialize de Bg system of a screen. It only hides all the backgrounds in reality...
      \~french Reinitialiser les fonds d'un écran. En fait ca ne fait que cacher tous les fonds
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
*/
#define PA_ResetBg(screen) _REG16(REG_BGSCREEN(screen)) &= ~(0xF00)



/*!
    \def PA_LoadBgTiles(screen, bg_select, bg_tiles)
    \brief
      \~english Load a tileset into memory
      \~french Charger un tileset en mémoire
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \param bg_tiles
      \~english Name of the tiles' info (example: ship_Tiles)
      \~french Nom du tableau contenant les tiles (exemple: ship_Tiles)
*/
#define PA_LoadBgTiles(screen, bg_select, bg_tiles) PA_LoadBgTilesEx(screen, bg_select, (void*)bg_tiles, SIZEOF_16BIT(bg_tiles))
//#define PA_LoadBgTiles(screen, bg_select, bg_tiles) {DMA_Copy((void*)(bg_tiles), (void*)CharBaseBlock(screen, bg_select),SIZEOF_16BIT(bg_tiles), DMA_16NOW);}


/*!
    \fn void PA_LoadBgTilesEx(bool screen, u8 bg_select, void* bg_tiles, u16 size)
    \brief
      \~english Load a tileset into memory with a given size
      \~french Charger un tileset en mémoire avec une taille donnée
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \param bg_tiles
      \~english Name of the tiles' info (example: ship_Tiles)
      \~french Nom du tableau contenant les tiles (exemple: ship_Tiles)
    \param size
      \~english 16 bit size...
      \~french Taille en 16 bits...

*/
void PA_LoadBgTilesEx(bool screen, u8 bg_select, void* bg_tiles, u16 size);


/*!
    \fn void PA_ReLoadBgTiles(bool screen, u8 bg_select, void* bg_tiles)
    \brief
      \~english ReLoad a tileset into memory
      \~french ReCharger un tileset en mémoire
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \param bg_tiles
      \~english Name of the tiles' info (example: ship_Tiles)
      \~french Nom du tableau contenant les tiles (exemple: ship_Tiles)
*/
void PA_ReLoadBgTiles(bool screen, u8 bg_select, void* bg_tiles);


/*!
    \fn void PA_DeleteTiles(bool screen, u8 bg_select)
    \brief
      \~english Delete a tilest in memory. Note that loading a tileset automatically deletes the preceding one, so you won't need to use this function often
      \~french Effacer un tileset en mémoire. A noter que charger un tileset efface automatiquement le tileset précédent, donc on n'aura pas souvent besoin de cette fonction...
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
*/
void PA_DeleteTiles(bool screen, u8 bg_select);


/*!
    \fn void PA_DeleteMap(bool screen, u8 bg_select)
    \brief
      \~english Delete a map in memory. Note that loading a map automatically deletes the preceding one, so you won't need to use this function often
      \~french Effacer une map en mémoire. A noter que charger une map efface automatiquement la map précédent, donc on n'aura pas souvent besoin de cette fonction...
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
*/
void PA_DeleteMap(bool screen, u8 bg_select);



/*!
    \fn extern inline void PA_DeleteBg(bool screen, u8 bg_select)
    \brief
      \~english Delete a complete background (tiles + map + hide it...)
      \~french Effacer un fond complètement (tiles + map + cacher)
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
*/
extern inline void PA_DeleteBg(bool screen, u8 bg_select) {
	PA_HideBg(screen, bg_select);
	PA_DeleteTiles(screen, bg_select);
	PA_DeleteMap(screen, bg_select);
}

/*!
    \fn void PA_LoadBgMap(bool screen, u8 bg_select, void* bg_map, u8 bg_size)
    \brief
      \~english Load a background's map info
      \~french Charge la carte d'un fond
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \param bg_map
      \~english Name of the map's info (example : (void*)ship_Map) Don't forget the void...
      \~french Nom du tableau contenant les infos sur la map (exemple : (void*)ship_Map) n'oublie pas le void...
    \param bg_size
      \~english Background size. This is important, because it also determines whether the Bg is rotatable or not. To use a normal background, use the macros BG_256X256, BG_256X512, etc... 
      \~french Taille du fond. Ceci est très important, car ça détermine aussi si le Bg est rotatif ou non. Pour un fond normal, on utilise les macros BG_256X256, BG_256X512, etc...
*/

void PA_LoadBgMap(bool screen, u8 bg_select, void* bg_map, u8 bg_size);





/*!
    \def PA_LoadTiledBg(screen, bg_number, bg_name)
    \brief
      \~english This will never get easier... Loads a background TiledBg converted with PAGfx, with it's tiles, map, and palette. Only 256 color mode available. 
      \~french On ne pourra jamais rendre ca plus simple... Charge un fond de type TiledBg converti avec PAGfx, en mettant les tiles, la map, et meme la palette ! Seulement en mode 256 couleurs
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_number
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \param bg_name
      \~english Background name, like bg0
      \~french Nom du fond, comme bg0
*/

#define PA_LoadTiledBg(screen, bg_number, bg_name){\
	PA_LoadBgPal(screen, bg_number, (void*)bg_name##_Pal); \
	PA_LoadSimpleBg(screen, bg_number, bg_name##_Tiles, bg_name##_Map, PA_GetPAGfxBgSize(bg_name##_Width, bg_name##_Height), 0, 1);}




/*!
    \def PA_LoadSimpleBg(screen, bg_select, bg_tiles, bg_map, bg_size, wraparound, color_mode)
    \brief
      \~english Simple way to load a Background. Combines PA_InitBg, PA_LoadBgTiles, and PA_LoadBgMap
      \~french Facon simple de cahrger un fond. Combine PA_InitBg, PA_LoadBgTiles, et PA_LoadBgMap
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \param bg_tiles
      \~english Name of the tiles' info (example: ship_Tiles)
      \~french Nom du tableau contenant les tiles (exemple: ship_Tiles)
    \param bg_map
      \~english Name of the map's info (example : ship_Map)
      \~french Nom du tableau contenant les infos sur la map (exemple : ship_Map)
    \param bg_size
      \~english Background size. To use a normal background, use the macros BG_256X256, BG_256X512, etc...
      \~french Taille du fond. Pour un fond normal, on utilise les macros BG_256X256, BG_256X512, etc... 
    \param wraparound
      \~english If the background wraps around or not. More important for rotating backgrounds. 
      \~french Si le fond boucle ou non. C'est plus important pour les fonds rotatifs... 
    \param color_mode
      \~english Color mode : 0 for 16 color mode, 1 for 256...
      \~french Nombre de couleurs : 0 pour 16 couleurs, 1 pour 256
*/

#define PA_LoadSimpleBg(screen, bg_select, bg_tiles, bg_map, bg_size, wraparound, color_mode) {\
PA_DeleteBg(screen, bg_select);\
PA_LoadBgTiles(screen, bg_select, bg_tiles); \
PA_LoadBgMap(screen, bg_select, (void*)bg_map, bg_size); \
PA_InitBg(screen, bg_select, bg_size, 0, color_mode);\
PA_BGScrollXY(screen, bg_select, 0, 0);}



/*!
    \def PA_LoadRotBg(screen, bg_select, bg_tiles, bg_map, bg_size, wraparound)
    \brief
      \~english Load a background fit for rotating/scaling ! Warning, you must use PA_SetVideoMode to 1 if you want 1 rotating background (Bg3 only !), or 2 for 2 rotating backgrounds (Bg2 and 3). The background MUST be in 256 colors
      \~french Charger un fond pour les rotations/zoom ! Attention, il faut avant utiliser PA_SetVideoMode avec 1 pour utiliser un fond rotatif (le fond 3 uniquement !), ou 2 pour 2 fonds (2 et 3). Le fond DOIT etre de 256 couleurs
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load
      \~french Numéro du fond que l'on veut charger
    \param bg_tiles
      \~english Name of the tiles' info (example: ship_Tiles)
      \~french Nom du tableau contenant les tiles (exemple: ship_Tiles)
    \param bg_map
      \~english Name of the map's info (example : ship_Map)
      \~french Nom du tableau contenant les infos sur la map (exemple : ship_Map)
    \param bg_size
      \~english Background size. Use the following macros : BG_ROT_128X128, or 256X256, 512X512, or 1024X1024
      \~french Taille du fond. Utiliser les macros suivantes : BG_ROT_128X128, ou 256X256, 512X512, ou enfin 1024X1024
    \param wraparound
      \~english If the background wraps around or not. 
      \~french Si le fond boucle ou non. 
*/
#define PA_LoadRotBg(screen, bg_select, bg_tiles, bg_map, bg_size, wraparound) {\
PA_DeleteBg(screen, bg_select);\
PA_LoadBgTiles(screen, bg_select, bg_tiles); \
PA_LoadRotBgMap(screen, bg_select, (void*)bg_map, bg_size); \
PA_InitBg(screen, bg_select, bg_size, wraparound, 1);\
PA_SetBgRot(screen, bg_select, 0, 0, 0, 0, 0, 256);\
}

void PA_SetBgRot(bool screen, u8 bg_select, s32 x_scroll, s32 y_scroll, s32 x_rotcentre, s32 y_rotcentre, s16 bg_angle, s32 bg_zoom);

/*!
    \def PA_LoadBg(screen, bg_select, bg_tiles, tile_size, bg_map, bg_size, wraparound, color_mode)
    \brief
      \~english Simplest way to load a Background. Combines PA_InitBg, PA_LoadBgTiles, and PA_LoadBgMap
      \~french Facon la plus simple de cahrger un fond. Combine PA_InitBg, PA_LoadBgTiles, et PA_LoadBgMap
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \param bg_tiles
      \~english Name of the tiles' info (example: ship_Tiles)
      \~french Nom du tableau contenant les tiles (exemple: ship_Tiles)
    \param tile_size
      \~english Size of your tileset
      \~french Taille du tilset
    \param bg_map
      \~english Name of the map's info (example : ship_Map)
      \~french Nom du tableau contenant les infos sur la map (exemple : ship_Map)
    \param bg_size
      \~english Background size. This is important, because it also determines whether the Bg is rotatable or not. To use a normal background, use the macros BG_256X256, BG_256X512, etc... For a rotatable Bg, use the macros BG_ROT_128X128...
      \~french Taille du fond. Ceci est très important, car ça détermine aussi si le Bg est rotatif ou non. Pour un fond normal, on utilise les macros BG_256X256, BG_256X512, etc... Por un fond rotatif, il suffit d'utiliser BG_ROT_128X128...
    \param wraparound
      \~english If the background wraps around or not. More important for rotating backgrounds. 
      \~french Si le fond boucle ou non. C'est plus important pour les fonds rotatifs... 
    \param color_mode
      \~english Color mode : 0 for 16 color mode, 1 for 256...
      \~french Nombre de couleurs : 0 pour 16 couleurs, 1 pour 256
*/

#define PA_LoadBg(screen, bg_select, bg_tiles, tile_size, bg_map, bg_size, wraparound, color_mode) {\
PA_LoadBgTilesEx(screen, bg_select, (void*)bg_tiles, tile_size); \
PA_LoadBgMap(screen, bg_select, (void*)bg_map, bg_size); \
PA_InitBg(screen, bg_select, bg_size, 0, color_mode);\
PA_BGScrollXY(screen, bg_select, 0, 0);}



/*!
    \def PA_BGScrollX(screen, bg_number, x)
    \brief
      \~english Scroll horizontaly any background
      \~french Scroll horizontal de n'importe quel fond
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_number
      \~english Background number (0-3)
      \~french Numéro du fond que l'on veut tourner (0-3)
    \param x
      \~english X value to scroll
      \~french Valeur X à déplacer, horizontalement...
*/
#define PA_BGScrollX(screen, bg_number, x) _REG16(REG_BGSCROLLX + ((screen) * 0x1000) + ((bg_number) << 2)) = (x)&1023

/*!
    \def PA_BGScrollY(screen, bg_number, y)
    \brief
      \~english Scroll vertically any background
      \~french Scroll vertical de n'importe quel fond
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_number
      \~english Background number (0-3)
      \~french Numéro du fond que l'on veut tourner (0-3)
    \param y
      \~english Y value to scroll
      \~french Valeur Y à déplacer, verticalement...
*/
#define PA_BGScrollY(screen, bg_number, y) _REG16(REG_BGSCROLLY + ((screen) * 0x1000) + ((bg_number) << 2)) = (y)&1023




/*!
    \def PA_BGScrollXY(screen, bg_number, x, y)
    \brief
      \~english Scroll horizontaly and vertically any background
      \~french Scroll horizontal et vertical de n'importe quel fond
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_number
      \~english Background number (0-3)
      \~french Numéro du fond que l'on veut tourner (0-3)
    \param x
      \~english X value to scroll
      \~french Valeur X à déplacer, horizontalement...
    \param y
      \~english Y value to scroll
      \~french Valeur Y à déplacer, verticalement...
*/
#define PA_BGScrollXY(screen, bg_number, x, y) {PA_BGScrollX(screen, bg_number, x); PA_BGScrollY(screen, bg_number, y);}




/*!
    \fn extern inline void PA_SetMapTile(bool screen, u8 bg_select, s16 x, s16 y, s16 tile_number)
    \brief
      \~english Change the tile gfx used by a given tile in the map
      \~french Change la tile gfx utilisée pour une tile donnée dans la map
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number (0-3)
      \~french Numéro du fond que l'on veut tourner (0-3)
    \param x
      \~english X value of the tile to change
      \~french Valeur X de la tile à changer
    \param y
      \~english Y value of the map tile to change
      \~french Valeur Y de la tile à changer dans la carte
    \param tile_number
      \~english New tile number to put
      \~french Nouveau numéro de tile que l'on veut mettre
*/
extern inline void PA_SetMapTile(bool screen, u8 bg_select, s16 x, s16 y, s16 tile_number) {
*(u16*)(PA_bgmap[screen][bg_select] + ((x) << 1) + ((y) << 6)) &= ALL_BUT(TILE_N); 
*(u16*)(PA_bgmap[screen][bg_select] + ((x) << 1) + ((y) << 6)) |= ((tile_number)&TILE_N);
}


/*!
    \fn extern inline void PA_SetMapTileAll(bool screen, u8 bg_select, s16 x, s16 y, u16 tile_info)
    \brief
      \~english Change the tile info used by a given tile in the map
      \~french Change les infos tiles utilisée pour une tile donnée dans la map
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number (0-3)
      \~french Numéro du fond que l'on veut tourner (0-3)
    \param x
      \~english X value of the tile to change
      \~french Valeur X de la tile à changer
    \param y
      \~english Y value of the map tile to change
      \~french Valeur Y de la tile à changer dans la carte
    \param tile_info
      \~english New tile to put (tile + palette + flips...)
      \~french Nouveau numéro de tile que l'on veut mettre (tile + palette + flips...)
*/
extern inline void PA_SetMapTileAll(bool screen, u8 bg_select, s16 x, s16 y, u16 tile_info){
	*(u16*)(PA_bgmap[screen][bg_select] + ((x) << 1) + ((y) << 6)) = tile_info;
}




/*!
    \fn extern inline void PA_SetLargeMapTile(bool screen, u8 bg_select, s32 x, s32 y, u32 tile_info)
    \brief
      \~english Change the tile info used by a given tile in the map, only for big background (512 large or wide)
      \~french Change les infos tiles utilisée pour une tile donnée dans la map, seulement pour les grands fonds (512 de large ou haut)
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number (0-3)
      \~french Numéro du fond que l'on veut tourner (0-3)
    \param x
      \~english X value of the tile to change
      \~french Valeur X de la tile à changer
    \param y
      \~english Y value of the map tile to change
      \~french Valeur Y de la tile à changer dans la carte
    \param tile_info
      \~english New tile to put (tile + palette + flips...)
      \~french Nouveau numéro de tile que l'on veut mettre (tile + palette + flips...)
*/
extern inline void PA_SetLargeMapTile(bool screen, u8 bg_select, s32 x, s32 y, u32 tile_info) {
	u32 truex;
	u32 mapblock = 0;
	truex = x&31;
	mapblock = (x >> 5) << 11; // Permet d'avoir le bon block...
	
	*(u16*)(PA_bgmap[screen][bg_select] + ((truex) << 1) + ((y) << 6) + mapblock) = tile_info;
}



/*!
    \def PA_SetMapTileHflip(screen, bg_select, x, y, hflip)
    \brief
      \~english Flip a given tile horizontaly
      \~french Flipper une tile de la carte, horizontalement
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number (0-3)
      \~french Numéro du fond que l'on veut tourner (0-3)
    \param x
      \~english X value of the tile to change
      \~french Valeur X de la tile à changer
    \param y
      \~english Y value of the map tile to change
      \~french Valeur Y de la tile à changer dans la carte
    \param hflip
      \~english Set the map tile to horizontal flip
      \~french Mettre la tile de la carte en flip horizontal
*/
#define PA_SetMapTileHflip(screen, bg_select, x, y, hflip) {*(u16*)(PA_bgmap[screen][bg_select] + ((x) << 1) + ((y) << 6)) &= ALL_BUT(TILE_HFLIP); *(u16*)(PA_bgmap[screen][bg_select] + ((x) << 1) + ((y) << 6)) |= ((hflip) << 10);}


/*!
    \def PA_SetMapTileVflip(screen, bg_select, x, y, vflip)
    \brief
      \~english Flip a given tile verticaly
      \~french Flipper une tile de la carte, verticalement
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number (0-3)
      \~french Numéro du fond que l'on veut tourner (0-3)
    \param x
      \~english X value of the tile to change
      \~french Valeur X de la tile à changer
    \param y
      \~english Y value of the map tile to change
      \~french Valeur Y de la tile à changer dans la carte
    \param vflip
      \~english Set the map tile to vertical flip
      \~french Mettre la tile de la carte en flip vertical
*/
#define PA_SetMapTileVflip(screen, bg_select, x, y, vflip) {*(u16*)(PA_bgmap[screen][bg_select] + ((x) << 1) + ((y) << 6)) &= ALL_BUT(TILE_VFLIP); *(u16*)(PA_bgmap[screen][bg_select] + ((x) << 1) + ((y) << 6)) |= ((vflip) << 11);}


/*!
    \def PA_SetMapTilePal(screen, bg_select, x, y, palette_number)
    \brief
      \~english Change the 16 color palette used by a tile. Works only if the Bg is in 16 colors...
      \~french Changer la palette de 16 couleurs utilisée par une tile de la carte. Marche uniquement en mode 16 couleurs pour le Bg.
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number (0-3)
      \~french Numéro du fond que l'on veut tourner (0-3)
    \param x
      \~english X value of the tile to change
      \~french Valeur X de la tile à changer
    \param y
      \~english Y value of the map tile to change
      \~french Valeur Y de la tile à changer dans la carte
    \param palette_number
      \~english Palette number (0-15)
      \~french Numéro de la palette (0-15)
*/
#define PA_SetMapTilePal(screen, bg_select, x, y, palette_number) {*(u16*)(PA_bgmap[screen][bg_select] + ((x) << 1) + ((y) << 6)) &= ALL_BUT(TILE_PAL); *(u16*)(PA_bgmap[screen][bg_select] + ((x) << 1) + ((y) << 6)) |= ((palette_number) << 12);}


/*!
    \def PA_SetMapTileEx(screen, bg_select, x, y, tile_number, hflip, vflip, palette_number)
    \brief
      \~english Change every aspect of a given map tile
      \~french Changer tous les aspect d'une tile donnée dans la map.
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number (0-3)
      \~french Numéro du fond que l'on veut tourner (0-3)
    \param x
      \~english X value of the tile to change
      \~french Valeur X de la tile à changer
    \param y
      \~english Y value of the map tile to change
      \~french Valeur Y de la tile à changer dans la carte
    \param tile_number
      \~english New tile number to put
      \~french Nouveau numéro de tile que l'on veut mettre
    \param hflip
      \~english Set the map tile to horizontal flip
      \~french Mettre la tile de la carte en flip horizontal
    \param vflip
      \~english Set the map tile to vertical flip
      \~french Mettre la tile de la carte en flip vertical
    \param palette_number
      \~english Palette number (0-15)
      \~french Numéro de la palette (0-15)
*/
extern inline void PA_SetMapTileEx(bool screen, u8 bg_select, s16 x, s16 y, u16 tile_number, bool hflip, bool vflip, u8 palette_number) {
	*(u16*)(PA_bgmap[screen][bg_select] + ((x) << 1) + ((y) << 6)) = (tile_number) + ((hflip) << 10) + ((vflip) << 11) + ((palette_number) << 12);
}



/*!
    \def PA_LoadTiledBitmap(screen, tiled_bitmap)
    \brief
		\~english Load a bitmap converted using gfx2gba, converted like a tileset but without any optimisations. No use to include the map. Carefull, takes the whole tile memory...
		\~french Charger un bitmap converti avec gfx2gba, comme pour un fond normal, mais sans aucune optimisation. Plus la peine d'inclure la map dans ce cas. Attention, prend tout la mémoire des fonds.
    \param screen
        \~english Chose de screen (0 or 1)
        \~french Choix de l'écran (0 ou 1)
    \param tiled_bitmap
		\~english Background namegiven by gfx2gba (like splashTiles)
		\~french Nom du fond donné par gfx2gba, du genre (splashTiles)
*/
#define PA_LoadTiledBitmap(screen, tiled_bitmap) PA_LoadBg(screen, 0, tiled_bitmap, SCREEN_TILES, bitmap, BG_256X256, 1, 1)



/*!
    \def PA_LoadPAGfxRotBg(screen, bg_select, bg_name, wraparound)
    \brief
      \~english Load a background fit for rotating/scaling ! Warning, you must use PA_SetVideoMode to 1 if you want 1 rotating background (Bg3 only !), or 2 for 2 rotating backgrounds (Bg2 and 3). The background MUST be in 256 colors
      \~french Charger un fond pour les rotations/zoom ! Attention, il faut avant utiliser PA_SetVideoMode avec 1 pour utiliser un fond rotatif (le fond 3 uniquement !), ou 2 pour 2 fonds (2 et 3). Le fond DOIT etre de 256 couleurs
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load
      \~french Numéro du fond que l'on veut charger
    \param bg_name
      \~english Background name, like bg0
      \~french Nom du fond, comme bg0
    \param wraparound
      \~english If the background wraps around or not. 
      \~french Si le fond boucle ou non. 
*/
#define PA_LoadPAGfxRotBg(screen, bg_select, bg_name, wraparound) {\
PA_LoadNormalBgPal(screen, (void*)bg_name##_Pal);\
PA_LoadRotBg(screen, bg_select, bg_name##_Tiles, bg_name##_Map, PA_GetPAGfxRotBgSize(bg_name##_Width), wraparound);\
}





/*!
    \def PA_LoadLargeBg(screen, bg_select, bg_tiles, bg_map, color_mode, lx, ly)
    \brief
      \~english Completely load and initialise a background with infinite scrolling (usefull if larger or wider than 512 pixels)
      \~french Charger et initialiser un fond pour le scrolling infini (pour les fonds de plus de 512 pixels de haut ou de large)
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \param bg_tiles
      \~english Name of the tiles' info (example: ship_Tiles)
      \~french Nom du tableau contenant les tiles (exemple: ship_Tiles)
    \param bg_map
      \~english Name of the map's info (example : ship_Map)
      \~french Nom du tableau contenant les infos sur la map (exemple : ship_Map)
   \param color_mode
      \~english Color mode : 0 for 16 color mode, 1 for 256...
      \~french Nombre de couleurs : 0 pour 16 couleurs, 1 pour 256
   \param lx
      \~english Width, in tiles. So a 512 pixel wide map is 64 tiles wide...
      \~french Largeur, en tiles. Un fond de 512 pixels de large fera 64 tiles de large.
   \param ly
      \~english Height, in tiles. So a 512 pixel high map is 64 tiles high...
      \~french Hauteur, en tiles. Un fond de 512 pixels de hauy fera 64 tiles de haut.	  
*/
#define PA_LoadLargeBg(screen, bg_select, bg_tiles, bg_map, color_mode, lx, ly) {\
PA_LoadSimpleBg(screen, bg_select, bg_tiles, Blank, BG_512X256, 0, color_mode);\
PA_InitLargeBg(screen, bg_select, lx, ly, (void*)bg_map);}




/*!
    \def PA_LoadPAGfxLargeBg(screen, bg_number, bg_name)
    \brief
      \~english Completely load and initialise a background with infinite scrolling (usefull if larger or wider than 512 pixels), converted with PAGfx
      \~french Charger et initialiser un fond pour le scrolling infini (pour les fonds de plus de 512 pixels de haut ou de large), converti avec PAGfx
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_number
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (0-3)
   \param  bg_name
       \~english Background name, in PAGfx
      \~french Nom du fond dans PAGfx 
*/
#define PA_LoadPAGfxLargeBg(screen, bg_number, bg_name){\
	PA_LoadBgPal(screen, bg_number, (void*)bg_name##_Pal); \
	PA_LoadLargeBg(screen, bg_number, bg_name##_Tiles, bg_name##_Map, 1, (bg_name##_Width) >> 3, (bg_name##_Height) >> 3);}



/*!
    \def PA_LoadLargeBgEx(screen, bg_select, bg_tiles, tile_size, bg_map, color_mode, lx, ly)
    \brief
      \~english Completely load and initialise a background with infinite scrolling (usefull if larger or wider than 512 pixels), but here you can put yourself the tile size...
      \~french Charger et initialiser un fond pour le scrolling infini (pour les fonds de plus de 512 pixels de haut ou de large), mais ici on met soi-meme la taille des tiles
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \param bg_tiles
      \~english Name of the tiles' info (example: ship_Tiles)
      \~french Nom du tableau contenant les tiles (exemple: ship_Tiles)
    \param tile_size
      \~english Size of your tileset
      \~french Taille du tilset
    \param bg_map
      \~english Name of the map's info (example : ship_Map)
      \~french Nom du tableau contenant les infos sur la map (exemple : ship_Map)
   \param color_mode
      \~english Color mode : 0 for 16 color mode, 1 for 256...
      \~french Nombre de couleurs : 0 pour 16 couleurs, 1 pour 256
   \param lx
      \~english Width, in tiles. So a 512 pixel wide map is 64 tiles wide...
      \~french Largeur, en tiles. Un fond de 512 pixels de large fera 64 tiles de large.
   \param ly
      \~english Height, in tiles. So a 512 pixel high map is 64 tiles high...
      \~french Hauteur, en tiles. Un fond de 512 pixels de hauy fera 64 tiles de haut.	  
*/
#define PA_LoadLargeBgEx(screen, bg_select, bg_tiles, tile_size, bg_map, color_mode, lx, ly) {\
PA_LoadBg(screen, bg_select, bg_tiles, tile_size, Blank, BG_512X256, 0, color_mode);\
PA_InitLargeBg(screen, bg_select, lx, ly, (void*)bg_map);}




/*!
    \fn void PA_InfLargeScrollX(bool screen, u8 bg_select, s32 x)
    \brief
      \~english Scroll a large infinite scrolling background horizontaly. It must have been initialised with PA_LoadLargeBg.
      \~french Déplacer un fond à scrolling 'infini' horizontalement. Doit etre initialisé avec PA_LoadLargeBg.
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \param x
      \~english X value to scroll
      \~french Valeur X à déplacer
*/
void PA_InfLargeScrollX(bool screen, u8 bg_select, s32 x);


/*!
    \fn void PA_InfLargeScrollY(bool screen, u8 bg_select, s32 y)
    \brief
      \~english Scroll a large infinite scrolling background vertically. It must have been initialised with PA_LoadLargeBg.
      \~french Déplacer un fond à scrolling 'infini' verticalement. Doit etre initialisé avec PA_LoadLargeBg.
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \param y
      \~english Y value to scroll
      \~french Valeur Y à déplacer
*/
void PA_InfLargeScrollY(bool screen, u8 bg_select, s32 y);



/*!
    \fn extern inline void PA_InfLargeScrollXY(bool screen, u8 bg_select, s32 x, s32 y)
    \brief
      \~english Scroll a large infinite scrolling background horizontaly and vertically. It must have been initialised with PA_LoadLargeBg.
      \~french Déplacer un fond à scrolling 'infini' horizontalement et verticalement. Doit etre initialisé avec PA_LoadLargeBg.
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \param x
      \~english X value to scroll
      \~french Valeur X à déplacer
    \param y
      \~english Y value to scroll
      \~french Valeur Y à déplacer	  
*/
extern inline void PA_InfLargeScrollXY(bool screen, u8 bg_select, s32 x, s32 y){
	PA_InfLargeScrollX(screen, bg_select, x);
	PA_InfLargeScrollY(screen, bg_select, y);
}

/*!
    \fn void PA_LargeScrollX(bool screen, u8 bg_select, s32 x)
    \brief
      \~english Scroll a large background horizontaly. It must have been initialised with PA_LoadLargeBg.  This function does not wrap around, but is faster than the InfLargeScroll...
      \~french Déplacer un grand fond à scrolling horizontalement.Doit etre initialisé avec PA_LoadLargeBg. Cette fonction ne permet pas au fond de 'boucler' sur lui-meme, mais est bien plus rapide que InfLargeScroll...
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \param x
      \~english X value to scroll
      \~french Valeur X à déplacer
*/
void PA_LargeScrollX(bool screen, u8 bg_select, s32 x);


/*!
    \fn void PA_LargeScrollY(bool screen, u8 bg_select, s32 y)
    \brief
      \~english Scroll a large background vertically. It must have been initialised with PA_LoadLargeBg. This function does not wrap around, but is faster than the InfLargeScroll...
      \~french Déplacer un grand fond à scrolling verticalement. Doit etre initialisé avec PA_LoadLargeBg.  Cette fonction ne permet pas au fond de 'boucler' sur lui-meme, mais est bien plus rapide que InfLargeScroll...
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \param y
      \~english Y value to scroll
      \~french Valeur Y à déplacer
*/
void PA_LargeScrollY(bool screen, u8 bg_select, s32 y);



/*!
    \fn extern inline void PA_LargeScrollXY(bool screen, u8 bg_select, s32 x, s32 y)
    \brief
      \~english Scroll a large background horizontaly and vertically. It must have been initialised with PA_LoadLargeBg. This function does not wrap around, but is faster than the InfLargeScroll...
      \~french Déplacer un grand fond à scrolling horizontalement et verticalement. Doit etre initialisé avec PA_LoadLargeBg. Cette fonction ne permet pas au fond de 'boucler' sur lui-meme, mais est bien plus rapide que InfLargeScroll...
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Numéro du fond que l'on veut charger (de 0 à 3 en mode 0, uniquement 2 et 3 en mode 2)
    \param x
      \~english X value to scroll
      \~french Valeur X à déplacer
    \param y
      \~english Y value to scroll
      \~french Valeur Y à déplacer	  
*/
extern inline void PA_LargeScrollXY(bool screen, u8 bg_select, s32 x, s32 y){
	PA_LargeScrollX(screen, bg_select, x);
	PA_LargeScrollY(screen, bg_select, y);
}



/*!
    \fn extern inline void PA_InitParallaxX(bool screen, s32 bg0, s32 bg1, s32 bg2, s32 bg3)
    \brief
      \~english Initialise Parallax Scrolling for multiple backgrounds, horizontaly. Chose the speed at which each background will scroll compared to the others. Then use PA_ParallaxScrollX to scroll...
      \~french Initialiser le Parallax Scrolling pour plusieurs fonds, horizontalement. Choix de la vitesse à laquelle les fonds vont défiler par rapport aux autres... Utiliser PA_ParallaxScrollX par la suite pour scroller
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg0
      \~english Value for the first background (0). Set to 256 for normal scroll speed, lower for lower speed (128 is half speed...), higher for faster (512 is twice as fast...). You can set negative values. 0 inactivates parallax scrolling for this background
      \~french Valeur pour le premier fond (0). 256 met en vitesse normal, moins pour moins lent (128 pour moitié de vitesse), plus pour plus rapide (512 équivaut à 2 fois plus vite). On peut utiliser des valeurs négatives. 0 désactive le scrolling parallax pour ce fond
    \param bg1
      \~english Same thing for Background 1
      \~french Idem, pour le Fond 1
    \param bg2
      \~english Same thing for Background 2
      \~french Idem, pour le Fond 2
    \param bg3
      \~english Same thing for Background 3
      \~french Idem, pour le Fond 3
*/
extern inline void PA_InitParallaxX(bool screen, s32 bg0, s32 bg1, s32 bg2, s32 bg3) {
	 PA_parallaxX[screen][0] = bg0;
	 PA_parallaxX[screen][1] = bg1;
	 PA_parallaxX[screen][2] = bg2;
	 PA_parallaxX[screen][3] = bg3;
}

/*!
    \fn extern inline void PA_InitParallaxY(bool screen, s32 bg0, s32 bg1, s32 bg2, s32 bg3)
    \brief
      \~english Initialise Parallax Scrolling for multiple backgrounds, horizontaly. Chose the speed at which each background will scroll compared to the others. Then use PA_ParallaxScrollX to scroll...
      \~french Initialiser le Parallax Scrolling pour plusieurs fonds, horizontalement. Choix de la vitesse à laquelle les fonds vont défiler par rapport aux autres... Utiliser PA_ParallaxScrollX par la suite pour scroller
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg0
      \~english Value for the first background (0). Set to 256 for normal scroll speed, lower for lower speed (128 is half speed...), higher for faster (512 is twice as fast...). You can set negative values. 0 inactivates parallax scrolling for this background
      \~french Valeur pour le premier fond (0). 256 met en vitesse normal, moins pour moins lent (128 pour moitié de vitesse), plus pour plus rapide (512 équivaut à 2 fois plus vite). On peut utiliser des valeurs négatives. 0 désactive le scrolling parallax pour ce fond
    \param bg1
      \~english Same thing for Background 1
      \~french Idem, pour le Fond 1
    \param bg2
      \~english Same thing for Background 2
      \~french Idem, pour le Fond 2
    \param bg3
      \~english Same thing for Background 3
      \~french Idem, pour le Fond 3
*/
extern inline void PA_InitParallaxY(bool screen, s32 bg0, s32 bg1, s32 bg2, s32 bg3) {
	 PA_parallaxY[screen][0] = bg0;
	 PA_parallaxY[screen][1] = bg1;
	 PA_parallaxY[screen][2] = bg2;
	 PA_parallaxY[screen][3] = bg3;
}



/*!
    \fn extern inline void PA_ParallaxScrollX(bool screen, s32 x)
    \brief
      \~english Scroll the backgrounds
      \~french Déplacer les fonds activés pour le parallax...
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param x
      \~english X value to scroll
      \~french Valeur X à déplacer
*/
extern inline void PA_ParallaxScrollX(bool screen, s32 x){
u8 i;
	for(i = 0; i < 4; i++) {
		if (PA_parallaxX[screen][i])  {
			if(scrollpos[screen][i].infscroll) PA_InfLargeScrollX(screen, i, (PA_parallaxX[screen][i] * x) >> 8); // Si bg en scrolling infini
			else PA_BGScrollX(screen, i, (PA_parallaxX[screen][i] * x) >> 8); // Sinon scrolling normal
		}
	}
}


/*!
    \fn extern inline void PA_ParallaxScrollY(bool screen, s32 y)
    \brief
      \~english Scroll the backgrounds
      \~french Déplacer les fonds activés pour le parallax...
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param y
      \~english Y value to scroll
      \~french Valeur Y à déplacer
*/
extern inline void PA_ParallaxScrollY(bool screen, s32 y){
u8 i;
	for(i = 0; i < 4; i++) {
		if (PA_parallaxY[screen][i])  {
			if(scrollpos[screen][i].infscroll) PA_InfLargeScrollY(screen, i, (PA_parallaxY[screen][i] * y) >> 8); // Si bg en scrolling infini
			else PA_BGScrollY(screen, i, (PA_parallaxY[screen][i] * y) >> 8); // Sinon scrolling normal
		}
	}
}


/*!
    \fn extern inline void PA_ParallaxScrollXY(bool screen, s32 x, s32 y)
    \brief
      \~english Scroll the backgrounds
      \~french Déplacer les fonds activés pour le parallax...
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param x
      \~english X value to scroll
      \~french Valeur X à déplacer
    \param y
      \~english Y value to scroll
      \~french Valeur Y à déplacer
*/
extern inline void PA_ParallaxScrollXY(bool screen, s32 x, s32 y){
	PA_ParallaxScrollX(screen, x);
	PA_ParallaxScrollY(screen, y);
}



/*!
    \fn extern inline void PA_SetBgPrio(bool screen, u8 bg, u8 prio)
    \brief
      \~english Change a backgrounds priority
      \~french Changer la priorité d'un fond
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \param bg
      \~english Background...
      \~french Numéro du fond...
    \param prio
      \~english Priority level (0-3, 0 being the highest)
      \~french Niveau de priorité, de 0 à 3, 0 étant priorité la plus élevée
*/
extern inline void PA_SetBgPrio(bool screen, u8 bg, u8 prio) {
	_REG16(REG_BGCNT(screen, bg)) &= ~3;
	_REG16(REG_BGCNT(screen, bg)) |= prio;
}

extern inline void PA_CreateBgFromTiles(bool screen, u8 bg_select, u8 bg_tiles, void *bg_map, u8 bg_size){
PA_LoadBgMap(screen, bg_select, bg_map, bg_size);
scrollpos[screen][bg_select].infscroll = 0; // Par défaut pas de scrolling infini...
PA_bgmap[screen][bg_select] = ScreenBaseBlock(screen, mapchar[screen][bg_select]);
tilesetchar[screen][bg_select] = tilesetchar[screen][bg_tiles];
tilesetsize[screen][bg_select] = tilesetsize[screen][bg_tiles];

_REG16(REG_BGSCREEN(screen)) |= (0x100 << (bg_select));
_REG16(REG_BGCNT(screen, bg_select)) = bg_select | (bg_size << 14) |(mapchar[screen][bg_select] << SCREEN_SHIFT) | (1 << 13) | (tilesetchar[screen][bg_select] << 2) | (1 << 7);
PA_BGScrollXY(screen, bg_select, 0, 0);	
}

/** @} */ // end of Backgrounds



// Pas utilisé en direct par le programmaeur
void PA_InitLargeBg(bool screen, u8 bg_select, s32 lx, s32 ly, void* bg_map);



#endif

