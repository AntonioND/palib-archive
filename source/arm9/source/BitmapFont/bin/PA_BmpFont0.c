#include <PA_BgStruct.h>

extern const char PA_BmpFont0_Tiles[];
extern const char PA_BmpFont0_Map[];
extern const char PA_BmpFont0_Sizes[];

const PA_BgStruct PA_BmpFont0 = {
	PA_Font1bit,
	256, 64,

	PA_BmpFont0_Tiles,
	PA_BmpFont0_Map,
	{PA_BmpFont0_Sizes},

	6400,
	{6}
};
