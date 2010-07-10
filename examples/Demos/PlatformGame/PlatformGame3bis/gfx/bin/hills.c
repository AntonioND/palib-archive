#include <PA_BgStruct.h>

extern const char hills_Tiles[];
extern const char hills_Map[];
extern const char hills_Pal[];

const PA_BgStruct hills = {
	PA_BgLarge,
	768, 256,

	hills_Tiles,
	hills_Map,
	{hills_Pal},

	2432,
	{6144}
};
