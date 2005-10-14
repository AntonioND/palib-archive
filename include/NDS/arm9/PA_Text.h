#ifndef _PA_Text
#define _PA_Text



/*! \file PA_Text.h
    \brief Text system in tile mode

    Not much to say, just a good debug option...
*/

//#include "../PA.h"
#include "PA_Tile.h"


//////////////////////////////////////////////////////////////////////
// Syst�me pour le texte
//////////////////////////////////////////////////////////////////////

extern const u16 PA_font_Map[256];
extern const u8 PA_font_Tiles[6080];

extern const u16 PA_font2_Map[256];
extern const u8 PA_font2_Tiles[6016];

extern u16 textcol[2]; // Garde en m�moire la couleur du texte
extern u8 PAbgtext[2];


extern u16 *PA_textmap[2]; //Pointeur vers la map et les tiles...
extern u16 *PA_texttiles[2];

extern s8 PA_font[2];  // 0 pour normal, 1 pour d�grad�, -1 pour custom




// Texte � taille variable

#include "text0.h" // La police � taille variable...
#include "text1.h" // La police � taille variable...
#include "text2.h" // La police � taille variable...
#include "text3.h" // La police � taille variable...
#include "text4.h" // La police � taille variable...



typedef void(*letterfp)(u8 size, bool screen, u16 x, u16 y, char lettertemp, u8 color);

extern const u8 *textData[5];
extern const u8 policeheight[5];
extern 	const u16 policewidth[5];
extern const u8 policesize[5][256];


extern letterfp letters[5];








/** @defgroup Text Text output system
 *  Allows you to output text...
 *  @{
 */



/*! \fn void PA_InitText(bool screen, u8 bg_select)
    \brief
         \~english Output text on the gba screen. Works only in modes 0-2
         \~french Ecrire du texte � l'�cran. Ne marche qu'en modes 0-2
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param bg_select
      \~english Background number (0-3)
      \~french Num�ro du fond que l'on veut tourner (0-3)
*/

void PA_InitText(bool screen, u8 bg_select);


/*! \fn extern inline void PA_SetTileLetter(bool screen, u16 x, u16 y, char letter)
    \brief
         \~english Output a letter on the DS screen.
         \~french Ecrire une lettre � l'�cran.
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param x
         \~english X coordinate in TILES (0-31) where to write the letter
         \~french Coordonn�e X en TILES (0-31) o� afficher la lettre
    \param y
         \~english Y coordinate in TILES (0-19) where to write the letter
         \~french Coordonn�e Y en TILES (0-19) o� afficher la lettre
    \param letter
         \~english Letter... 'a', 'Z', etc...
         \~french Lettre... 'a', 'Z', etc...
*/
extern inline void PA_SetTileLetter(bool screen, u16 x, u16 y, char letter) {
	PA_SetMapTileAll(screen, PAbgtext[screen], x, y, PA_textmap[screen][(u16)letter]);
}

/*! \fn void PA_OutputText(bool screen, u16 x, u16 y, char* text, ...)
    \brief
         \~english Output text on the DS screen. Works only in modes 0-2
         \~french Ecrire du texte � l'�cran. Ne marche qu'en modes 0-2
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param x
         \~english X coordinate in TILES (0-31) where to begin writing the text
         \~french Coordonn�e X en TILES (0-31) o� commencer � afficher le text
    \param y
         \~english Y coordinate in TILES (0-19) where to begin writing the text
         \~french Coordonn�e Y en TILES (0-19) o� commencer � afficher le text
    \param text
         \~english String to output. The following commands are avaiblable : \%s to output another string, \%d to output a value, \%fX to output a float with X digits, \\n to go to the line. Here's an example : PA_OutputText(0, 0, 1, "My name is %s and I have only %d teeth", "Mollusk", 20);
         \~french Chaine de caract�re � �crire. On dispose des commandes suivantes : \%s pour une autre chaine de caract�res, \%d pour �crire la valeur d'une variables, \%fX pour afficher un nombre avec X chiffres apr�s la virgule, \\n pour aller � la ligne. Voici un exemple : PA_OutputText(0, 0, 1, "Mon nom est %s et je n'ai que %d dents...", "Mollusk", 20);
*/
void PA_OutputText(bool screen, u16 x, u16 y, char* text, ...);

/*! \fn u16 PA_OutputSimpleText(bool screen, u16 x, u16 y, const char *text)
    \brief
         \~english Output simple text on the DS screen. Works only in modes 0-2. Much faster than PA_OutputText, but much more limited... Returns the number of letters
         \~french Ecrire du texte tout simple � l'�cran. Ne marche qu'en modes 0-2. Beaucoup plus rapide que PA_OutputText, masi aussi beaucoup plus limit�... Renvoie le nombre de lettres
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param x
         \~english X coordinate in TILES (0-31) where to begin writing the text
         \~french Coordonn�e X en TILES (0-31) o� commencer � afficher le text
    \param y
         \~english Y coordinate in TILES (0-19) where to begin writing the text
         \~french Coordonn�e Y en TILES (0-19) o� commencer � afficher le text
    \param text
         \~english String to output. 
         \~french Chaine de caract�re � �crire.
*/
u16 PA_OutputSimpleText(bool screen, u16 x, u16 y, const char *text);

/*! \fn u32 PA_BoxText(bool screen, u16 basex, u16 basey, u16 maxx, u16 maxy, const char *text, u32 limit)
    \brief
         \~english Output text on the DS screen. This text is limited to a chosen box, and you can chose the number of letters to output (can be used to show 'typed' text, just put 10000 if you want to show all the text...). Returns the number of letters outputed
         \~french Permet d'�crire du texte � l'�cran, dans une boite d�limit�e au choix, et en choisissant le nombre de lettres � afficher (peut �tre utile pour afficher du texte en train de se taper, sinon suffit de mettre 10000 pour afficher tout d'un coup) Renvoie le nmobre de lettre �crites
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param basex
         \~english X coordinate in TILES (0-31) where to begin writing the text
         \~french Coordonn�e X en TILES (0-31) o� commencer � afficher le text
    \param basey
         \~english Y coordinate in TILES (0-19) where to begin writing the text
         \~french Coordonn�e Y en TILES (0-19) o� commencer � afficher le text
    \param maxx
         \~english X coordinate in TILES (0-31) where to stop writing the text
         \~french Coordonn�e X en TILES (0-31) o� finir d'afficher le text
    \param maxy
         \~english Y coordinate in TILES (0-19) where to stop writing the text
         \~french Coordonn�e Y en TILES (0-19) o� finir d'afficher le text		 
    \param text
         \~english String to output. 
         \~french Chaine de caract�re � �crire.
    \param limit
         \~english Maximum number of letters to show this time
         \~french Nombre maximum de lettres � afficher pour ce coup-ci 
*/
u32 PA_BoxText(bool screen, u16 basex, u16 basey, u16 maxx, u16 maxy, const char *text, u32 limit);


/*!
    \fn void PA_SetTextCol(bool screen, u8 r, u8 g, u8 b)
    \brief
      \~english Change the screen text's color (last color in the palette)
      \~french Changer la couleur du texte � l'�cran (derni�re couleur de la palette)
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param r
      \~english Red amount (0-31)
      \~french Quantit� de rouge (0-31)
    \param g
      \~english Green amount (0-31)
      \~french Quantit� de vert (0-31)
    \param b
      \~english Blue amount (0-31)
      \~french Quantit� de bleu (0-31)
*/
void PA_SetTextCol(bool screen, u8 r, u8 g, u8 b);



/*!
    \fn void PA_SetTextFont(bool screen, u8 font)
    \brief
      \~english Change the screen's font, using one of those given in PA_lib...
      \~french Changer la police d'un des �crans en utilisant une des polices de PA_lib
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param font
      \~english Font (0 for normal, 1 for 3 colored font)
      \~french Police (0 pour normale, 1 pour tri-color

*/
void PA_SetTextFont(bool screen, u8 font);


/*!
    \def PA_ShowFont(screen)
    \brief
      \~english Show the current font used. This is just for debug, no real use ingame
      \~french Affiche la police utilis�e. C'est juste utile pour du d�buggage, aucun int�ret autrement
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
*/
#define PA_ShowFont(screen) PA_LoadBgMap(screen, PAbgtext[screen], (void*)PA_textmap[screen], BG_256X256)



/*!
    \fn void PA_SetTextCustomFont(bool screen, void* tiles, void* map)
    \brief
      \~english Change the screen's font using a custom font
      \~french Changer la police d'un des �crans en utilisant une police faite-maison
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param tiles
      \~english Font tiles
      \~french Tiles de la police
    \param map
      \~english Font map
      \~french Map de la police

*/
void PA_SetTextCustomFont(bool screen, void* tiles, void* map);



/*! \fn s16 PA_SmartText(bool screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u8 color, u8 size, u8 transp,  s32 limit)
    \brief
         \~english This is a variable width and variable size function to draw text on the screen. It draws on an 8 bit background (see PA_Init8bitBg for more info), and has options such as size, transaprency, and box limits, as well as the color. Only problem : it does not take commands such as %d, etc... The function returns the number of characters it outputed
         \~french Cette fonction permet d'�crire du texte � chasse variable � l'�cran. Elle n�cessite d'avoir un fond dessinable de 8 bits (cf PA_Init8bitBg). Les options sont la taille, la transparence, et les limites, ainsi que la couleur. Seul inconv�nient : il n'accepte pas les commande comme %d, etc... La fonction renvoie le nombre de charact�res �crits
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param basex
         \~english X coordinate of the top left corner
         \~french Coordonn�e X du coin sup�rieur gauche
    \param basey
         \~english Y coordinate of the top left corner
         \~french Coordonn�e Y du coin sup�rieur gauche
    \param maxx
         \~english X coordinate of the down right corner
         \~french Coordonn�e X du coin inf�rieur droit
    \param maxy
         \~english Y coordinate of the down right corner
         \~french Coordonn�e Y du coin inf�rieur droit
    \param text
         \~english Text, such as "Hello World"
         \~french Texte, tel que "Hello World"
    \param color
         \~english Palette color to use (0-255)
         \~french Couleur de la palette � utiliser (0-255)
    \param size
         \~english Size of the text, from 0 (really small) to 4 (pretty big)
         \~french Taille du texte, de 0 (vraiment petit) � 4 (assez grand) 
    \param transp
         \~english Transparency. Setting this to 0 will overwrite all drawing in the text zone. 1 will write the text without erasing the drawing. 2 won't output anything (just to count the letters), 3 is rotated one way, 4 rotated the other way
         \~french Transparence. Mettre � 0 effecera tout dessin de la zone de texte. 1 �crira le texte par-dessus le dessin sans l'effacer. 2 n'�crira rien (juste pour compter les lettres). 3 fera un texte tourn� � 90�. 4 est un texte tourn� dans l'autre sens.	 
    \param limit
         \~english You can give a maximum number of characters to output. This can be usefull to have a slowing drawing text (allow to draw 1 more character each frame...)
         \~french On peut fixer une limite au nombre de caract�res. Ceci peut etre utile pour dessiner un texte progressivement, en augmentant de 1 le nombre de caract�res � chaque boucle....
*/
s16 PA_SmartText(bool screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u8 color, u8 size, u8 transp,  s32 limit);



/*! \fn s16 PA_CenterSmartText(bool screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u8 color, u8 size, u8 transp)
    \brief
         \~english Basicaly the same as the SmartText function, but this time centered...
         \~french En gros la meme chose que SmartText, mais en centr�...
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param basex
         \~english X coordinate of the top left corner
         \~french Coordonn�e X du coin sup�rieur gauche
    \param basey
         \~english Y coordinate of the top left corner
         \~french Coordonn�e Y du coin sup�rieur gauche
    \param maxx
         \~english X coordinate of the down right corner
         \~french Coordonn�e X du coin inf�rieur droit
    \param maxy
         \~english Y coordinate of the down right corner
         \~french Coordonn�e Y du coin inf�rieur droit
    \param text
         \~english Text, such as "Hello World"
         \~french Texte, tel que "Hello World"
    \param color
         \~english Palette color to use (0-255)
         \~french Couleur de la palette � utiliser (0-255)
    \param size
         \~english Size of the text, from 0 (really small) to 4 (pretty big)
         \~french Taille du texte, de 0 (vraiment petit) � 4 (assez grand) 
    \param transp
         \~english Transparency. Setting this to 0 will overwrite all drawing in the text zone. 1 will write the text without erasing the drawing. 2 won't output anything (just to count the letters), 3 is rotated one way, 4 rotated the other way
         \~french Transparence. Mettre � 0 effecera tout dessin de la zone de texte. 1 �crira le texte par-dessus le dessin sans l'effacer. 2 n'�crira rien (juste pour compter les lettres). 3 fera un texte tourn� � 90�. 4 est un texte tourn� dans l'autre sens.	 
*/
s16 PA_CenterSmartText(bool screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char* text, u8 color, u8 size, u8 transp);



/*! \fn extern inline bool PA_CompareText(char *text1, char *text2)
    \brief
         \~english Compare a string to a second. Example : PA_CompareText(Playname, "Mollusk");
         \~french Comparer une chaine de caract�re � une autre. Exemple : PA_CompareText(Playname, "Mollusk");
    \param text1
         \~english First string
         \~french Premi�re chaine
    \param text2
         \~english Second string...
         \~french Seconde chaine...
*/
extern inline bool PA_CompareText(char *text1, char *text2){
	bool ok = 1; // Devrait etre bon
	u8 i = 0; 
	while(ok && text2[i]){
		if (text1[i] != text2[i]) ok = 0;
		i++;
	}
	return ok;
}


/*! \fn extern inline void PA_CopyText(char *text1, char *text2)
    \brief
         \~english Copy one string into another
         \~french Comparer une chaine de caract�re � une autre. Exemple : PA_CompareText(Playname, "Mollusk");
    \param text1
         \~english String to change
         \~french Chaine � changer
    \param text2
         \~english String to copy into the other
         \~french Chaine � copier dans l'autre
*/
extern inline void PA_CopyText(char *text1, char *text2){
u8 i = 0;
text1[i] = text2[i];
	do{
		i++;
		text1[i] = text2[i];
	}
	while (text2[i]);
}





/** @} */ // end of Text



#endif


