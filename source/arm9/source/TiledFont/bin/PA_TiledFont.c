#include <PA_BgStruct.h>

extern const char PA_TiledFont_Tiles[];
extern const char PA_TiledFont_Map[];
extern const char PA_TiledFont_Pal[];

const PA_BgStruct PA_TiledFont = {
	PA_BgNormal,
	256, 192,

	PA_TiledFont_Tiles,
	PA_TiledFont_Map,
	{PA_TiledFont_Pal},

	13248,
	{1536}
};
