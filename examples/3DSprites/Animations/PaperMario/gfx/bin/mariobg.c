#include <PA_BgStruct.h>

extern const char mariobg_Tiles[];
extern const char mariobg_Map[];
extern const char mariobg_Pal[];

const PA_BgStruct mariobg = {
	PA_BgNormal,
	256, 192,

	mariobg_Tiles,
	mariobg_Map,
	{mariobg_Pal},

	11840,
	{1536}
};
