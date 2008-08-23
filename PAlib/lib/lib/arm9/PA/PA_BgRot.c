#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"

u8 rotbg_size[2][4]; // Background size of each possible rotbg
u8  rotbg_place[4] = {1, 1, 2, 8};
u16  rotbg_sizes[4] = {1024, 1024, 2048, 8192};


void PA_LoadRotBgMap(u8 screen, u8 bg_select, void* bg_map, u8 bg_size) {
s16 charset = -1;
u8 charsetok = 0;
u8 blocksize = rotbg_place[bg_size];
u8 i;
u8 tempsize;
rotbg_size[screen][bg_select] = bg_size;  // Save rot bg size (values 0-3 for 128, 256, 512 and 1024)

PA_DeleteMap(screen, bg_select);

while ((charset < 31 ) && (!charsetok)) {
	++charset;
	tempsize = blocksize;
	for(i = 0; i < blocksize; i++) {
		if (charblocks[screen][charset + i] == 0) { // Si on a de la place...
			--tempsize;
		}
	}
	charsetok = !tempsize;  // Si on a trouvé suffisament de blocs, on peut continuer
}

	PA_BgInfo[screen][bg_select].mapchar = charset; // On place la map à un endroit précis...
	PA_BgInfo[screen][bg_select].mapsize = blocksize;

	DMA_Copy(bg_map, (void*)ScreenBaseBlock(screen, charset), rotbg_sizes[bg_size], DMA_16NOW);

	for (i = 0; i < blocksize; i++) charblocks[screen][charset + i] = 1;  // Les blocs sont occupés
}





#ifdef __cplusplus
}
#endif



