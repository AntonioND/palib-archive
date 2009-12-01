#define PA_NO_DEPRECATION
#include <PA9.h>

extern const unsigned char keyboard_Tiles[10400];
extern const unsigned short keyboard_Map[4096];
extern const unsigned short keyboard_Palette[16];

u8 keyb_screen = 0;

Keyboards PA_Keyboard_Struct;
void PA_ReloadKeyboardCol(void);

void PA_LoadDefaultKeyboard(u8 bg_number) {
// On charge le fond...
	PA_LoadSimpleBg(keyb_screen, bg_number, keyboard_Tiles, keyboard_Map, BG_256X512, 0, 0);
	PA_Keyboard_Struct.Bg = bg_number;
	PA_Keyboard_Struct.Type = 0;
	PA_Keyboard_Struct.Repeat = 0;
	/*
		PA_LoadBgExtPal(0, base_palette, (void*)keyboardPal1);
		PA_LoadBgExtPal(0, held_palette, (void*)keyboardPal2);*/
	PA_ReloadKeyboardCol();
	int i, j;

	for (j = 0; j < 12; j++)   // On parcourt tout le fond pour mettre la bonne palette...
		for (i = 0; i < 32; i++)
			PA_SetMapTilePal(keyb_screen, bg_number, i, j, 15);

	PA_SetKeyboardColor(0, 1); // Blue and Red
}





void PA_ReloadKeyboardCol(void) {
	int i;
	u16 r, g, b, color;
	u8 tempr, tempg, tempb;

	for (i = 1; i < 11; i++) { // On va mettre appuy� en rouge
		color = keyboard_Palette[i];
		tempb = b = (color >> 10) & 31;	tempg = g = (color >> 5) & 31;
		tempr = r = (color) & 31;

		// Si dominante rouge
		if (PA_Keyboard_Struct.Color1 == 1) {
			tempr = b; tempb = r;
		} //rouge
		else if (PA_Keyboard_Struct.Color1 == 2) {
			tempg = b; tempb = g;
		} //vert

		BG_PALETTE[(keyb_screen << 9) + (15 << 4) + i] = PA_RGB(tempr, tempg, tempb);

		if (PA_Keyboard_Struct.Color2 == 1) {
			tempr = b; tempb = r;
		} //rouge
		else if (PA_Keyboard_Struct.Color2 == 2) {
			tempg = b; tempb = g;
		} //vert

		BG_PALETTE[(keyb_screen << 9) + (14 << 4) + i] = PA_RGB(tempr, tempg, tempb);
	}
}


// Clavier, avec 5 lignes de 12 touches...
const u8 PA_Keyboard[2][5][24] = {
	{
		{'1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7', '7', '8', '8', '9', '9', '0', '0', '-', '-', '=', '='}, // Touches de 0 � 9, puis - et =
		{PA_TAB, 'q', 'q', 'w', 'w', 'e', 'e', 'r', 'r', 't', 't', 'y', 'y', 'u', 'u', 'i', 'i', 'o', 'o', 'p', 'p', PA_BACKSPACE, PA_BACKSPACE, PA_BACKSPACE}, // qwertyuiop, Backspace
		{PA_CAPS, PA_CAPS, 'a', 'a', 's', 's', 'd', 'd', 'f', 'f', 'g', 'g', 'h', 'h', 'j', 'j', 'k', 'k', 'l', 'l', PA_ENTER, PA_ENTER, PA_ENTER, PA_ENTER}, // Caps, asdfghjkl, Enter
		{PA_SHIFT, PA_SHIFT, PA_SHIFT, 'z', 'z', 'x', 'x', 'c', 'c', 'v', 'v', 'b', 'b', 'n', 'n', 'm', 'm', ',', ',', '.', '.', '/', '/', PA_RIEN},  // Shift, zxcvbnm , . /
		{PA_RIEN, PA_RIEN, '`', '`', ';', ';', '\'', '\'', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '[', '[', ']', ']', '\\', '\\'}
	},
	{
		{'!', '!' , '@', '@', '#', '#', '$', '$', '%', '%', '^', '^', '&', '&', '*', '*', '(', '(', ')', ')', '_', '_', '+', '+'}, // Touches de 0 � 9, puis - et =
		{PA_TAB, 'Q', 'Q', 'W', 'W', 'E', 'E', 'R', 'R', 'T', 'T', 'Y', 'Y', 'U', 'U', 'I', 'I', 'O', 'O', 'P', 'P', PA_BACKSPACE, PA_BACKSPACE, PA_BACKSPACE}, // qwertyuiop, Backspace
		{PA_CAPS, PA_CAPS, 'A', 'A', 'S', 'S', 'D', 'D', 'F', 'F', 'G', 'G', 'H', 'H', 'J', 'J', 'K', 'K', 'L', 'L', PA_ENTER, PA_ENTER, PA_ENTER, PA_ENTER}, // Caps, asdfghjkl, Enter
		{PA_SHIFT, PA_SHIFT, PA_SHIFT, 'Z', 'Z', 'X', 'X', 'C', 'C', 'V', 'V', 'B', 'B', 'N', 'N', 'M', 'M', '<', '<', '>', '>', '?', '?', PA_RIEN},  // Shift, zxcvbnm , . /
		{PA_RIEN, PA_RIEN, '~', '~', ':', ':', '"', '"', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '{', '{', '}', '}', '|', '|'}
	}
}; // On prend en compte les trous...

// A noter : d�but du clavier : x = 57, y = 109... fin du clavier : x = 250, y = 186



void PA_ChangeKeyboardType(void) {
	PA_Keyboard_Struct.Type = !PA_Keyboard_Struct.Type;
	int i, j;

	if (!PA_Keyboard_Struct.Custom) {
		DMA_Copy((void*)(keyboard_Map + (PA_Keyboard_Struct.Type << 11)), (void*)ScreenBaseBlock(keyb_screen, PA_BgInfo[keyb_screen][PA_Keyboard_Struct.Bg].mapchar), 32*12 , DMA_16NOW);

		for (j = 0; j < 12; j++)   // On parcourt tout le fond pour mettre la bonne palette...
			for (i = 0; i < 32; i++)
				PA_SetMapTilePal(keyb_screen, PA_Keyboard_Struct.Bg, i, j, 15);
	} else {
		DMA_Copy((void*)(PA_BgInfo[keyb_screen][PA_Keyboard_Struct.Bg].Map + (PA_Keyboard_Struct.Type << 12)), (void*)ScreenBaseBlock(keyb_screen, PA_BgInfo[keyb_screen][PA_Keyboard_Struct.Bg].mapchar), 32*12 , DMA_16NOW);
	}
}



char PA_CheckKeyboard(void) {  // V�rifier si on appuye sur le clavier avec ces touches...
// Vont prendre la position que l'on appuye sur le clavier...
	int x = Stylus.X;
	int y = Stylus.Y;
	x -= PA_Keyboard_Struct.ScrollX + 8; // Ca commence � 57, donc c'est comme si en 57 on �tait � 0
	y -= PA_Keyboard_Struct.ScrollY + 8; // Pareil mais pour y...
	// On commence par v�rifier si on est bien sur le clavier... Si ce n'est pas le cas, on renverra 0.
	if ((x >= 0) && (x < 192) && (y >= 0) && (y < 80)) {
		//Les cases font 8 de largeur, donc de 0-7 c'est la case 0... il faut diviser par 8
		y = y >> 4;
		x = x >> 3;

		if (Stylus.Newpress) {
			PA_Keyboard_Struct.oldX = x;
			PA_Keyboard_Struct.oldY = y;
			PA_Keyboard_Struct.Repeat = 50;

			if (PA_Keyboard[PA_Keyboard_Struct.Type][y][x] == PA_CAPS) {
				PA_ChangeKeyboardType();
			}

			// Majuscule si jamais c'est Shift appuy�
			if (PA_Keyboard_Struct.Letter == PA_SHIFT) {
				// Si on rappuye sur Shift ca le vire sans rien faire
				if (PA_Keyboard[PA_Keyboard_Struct.Type][y][x] == PA_SHIFT) PA_Keyboard_Struct.Letter = 0;
				else PA_Keyboard_Struct.Letter = PA_Keyboard[PA_Keyboard_Struct.Type][y][x];

				PA_SetLetterPal(0, 3, 15); // On efface le shift
				PA_ChangeKeyboardType();
			} else {
				PA_Keyboard_Struct.Letter = PA_Keyboard[PA_Keyboard_Struct.Type][y][x];

				if (PA_Keyboard_Struct.Letter == PA_SHIFT) PA_ChangeKeyboardType();
			}

			PA_SetLetterPal(x, y, 14);
			return (PA_Keyboard_Struct.Letter);  // Renvoie la valeur dans le clavier...	si nouvelle pression
		} else {
			if (Stylus.Held && (PA_Keyboard[PA_Keyboard_Struct.Type][y][x] == PA_Keyboard_Struct.Letter)) {
				--PA_Keyboard_Struct.Repeat;

				if (PA_Keyboard_Struct.Repeat == 0) {
					PA_Keyboard_Struct.Repeat = 10;
					return(PA_Keyboard_Struct.Letter);
				}
			}
		}
	}

	if (!Stylus.Held) {
		if (PA_Keyboard_Struct.Letter != PA_SHIFT) PA_SetLetterPal(PA_Keyboard_Struct.oldX, PA_Keyboard_Struct.oldY, 15);
	}

	return 0;
}



void PA_SetLetterPal(s16 x, s16 y, u8 Pal) {
	u8 value = PA_Keyboard[0][y][x];

	if (value && (!PA_Keyboard_Struct.Custom)) {
		while (PA_Keyboard[0][y][x-1] == value) --x;

		while (PA_Keyboard[0][y][x] == value) {
			PA_SetMapTilePal(keyb_screen, PA_Keyboard_Struct.Bg, x + 1, (y << 1) + 1, Pal);
			PA_SetMapTilePal(keyb_screen, PA_Keyboard_Struct.Bg, x + 1, (y << 1) + 2, Pal);
			x++;
		}
	}
}
