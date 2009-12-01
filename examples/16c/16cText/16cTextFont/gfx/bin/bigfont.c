#include <PA_BgStruct.h>

extern const char bigfont_Tiles[];
extern const char bigfont_Map[];
extern const char bigfont_Sizes[];

const PA_BgStruct bigfont = {
	PA_Font4bit,
	512, 128,

	bigfont_Tiles,
	bigfont_Map,
	{bigfont_Sizes},

	8256,
	{14}
};
