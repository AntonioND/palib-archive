#include <PA_BgStruct.h>

extern const char Rot_Tiles[];
extern const char Rot_Map[];
extern const char Rot_Pal[];

const PA_BgStruct Rot = {
	PA_BgRot,
	256, 256,

	Rot_Tiles,
	Rot_Map,
	{Rot_Pal},

	1024,
	{1024}
};
