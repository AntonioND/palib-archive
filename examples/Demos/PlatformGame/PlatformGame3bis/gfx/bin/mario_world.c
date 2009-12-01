#include <PA_BgStruct.h>

extern const char mario_world_Tiles[];
extern const char mario_world_Map[];
extern const char mario_world_Pal[];

const PA_BgStruct mario_world = {
	PA_BgLarge,
	1024, 256,

	mario_world_Tiles,
	mario_world_Map,
	{mario_world_Pal},

	1536,
	{8192}
};
