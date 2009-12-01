#include <PA9.h>

#include "TransBg/all_gfx.h"

u8 pa_bg_trans[2];

void PA_InitBgTransEx(u8 screen, u8 bg) {
	pa_bg_trans[screen] = bg;
	PA_LoadBackground(screen, bg, &PA_TransBg);
	PA_BgTransUpDown(screen, 0, 0, TRANS_LENGTH); // Hide
}

void PA_BgTransTile(u8 screen, u16 type, s16 x, s16 y, s16 temptile) {
	u16* map = (u16*) PA_TransBg.BgMap;
	PA_SetMapTileAll(screen, pa_bg_trans[screen], x, y, map[temptile*2+type]);
	PA_SetMapTileAll(screen, pa_bg_trans[screen], x + 1, y, map[temptile*2+1+type]);
	PA_SetMapTileAll(screen, pa_bg_trans[screen], x, y + 1, map[temptile*2+32+type]);
	PA_SetMapTileAll(screen, pa_bg_trans[screen], x + 1, y + 1, map[temptile*2+1+32+type]);
}

void PA_BgTransUpDown(u8 screen, u16 type, u8 vflip, s16 state) {
	int i, j;
	s16 tile = state >> 1;
	s16 temptile;
	int y;
	type = type << 6;
	int x;

	for (j = 0; j < 12; j++) {
		y = j * 2;

		if (vflip) y = (11 - j) * 2;

		temptile = tile;

		if (temptile > 15) temptile = 15; // limit range...

		for (i = 0; i < 16; i++) {
			x = i * 2;
			PA_BgTransTile(screen, type, x, y, temptile);
		}

		if (tile) tile--;
	}
}

void PA_BgTransLeftRight(u8 screen, u16 type, u8 hflip, s16 state) {
	int i, j;
	s16 tile = state >> 1;
	s16 temptile;
	int y;
	type = type << 6;
	int x;

	for (j = 0; j < 16; j++) {
		x = j * 2;

		if (hflip) x = (15 - j) * 2;

		temptile = tile;

		if (temptile > 15) temptile = 15; // limit range...

		for (i = 0; i < 12; i++) {
			y = i * 2;
			PA_BgTransTile(screen, type, x, y, temptile);
		}

		if (tile) tile--;
	}
}


void PA_BgTransDiag(u8 screen, u16 type, u8 hflip, u8 vflip, s16 state) {
	int i, j;
	s16 tile = state >> 1;
	s16 temptile;
	int y;
	type = type << 6;
	y = 0;
	int tempx, tempy;

	for (j = 0; j < 32; j++) {
		y = j;
		temptile = tile;

		if (temptile > 15) temptile = 15; // limit range...

		for (i = 0; (i  < 16) && (y >= 0); i++) {

			tempx = i * 2; if (hflip) tempx = (15 - i) * 2;

			if ((y >= 0) && (y < 12)) {

				tempy = y * 2; if (vflip) tempy = (11 - y) * 2;

				PA_BgTransTile(screen, type, tempx, tempy, temptile);
			}

			if (i&1)y--;

			//	y--;
		}

		if (tile) tile--;
	}
}



void PA_BgTransCenter(u8 screen, u16 type, u8 invert, s16 state) {
	int i, j;
	s16 tile = state >> 1;
	s16 temptile;
	int y;
	type = type << 6;
	y = 0;

	for (j = 0; j < 16; j++) {
		y = j;

		if (invert) y = 15 - j;

		temptile = tile;

		if (temptile > 15) temptile = 15; // limit range...

		for (i = 0; (i  < 8) && (y >= 0); i++) {
			if ((y >= 0) && (y < 6)) {
				PA_BgTransTile(screen, type, i*2, y*2, temptile);
				PA_BgTransTile(screen, type, i*2, (11 - y)*2, temptile);
				PA_BgTransTile(screen, type, (15 - i)*2, y*2, temptile);
				PA_BgTransTile(screen, type, (15 - i)*2, (11 - y)*2, temptile);
			}

			if (i&1)y--;

		}

		if (tile) tile--;
	}
}
