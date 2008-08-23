#ifndef _PA_FATLOAD
#define _PA_FATLOAD

#ifdef __cplusplus
extern "C" {
#endif

/*! \file PA_FATLoad.h
    \brief Functions for loading graphics and sound effects from FAT

    Load graphics or audio assets from FAT (EFS or regular FAT with DLDI) to RAM and then use them. Includes a slot-based management system for shadowed assets in RAM and a bebug error message system. These functions are provided by NightFox.
*/


#include "PA9.h"

// Define las constantes globlales
#define FAT_BG_SLOTS 32					// Slots para fondos
#define FAT_SFX_SLOTS 32				// Slots para efectos de sonido
#define FAT_PALETTE_SLOTS 32			// Slots para paletas
#define FAT_SPRITE_SLOTS 256				// Slots para sprites
#define FAT_TEXTURE_SLOTS 256				// Slots para textures
#define FAT_GENERAL_SLOTS 32

extern char fat_base_path[256];

// Define Buffers
// Buffers de memoria (Graficos) [32] - FAT_BG_SLOTS
extern char* buffer_info[FAT_BG_SLOTS];		
extern char* buffer_map[FAT_BG_SLOTS];		
extern char* buffer_pal[FAT_BG_SLOTS];
extern char* buffer_tiles[FAT_BG_SLOTS];

typedef struct {	// Informacion de los fondos cargados
	char name[32];	// Nombre del fondo
	u32 tiles;		// Tamaño de los tiles
	u32 map;		// Tamaño del mapa
	u8 slot;		// Estado del slot (libre o en uso)
} type_bginfo;
extern type_bginfo bginfo[FAT_BG_SLOTS];

// Buffers de memoria (Sfx) [32] - FAT_SFX_SLOTS
extern char* buffer_sfx[FAT_SFX_SLOTS];

typedef struct {	// Informacion de los sonidos cargados
	char name[32];	// Nombre del fondo
	u32 size;		// Tamaño del archivo
	u8 slot;		// Estado del slot (libre o en uso)
} type_sfxinfo;
extern type_sfxinfo sfxinfo[FAT_SFX_SLOTS];

// Buffers de memoria (Paletas) [32] - FAT_PALETTE_SLOTS
extern char* sprite_palette[FAT_PALETTE_SLOTS];

// Buffers de memoria (Sprites) [256] - FAT_SPRITE_SLOTS
extern char* sprite_gfx[FAT_SPRITE_SLOTS];

// Buffers de memoria (Sprites) [256] - FAT_TEXTURE_SLOTS
extern char* texture_gfx[FAT_TEXTURE_SLOTS];

extern char* my_buffer[FAT_GENERAL_SLOTS];

typedef struct {	// Informacion de los mapas cargados
	char name[32];	// Nombre del fondo
	u32 size;		// Tamaño del archivo
	u8 slot;		// Estado del slot (libre o en uso)
} type_geninfo;
extern type_geninfo fat_myinfo[FAT_GENERAL_SLOTS];



/** @defgroup FATLoad FAT Loading
 *  Load graphics or audio assets from FAT (EFS or regular FAT with DLDI) to RAM and then use them. Includes a slot-based management system for shadowed assets in RAM and a bebug error message system. These functions are provided by NightFox.
 *  @{
 */


/*! \fn void PA_FatError(u8 code, const char* description)
    \brief
         \~english Error messages for debugging
         \~french 
    \~\param code
         \~english Error code
         \~french 
    \~\param description
         \~english Description of the error or filename
         \~french 
*/
void PA_FatError(u8 code, const char* description);		// Mensages de error para debug


/*! \fn void PA_FatFreeBgBuffers(void)
    \brief
         \~english Free the background memory buffers and reset them.
         \~french 
*/
void PA_FatFreeBgBuffers(void);


/*! \fn void PA_FatFreeSfxBuffers(void)
    \brief
         \~english Free the sound memory buffers and reset them.
         \~french 
*/
void PA_FatFreeSfxBuffers(void);



/*! \fn void PA_FatFreePalBuffers(void)
    \brief
         \~english Free the palette memory buffers and reset them.
         \~french 
*/
void PA_FatFreePalBuffers(void);


/*! \fn void PA_FatFreeSprBuffers(void)
    \brief
         \~english Free the sprite memory buffers and reset them.
         \~french 
*/
void PA_FatFreeSprBuffers(void);


/*! \fn void PA_FatFreeTexBuffers(void)
    \brief
         \~english Free the texture memory buffers and reset them.
         \~french 
*/
void PA_FatFreeTexBuffers(void);


/*! \fn void PA_FatFreeGenBuffers(void)
    \brief
         \~english Free the general use memory buffers and reset them.
         \~french 
*/
void PA_FatFreeGenBuffers(void);


/*! \fn void PA_FatInitAllBuffers(void)
    \brief
         \~english Initialize all the memory buffers. Call this first.
         \~french 
*/
void PA_FatInitAllBuffers(void);		// Inicializa los buffers de memoria


/*! \fn extern inline void PA_FatSetBasePath(const char* path)
    \brief
         \~english Set the path where loading functions will search for your files.
         \~french 
    \~\param path
         \~english Name of the base path where your date files will be stored. Do not include beginnig or trailing slashes
         \~french 
*/
extern inline void PA_FatSetBasePath(const char* path)
{
  sprintf(fat_base_path, "%s", path);
};


/*! \fn void PA_FatLoadFile(const char* reference, const char* name)
    \brief
         \~english Load a file from FAT into RAM for any use.
         \~french 
   \~\param reference
         \~english Reference name to assign the file
         \~french 
   \~\param name
         \~english Name of a file stored in FAT (can be the same as the reference name)
         \~french 
*/
void PA_FatLoadFile(const char* reference, const char* name);


/*! \fn u8 PA_FatGetFileSlot(const char* reference)
    \brief
         \~english Returns the slot number of a general use file loaded into RAM with PA_FatLoadFile.
         \~french 
   \~\param reference
         \~english Reference name of the file to check
         \~french 
*/
u8 PA_FatGetFileSlot(const char* reference);		// Devuelve el SLOT



/*! \fn void PA_FatUnloadFile(const char* reference)
    \brief
         \~english Remove a collision map from RAM.
         \~french 
   \~\param reference
         \~english Reference name of the file in RAM to delete
         \~french 
*/
void PA_FatUnloadFile(const char* reference);



/*! \fn void PA_FatBgLoad(const char* reference, const char* name)
    \brief
         \~english Load a background from FAT into RAM and set up a reference for it.
         \~french 
    \~\param reference
         \~english Reference name to assign the background
         \~french 
    \~\param name
         \~english Name of the background stored in FAT (can be the same as the reference name)
         \~french 
*/
void PA_FatBgLoad(const char* reference, const char* name);	// Carga un fondo desde la FAT a la RAM


/*! \fn void PA_FatRamBgLoad(u8 screen, u8 bg_select, const char* reference)
    \brief
         \~english Display a background on screen from a shadow loaded to RAM with PA_FatBgLoad.
         \~french 
    \~\param screen
         \~english Screen (0 or 1)
         \~french 
    \~\param bg_select
         \~english Background number (0-3)
         \~french 
    \~\param reference
         \~english Reference name of the background in RAM
         \~french 
*/
void PA_FatRamBgLoad(u8 screen, u8 bg_select, const char* reference);		// Carga un fondo de la RAM a la pantalla


/*! \fn void PA_FatUnloadBg(const char* reference)
    \brief
         \~english Remove a background from RAM (but not from VRAM).
         \~french 
    \~\param reference
         \~english Reference name of the background to delete
         \~french 
*/
void PA_FatUnloadBg(const char* reference);		// Descarga un fondo de la RAM


/*! \fn extern inline void PA_FatEasyBgLoad(u8 screen, u8 bg_select, const char* name)
    \brief
         \~english Easiest way to load a background from FAT and display it on the screen. Backgrounds should be converted with PAGfx.
         \~french 
    \~\param screen
         \~english Screen (0 or 1)
         \~french 
    \~\param bg_select
         \~english Background number (0-3)
         \~french 
    \~\param name
         \~english Name of the background stored in FAT
         \~french 
*/
extern inline void PA_FatEasyBgLoad(u8 screen, u8 bg_select, const char* name)
{
  PA_FatBgLoad(name, name);
  PA_FatRamBgLoad(screen, bg_select, name);
  PA_FatUnloadBg(name);
};


/*! \fn void PA_FatLoadSfx(const char* reference, const char* name)
    \brief
         \~english Load a sound from FAT into RAM and set up a reference for it.
         \~french 
      \~\param reference
         \~english Reference name to assign the sound
         \~french 
    \~\param name
         \~english Name of the sound stored in FAT (can be the same as the reference name)
         \~french 
*/
void PA_FatLoadSfx(const char* reference, const char* name);		// Carga un archivo RAW desde la FAT


/*! \fn void PA_FatPlaySfx(const char* reference)
    \brief
         \~english Play a sound from RAM which has been loaded with PA_FatLoadSfx.
         \~french 
      \~\param reference
         \~english Reference name of sound to play
         \~french 
*/
void PA_FatPlaySfx(const char* reference);		// Reproduce un archivo cargado con LoadFatSfx();


/*! \fn void PA_FatUnloadSfx(const char* reference)
    \brief
         \~english Remove a sound from RAM.
         \~french 
      \~\param reference
         \~english Reference name of sound to delete
         \~french 
*/
void PA_FatUnloadSfx(const char* reference);		// Descarga un sonido de la RAM

/*! \fn extern inline void PA_FatEasyPlaySfx(const char* name)
    \brief
         \~english Easiest way to play a RAW sound effect from FAT. (This function is currently not working perfectly. PA_FatLoadSfx and PA_FatPlaySfx are the preferred method.)
         \~french 
   \~\param name
         \~english Name of sound stored in FAT
         \~french 
*/
extern inline void PA_FatEasyPlaySfx(const char* name)
{
  PA_FatLoadSfx(name, name);
  PA_FatPlaySfx(name);
  PA_FatUnloadSfx(name);
};


/*! \fn void PA_FatLoadPalette(u8 slot, const char* name)
    \brief
         \~english Load a palette from FAT into regular RAM. Palette files will be generated when converting a sprite in PAGfx.
         \~french 
   \~\param slot
         \~english RAM slot to use (0-31)
         \~french 
   \~\param name
         \~english Name of palette in FAT to load
         \~french 
*/
void PA_FatLoadPalette(u8 slot, const char* name);	// Carga una paleta desde la FAT en el Slot dado


/*! \fn void PA_FatFreePalette(u8 slot)
    \brief
         \~english Remove a palette from RAM (but not VRAM).
         \~french 
   \~\param slot
         \~english RAM slot to free(0-31)
         \~french 
*/
void PA_FatFreePalette(u8 slot);	// Libera la RAM usada por una Paleta una vez cargada en VRAM


/*! \fn extern inline void PA_FatEasyLoadSpritePal(u8 screen, u8 palette_num, const char* name)
    \brief
         \~english Easiest way to load a sprite palette from FAT into VRAM for direct use. Palette files will be generated when converting a sprite in PAGfx.
         \~french 
 \~\param screen
         \~english screen (0 or 1)
         \~french 
\~\param palette_num
         \~english Palette number (0-15, per screen) 
         \~french 
\~\param name
         \~english Name of the palette to load from FAT
         \~french 
*/
extern inline void PA_FatEasyLoadSpritePal(u8 screen, u8 palette_num, const char* name)
{
  PA_FatLoadPalette(FAT_PALETTE_SLOTS-1, name);
  PA_LoadSpritePal(screen, palette_num, (void*)sprite_palette[FAT_PALETTE_SLOTS-1]);
  PA_FatFreePalette(FAT_PALETTE_SLOTS-1);
};


/*! \fn extern inline void PA_FatEasyLoad3DSpritePal(u8 palette_num, const char* name)
    \brief
         \~english Easiest way to load a 3D sprite palette from FAT into VRAM for direct use. Palette files will be generated when converting a sprite in PAGfx.
         \~french 
 \~\param palette_num
         \~english Palette number (0-15, per screen) 
         \~french 
\~\param name
         \~english Name of the palette to load from FAT
         \~french 
*/
extern inline void PA_FatEasyLoad3DSpritePal(u8 palette_num, const char* name)
{
  PA_FatLoadPalette(FAT_PALETTE_SLOTS-1, name);
  PA_Load3DSpritePal(palette_num, (void*)sprite_palette[FAT_PALETTE_SLOTS-1]);
  PA_FatFreePalette(FAT_PALETTE_SLOTS-1);
};


/*! \fn void PA_FatLoadSprite(u8 slot, const char* name)
    \brief
         \~english Load a sprite from FAT into regular RAM. Sprites should be converted with PAGfx.
         \~french 
   \~\param slot
         \~english RAM slot to use (0-255)
         \~french 
   \~\param name
         \~english Name of sprite in FAT to load
         \~french 
*/
void PA_FatLoadSprite(u8 slot, const char* name);	// Carga un Sprite desde la FAT en el Slot dado


/*! \fn void PA_FatFreeSprite(u8 slot)
    \brief
         \~english Remove a sprite from RAM (but not VRAM).
         \~french 
   \~\param slot
         \~english RAM slot to free(0-255)
         \~french 
*/
void PA_FatFreeSprite(u8 slot);	// Libera la RAM usada por un Sprite una vez cargado en VRAM



/*! \fn extern inline void PA_FatEasyCreateSprite(u8 screen, u8 obj_number, const char* name, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, s16 x, s16 y)
    \brief
         \~english Easiest way to load a sprite palette from FAT and display it on the screen. Sprites should be converted with PAGfx.
         \~french 
   \~\param screen
         \~english Screen (0 or 1)
         \~french 
\~\param obj_number
         \~english Object number you want to use (0-127 for each screen seperately)
         \~french 
\~\param name
         \~english Name of the sprite to load from FAT
         \~french 
\~\param obj_shape
         \~english Object shape. You can use the OBJ_SIZE_32X32 (etc.) macros for object shape and obj_size... 
         \~french 
\~\param obj_size
         \~english Object size. You can use the OBJ_SIZE_32X32 ( etc.) macros for object shape and obj_size... 
         \~french 
\~\param color_mode
         \~english 256 or 16 color mode (1 or 0).  
         \~french 
\~\param palette
         \~english  Palette to use (0-15).  
         \~french 
\~\param x
         \~english X axis position of sprite
         \~french 
\~\param y
         \~english Y axis position of sprite
         \~french 
*/
extern inline void PA_FatEasyCreateSprite(u8 screen, u8 obj_number, const char* name, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, s16 x, s16 y)
{
  PA_FatLoadSprite(FAT_SPRITE_SLOTS-1, name);
  PA_CreateSprite(screen, obj_number, (void*)sprite_gfx[FAT_SPRITE_SLOTS-1], obj_shape, obj_size, color_mode, palette, x, y);		// Crealos
  PA_FatFreeSprite(FAT_SPRITE_SLOTS-1);
};


/*! \fn extern inline u16 PA_FatEasyCreateGfx(u8 screen, const char* name, u8 obj_shape, u8 obj_size, u8 color_mode)
    \brief
         \~english Easiest way to load sprite gfx from FAT into VRAM for later use. Returns the gfx's number in memory. Sprites should be converted with PAGfx.
         \~french 
   \~\param screen
         \~english Screen (0 or 1)
         \~french 
\~\param name
         \~english Name of the sprite to load from FAT
         \~french 
\~\param obj_shape
         \~english Object shape. You can use the OBJ_SIZE_32X32 (etc.) macros for object shape and obj_size... 
         \~french 
\~\param obj_size
         \~english Object size. You can use the OBJ_SIZE_32X32 ( etc.) macros for object shape and obj_size... 
         \~french 
\~\param color_mode
         \~english 256 or 16 color mode (1 or 0).  
         \~french 
*/
extern inline u16 PA_FatEasyCreateGfx(u8 screen, const char* name, u8 obj_shape, u8 obj_size, u8 color_mode)
{
  PA_FatLoadSprite(FAT_SPRITE_SLOTS-1, name);
  u16 gfx = PA_CreateGfx(screen, (void*)sprite_gfx[FAT_SPRITE_SLOTS-1], obj_shape, obj_size, color_mode);
  PA_FatFreeSprite(FAT_SPRITE_SLOTS-1);

  return gfx;
}


/*! \fn void PA_FatLoadTexture(u8 slot, const char* name)
    \brief
         \~english Load a texture from FAT into regular RAM for 3d sprites. Textures should be converted with PAGfx.
         \~french 
   \~\param slot
         \~english RAM slot to use (0-255)
         \~french 
   \~\param name
         \~english Name of texture in FAT to load
         \~french 
*/
void PA_FatLoadTexture(u8 slot, const char* name);	// Carga un Texture desde la FAT en el Slot dado


/*! \fn void PA_FatFreeTexture(u8 slot)
    \brief
         \~english Remove a texture from RAM (but not VRAM).
         \~french 
   \~\param slot
         \~english RAM slot to free(0-255)
         \~french 
*/
void PA_FatFreeTexture(u8 slot);	// Libera la RAM usada por un Sprite una vez cargado en VRAM



/*! \fn extern inline s16 PA_FatEasy3DCreateTex(const char* name, u16 width, u16 height, u8 type)
    \brief
         \~english Easiest way to load a texture for 3D sprites from FAT into VRAM for later use. Returns the gfx's number in memory. Textures should be converted with PAGfx.
         \~french 
\~\param name
         \~english Name of the texture to load from FAT
         \~french 
\~\param width
         \~english Texture width. Must be a factor of two (2, 4, 8, 16, 32, 64, 128, etc...)
         \~french 
\~\param height
         \~english Texture height. Must be a factor of two (2, 4, 8, 16, 32, 64, 128, etc...)
         \~french 
\~\param type
         \~english Color type. You can use the TEX_256COL, TEX_16COL, TEX_4COL (etc...) macros.
         \~french 
*/
extern inline u16 PA_FatEasy3DCreateTex(const char* name, u16 width, u16 height, u8 type)
{
  PA_FatLoadTexture(FAT_TEXTURE_SLOTS-1, name);
  s16 tex = PA_3DCreateTex((void*)texture_gfx[FAT_TEXTURE_SLOTS-1], width, height, type);
  PA_FatFreeTexture(FAT_TEXTURE_SLOTS-1);

  return tex;
}


/** @} */ // end of Fat Loading group


#ifdef __cplusplus
}
#endif


#endif
