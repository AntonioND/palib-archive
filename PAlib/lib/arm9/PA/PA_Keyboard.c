#include "PA9.h"

#include "keyboard.raw.c"
#include "keyboard.map.c"

const unsigned short keyboardPal1[16] = {
0x7c1f, 0x6f7b, 0x6318, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};
const unsigned short keyboardPal2[16] = {
0x7c1f, 0x6f7b, 0x6318, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};


Keyboards Keyboard;
void PA_ReloadKeyboardCol(void);

void PA_InitKeyboard(u8 bg_number) {
// On charge le fond...
	PA_LoadSimpleBg(0, bg_number, keyboard_Tiles, keyboard_Map, BG_256X512, 0, 0);
	
Keyboard.Bg = bg_number;
Keyboard.Type = 0;
Keyboard.Repeat = 0;
/*
	PA_LoadBgExtPal(0, base_palette, (void*)keyboardPal1);
	PA_LoadBgExtPal(0, held_palette, (void*)keyboardPal2);*/

PA_ReloadKeyboardCol();

s16 i, j; 
for (j = 0; j < 12; j++)   // On parcourt tout le fond pour mettre la bonne palette...
	for (i = 0; i < 32; i++) 
		PA_SetMapTilePal(0, bg_number, i, j, 15);

}

void PA_ReloadKeyboardCol(void){
	BG_PALETTE[(15 << 4) + 1] = 0;
	BG_PALETTE[(15 << 4) + 2] = 0x6f7b;
	BG_PALETTE[(15 << 4) + 3] = 0x6318;
	BG_PALETTE[(15 << 4) + 4] = 0x7fff;
	BG_PALETTE[(14 << 4) + 1] = PA_RGB(0, 0, 31);		
	BG_PALETTE[(14 << 4) + 2] = PA_RGB(25, 25, 31);
	BG_PALETTE[(14 << 4) + 3] = PA_RGB(20, 20, 31);
	BG_PALETTE[(14 << 4) + 4] = BG_PALETTE[(15 << 4) + 4];
}

//#define SPACE     32   // La vrai valeur d'un espace

// Clavier, avec 5 lignes de 12 touches...
const u8 PA_Keyboard[2][5][13] = {
	  {{'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '='}, // Touches de 0 à 9, puis - et =
	  {PA_RIEN, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', PA_BACKSPACE, PA_BACKSPACE}, // qwertyuiop, Backspace
	  {PA_CAPS, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', PA_ENTER, PA_ENTER}, // Caps, asdfghjkl, Enter
	  {PA_SHIFT, PA_SHIFT, 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/'},  // Shift, zxcvbnm , . /
	  {PA_RIEN, PA_RIEN, ';', '`', ' ', ' ', ' ', ' ', ' ', '[', ']'}},
	  
	  {{'!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+'}, // Touches de 0 à 9, puis - et =
	  {PA_RIEN, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', PA_BACKSPACE, PA_BACKSPACE}, // qwertyuiop, Backspace
	  {PA_CAPS, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', PA_ENTER, PA_ENTER}, // Caps, asdfghjkl, Enter
	  {PA_SHIFT, PA_SHIFT, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?'},  // Shift, zxcvbnm , . /
	  {PA_RIEN, PA_RIEN, ':', '`', ' ', ' ', ' ', ' ', ' ', '{', '}'}} 
	  
	  }; // On prend en compte les trous...
	  
	  



// A noter : début du clavier : x = 57, y = 109... fin du clavier : x = 250, y = 186



void PA_ChangeKeyboardType(void){
	Keyboard.Type = !Keyboard.Type;
	DMA_Copy((void*)(keyboard_Map + (Keyboard.Type << 11)), (void*)ScreenBaseBlock(0, mapchar[0][Keyboard.Bg]), 32*12 , DMA_16NOW);
	s16 i, j; 
	for (j = 0; j < 12; j++)   // On parcourt tout le fond pour mettre la bonne palette...
		for (i = 0; i < 32; i++) 
			PA_SetMapTilePal(0, Keyboard.Bg, i, j, 15);	
}



char PA_CheckKeyboard(void) {  // Vérifier si on appuye sur le clavier avec ces touches...
// Vont prendre la position que l'on appuye sur le clavier...
s16 x = Stylus.X;
s16 y = Stylus.Y;  


	x -= Keyboard.ScrollX + 8; // Ca commence à 57, donc c'est comme si en 57 on était à 0
	y -= Keyboard.ScrollY + 8; // Pareil mais pour y...
	

		
	// On commence par vérifier si on est bien sur le clavier... Si ce n'est pas le cas, on renverra 0.
	if ((x >= 0) && (x < 192) && (y >= 0) && (y < 80)) {		
		//Les cases font 16 de hauteur/largeur, donc de 0-15 c'est la case 0, de 16-31 la 1, etc... donc faut diviser par 16!
		y = y >> 4;   // Idem.		
		if (y&1) x = (x + 8) >> 4;   // En fait, >> 4 ca veut dire diviser par 16 (par 2^4), mais c'est beaucoup plus rapide...
		else x = x >> 4;
		
		if (Stylus.Newpress) {
			Keyboard.oldX = x;
			Keyboard.oldY = y;
			Keyboard.Repeat = 60;
			
			if (PA_Keyboard[0][y][x] == PA_CAPS) {
				PA_ChangeKeyboardType();
			}	
			
			// Majuscule si jamais c'est Shift appuyé
			if (Keyboard.Letter == PA_SHIFT) {
				// Si on rappuye sur Shift ca le vire sans rien faire
				if(PA_Keyboard[Keyboard.Type][y][x] == PA_SHIFT) Keyboard.Letter = 0;
				else Keyboard.Letter = PA_Keyboard[Keyboard.Type][y][x];
				
				PA_SetLetterPal(0, 3, 15); // On efface le shift
				PA_ChangeKeyboardType();				
			}
			else {
				Keyboard.Letter = PA_Keyboard[Keyboard.Type][y][x];
				if (Keyboard.Letter == PA_SHIFT) PA_ChangeKeyboardType();
			}
			PA_SetLetterPal(x, y, 14);	
			
			return (Keyboard.Letter);  // Renvoie la valeur dans le clavier...	si nouvelle pression
		}
		else {
			if (Stylus.Held && (PA_Keyboard[0][y][x] == Keyboard.Letter)) {
				--Keyboard.Repeat;
				if (Keyboard.Repeat == 0) {
					Keyboard.Repeat = 10;
					return(Keyboard.Letter);
				}
			}
		}	
	}

	if (Stylus.Released) {
		if (Keyboard.Letter != PA_SHIFT) PA_SetLetterPal(Keyboard.oldX, Keyboard.oldY, 15);		
	}
	return 0;

}



void PA_SetLetterPal(s16 x, s16 y, u8 Pal) {
u8 value = PA_Keyboard[0][y][x];
	if (value) {
		while(PA_Keyboard[0][y][x-1] == value) --x;
		while(PA_Keyboard[0][y][x] == value) {
			PA_SetMapTilePal(0, Keyboard.Bg, ((y+1)&1) + (x << 1), 1 + (y << 1), Pal);
			PA_SetMapTilePal(0, Keyboard.Bg, ((y+1)&1) + 1+(x << 1), 1 + (y << 1), Pal);			
			PA_SetMapTilePal(0, Keyboard.Bg, ((y+1)&1) + (x << 1), 2 + (y << 1), Pal);				
			PA_SetMapTilePal(0, Keyboard.Bg, ((y+1)&1) + 1+(x << 1), 2 + (y << 1), Pal);
			x++;
		}
	}
}







