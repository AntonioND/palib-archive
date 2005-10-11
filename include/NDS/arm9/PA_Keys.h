#ifndef _PA_KEYS
#define _PA_KEYS

#include "PA9.h"
/*! \file PA_Keys.h
    \brief Everything concerning the keys and stylus

    Update the keys and stylus inputs
*/
//////////////////////////////////////////////////////////////////////
// Syst�me pour les touches
//////////////////////////////////////////////////////////////////////





typedef struct {
   bool A, B, X, Y, L, R, Up, Down, Right, Left, Start, Select;
} PA_Pad;

typedef struct {
   bool Held, Released, Newpress, Newpress0;
   s16 X, Y, altX, altY, Pressure;
} PA_Stylus;

extern PA_Stylus Stylus;

typedef struct {
   PA_Pad Held, Released, Newpress; // Pressed si on appuye, Held si on garde appuy�, et Released si on relache
} Pads;

extern Pads Pad;
extern PA_Pad* PadPointer;

// Structure pour les sprites que l'on d�place...
typedef struct {
	bool Moving;
	u8 Sprite;
	s16 X, Y, Vx, Vy;
	u8 Time; // Si trop longtemps sans bouger le sprite, il d�croche...
} PA_movingsprite;

extern PA_movingsprite PA_MovedSprite;

// Button info
#define PA_BUTTONS (*(volatile u16*)0x04000130)

#ifndef REG_KEYCNT
	#define REG_KEYCNT (*(volatile u16*)0x04000132)
#endif

#define BUTTON_A 1
#define BUTTON_B 2
#define BUTTON_SELECT 4
#define BUTTON_START 8
#define BUTTON_RIGHT 16
#define BUTTON_LEFT 32
#define BUTTON_UP 64
#define BUTTON_DOWN 128
#define BUTTON_R 256
#define BUTTON_L 512
#define BUTTON_X 1024
#define BUTTON_Y 2048



extern u16 CompletePad, ExPad, TempPad;
#define UPDATEPAD(type, pad)  type.A = pad & BUTTON_A;\
   type.B = (pad & BUTTON_B) >> 1;\
   type.Select = (pad & BUTTON_SELECT) >> 2;\
   type.Start = (pad & BUTTON_START) >> 3;\
   type.Right = (pad & BUTTON_RIGHT) >> 4;\
   type.Left = (pad & BUTTON_LEFT) >> 5;\
   type.Up = (pad & BUTTON_UP) >> 6;\
   type.Down = (pad & BUTTON_DOWN) >> 7;\
   type.R = (pad & BUTTON_R) >> 8;\
   type.L = (pad & BUTTON_L) >> 9;\
   type.X = (pad & BUTTON_X) >> 10;\
   type.Y = (pad & BUTTON_Y) >> 11;
  
#define COPYPAD(new, old)  new.A = old.A;\
   new.B = old.B;\
   new.Select = old.Select;\
   new.Start = old.Start;\
   new.Right = old.Right;\
   new.Left = old.Left;\
   new.Up = old.Up;\
   new.Down = old.Down;\
   new.R = old.R;\
   new.L =  old.L;\
   new.X = old.X;\
   new.Y = old.Y;
/*
#define PA_UpdatePad() ExPad = CompletePad;\
   CompletePad = ~(BUTTONS + (((IPC->buttons)&3) << 10));\
   UPDATEPAD(Pad.Held, CompletePad);\
   UPDATEPAD(Pad.Released, (ExPad & (~CompletePad)));\
   UPDATEPAD(Pad.Newpress, (CompletePad & (~ExPad)));

#define PA_UpdateStylus() bool temp = ((~IPC->buttons) >> 6) & 1;\
	Stylus.Newpress = temp & (!Stylus.Held);\
	Stylus.Released = (!temp) & Stylus.Held;\
	Stylus.Held = temp;\
	if(Stylus.Held)Stylus.altX =  ((IPC->touchX - 0x0113) / 14);\
	if(Stylus.Held)Stylus.altY =  ((IPC->touchY - 0x00E0) / 19);\
	if(Stylus.Held)Stylus.X =  IPC->touchXpx;\
	if(Stylus.Held)Stylus.Y =  IPC->touchYpx;\
	if(Stylus.Held)Stylus.Pressure = (((Stylus.X * IPC->touchZ2) >> 6) / IPC->touchZ1) - (Stylus.X >> 6);
*/
//#define BUTTON_AND 16384
//#define BUTTON_OR 0

/** @defgroup Keys Key input system
 *  Check which keys are pressed...
 *  @{
 */

/*! \fn void PA_UpdatePad(void)
    \brief
         \~english Update the Keypad, use it once per frame (in the VBL for example). You can then retrieve the held down keys with Pad.Held.A (or Up, Down...), Newly pressed keys with Pad.Newpress.R, and the just released keys with Pad.Released.Up...
         \~french Permet de mettre � jour les touches appuy�es. A utilis� une fois par frame (genre dans le vbl). On a ensuite acc�s aux touches press�es avec Pad.Held.A (ou Up, Down, L...), aux touches nouvellement press�es avec Pad.Newpress.R, et aux touches tout juste relach�es avec Pad.Released.Up...

*/
void PA_UpdatePad(void);



/*! \fn void PA_UpdateStylus(void)
    \brief
         \~english Update the Stylus position. You can then check if the stylus is current in use (Stylus.Held), newly pressed (Stylus.Newpress), or released (Stylus.Released), and get it's position (Stylus.X, Stylus.Y).
         \~french Mettre � jour la position du stylet. On peut v�rifier si le stylet est actuellement sur l'�cran (Stylus.Held), tout just appuy� (Stylus.Newpress), ou relach� (Stylus.Released), et obtenir sa position (Stylus.X, Stylus.Y).

*/
void PA_UpdateStylus(void);

/*! \def PA_MoveSprite(sprite)
    \brief
         \~english Move a sprite according to the stylus's position. The sprite will be 'hooked' if the stylus passes over it, and then they'll be linked together. Returns 1 if the sprite is moved. You can also get information from PA_MovedSprite.Moving (1 if you are moving a sprite), .Sprite (sprite moved), .X (X position of the center of the sprite), .Y (Y position of the center of the sprite), .Vx (horizontal speed ! useful if you want to make the sprite continue to move when you release the stylus...), and .Vy
         \~french D�placer un sprite en fonction du stylet. Le sprite sera accroch� si le stylet passe aud-dessus, puis il sera d�plac� en fonction... Donne 1 si on a d�plac� ce sprite, sinon 0. On peut ensuite r�cup�rer des infos avec PA_MovedSprite.Moving (1 si on d�place un sprite), .Sprite (num�ro du sprite d�plac�), .X (position X du centre du sprite), .Y (position Y du centre du sprite d�plac�), .Vx (vitesse horizontale du sprite d�plac� !! Utile si l'on veut que le sprite continue � se d�placer par la suite...), et .Vy
    \param sprite
         \~english Object number in the sprite system
         \~french Num�ro de l'objet dans le systeme de sprite
*/
#define PA_MoveSprite(sprite) PA_MoveSpriteEx(PA_Screen, sprite, PA_GetSpriteLx(0, sprite), PA_GetSpriteLy(0, sprite))



/*! \fn bool PA_MoveSpriteEx(bool screen, u8 sprite, u8 lx, u8 ly)
    \brief
         \~english Move a sprite according to the stylus's position. See PA_MoveSprite for more details... The difference is that here you chose the sprite dimension (lx and ly), which is useful if the sprite is smaller than the DS standard sizes... (for example 20x20...). This will also limit the 'hooking' distance
         \~french D�placer un sprite en fonction du stylet. Voir PA_MoveSprite pour plus de d�tails. La diff�rence est qu'ici on pr�cise la largeur et la hauteur du sprite, utile si le sprite ne fait pas vraiment la meme taille que la taille standard DS (genre si c'est un sprite de 20x20). Ceci limitera donc aussi la distance d'accrochage
    \param screen
         \~english On what screen to do it
         \~french Sur quel �cran le faire...
    \param sprite
         \~english Object number in the sprite system
         \~french Num�ro de l'objet dans le systeme de sprite
    \param lx
         \~english Sprite length
         \~french Largeur du sprite
    \param ly
         \~english Sprite height
         \~french Hauteur du sprite
*/
bool PA_MoveSpriteEx(bool screen, u8 sprite, u8 lx, u8 ly);


/*! \fn bool PA_MoveSpriteDistance(u8 sprite, u8 distance)
    \brief
         \~english Move a sprite according to the stylus's position. See PA_MoveSprite for more details... The difference is that here you chose the hooking distance in pixels
         \~french D�placer un sprite en fonction du stylet. Voir PA_MoveSprite pour plus de d�tails. La diff�rence est qu'ici on pr�cise la distance d'accrochage, en pixels
    \param sprite
         \~english Object number in the sprite system
         \~french Num�ro de l'objet dans le systeme de sprite
    \param distance
         \~english Hooking distance
         \~french Distance d'accrochage
*/
bool PA_MoveSpriteDistance(u8 sprite, u8 distance);

/*! \fn void PA_UpdateMoveSprite(void)
    \brief
         \~english Update the moving sprite info. If you don't put this in the VBL (it already is in PA_InitVBL), then if you stop moving around a sprite, it might not be able to pick up other sprites...
         \~french Met � jour les infos de d�placement de sprites... Si on ne met pas ca dans le VBL (c'est d�j� dans PA_InitVBL), alors on risque de ne pas pouvoir d�placer d'autre sprite...

*/
extern inline void PA_UpdateMoveSprite(void) {
	PA_MovedSprite.Time++;
	if ((PA_MovedSprite.Time > 2) || Stylus.Released) {
		PA_MovedSprite.Moving = 0;
		PA_MovedSprite.Time = 0;
	}
}



/*! \fn extern inline bool PA_SpriteTouchedEx(u8 sprite, u8 lx, u8 ly)
    \brief
         \~english Check if a given sprite is touched. Returns 1 if touched... You can chose the width and height around the sprite
         \~french V�rifie si l'on touche un sprite donn�. Renvoie 1 si touch�... On peut choisir la hauteur et la largeur autour du sprite
    \param sprite
         \~english Sprite number in the sprite system
         \~french Num�ro du sprite dans le systeme de sprite
    \param lx
         \~english Wideness
         \~french Largeur
    \param ly
         \~english Height
         \~french Hauter		 
		 
*/
extern inline bool PA_SpriteTouchedEx(u8 sprite, u8 lx, u8 ly){
	if ((Stylus.X > PA_GetSpriteX(PA_Screen, sprite)) && (Stylus.X < PA_GetSpriteX(PA_Screen, sprite) + lx)
		&& (Stylus.Y > PA_GetSpriteY(PA_Screen, sprite)) && (Stylus.Y < PA_GetSpriteY(PA_Screen, sprite) + ly))
		return 1;
	else return 0;
}



/*! \def PA_SpriteTouched(sprite)
    \brief
         \~english Check if a given sprite is touched. Returns 1 if touched...
         \~french V�rifie si l'on touche un sprite donn�. Renvoie 1 si touch�...
    \param sprite
         \~english Sprite number in the sprite system
         \~french Num�ro du sprite dans le systeme de sprite
*/
#define PA_SpriteTouched(sprite) PA_SpriteTouchedEx(sprite, PA_GetSpriteLx(PA_Screen, sprite), PA_GetSpriteLy(PA_Screen, sprite))



/*! \def PA_StylusInZone(x1, y1, x2, y2)
    \brief
         \~english Check if the stylus is in a given zone... Returns 1 if yes, 0 if not
         \~french V�rifie si le stylet est dans une zone d�limit�e donn�e... Renvoie 1 si oui, 0 sinon
    \param x1
         \~english X value of the upper left corner
         \~french Valeur X du coin sup�rieur gauche
    \param y1
         \~english Y value of the upper left corner
         \~french Valeur Y du coin sup�rieur gauche
    \param x2
         \~english X value of the lower right corner
         \~french Valeur X du coin inf�rieur droit
    \param y2
         \~english Y value of the lower right corner
         \~french Valeur Y du coin inf�rieur droit
*/
#define PA_StylusInZone(x1, y1, x2, y2) ((Stylus.X>=x1)&&(Stylus.Y>=y1)&&(Stylus.X<x2)&&(Stylus.Y<y2))

/** @} */ // end of Keys


#endif
