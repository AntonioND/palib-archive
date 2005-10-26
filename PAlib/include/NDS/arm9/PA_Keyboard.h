#ifndef _PA_Keyboard
#define _PA_Keyboard


#include "PA9.h"




/*! \file PA_Keyboard.h
    \brief Keyboard functions

    This will allow you to suma keyboard, use it, etc...
*/



void PA_SetLetterPal(s16 x, s16 y, u8 Pal);


//////////////////////////////////////////////////////////////////////
// Syst�me de Tiles
//////////////////////////////////////////////////////////////////////



extern const unsigned char keyboard_Tiles[7264];
extern const unsigned short keyboard_Map[4096];
extern const unsigned short keyboardPal1[16];
extern const unsigned short keyboardPal2[16];

typedef struct {
	u8 Bg; // Num�ro du fond
	u8 Type; // 0 pour normal, 1 pour majuscules..
	char Letter;  // Lettre
	s16 ScrollX, ScrollY;  // Scroll x et y...
	s16 Repeat;  // Temps avant de r�p�ter...
	s16 oldX, oldY; // Ancienne zone touch�e
} Keyboards;
extern Keyboards Keyboard;

extern const u8 PA_Keyboard[2][5][13];

#define PA_BACKSPACE 1    // On va dire que la touche backspace a pour valeur 1...
#define PA_CAPS      2    // 2 pour CAPS
#define PA_ENTER     '\n'    // 3 pour Enter
#define PA_SHIFT     4    // etc
#define PA_RIEN      0    // Si y'a rien, faudra faire comme si on appuye sur rien





/** @defgroup Keyboard Keyboard...
 *  Load a keyboard and have fun
 *  @{
 */

/*!
    \fn void PA_InitKeyboard(u8 bg_number)
    \brief
		\~english Initialise the Keyboard on a given background. Uses 16 color palettes 14 and 15 (doesn't mix with text though, don't worry)
		\~french Initialiser le calvier sur un fond donn�. Utilise les palettes de 16 couleurs 14 et 15 (n'interf�re pas avec le texte)
    \param bg_number
      \~english Background number (0-3)
      \~french Num�ro du fond que l'on veut tourner (0-3)
*/
void PA_InitKeyboard(u8 bg_number);


/*!
    \fn char PA_CheckKeyboard(void)
    \brief
		\~english Checks if the keyboard is used, and return the letter :)  Use this every turn (even if the stylus isn't pressed)
		\~french V�rifie le clavier, s'il est utilis�, et renvoie la lettre appuy�e (0 si pas de nouvel appuye). A utiliser tout le temps, m�me si le stylet ne touche pas l'�cran
*/
char PA_CheckKeyboard(void);



/*!
    \fn extern inline void PA_ScrollKeyboardX(s16 x)
    \brief
		\~english Set the Keyboard's X position		
		\~french Placer le Clavier � la position X
    \param x
      \~english X position...
      \~french Position X
*/
extern inline void PA_ScrollKeyboardX(s16 x) {
	PA_BGScrollX(0, Keyboard.Bg, -x);
	Keyboard.ScrollX = x;
}


/*!
    \fn extern inline void PA_ScrollKeyboardY(s16 y)
    \brief
		\~english Set the Keyboard's Y position		
		\~french Placer le Clavier � la position Y
    \param y
      \~english Y position...
      \~french Position Y
*/
extern inline void PA_ScrollKeyboardY(s16 y) {
	PA_BGScrollY(0, Keyboard.Bg, -y);
	Keyboard.ScrollY = y;
}


/*!
    \fn extern inline void PA_ScrollKeyboardXY(s16 x, s16 y)
    \brief
		\~english Set the Keyboard's position		
		\~french Placer le Clavier � une position donn�e
    \param x
      \~english X position...
      \~french Position X
    \param y
      \~english Y position...
      \~french Position Y
*/
extern inline void PA_ScrollKeyboardXY(s16 x, s16 y) {
	PA_BGScrollXY(0, Keyboard.Bg, -x, -y);
	Keyboard.ScrollX = x;
	Keyboard.ScrollY = y;
}


/*!
    \def PA_EraseLastKey()
    \brief
		\~english Erase the last key lit up (if it didn't on it's own)
		\~french Effacer la derni�re touche press�e, si ca ne le fait pas tout seul
*/
#define PA_EraseLastKey() PA_SetLetterPal(Keyboard.oldX, Keyboard.oldY, 15)


/*!
    \fn extern inline void PA_KeyboardIn(s16 x, s16 y)
    \brief
		\~english Make the keyboard enter to position (x, y), scrolling from the bottom of the screen	
		\~french Faire entrer le clavier � la position (x, y) en glissant depuis le bas de l'�cran
    \param x
      \~english X position...
      \~french Position X
    \param y
      \~english Y position...
      \~french Position Y
*/
extern inline void PA_KeyboardIn(s16 x, s16 y){  // Faire rentrer le clavier... 
s16 i;
	PA_ScrollKeyboardXY(x, 192);  // On le sort et met en X. Puis on fait glisser...
	for (i = 192; i > y; i-=8){
		PA_ScrollKeyboardY(i);
		PA_WaitForVBL();
	}
	PA_ScrollKeyboardY(y);  // On am�ne pile au bon y...		
}


/*!
    \fn extern inline void PA_KeyboardOut(void)
    \brief
		\~english Make the keyboard scroll out
		\~french Faire sortir le clavier
*/
extern inline void PA_KeyboardOut(void){  // Faire sortir le clavier... 
s16 i;
PA_EraseLastKey(); // Efface s'il reste une touche press�e...
	for (i = Keyboard.ScrollY; i < 200; i+=8){
		PA_ScrollKeyboardY(i);
		PA_WaitForVBL();
	}
}





/** @} */ // end of Keyboard



#endif

