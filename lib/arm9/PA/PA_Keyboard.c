#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"

/*
#include "keyboard.raw.c"
#include "keyboard.map.c"
#include "keyboard.pal.c"*/
extern const unsigned char keyboard_Tiles[10240];
extern const unsigned short keyboard_Map[4096];
extern const unsigned short keyboard_Palette[16];

/*
const unsigned short keyboardPal1[16] = {
0x7c1f, 0x6f7b, 0x6318, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};
const unsigned short keyboardPal2[16] = {
0x7c1f, 0x6f7b, 0x6318, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};*/


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

PA_SetKeyboardColor(0, 1); // Blue and Red

}




void PA_ReloadKeyboardCol(void){
u8 i;
u16 r, g, b, color;
u8 tempr, tempg, tempb;
	for (i = 1; i < 11; i++) { // On va mettre appuyé en rouge
		color = keyboard_Palette[i];
		
		tempb = b = (color>>10)&31;	tempg = g = (color>>5)&31;
		tempr = r = (color)&31;	
		
		// Si dominante rouge
		if (Keyboard.Color1 == 1) {tempr = b; tempb = r;} //rouge
		else if (Keyboard.Color1 == 2) {tempg = b; tempb = g;} //vert
		BG_PALETTE[(15 << 4) + i] = PA_RGB(tempr, tempg, tempb);
		if (Keyboard.Color2 == 1) {tempr = b; tempb = r;} //rouge
		else if (Keyboard.Color2 == 2) {tempg = b; tempb = g;} //vert		
		BG_PALETTE[(14 << 4) + i] = PA_RGB(tempr, tempg, tempb);
	}
}

//#define SPACE     32   // La vrai valeur d'un espace

// Clavier, avec 5 lignes de 12 touches...
const u8 PA_Keyboard[2][5][24] = {
	  {
	  {'1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7', '7', '8', '8', '9', '9', '0', '0', '-', '-', '=', '='}, // Touches de 0 à 9, puis - et =  
	  {PA_TAB, 'q', 'q', 'w', 'w', 'e', 'e', 'r', 'r', 't', 't', 'y', 'y', 'u', 'u', 'i', 'i', 'o', 'o', 'p', 'p',PA_BACKSPACE, PA_BACKSPACE, PA_BACKSPACE}, // qwertyuiop, Backspace
	  {PA_CAPS, PA_CAPS, 'a', 'a', 's', 's', 'd', 'd', 'f', 'f', 'g', 'g', 'h', 'h', 'j', 'j', 'k', 'k', 'l', 'l', PA_ENTER, PA_ENTER, PA_ENTER, PA_ENTER}, // Caps, asdfghjkl, Enter
	  {PA_SHIFT, PA_SHIFT, PA_SHIFT, 'z', 'z', 'x', 'x', 'c', 'c', 'v', 'v', 'b', 'b', 'n', 'n', 'm', 'm', ',', ',', '.', '.', '/', '/', PA_RIEN},  // Shift, zxcvbnm , . /
	  {PA_RIEN, PA_RIEN, ';', ';', '`', '`', '`', '`', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '[', '[', ']', ']', '\\', '\\'}	  	  
	  },
	  {
	  {'!', '!' , '@', '@', '#', '#', '$', '$', '%', '%', '^', '^', '&', '&', '*', '*', '(', '(', ')', ')', '_', '_', '+', '+'}, // Touches de 0 à 9, puis - et =  
	  {PA_TAB, 'Q', 'Q', 'W', 'W', 'E', 'E', 'R', 'R', 'T', 'T', 'Y', 'Y', 'U', 'U', 'I', 'I', 'O', 'O', 'P', 'P',PA_BACKSPACE, PA_BACKSPACE, PA_BACKSPACE}, // qwertyuiop, Backspace
	  {PA_CAPS, PA_CAPS, 'A', 'A', 'S', 'S', 'D', 'D', 'F', 'F', 'G', 'G', 'H', 'H', 'J', 'J', 'K', 'K', 'L', 'L', PA_ENTER, PA_ENTER, PA_ENTER, PA_ENTER}, // Caps, asdfghjkl, Enter
	  {PA_SHIFT, PA_SHIFT, PA_SHIFT, 'Z', 'Z', 'X', 'X', 'C', 'C', 'V', 'V', 'B', 'B', 'N', 'N', 'M', 'M', '<', '<', '>', '>', '?', '?', PA_RIEN},  // Shift, zxcvbnm , . /
	  {PA_RIEN, PA_RIEN, PA_RIEN, PA_RIEN, ':', ':', '"', '"', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '[', '[', ']', ']', '\\', '\\'}
	  }
	  }; // On prend en compte les trous...
	  
	  



// A noter : début du clavier : x = 57, y = 109... fin du clavier : x = 250, y = 186



void PA_ChangeKeyboardType(void){
	Keyboard.Type = !Keyboard.Type;
	DMA_Copy((void*)(keyboard_Map + (Keyboard.Type << 11)), (void*)ScreenBaseBlock(0, PA_BgInfo[0][Keyboard.Bg].mapchar), 32*12 , DMA_16NOW);
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
	   //Les cases font 8 de largeur, donc de 0-7 c'est la case 0... il faut diviser par 8
		y = y >> 4;  
		x = x >> 3;
		
		if (Stylus.Newpress) {
			Keyboard.oldX = x;
			Keyboard.oldY = y;
			Keyboard.Repeat = 50;
			
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
			PA_SetMapTilePal(0, Keyboard.Bg, x+1, (y << 1) + 1, Pal);
			PA_SetMapTilePal(0, Keyboard.Bg, x+1, (y << 1) + 2, Pal);			
			x++;
		}
	}
}




#ifdef __cplusplus
}
#endif


