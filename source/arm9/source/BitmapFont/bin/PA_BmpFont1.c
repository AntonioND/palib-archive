#include <PA_BgStruct.h>

extern const char PA_BmpFont1_Tiles[];
extern const char PA_BmpFont1_Map[];
extern const char PA_BmpFont1_Sizes[];

const PA_BgStruct PA_BmpFont1 = {
	PA_Font1bit,
	256, 64,

	PA_BmpFont1_Tiles,
	PA_BmpFont1_Map,
	{PA_BmpFont1_Sizes},

	6720,
	{8}
};
