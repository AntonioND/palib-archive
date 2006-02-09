#ifndef _PA_PAPI
#define _PA_API

#ifdef __cplusplus
extern "C" {
#endif

/*! \file PA_API.h
    \brief PA API functions

    API functions, to create buttons, checkkboxes, and more to come !
*/

#include "PA_Draw.h"
#include "PA_Text.h"
#include "PA_Palette.h"



/** @defgroup PAPI PA API functions
 *  PA API functions
 *  @{
 */


typedef struct{
	s8 n; // Nombre
	u8 first, last; // Premier et dernier
	u8 free[128]; //Emplacements dispos...
	u8 touched; // Touch�
	u8 *image[4]; // Different images...
} PAPI_all;

typedef struct{
	bool *var; // Variable � changer, stocke l'�tat...
	bool value; // valeur, pour voir si l'utilisateur a chang� la valeur ou pas...
	s16 x, y; // Poour la position...
	s8 previous, next; // Pour savoir qui tester avant/apres
} PAPI_checks;

extern PAPI_checks PAPI_check[2][32]; // 32 possibles par �cran
extern PAPI_all PAPI_checkinfo[2];

typedef struct{
	fp funct; // Variable � changer, stocke l'�tat...
	char text[20]; // Texte � afficher sur le bouton...
	s8 textsize; // Taille du texte, 2 par d�faut
	u8 color; // Couleur de la palette pour le texte
	s16 x, y, lx, ly; // Pour la position... et la taille
	s8 previous, next; // Pour savoir qui tester avant/apres
} PAPI_buttons;

typedef struct{
	s32* var; // Variable � changer, stocke l'�tat...
	s32 value; // valeur, pour voir si l'utilisateur a chang� la valeur ou pas...
	bool sens; // 0 pour hori, 1 pour verti
	s32 min, max; // Valeurs min et max � prendre
	s16 x, y, lx; // Pour la position... et la taille
	s8 previous, next; // Pour savoir qui tester avant/apres
} PAPI_sliders;

extern PAPI_buttons PAPI_button[2][64]; //64 possibles par �cran
extern PAPI_all PAPI_buttoninfo[2];


// Init functions


/*! \fn void PAPI_Init(bool screen)
    \brief
         \~english Initialise the PA API system. MUST be used prior to any API usage. MUST be set on a 8bit background, don't forget to intialise it too !
         \~french Initialise le systeme d'API PA... DOIT �tre utilis� avant tout utilisation de l'API. Ne peut �tre utilis� QUE sur un fond de 8 bit, ne pas oublier de le charger...
    \param screen
         \~english Screen on which to initialise it
         \~french Ecran sur lequel l'initialiser
*/
void PAPI_Init(bool screen);


/*! \fn void PAPI_SetImages(bool screen, void *images, void *palette)
    \brief
         \~english Change the image set used by the API system. This allows you to have your own buttons and everything... I'll explain later on how to correctly use this
         \~french Changer le set d'images utilis� par l'API... Ceci permet d'utiliser ses propres boutons, etc... J'expliquerai un peu plus tard comment utiliser tout ca correctement
    \param screen
         \~english Screen on which to initialise it
         \~french Ecran sur lequel l'initialiser
    \param images
         \~english Image set
         \~french Set d'images
    \param palette
         \~english Palette used by the image set, 16 colors...
         \~french Palette utilis�e par le set d'images, 16 couleurs... 
*/
void PAPI_SetImages(bool screen, void *images, void *palette);

/*! \fn void PAPI_ReInit(bool screen)
    \brief
         \~english ReInitialise the PA API system. This resets all the buttons, checkboxess, and all, you'll just have to clean the screen
         \~french ReInitialise le systeme d'API PA... Ceci remet � 0 les boutons, les checkbox, etc... Tout ce qu'il reste � faire c'est d'effacer l'�cran
    \param screen
         \~english Screen on which to initialise it
         \~french Ecran sur lequel l'initialiser
*/
void PAPI_ReInit(bool screen);

/*! \fn void PAPI_LoadPal(bool screen)
    \brief
         \~english Loads the API palette in background colors 208. Use this each time you change the background palette
         \~french Charge la palette de l'API, dans les couleurs des fonds, position 208. A utiliser � chaque fois que l'on change la palette des fonds
    \param screen
         \~english Screen on which to initialise it
         \~french Ecran sur lequel l'initialiser
*/
void PAPI_LoadPal(bool screen);


/*! \fn u8 PAPI_CreateCheck(bool screen, s16 x, s16 y, bool *var)
    \brief
         \~english Creates a new Checkbox. Returns the number of the checkbox in the API system
         \~french Cr�� une nouvelle boite � cocher. Renvoie le num�ro de la boite dans l'API
    \param screen
         \~english Screen on which to create the checkbox
         \~french Ecran sur lequel charger un checkbox
    \param x
         \~english X position of the upper left corner
         \~french Position X du coin sup�rieur gauche
    \param y
         \~english Y position of the upper left corner
         \~french Position Y du coin sup�rieur gauche
    \param var
         \~english Bool variable to change when the checkbox is touched. Usage : &variablename
         \~french Variable de type bool � changer quand on touche la boite. Utilisation : &nomdelavariable		 
*/
u8 PAPI_CreateCheck(bool screen, s16 x, s16 y, bool *var);




/*! \fn void PAPI_CheckCheck(void)
    \brief
         \~english Checks all the checkboxes for a new press
         \~french V�rifie toutes les boites � cocher
*/
void PAPI_CheckCheck(void);



/*! \fn u8 PAPI_CreateButton(bool screen, s16 x, s16 y, s16 lx, s16 ly, fp funct, char* text, u8 color, s8 textsize)
    \brief
         \~english Creates a new Button. Returns the number of the button in the API system
         \~french Cr�� nouveau bouton. Renvoie le num�ro du bouton dans l'API
    \param screen
         \~english Screen on which to create the button
         \~french Ecran sur lequel charger le bouton
    \param x
         \~english X position of the upper left corner
         \~french Position X du coin sup�rieur gauche
    \param y
         \~english Y position of the upper left corner
         \~french Position Y du coin sup�rieur gauche
    \param lx
         \~english Width (typically 16 or 32, but can be anything)
         \~french Largeur, typiquement 16 ou 32, peut �tre n'importe quoi
    \param ly
         \~english Height...
         \~french Hauteur...
    \param funct
         \~english Fonction to load when the button is pressed. Usage : &functionname
         \~french Fonction � charger quand le bouton est touch�. Utilisation : &nomdelafonction
    \param text
         \~english Text to show on the button
         \~french Texte � afficher sur le bouton
    \param color
         \~english Text color in the palette index (0-255)
         \~french Couleur du texte dans la palette (0-255)
    \param textsize
         \~english Text size, typically 1, but can be bigger for big buttons. !!!! the define TEXT_ALLSIZES must be set in PA9.h (and install.bat used again) to enable other sizes than 1 !!!!
         \~french Taille du texte, en g�n�ral 1, mais peut �tre plus pour des gros boutons !!! la macro define TEXT_ALLSIZES doit �tre mise dans PA9.h (et install.bat r�utilis�) pour activer les autres taille que 1
*/
u8 PAPI_CreateButton(bool screen, s16 x, s16 y, s16 lx, s16 ly, fp funct, char* text, u8 color, s8 textsize);

/*! \fn void PAPI_CheckButton(void)
    \brief
         \~english Checks all the buttons for a new press
         \~french V�rifie toutes les boutons
*/
void PAPI_CheckButton(void);




/** @} */ // end of PAPI
#ifdef __cplusplus
}
#endif

#endif


