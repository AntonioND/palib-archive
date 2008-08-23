#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"

#include <unistd.h>

char fat_base_path[256];

// Define Buffers
// Buffers de memoria (Graficos)
char* buffer_info[FAT_BG_SLOTS];		
char* buffer_map[FAT_BG_SLOTS];		
char* buffer_pal[FAT_BG_SLOTS];
char* buffer_tiles[FAT_BG_SLOTS];

type_bginfo fat_bginfo[FAT_BG_SLOTS];

// Buffers de memoria (Sfx)
char* buffer_sfx[FAT_SFX_SLOTS];

type_sfxinfo fat_sfxinfo[FAT_SFX_SLOTS];

// Buffers de memoria (Paletas)
char* sprite_palette[FAT_PALETTE_SLOTS];

// Buffers de memoria (Sprites)
char* sprite_gfx[FAT_SPRITE_SLOTS];

// Buffers de memoria (Textures)
char* texture_gfx[FAT_TEXTURE_SLOTS];

char* my_buffer[FAT_GENERAL_SLOTS];

type_geninfo fat_myinfo[FAT_GENERAL_SLOTS];


// Errores de sistema (debug)
void PA_FatError(u8 code, const char* description) {

		PA_Init();					// Inicializa la PA_Lib
		PA_InitVBL();				// Inicializa la VBL estandar
		PA_SetVideoMode(0, 0);		// Inicializa pantalla
		PA_SetVideoMode(1, 0);
		PA_ResetBgSysScreen(0);		// Reinicia el sistema de pantalla
		PA_ResetBgSysScreen(1);
		PA_ResetSpriteSys();		// Reinicia el sistema de Sprites
		PA_DisableSpecialFx(0);		// Deshabilita los efectos especiales
		PA_DisableSpecialFx(1);
		PA_InitText(0, 0);			// Inicializa el texto en ambas pantallas
		PA_InitText(1, 0);

		PA_SetTextCol(0, 31, 31, 31);

		switch (code) {		// Mensage segun el codigo

			case 0:
				PA_OutputText(0, 1, 1, "SIN ERRORES");
				PA_OutputText(1, 1, 1, "NO ERRORS");
				break;

			case 1:
				PA_OutputText(0, 1, 1, "EL ARCHIVO");
				PA_OutputText(0, 1, 3, "%s", description);
				PA_OutputText(0, 1, 5, "NO SE HA ENCONTRADO.");
				PA_OutputText(1, 1, 1, "FILE");
				PA_OutputText(1, 1, 3, "%s", description);
				PA_OutputText(1, 1, 5, "NOT FOUND.");
				break;

			case 2:
				PA_OutputText(0, 1, 1, "MEMORIA RAM INSUFICIENTE.");
				PA_OutputText(1, 1, 1, "OUT OF MEMORY.");
				break;

			case 3:
				PA_OutputText(0, 1, 1, "PARAMETRO EN LA FUNCION");
				PA_OutputText(0, 1, 3, "%s", description);
				PA_OutputText(0, 1, 5, "NO VALIDO.");
				PA_OutputText(1, 1, 1, "INVALID ARGUMENT IN");
				PA_OutputText(1, 1, 3, "%s", description);
				PA_OutputText(1, 1, 5, "FUNCTION.");
				break;

			case 4:
				PA_OutputText(0, 1, 1, "EL FONDO");
				PA_OutputText(0, 1, 3, "%s", description);
				PA_OutputText(0, 1, 5, "NO SE HA ENCONTRADO.");
				PA_OutputText(1, 1, 1, "BACKGROUND");
				PA_OutputText(1, 1, 3, "%s", description);
				PA_OutputText(1, 1, 5, "NOT FOUND.");
				break;

			case 5:
				PA_OutputText(0, 1, 1, "SLOTS DE FONDOS LLENOS.");
				PA_OutputText(1, 1, 1, "OUT OF BACKGROUND SLOTS.");
				break;

			case 255:
				PA_OutputText(0, 1, 1, "ERROR DE CODIGO");
				PA_OutputText(0, 1, 3, "EN LA FUNCION");
				PA_OutputText(0, 1, 5, "%s", description);
				PA_OutputText(1, 1, 1, "CODE ERROR ON");
				PA_OutputText(1, 1, 3, "%s", description);
				PA_OutputText(1, 1, 5, "FUNCTION");
				break;

			default:
				PA_OutputText(0, 1, 1, "ERROR DESCONOCIDO.");
				PA_OutputText(1, 1, 1, "UNKNOW ERROR.");
				break;
			
		}
		
		PA_OutputText(0, 1, 18, "CODIGO DE ERROR: %d.", code);
		PA_OutputText(0, 1, 20, "SISTEMA DETENIDO.");
		PA_OutputText(1, 1, 18, "ERROR CODE: %d.", code);
		PA_OutputText(1, 1, 20, "SYSTEM STOPPED.");
		
		while(1) {		// Repite para siempre
			PA_WaitForVBL();
		}

}


void PA_FatFreeBgBuffers(void) {
	u16 x;
	for (x = 0; x < FAT_BG_SLOTS; x ++) {
		free(buffer_info[x]);
		buffer_info[x] = NULL;		
		free(buffer_map[x]);
		buffer_map[x] = NULL;			
		free(buffer_pal[x]);
		buffer_pal[x] = NULL;
		free(buffer_tiles[x]);
		buffer_tiles[x] = NULL;
		fat_bginfo[x].tiles = 0;
		fat_bginfo[x].map = 0;
		fat_bginfo[x].slot = 0;
	}
}


void PA_FatFreeSfxBuffers(void) {
	u16 x;
	for (x = 0; x < FAT_SFX_SLOTS; x++) {
		free(buffer_sfx[x]);	
		buffer_sfx[x] = NULL;
		fat_sfxinfo[x].size = 0;
		fat_sfxinfo[x].slot = 0;	
	}
}



void PA_FatFreePalBuffers(void) {
	u16 x;
	for (x = 0; x < FAT_PALETTE_SLOTS; x++) {
		free(sprite_palette[x]);
		sprite_palette[x] = NULL;
	}
}

void PA_FatFreeSprBuffers(void) {
	u16 x;
	for (x = 0; x < FAT_SPRITE_SLOTS; x++) {
		free(sprite_gfx[x]);
		sprite_gfx[x] = NULL;
	}
}

void PA_FatFreeTexBuffers(void) {
	u16 x;
	for (x = 0; x < FAT_TEXTURE_SLOTS; x++) {
		free(texture_gfx[x]);
		texture_gfx[x] = NULL;
	}
}

void PA_FatFreeGenBuffers(void) {
	u16 x;
	for (x = 0; x < FAT_GENERAL_SLOTS; x++) {
		free(my_buffer[x]);
		my_buffer[x] = NULL;
		fat_myinfo[x].size = 0;
		fat_myinfo[x].slot = 0;
	}
}

void PA_FatInitAllBuffers(void) {
  PA_FatFreeBgBuffers();
  PA_FatFreeSfxBuffers();
  PA_FatFreePalBuffers();
  PA_FatFreeSprBuffers();
  PA_FatFreeTexBuffers();
  PA_FatFreeGenBuffers();
}



void PA_FatLoadFile(const char* reference, const char* name) {

	// Busca un slot libre
	u8 n = 0;
	u8 slot = 255;
	for (n = 0; n < FAT_GENERAL_SLOTS; n ++) {
		if (fat_myinfo[n].slot == 0) {	// Si esta libre, asignalo y sal
			slot = n;
			fat_myinfo[n].slot = 1;
			n = FAT_GENERAL_SLOTS;
		}
	}
	if (slot == 255) {		// Si no hay ningun slot libre, error
		PA_FatError(5, NULL);
	}

	free(my_buffer[slot]);		// Vacia el buffer
	my_buffer[slot] = NULL;

	u32 sizeof_file = 0;	// Tamaño del archivo a cargar

	FILE* file_gen;		// Nombres de archivo

	char filename[256];		// Nombre del archivo

	sprintf(filename, "/%s/%s", fat_base_path, name);
	file_gen = fopen(filename, "rb");
	if (file_gen) {
		// Obten el tamaño del archivo
		fseek(file_gen, 0, SEEK_END);
		sizeof_file = ftell(file_gen);
		rewind(file_gen);
		// Reserva el espacio en RAM
		my_buffer[slot] = (char*) calloc (sizeof_file, sizeof(char));
		if (my_buffer[slot] == NULL) {
			PA_FatError(2, NULL);
		}
		// Carga el archivo en RAM
		fread (my_buffer[slot], 1, sizeof_file, file_gen);
	} else {			// Si no existe...
		PA_FatError(1, name);
	}
	fclose(file_gen);	// Cierra el archivo
	PA_WaitForVBL();		// Espera al cierre de los archivos

	fat_myinfo[slot].size = sizeof_file;
	sprintf(fat_myinfo[slot].name, "%s", reference);	// Y el nombre

}



u8 PA_FatGetFileSlot(const char* reference) {

	// Busca si existe el archivo
	u8 n;
	u8 slot = 255;
	char name[32];

	sprintf(name, "%s", reference);		// Obten el nombre a buscar

	for (n = 0; n < FAT_GENERAL_SLOTS; n ++) {
		if (PA_CompareText(name, fat_myinfo[n].name) && fat_myinfo[n].slot) {	// Si coincide
			slot = n;		// Guarda el SLOT donde esta
			n = FAT_GENERAL_SLOTS;	// Y sal
		}
	}

	if (slot == 255) {		// Si el mapa no se encuentra, da error
		PA_FatError(4, reference); 
	}

	return slot;	// Devuelve el SLOT del mapa solicitado

}



void PA_FatUnloadFile(const char* reference) {

	// Busca si existe el archivo
	u8 slot = PA_FatGetFileSlot(reference);		// Guarda el SLOT donde esta

	free(my_buffer[slot]);				// Vacia el buffer
	my_buffer[slot] = NULL;
	fat_myinfo[slot].size = 0;
	fat_myinfo[slot].slot = 0;		// Y marca el slot como libre

}



// Funcion PA_FatBgLoad();		// Carga un fondo desde la FAT
void PA_FatBgLoad(const char* reference, const char* name) {

	// Busca un slot libre
	u8 n = 0;
	u8 slot = 255;
	for (n = 0; n < FAT_BG_SLOTS; n ++) {
		if (fat_bginfo[n].slot == 0) {	// Si esta libre, asignalo y sal
			slot = n;
			fat_bginfo[n].slot = 1;
			n = FAT_BG_SLOTS;
		}
	}
	if (slot == 255) {		// Si no hay ningun slot libre, error
		PA_FatError(5, NULL);
	}

	free(buffer_info[slot]);		// Vacia el buffer
	buffer_info[slot] = NULL;		
	free(buffer_map[slot]);
	buffer_map[slot] = NULL;
	free(buffer_pal[slot]);
	buffer_pal[slot] = NULL;
	free(buffer_tiles[slot]);
	buffer_tiles[slot] = NULL;

	u32 sizeof_info = 0;		// Tamaño del archivo a cargar
	u32 sizeof_map = 0;
	u32 sizeof_pal = 0;
	u32 sizeof_tiles = 0;

	FILE* file_info;		// Nombres de archivo
	FILE* file_map;
	FILE* file_pal;
	FILE* file_tiles;

	char filename[256];		// Nombre del archivo

	// Archivo INFO
	sprintf(filename, "/%s/bg/%s_Info.bin", fat_base_path, name);	// Abre Info.bin
	file_info = fopen(filename, "rb");
	if (file_info) {		// Si el archivo "Info" existe...
		// Obten el tamaño del archivo
		fseek(file_info, 0, SEEK_END);
		sizeof_info = ftell(file_info);
		rewind(file_info);
		// Reserva el espacio en RAM
		buffer_info[slot] = (char*) calloc (sizeof_info, sizeof(char));
		if (buffer_info[slot] == NULL) {
			PA_FatError(2, NULL);
		}
		// Carga el archivo en RAM
		fread (buffer_info[slot], 1, sizeof_info, file_info);
	} else {			// Si no existe...
		PA_FatError(1, name);
	}
	fclose(file_info);		// Cierra el archivo
	PA_WaitForVBL();		// Espera al cierre de los archivos

	// Archivo MAP
	sprintf(filename, "/%s/bg/%s_Map.bin", fat_base_path, name);	// Abre Map.bin
	file_map = fopen(filename, "rb");
	if (file_map) {		// Si el archivo "Map" existe...
		// Obten el tamaño del archivo
		fseek(file_map, 0, SEEK_END);
		sizeof_map = ftell(file_map);
		rewind(file_map);
		// Reserva el espacio en RAM
		buffer_map[slot] = (char*) calloc (sizeof_map, sizeof(char));
		if (buffer_map[slot] == NULL) {
			PA_FatError(2, NULL);
		}
		// Carga el archivo en RAM
		fread (buffer_map[slot], 1, sizeof_map, file_map);
	} else {			// Si no existe...
		PA_FatError(1, name);
	}
	fclose(file_map);		// Cierra el archivo
	PA_WaitForVBL();		// Espera al cierre de los archivos

	// Archivo PAL
	sprintf(filename, "/%s/bg/%s_Pal.bin", fat_base_path, name);	// Abre Pal.bin
	file_pal = fopen(filename, "rb");
	if (file_pal) {		// Si el archivo "Pal" existe...
		// Obten el tamaño del archivo
		fseek(file_pal, 0, SEEK_END);
		sizeof_pal = ftell(file_pal);
		rewind(file_pal);
		// Reserva el espacio en RAM
		buffer_pal[slot] = (char*) calloc (sizeof_pal, sizeof(char));
		if (buffer_pal[slot] == NULL) {
			PA_FatError(2, NULL);
		}
		// Carga el archivo en RAM
		fread (buffer_pal[slot], 1, sizeof_pal, file_pal);
	} else {			// Si no existe...
		PA_FatError(1, name);
	}
	fclose(file_pal);		// Cierra el archivo
	PA_WaitForVBL();		// Espera al cierre de los archivos

	// Archivo TILES
	sprintf(filename, "/%s/bg/%s_Tiles.bin", fat_base_path, name);	// Abre Tiles.bin
	file_tiles = fopen(filename, "rb");
	if (file_tiles) {		// Si el archivo "Tiles" existe...
		// Obten el tamaño del archivo
		fseek(file_tiles, 0, SEEK_END);
		sizeof_tiles = ftell(file_tiles);
		rewind(file_tiles);
		// Reserva el espacio en RAM
		buffer_tiles[slot] = (char*) calloc (sizeof_tiles, sizeof(char));
		if (buffer_tiles[slot] == NULL) {
			PA_FatError(2, NULL);
		}
		// Carga el archivo en RAM
		fread (buffer_tiles[slot], 1, sizeof_tiles, file_tiles);
	} else {			// Si no existe...
		PA_FatError(1, name);
	}
	fclose(file_tiles);		// Cierra el archivo
	PA_WaitForVBL();		// Espera al cierre de los archivos

	fat_bginfo[slot].tiles = sizeof_tiles;	// Guarda las medidas del mapa
	fat_bginfo[slot].map = sizeof_map;
	sprintf(fat_bginfo[slot].name, "%s", reference);

}



// Funcion PA_FatRamBgLoad()
void PA_FatRamBgLoad(u8 screen, u8 bg_select, const char* reference) {

	// Busca si existe el archivo
	u8 n;
	u8 slot = 255;
	char name[32];

	sprintf(name, "%s", reference);		// Obten el nombre a buscar

	for (n = 0; n < FAT_BG_SLOTS; n ++) {
		if (PA_CompareText(name, fat_bginfo[n].name) && fat_bginfo[n].slot) {	// Si coincide
			slot = n;		// Guarda el SLOT donde esta
			n = FAT_BG_SLOTS;	// Y sal
		}
	}

	if (slot == 255) {		// Si el fondo no se encuentra, da error
		PA_FatError(4, reference); 
	}

	PA_DeleteBg(screen, bg_select);			// Borra el fondo

	// Transfiere el fondo a la VRAM
	PA_EasyBgLoadEx(screen, bg_select, (u32*)buffer_info[slot], (void*)buffer_tiles[slot], fat_bginfo[slot].tiles, (void*)buffer_map[slot], (fat_bginfo[slot].map / 2), (void*)buffer_pal[slot]);

}



// Funcion PA_FatUnloadBg()
void PA_FatUnloadBg(const char* reference) {

	// Busca si existe el fondo
	u8 n;
	u8 slot = 255;
	char name[32];

	sprintf(name, "%s", reference);		// Obten el nombre a buscar

	for (n = 0; n < FAT_BG_SLOTS; n ++) {
		if (PA_CompareText(name, fat_bginfo[n].name) && fat_bginfo[n].slot) {	// Si coincide
			slot = n;		// Guarda el SLOT donde esta
			n = FAT_BG_SLOTS;	// Y sal
		}
	}

	if (slot == 255) {		// Si el fondo no se encuentra, da error
		PA_FatError(4, reference); 
	}

	// Descarga el fondo de la RAM
	free(buffer_info[slot]);
	buffer_info[slot] = NULL;		
	free(buffer_map[slot]);
	buffer_map[slot] = NULL;			
	free(buffer_pal[slot]);
	buffer_pal[slot] = NULL;
	free(buffer_tiles[slot]);
	buffer_tiles[slot] = NULL;
	fat_bginfo[slot].tiles = 0;
	fat_bginfo[slot].map = 0;
	fat_bginfo[slot].slot = 0;

}


// Funcion PA_FatLoadSfx();	// Carga un archivo RAW desde la FAT
void PA_FatLoadSfx(const char* reference, const char* name) {

	// Busca un slot libre
	u8 n = 0;
	u8 slot = 255;
	for (n = 0; n < FAT_SFX_SLOTS; n ++) {
		if (fat_sfxinfo[n].slot == 0) {	// Si esta libre, asignalo y sal
			slot = n;
			fat_sfxinfo[n].slot = 1;
			n = FAT_SFX_SLOTS;
		}
	}
	if (slot == 255) {		// Si no hay ningun slot libre, error
		PA_FatError(5, NULL);
	}

	free(buffer_sfx[slot]);		// Vacia el buffer
	buffer_sfx[slot] = NULL;
	fat_sfxinfo[slot].size = 0;

	u32 sizeof_sfx = 0;	// Tamaño del archivo a cargar

	FILE* file_sfx;		// Nombres de archivo

	char filename[256];		// Nombre del archivo

	// Abre el archivo de audio
	sprintf(filename, "/%s/sfx/%s.raw", fat_base_path, name);	// Abre el RAW
	file_sfx = fopen(filename, "rb");
	if (file_sfx) {		// Si el archivo "RAW" existe...
		// Obten el tamaño del archivo
		fseek(file_sfx, 0, SEEK_END);
		sizeof_sfx = ftell(file_sfx);
		rewind(file_sfx);
		// Reserva el espacio en RAM
		buffer_sfx[slot] = (char*) calloc (sizeof_sfx, sizeof(char));
		if (buffer_sfx[slot] == NULL) {
			PA_FatError(2, NULL);
		}
		// Carga el archivo en RAM
		fread (buffer_sfx[slot], 1, sizeof_sfx, file_sfx);
	} else {			// Si no existe...
		PA_FatError(1, name);
	}
	fclose(file_sfx);		// Cierra el archivo
	PA_WaitForVBL();		// Espera al cierre de los archivos

	fat_sfxinfo[slot].size = sizeof_sfx;				// Guarda el tamaño del archivo
	sprintf(fat_sfxinfo[slot].name, "%s", reference);	// Y el nombre

}



// Funcion PA_FatPlaySfx();		// Reproduce un archivo cargado previamente con LoadFatSfx();
void PA_FatPlaySfx(const char* reference) {

	// Busca si existe el archivo
	u8 n;
	u8 slot = 255;
	char name[32];

	sprintf(name, "%s", reference);		// Obten el nombre a buscar

	for (n = 0; n < FAT_SFX_SLOTS; n ++) {
		if (PA_CompareText(name, fat_sfxinfo[n].name) && fat_sfxinfo[n].slot) {	// Si coincide
			slot = n;		// Guarda el SLOT donde esta
			n = FAT_SFX_SLOTS;	// Y sal
		}
	}

	if (slot == 255) {		// Si el fondo no se encuentra, da error
		PA_FatError(4, reference); 
	}

	char* buffer;
	buffer = NULL;
	u32 buffer_size = 0;

	buffer = buffer_sfx[slot];
	buffer_size = fat_sfxinfo[slot].size;

	AS_SoundQuickPlay(buffer);	// Reproduce el archivo
	
}



// Funcion PA_FatUnloadSfx();		// Reproduce un archivo cargado previamente con LoadFatSfx();
void PA_FatUnloadSfx(const char* reference) {

	// Busca si existe el archivo
	u8 n;
	u8 slot = 255;
	char name[32];

	sprintf(name, "%s", reference);		// Obten el nombre a buscar

	for (n = 0; n < FAT_SFX_SLOTS; n ++) {
		if (PA_CompareText(name, fat_sfxinfo[n].name) && fat_sfxinfo[n].slot) {	// Si coincide
			slot = n;		// Guarda el SLOT donde esta
			n = FAT_SFX_SLOTS;	// Y sal
		}
	}

	if (slot == 255) {		// Si el fondo no se encuentra, da error
		PA_FatError(4, reference); 
	}

	free(buffer_sfx[slot]);		// Vacia el buffer
	buffer_sfx[slot] = NULL;
	fat_sfxinfo[slot].size = 0;
	fat_sfxinfo[slot].slot = 0;		// Y marca el slot como libre

}


// Funcion PA_FatLoadPalette();		// Carga una paleta desde la FAT al Slot dado
void PA_FatLoadPalette(u8 slot, const char* name){

	char filename[256];		// Nombre del archivo

	// Carga la paleta del Sprite
	free(sprite_palette[slot]);		// Vacia el buffer
	sprite_palette[slot] = NULL;

	u32 sizeof_palette = 0;	// Tamaño del archivo a cargar

	FILE* file_palette;		// Nombres de archivo

	// Abre el archivo de paletas
	sprintf(filename, "/%s/sprites/%s_Pal.bin", fat_base_path, name);	// Abre el _Pal.bin
	file_palette = fopen(filename, "rb");
	if (file_palette) {		// Si el archivo "RAW" existe...
		// Obten el tamaño del archivo
		fseek(file_palette, 0, SEEK_END);
		sizeof_palette = ftell(file_palette);
		rewind(file_palette);
		// Reserva el espacio en RAM
		sprite_palette[slot] = (char*) calloc (sizeof_palette, sizeof(char));
		if (sprite_palette[slot] == NULL) {
			PA_FatError(2, NULL);
		}
		// Carga el archivo en RAM
		fread (sprite_palette[slot], 1, sizeof_palette, file_palette);
	} else {			// Si no existe...
		PA_FatError(1, name);
	}
	fclose(file_palette);	// Cierra el archivo
	PA_WaitForVBL();		// Espera al cierre de los archivos

}


// Funcion PA_FatFreePalette();		// Libera la paleta de la RAM una vez cargado en VRAM
void PA_FatFreePalette(u8 slot) {
	free(sprite_palette[slot]);
	sprite_palette[slot] = NULL;
}



// Funcion PA_FatLoadSprite();		// Carga un Sprite desde la FAT al Slot dado
void PA_FatLoadSprite(u8 slot, const char* name){

	char filename[256];		// Nombre del archivo

	// Carga el grafico del Sprite
	free(sprite_gfx[slot]);		// Vacia el buffer
	sprite_gfx[slot] = NULL;

	u32 sizeof_sprite = 0;	// Tamaño del archivo a cargar

	FILE* file_sprite;		// Nombres de archivo

	// Abre el archivo de graficos
	sprintf(filename, "/%s/sprites/%s_Sprite.bin", fat_base_path, name);	// Abre el _Sprite.bin
	file_sprite = fopen(filename, "rb");
	if (file_sprite) {		// Si el archivo "RAW" existe...
		// Obten el tamaño del archivo
		fseek(file_sprite, 0, SEEK_END);
		sizeof_sprite = ftell(file_sprite);
		rewind(file_sprite);
		// Reserva el espacio en RAM
		sprite_gfx[slot] = (char*) calloc (sizeof_sprite, sizeof(char));
		if (sprite_gfx[slot] == NULL) {
			PA_FatError(2, NULL);
		}
		// Carga el archivo en RAM
		fread (sprite_gfx[slot], 1, sizeof_sprite, file_sprite);
	} else {			// Si no existe...
		PA_FatError(1, name);
	}
	fclose(file_sprite);	// Cierra el archivo
	PA_WaitForVBL();		// Espera al cierre de los archivos

}


// Funcion PA_FatFreeSprite();		// Libera el Sprite de la RAM una vez cargado en VRAM
void PA_FatFreeSprite(u8 slot) {
	free(sprite_gfx[slot]);
	sprite_gfx[slot] = NULL;
}


// Funcion PA_FatLoadTexture();		// Carga un Texture desde la FAT al Slot dado
void PA_FatLoadTexture(u8 slot, const char* name){

	char filename[256];		// Nombre del archivo

	// Carga el grafico del Texture
	free(texture_gfx[slot]);		// Vacia el buffer
	texture_gfx[slot] = NULL;

	u32 sizeof_texture = 0;	// Tamaño del archivo a cargar

	FILE* file_texture;		// Nombres de archivo

	// Abre el archivo de graficos
	sprintf(filename, "/%s/sprites/%s_Texture.bin", fat_base_path, name);	// Abre el _Sprite.bin
	file_texture = fopen(filename, "rb");
	if (file_texture) {		// Si el archivo "RAW" existe...
		// Obten el tamaño del archivo
		fseek(file_texture, 0, SEEK_END);
		sizeof_texture = ftell(file_texture);
		rewind(file_texture);
		// Reserva el espacio en RAM
		texture_gfx[slot] = (char*) calloc (sizeof_texture, sizeof(char));
		if (texture_gfx[slot] == NULL) {
			PA_FatError(2, NULL);
		}
		// Carga el archivo en RAM
		fread (texture_gfx[slot], 1, sizeof_texture, file_texture);
	} else {			// Si no existe...
		PA_FatError(1, name);
	}
	fclose(file_texture);	// Cierra el archivo
	PA_WaitForVBL();		// Espera al cierre de los archivos

}


// Funcion PA_FatFreeTexture();		// Libera el Texture de la RAM una vez cargado en VRAM
void PA_FatFreeTexture(u8 slot) {
	free(texture_gfx[slot]);
	texture_gfx[slot] = NULL;
}

#ifdef __cplusplus
}
#endif

