#ifndef _PA_SpriteDual
#define _PA_SpriteDual



/*! \file PA_SpriteDual.h
    \brief Everything concerning the sprite system, but for 2 screens !

    This file contains all macros, variables, and prototypes regarding the sprite system (OAM, Sprite mouvement, Gfx loading...) on 2 screens, easily, like if it was just one screen
*/

#include "PA_Sprite.h"


/** @defgroup SpriteDual Sprite system for Dual Screen
 *  Load Sprite, move them around, rotate them...
 *  @{
 */


/*! \fn extern inline void PA_SetScreenSpace(s16 ScreenSpace)
    \brief
         \~english Set the space between the 2 screens for the Dual Fonctions. 48 pixels by default
         \~french D�signer l'espace entre les 2 �crans, 48 pixels par d�faut
    \param ScreenSpace
         \~english Space in pixels
         \~french Espace en pixels
*/
extern inline void PA_SetScreenSpace(s16 ScreenSpace){
	PA_ScreenSpace = 192 + ScreenSpace;
}






/*! \fn extern inline void PA_DualSetSpriteY(u8 obj, s16 y)
    \brief
         \~english Set the Y position of a sprite on screen
         \~french Position Y du sprite � l'�cran
    \param obj
         \~english Object number in the sprite system
         \~french Num�ro de l'objet dans le systeme de sprite
    \param y
         \~english Y position
         \~french Position Y
*/
extern inline void PA_DualSetSpriteY(u8 obj, s16 y){
s16 y0 = 192; s16 y1 = 192;
	if (y < 192) y1 = y;
	if (y > PA_ScreenSpace-64) y0 = y - PA_ScreenSpace;
	PA_SetSpriteY(0, obj, y0);
	PA_SetSpriteY(1, obj, y1);	
}



/*! \fn extern inline void PA_DualCreateSprite(u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, s16 x, s16 y)
    \brief
         \~english Create a sprite with it's gfx, on 2 screens
         \~french Creer un sprite avec ses gfxsur les 2 �crans
    \param obj_number
         \~english Object number you want to use (0-127 for each screen seperately). 
         \~french Num�ro du sprite que vous voulez utiliser (de 0 � 127 pour chaque �cran s�paremment).
    \param obj_data
         \~english Gfx to load
         \~french Gfx � charger
    \param obj_shape
         \~english Object shape, from 0 to 2. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Forme du sprite � charger, de 0 � 2. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \param obj_size
         \~english Object size. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Taille du sprite. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \param color_mode
         \~english 256 or 16 color mode (1 or 0).
         \~french Mode 256 ou 16 couleurs (1 ou 0).
     \param palette
         \~english Palette to use (0-15).
         \~french Palette � utiliser (0-15). 
    \param x
         \~english X position of the sprite
         \~french Position X du sprite
    \param y
         \~english Y position of the sprite
         \~french Position Y du sprite
*/
extern inline void PA_DualCreateSprite(u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, s16 x, s16 y) {
	PA_CreateSprite(0, obj_number, obj_data, obj_shape, obj_size, color_mode, palette, x, 192);
	PA_CreateSprite(1, obj_number, obj_data, obj_shape, obj_size, color_mode, palette, x, 192);	
	PA_DualSetSpriteY(obj_number, y);
};

/*! \fn extern inline void PA_DualCreateSpriteEx(u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, u8 obj_mode, bool mosaic, bool hflip, bool vflip, u8 prio, bool dblsize, s16 x, s16 y)
    \brief
         \~english Create a sprite with it's gfx. This is the complex version of the function
         \~french Creer un sprite avec ses gfx... Ceci est la version complexe de la fonction
    \param obj_number
         \~english Object number you want to use (0-127 for each screen seperately). 
         \~french Num�ro du sprite que vous voulez utiliser (de 0 � 127 pour chaque �cran s�paremment).
    \param obj_data
         \~english Gfx to load
         \~french Gfx � charger
    \param obj_shape
         \~english Object shape, from 0 to 2. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Forme du sprite � charger, de 0 � 2. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \param obj_size
         \~english Object size. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Taille du sprite. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \param color_mode
         \~english 256 or 16 color mode (1 or 0).
         \~french Mode 256 ou 16 couleurs (1 ou 0).
     \param palette
         \~english Palette to use (0-15). 
         \~french Palette � utiliser (0-15). 
    \param obj_mode
         \~english Object mode (normal, transparent, window). Not functionnal yet, please leave to 0 for now
         \~french Mode du sprite (normal, transparent, fenetre). Pas encore op�rationnel, laisser � 0...
     \param mosaic
         \~english Activate Mosaic for the sprite or not. Not yet functionnal either :p
         \~french Activer le mode mosaique pour ce sprite. Pas encore au point...
    \param hflip
         \~english Horizontal flip on or off...
         \~french Flip horizontal activ� ou non.
     \param vflip
         \~english Vertical flip...
         \~french Flip vertical...
    \param prio
         \~english Sprite priority regarding backgrounds : in front of which background to show it (0-3)
         \~french Priorit� du sprite vis-�-vis des fonds : devant quel fond l'afficher... (0-3)
     \param dblsize
         \~english Double the possible sprite size. Activate only if you are going to rotate and zoom in the sprite
         \~french Doubler la taille possible du sprite. A activer uniquement si on compte grossir et faire tourner le sprite
    \param x
         \~english X position of the sprite
         \~french Position X du sprite
    \param y
         \~english Y position of the sprite
         \~french Position Y du sprite
*/
extern inline void PA_DualCreateSpriteEx(u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, u8 obj_mode, bool mosaic, bool hflip, bool vflip, u8 prio, bool dblsize, s16 x, s16 y){
	PA_CreateSpriteEx(0, obj_number, obj_data, obj_shape, obj_size, color_mode, palette, obj_mode, mosaic, hflip, vflip, prio, dblsize, x, 192);
	PA_CreateSpriteEx(1, obj_number, obj_data, obj_shape, obj_size, color_mode, palette, obj_mode, mosaic, hflip, vflip, prio, dblsize, x, 192);
	PA_DualSetSpriteY(obj_number, y);
}





/*! \fn extern inline void PA_DualCreate16bitSpriteEx(u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, bool mosaic, bool hflip, bool vflip, u8 prio, bool dblsize, s16 x, s16 y)
    \brief
         \~english Create a 16 bit sprite with it's gfx. This is the complex version of the function. Warning : a 16bit sprite MUST be 128 pixels large, even if you sprite only takes up a small part on the left...
         \~french Creer un sprite de 16 bits avec ses gfx... Ceci est la version complexe de la fonction. Attention : un sprite de 16 bits DOIT etre large de 128 pixels, meme si ce sprite ne prend qu'une petite partie sur la gauche
    \param obj_number
         \~english Object number you want to use (0-127 for each screen seperately). 
         \~french Num�ro du sprite que vous voulez utiliser (de 0 � 127 pour chaque �cran s�paremment).
    \param obj_data
         \~english Gfx to load
         \~french Gfx � charger
    \param obj_shape
         \~english Object shape, from 0 to 2. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Forme du sprite � charger, de 0 � 2. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \param obj_size
         \~english Object size. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Taille du sprite. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
     \param mosaic
         \~english Activate Mosaic for the sprite or not. Not yet functionnal either :p
         \~french Activer le mode mosaique pour ce sprite. Pas encore au point...
    \param hflip
         \~english Horizontal flip on or off...
         \~french Flip horizontal activ� ou non.
     \param vflip
         \~english Vertical flip...
         \~french Flip vertical...
    \param prio
         \~english Sprite priority regarding backgrounds : in front of which background to show it (0-3)
         \~french Priorit� du sprite vis-�-vis des fonds : devant quel fond l'afficher... (0-3)
     \param dblsize
         \~english Double the possible sprite size. Activate only if you are going to rotate and zoom in the sprite
         \~french Doubler la taille possible du sprite. A activer uniquement si on compte grossir et faire tourner le sprite
    \param x
         \~english X position of the sprite
         \~french Position X du sprite
    \param y
         \~english Y position of the sprite
         \~french Position Y du sprite
*/
extern inline void PA_DualCreate16bitSpriteEx(u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, bool mosaic, bool hflip, bool vflip, u8 prio, bool dblsize, s16 x, s16 y){
u16 mem_size = PA_obj_sizes[obj_size][obj_shape] << 1;
u16 *gfx = (u16*)malloc(mem_size);
mem_size = mem_size >> 1;
s32 i;
u16 *data = (u16*)obj_data;
for (i = 0; i < mem_size; i++) gfx[i] = data[i] + (1 << 15);

   PA_obj[0][obj_number].atr2 = PA_CreateGfx(0, gfx, obj_shape, obj_size, 2) + (prio << 10) + (15 << 12);
   PA_obj[1][obj_number].atr2 = PA_CreateGfx(1, gfx, obj_shape, obj_size, 2) + (prio << 10) + (15 << 12);   
   PA_obj[1][obj_number].atr0 = PA_obj[0][obj_number].atr0 = (192 & OBJ_Y) + (dblsize << 9) + (3 << 10) + (mosaic << 12) + (0 << 13) + (obj_shape << 14);
   PA_obj[1][obj_number].atr1 = PA_obj[0][obj_number].atr1 = (x & OBJ_X) + (hflip << 12) + (vflip << 13) + (obj_size << 14);
   PA_DualSetSpriteY(obj_number, y);

free(gfx);
}



/*! \fn extern inline void PA_DualCreate16bitSprite(u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, s16 x, s16 y)
    \brief
         \~english Create a 16 bit sprite with it's gfx. This is the simple version of the function. Warning : a 16bit sprite MUST be 128 pixels large, even if you sprite only takes up a small part on the left...
         \~french Creer un sprite de 16 bits avec ses gfx... Ceci est la version simple de la fonction. Attention : un sprite de 16 bits DOIT etre large de 128 pixels, meme si ce sprite ne prend qu'une petite partie sur la gauche
    \param obj_number
         \~english Object number you want to use (0-127 for each screen seperately). 
         \~french Num�ro du sprite que vous voulez utiliser (de 0 � 127 pour chaque �cran s�paremment).
    \param obj_data
         \~english Gfx to load
         \~french Gfx � charger
    \param obj_shape
         \~english Object shape, from 0 to 2. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Forme du sprite � charger, de 0 � 2. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \param obj_size
         \~english Object size. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Taille du sprite. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \param x
         \~english X position of the sprite
         \~french Position X du sprite
    \param y
         \~english Y position of the sprite
         \~french Position Y du sprite
*/
extern inline void PA_DualCreate16bitSprite(u8 obj_number, void* obj_data, u8 obj_shape, u8 obj_size, s16 x, s16 y){
PA_DualCreate16bitSpriteEx(obj_number, obj_data, obj_shape, obj_size, 0, 0, 0, 0, 0, x, y);
}




/*! \fn extern inline void PA_DualCreateSpriteFromGfx(u8 obj_number, u16 *obj_gfx, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, s16 x, s16 y)
    \brief
         \~english Create a sprite with it's gfx. This is the simple version of the function
         \~french Creer un sprite avec ses gfx... Ceci est la version simple de la fonction
    \param obj_number
         \~english Object number you want to use (0-127 for each screen seperately). 
         \~french Num�ro du sprite que vous voulez utiliser (de 0 � 127 pour chaque �cran s�paremment).
    \param obj_gfx
         \~english Memory gfx to use. Get it by using PA_GetSpriteGfx or PA_CreateGfx
         \~french Gfx en m�moire � utiliser. On peut en avoir avec PA_GetSpriteGfx ou PA_CreateGfx
    \param obj_shape
         \~english Object shape, from 0 to 2. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Forme du sprite � charger, de 0 � 2. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \param obj_size
         \~english Object size. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Taille du sprite. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \param color_mode
         \~english 256 or 16 color mode (1 or 0).
         \~french Mode 256 ou 16 couleurs (1 ou 0).
     \param palette
         \~english Palette to use (0-15). 
         \~french Palette � utiliser (0-15). 
    \param x
         \~english X position of the sprite
         \~french Position X du sprite
    \param y
         \~english Y position of the sprite
         \~french Position Y du sprite
*/
extern inline void PA_DualCreateSpriteFromGfx(u8 obj_number, u16 *obj_gfx, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, s16 x, s16 y) {
   PA_CreateSpriteFromGfx(0, obj_number, obj_gfx[0], obj_shape, obj_size, color_mode, palette, x, 192);
   PA_CreateSpriteFromGfx(1, obj_number, obj_gfx[1], obj_shape, obj_size, color_mode, palette, x, 192);  
PA_DualSetSpriteY(obj_number, y);
}

/*! \fn extern inline void PA_DualCreateSpriteExFromGfx(u8 obj_number, u16 *obj_gfx, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, u8 obj_mode, bool mosaic, bool hflip, bool vflip, u8 prio, bool dblsize, s16 x, s16 y)
    \brief
         \~english Create a sprite with it's gfx. This is the complex version of the function
         \~french Creer un sprite avec ses gfx... Ceci est la version complexe de la fonction
    \param obj_number
         \~english Object number you want to use (0-127 for each screen seperately). 
         \~french Num�ro du sprite que vous voulez utiliser (de 0 � 127 pour chaque �cran s�paremment).
    \param obj_gfx
         \~english Memory gfx to use. Get it by using PA_GetSpriteGfx or PA_CreateGfx
         \~french Gfx en m�moire � utiliser. On peut en avoir avec PA_GetSpriteGfx ou PA_CreateGfx
    \param obj_shape
         \~english Object shape, from 0 to 2. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Forme du sprite � charger, de 0 � 2. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \param obj_size
         \~english Object size. Use the OBJ_SIZE_32X32 (...) macros for object shape and obj_size...
         \~french Taille du sprite. Utiliser la macro OBJ_SIZE_32X32 (...) pour charger la forme et la taille...
    \param color_mode
         \~english 256 or 16 color mode (1 or 0).
         \~french Mode 256 ou 16 couleurs (1 ou 0).
     \param palette
         \~english Palette to use (0-15). 
         \~french Palette � utiliser (0-15). 
    \param obj_mode
         \~english Object mode (normal, transparent, window). Not functionnal yet, please leave to 0 for now
         \~french Mode du sprite (normal, transparent, fenetre). Pas encore op�rationnel, laisser � 0...
     \param mosaic
         \~english Activate Mosaic for the sprite or not. Not yet functionnal either :p
         \~french Activer le mode mosaique pour ce sprite. Pas encore au point...
    \param hflip
         \~english Horizontal flip on or off...
         \~french Flip horizontal activ� ou non.
     \param vflip
         \~english Vertical flip...
         \~french Flip vertical...
    \param prio
         \~english Sprite priority regarding backgrounds : in front of which background to show it (0-3)
         \~french Priorit� du sprite vis-�-vis des fonds : devant quel fond l'afficher... (0-3)
     \param dblsize
         \~english Double the possible sprite size. Activate only if you are going to rotate and zoom in the sprite
         \~french Doubler la taille possible du sprite. A activer uniquement si on compte grossir et faire tourner le sprite
    \param x
         \~english X position of the sprite
         \~french Position X du sprite
    \param y
         \~english Y position of the sprite
         \~french Position Y du sprite
*/
extern inline void PA_DualCreateSpriteExFromGfx(u8 obj_number, u16 *obj_gfx, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, u8 obj_mode, bool mosaic, bool hflip, bool vflip, u8 prio, bool dblsize, s16 x, s16 y) {
PA_CreateSpriteExFromGfx(0, obj_number, obj_gfx[0], obj_shape, obj_size, color_mode, palette, obj_mode, mosaic, hflip, vflip, prio, dblsize, x, 192);
PA_CreateSpriteExFromGfx(1, obj_number, obj_gfx[1], obj_shape, obj_size, color_mode, palette, obj_mode, mosaic, hflip, vflip, prio, dblsize, x, 192);
PA_DualSetSpriteY(obj_number, y);
}


/*! \fn extern inline void PA_DualUpdateSpriteGfx(u8 obj_number, void *obj_data)
    \brief
         \~english Update the Gfx of a given sprite
         \~french Mettre � jour les Gfx d'un sprite donn�
    \param obj_number
         \~english Object number in the sprite system
         \~french Num�ro de l'objet dans le systeme de sprite
    \param obj_data
         \~english Gfx to load
         \~french Graphisme � charger
*/
extern inline void PA_DualUpdateSpriteGfx(u8 obj_number, void *obj_data){
	PA_UpdateSpriteGfx(0, obj_number, obj_data);
	PA_UpdateSpriteGfx(1, obj_number, obj_data);
}


/*! \fn extern inline void PA_DualUpdateGfx(u16 gfx_number, void *obj_data)
    \brief
         \~english Update the Gfx of a given sprite
         \~french Mettre � jour les Gfx d'un sprite donn�
    \param gfx_number
         \~english Gfx number in memory
         \~french Num�ro du Gfx en m�moire
    \param obj_data
         \~english Gfx to load
         \~french Graphisme � charger
*/
extern inline void PA_DualUpdateGfx(u16 gfx_number, void *obj_data){
	PA_UpdateGfx(0, gfx_number, obj_data);
	PA_UpdateGfx(1, gfx_number, obj_data);	
}




/*! \fn extern inline void PA_DualDeleteSprite(u8 obj_number)
    \brief
         \~english Delete a given sprite. If it is the only one to use it's gfx, it'll be deleted too.
         \~french Effacer un sprite. S'il �tait le seul � utiliser un gfx, il sera effac� lui aussi
    \param obj_number
         \~english Sprite number
         \~french Num�ro du sprite
*/
extern inline void PA_DualDeleteSprite(u8 obj_number){
	PA_DeleteSprite(0, obj_number);
	PA_DeleteSprite(1, obj_number);	
}



/*! \fn extern inline void PA_DualSetSpriteRotEnable(u8 sprite, u8 rotset)
    \brief
         \~english Rotate and zoom a sprite
         \~french Faire tourner et zoomer un sprite
    \param sprite
         \~english Sprite you want to rotate
         \~french Sprite que l'on veut faire tourner
    \param rotset
         \~english Rotset you want to give to that sprite (0-31). You can apparently use a rotset for multiple sprites if zoomed/rotated identically...
         \~french Rotset que l'on veut pour un sprite donn� (0-31). On peut a priori utiliser un rotset pour plusieurs sprites, s'ils sont zoom�s/tourn�s pareil...
*/
extern inline void PA_DualSetSpriteRotEnable(u8 sprite, u8 rotset){
	PA_SetSpriteRotEnable(0, sprite, rotset);
	PA_SetSpriteRotEnable(1, sprite, rotset);	
}



/*! \fn extern inline void PA_DualSetSpriteRotDisable(u8 sprite)
    \brief
         \~english Stop rotating and zooming a sprite
         \~french Arreter de faire tourner et zoomer un sprite
    \param sprite
         \~english Sprite you want to rotate
         \~french Sprite que l'on veut faire tourner
*/
extern inline void PA_DualSetSpriteRotDisable(u8 sprite){
	PA_SetSpriteRotDisable(0, sprite);
	PA_SetSpriteRotDisable(1, sprite);	
}

/*! \fn extern inline void PA_DualSetRotset(u8 rotset, s16 angle, u16 zoomx, u16 zoomy)
    \brief
         \~english Rotate and zoom a sprite
         \~french Faire tourner et zoomer un sprite
    \param rotset
         \~english Rotset you want to change. To give a sprite a rotset, use PA_SetSpriteRotEnable...
         \~french Rotset que l'on veut changer. Pour attribuer un rotset � un sprite, utiliser PA_SetSpriteRotEnable
    \param angle
         \~english Angle, between 0 and 512 (not 360, be carefull)
         \~french Angle, entre 0 et 512 (et non 360, attention !)
    \param zoomx
         \~english Horizontal zoom. 256 is unzoomed, 512 is 2 times smaller, and 128 twice as big... So adjust at will ! :p
         \~french Zoom horizontal. 256 est pas de zoom, 512 2 fois plus petit, et 128 2 fois plus grand... Ajuster au mieux ! :p
    \param zoomy
         \~english Vertical zoom. 256 is unzoomed, 512 is 2 times smaller, and 128 twice as big... So adjust at will ! :p
         \~french Zoom vertical. 256 est pas de zoom, 512 2 fois plus petit, et 128 2 fois plus grand... Ajuster au mieux ! :p
*/

extern inline void PA_DualSetRotset(u8 rotset, s16 angle, u16 zoomx, u16 zoomy) {
u8 obj_num = (rotset << 2);
	PA_obj[0][obj_num].atr3 = PA_obj[1][obj_num].atr3 = (PA_Cos(angle) * zoomx) >> 8;
	PA_obj[0][obj_num + 1].atr3 = PA_obj[1][obj_num + 1].atr3 = (-PA_Sin(angle) * zoomy) >> 8;
	PA_obj[0][obj_num + 2].atr3 = PA_obj[1][obj_num + 2].atr3 = (PA_Sin(angle) * zoomx) >> 8;
	PA_obj[0][obj_num + 2].atr3 = PA_obj[1][obj_num + 2].atr3 = (PA_Sin(angle) * zoomx) >> 8;
	PA_obj[0][obj_num + 3].atr3 = PA_obj[1][obj_num + 3].atr3 = (PA_Cos(angle) * zoomy) >> 8;
}

/*! \fn extern inline void PA_DualSetRotsetNoZoom(u8 rotset, s16 angle)
    \brief
         \~english Rotate a sprite without zooming. It's a bit faster than the normal PA_SetRotset function
         \~french Faire tourner un sprite sans zoomer. C'est un peu plus rapide que la fonction PA_SetRotset
    \param rotset
         \~english Rotset you want to change. To give a sprite a rotset, use PA_SetSpriteRotEnable...
         \~french Rotset que l'on veut changer. Pour attribuer un rotset � un sprite, utiliser PA_SetSpriteRotEnable
    \param angle
         \~english Angle, between 0 and 512 (not 360, be carefull)
         \~french Angle, entre 0 et 512 (et non 360, attention !)
*/
extern inline void PA_DualSetRotsetNoZoom(u8 rotset, s16 angle)   {
u8 obj_num = (rotset << 2);
	PA_obj[0][obj_num].atr3 = PA_obj[1][obj_num].atr3 = PA_Cos(angle);
	PA_obj[0][obj_num + 1].atr3 = PA_obj[1][obj_num + 1].atr3 = -PA_Sin(angle);
	PA_obj[0][obj_num + 2].atr3 = PA_obj[1][obj_num + 2].atr3 = PA_Sin(angle);
	PA_obj[0][obj_num + 3].atr3 = PA_obj[1][obj_num + 3].atr3 = PA_Cos(angle);
}


/*! \fn extern inline void PA_DualSetRotsetNoAngle(u8 rotset, u16 zoomx, u16 zoomy)
    \brief
         \~english Zoom a sprite without rotating. It's a bit faster than the normal PA_SetRotset function
         \~french Zoomer un sprite sans le faire tourner. C'est un peu plus rapide que la fonction PA_SetRotset
    \param rotset
         \~english Rotset you want to change. To give a sprite a rotset, use PA_SetSpriteRotEnable...
         \~french Rotset que l'on veut changer. Pour attribuer un rotset � un sprite, utiliser PA_SetSpriteRotEnable
    \param zoomx
         \~english Horizontal zoom. 256 is unzoomed, 512 is 2 times smaller, and 128 twice as big... So adjust at will ! :p
         \~french Zoom horizontal. 256 est pas de zoom, 512 2 fois plus petit, et 128 2 fois plus grand... Ajuster au mieux ! :p
    \param zoomy
         \~english Vertical zoom. 256 is unzoomed, 512 is 2 times smaller, and 128 twice as big... So adjust at will ! :p
         \~french Zoom vertical. 256 est pas de zoom, 512 2 fois plus petit, et 128 2 fois plus grand... Ajuster au mieux ! :p
*/
extern inline void PA_DualSetRotsetNoAngle(u8 rotset, u16 zoomx, u16 zoomy) {
u8 obj_num = (rotset << 2);
	PA_obj[0][obj_num].atr3 = PA_obj[1][obj_num].atr3 = zoomx;
	PA_obj[0][obj_num + 1].atr3 = PA_obj[1][obj_num + 1].atr3 = 0;
	PA_obj[0][obj_num + 2].atr3 = PA_obj[1][obj_num + 2].atr3 = 0;
	PA_obj[0][obj_num + 3].atr3 = PA_obj[1][obj_num + 3].atr3 = zoomy;
}







/*! \fn extern inline void PA_DualSetSpriteX(u8 obj, s16 x)
    \brief
         \~english Set the X position of a sprite on screen
         \~french Position X du sprite � l'�cran
    \param obj
         \~english Object number in the sprite system
         \~french Num�ro de l'objet dans le systeme de sprite
    \param x
         \~english X position
         \~french Position X
*/
extern inline void PA_DualSetSpriteX(u8 obj, s16 x){
	PA_obj[0][obj].atr1 = PA_obj[1][obj].atr1 = (PA_obj[0][obj].atr1 & ALL_BUT(OBJ_X)) + ((x) & OBJ_X);
}








/*! \fn extern inline void PA_DualSetSpriteXY(u8 sprite, s16 x, s16 y)
    \brief
         \~english Set the X and Y position of a sprite on screen
         \~french Position X et Y du sprite � l'�cran
    \param sprite
         \~english sprite number in the sprite system
         \~french Num�ro du sprite dans le systeme de sprite
    \param x
         \~english X position
         \~french Position Y
    \param y
         \~english X position
         \~french Position Y
*/
extern inline void PA_DualSetSpriteXY(u8 sprite, s16 x, s16 y) {
	PA_DualSetSpriteX(sprite, x); 
	PA_DualSetSpriteY(sprite, y);
}



/*! \fn extern inline void PA_DualSetSpritePal(u8 obj, u8 pal)
    \brief
         \~english Set the color palette used by a sprite
         \~french Changer la palette d'un sprite
    \param obj
         \~english Object number in the sprite system
         \~french Num�ro de l'objet dans le systeme de sprite
    \param pal
         \~english Palette number (0 - 15)
         \~french Num�ro de la palette (de 0 � 15)
*/
extern inline void PA_DualSetSpritePal(u8 obj, u8 pal){
	PA_SetSpritePal(0, obj, pal);
	PA_SetSpritePal(1, obj, pal);	
}



/*! \fn extern inline void PA_DualSetSpriteDblsize(u8 obj, bool dblsize)
    \brief
         \~english Enable or disable double size for a given sprite
         \~french Activer ou d�sactiver le mode Doublesize pour un sprite
    \param obj
         \~english Object number in the sprite system
         \~french Num�ro de l'objet dans le systeme de sprite
    \param dblsize
         \~english 1 to enable doublesize, 0 to disable it...
         \~french 1 pour l'activer, 0 pour l'inactiver
*/
extern inline void PA_DualSetSpriteDblsize(u8 obj, bool dblsize){
	PA_SetSpriteDblsize(0, obj, dblsize);
	PA_SetSpriteDblsize(1, obj, dblsize);
}




/*! \fn extern inline void PA_DualSetSpriteColors(u8 sprite, bool n_colors)
    \brief
         \~english Change the sprite's color mode
         \~french Changer le mode de couleur du sprite
    \param sprite
         \~english Object number in the sprite system
         \~french Num�ro de l'objet dans le systeme de sprite
    \param n_colors
         \~english 0 for 16 colors, 1 for 256
         \~french 0 pour 16 couleurs, 1 pour 256
*/
extern inline void PA_DualSetSpriteColors(u8 sprite, bool n_colors){
	PA_SetSpriteColors(0, sprite, n_colors);
	PA_SetSpriteColors(1, sprite, n_colors);

}




/*! \fn extern inline void PA_DualSetSpriteMode(u8 sprite, u8 obj_mode)
    \brief
         \~english Set the sprite's mode : 0 for normal, 1 for alpha blending, 2 for window
         \~french R�gler le mode d'un sprite : 0 pour normal, 1 pour transparent, 2 pour fenetre
    \param sprite
         \~english Object number in the sprite system
         \~french Num�ro de l'objet dans le systeme de sprite
    \param obj_mode
         \~english Object mode : 0 for normal, 1 for alpha blending, 2 for window ; not working yet
         \~french Mode : 0 pour normal, 1 pour transparent, 2 pour fenetre ; ne marche pas encore
*/
extern inline void PA_DualSetSpriteMode(u8 sprite, u8 obj_mode){
	PA_SetSpriteMode(0, sprite, obj_mode);
	PA_SetSpriteMode(1, sprite, obj_mode);
}






/*! \fn extern inline void PA_DualSetSpriteMosaic(u8 obj, bool mosaic)
    \brief
         \~english Enable or disable mosaic mode for a given sprite
         \~french Mettre ou non un sprite en mode mosaic
    \param obj
         \~english Object number in the sprite system
         \~french Num�ro de l'objet dans le systeme de sprite
    \param mosaic
         \~english Set mosaic on (1) or off (0)
         \~french Mode mosaic activ� (1) ou d�sactiv� (0)
*/
extern inline void PA_DualSetSpriteMosaic(u8 obj, bool mosaic){
	PA_SetSpriteMosaic(0, obj, mosaic);
	PA_SetSpriteMosaic(1, obj, mosaic);
}






/*! \fn extern inline void PA_DualSetSpriteHflip(u8 obj, bool hflip)
    \brief
         \~english Enable or disable horizontal flip for a given sprite
         \~french Utiliser ou non le flip horizontal pour un sprite
    \param obj
         \~english Object number in the sprite system
         \~french Num�ro de l'objet dans le systeme de sprite
    \param hflip
         \~english Horizontal flip, 1 to enable, 0 to disable...
         \~french Flip horizontal, 1 pour oui, 0 pour non...
*/
extern inline void PA_DualSetSpriteHflip(u8 obj, bool hflip){
	PA_SetSpriteHflip(0, obj, hflip);
	PA_SetSpriteHflip(1, obj, hflip);
}	




/*! \fn extern inline void PA_DualSetSpriteVflip(u8 obj, bool vflip)
    \brief
         \~english Enable or disable vertical flip for a given sprite
         \~french Utiliser ou non le flip vertical pour un sprite
    \param obj
         \~english Object number in the sprite system
         \~french Num�ro de l'objet dans le systeme de sprite
    \param vflip
         \~english Vertical flip, 1 to enable, 0 to disable...
         \~french Flip vertical, 1 pour oui, 0 pour non...
*/
extern inline void PA_DualSetSpriteVflip(u8 obj, bool vflip){
	PA_SetSpriteVflip(0, obj, vflip);
	PA_SetSpriteVflip(1, obj, vflip);
}	






/*! \fn extern inline void PA_DualSetSpriteGfx(u8 obj, u16 *gfx)
    \brief
         \~english Change the gfx used by a sprite
         \~french Modifier les graphismes utilis�s par un sprite
    \param obj
         \~english Object number in the sprite system
         \~french Num�ro de l'objet dans le systeme de sprite
    \param gfx
         \~english Gfx number ; you can get one by using PA_CreateGfx or PA_GetSpriteGfx(obj_number);
         \~french Num�ro du gfx en m�moire ; on peut obtenir un num�ro avec PA_CreateGfx ou PA_GetSpriteGfx(obj_number);
*/
extern inline void PA_DualSetSpriteGfx(u8 obj, u16 *gfx){
	PA_SetSpriteGfx(0, obj, gfx[0]);
	PA_SetSpriteGfx(1, obj, gfx[1]);
}





/*! \fn extern inline void PA_DualSetSpritePrio(u8 obj, u8 prio)
    \brief
         \~english Set a sprite's Background priority
         \~french R�gler la priorit� d'un sprite par rapport au Bg.
    \param obj
         \~english Object number in the sprite system
         \~french Num�ro de l'objet dans le systeme de sprite
    \param prio
         \~english Sprite priority : 0 is over background 0, 1 over Bg 1, etc... (0-3)
         \~french Priorit� du sprite : 0 est au-dessus du fond 0, 1 au-dessus du 1, etc... (0-3)
*/
extern inline void PA_DualSetSpritePrio(u8 obj, u8 prio){
	PA_SetSpritePrio(0, obj, prio);
	PA_SetSpritePrio(1, obj, prio);
}	





/*! \fn extern inline void PA_DualCloneSprite(u8 obj, u8 target)
    \brief
         \~english Clone a sprite. Works only for sprites on the same screen
         \~french Cloner un sprite. Marche uniquement pour les sprites sur un meme �cran
    \param obj
         \~english Object number in the sprite system
         \~french Num�ro de l'objet dans le systeme de sprite
    \param target
         \~english Target sprite to clone
         \~french Num�ro de la cible � cloner
*/
extern inline void PA_DualCloneSprite(u8 obj, u8 target){
	PA_CloneSprite(0, obj, target);
	PA_CloneSprite(1, obj, target);
}







/*! \fn extern inline void PA_DualSetSpriteAnimEx(u8 sprite, u8 lx, u8 ly, u8 ncolors, s16 animframe)
    \brief
         \~english Set the animation frame for a given sprite. This function is faster than the normal PA_SetSpriteAnim because it doesn't have to lookup the sprite dimensions...
         \~french R�gler l'image du sprite dans l'animation. Cette fonction est plus rapide que PA_SetSpriteAnim parce qu'elle n'a pas � rechercher les dimensions du sprite
    \param sprite
         \~english sprite number in the sprite system
         \~french Num�ro du sprite dans le systeme de sprite
    \param lx
         \~english Sprite width (8, 16, 32, 64)
         \~french Largeur du sprite (8, 16, 32, 64)
    \param ly
         \~english Sprite height (8, 16, 32, 64)
         \~french Hauteur du sprite (8, 16, 32, 64)		 
    \param ncolors
         \~english Sprite color mode (0 for 16 colors, 1 for 256)
         \~french Mode couleur du sprite (0 pour 16 couleurs, 1 pour 256)
    \param animframe
         \~english Sprite animation frame (0, 1, 2, etc...)
         \~french Frame de l'animation du sprite (0, 1, 2, etc...)
*/
extern inline void PA_DualSetSpriteAnimEx(u8 sprite, u8 lx, u8 ly, u8 ncolors, s16 animframe){
	PA_SetSpriteAnimEx(0, sprite, lx, ly, ncolors, animframe);
	PA_SetSpriteAnimEx(1, sprite, lx, ly, ncolors, animframe);	
}





/*! \fn extern inline void PA_DualSetSpriteAnim(u8 sprite, s16 animframe)
    \brief
         \~english Set the animation frame for a given sprite. Same as PA_SetSpriteAnimEx, but a bit slower and easier to use...
         \~french R�gler l'image du sprite dans l'animation. Identique � PA_SetSpriteAnimEx, mais plus simple � utiliser, par contre plus lent
    \param sprite
         \~english sprite number in the sprite system
         \~french Num�ro du sprite dans le systeme de sprite	 
    \param animframe
         \~english Sprite animation frame (0, 1, 2, etc...)
         \~french Frame de l'animation du sprite (0, 1, 2, etc...)
*/
extern inline void PA_DualSetSpriteAnim(u8 sprite, s16 animframe){
u8 lx = PA_GetSpriteLx(0, sprite); u8 ly = PA_GetSpriteLy(0, sprite);
bool ncolors = PA_GetSpriteColors(0, sprite);
	PA_SetSpriteAnimEx(0, sprite, lx, ly, ncolors, animframe);
	PA_SetSpriteAnimEx(1, sprite, lx, ly, ncolors, animframe);	
}








/*! \fn extern inline void PA_DualSetSpritePixelEx(u8 sprite, u8 hsize, u8 n_colors, u8 x, u8 y, u8 color)
    \brief
         \~english Set a sprite's pixel to a given palette color
         \~french Mettre un pixel d'un sprite � une couleur donn�e
    \param sprite
         \~english Sprite number in the sprite system
         \~french Num�ro du sprite dans le systeme de sprite
    \param hsize
         \~english Horizontal size '8, 16, 32...)
         \~french Taille horizontale (8, 16...)
    \param n_colors
         \~english 0 or 1 for 16 or 256 colors...
         \~french 0 ou 1 pour 16 ou 256 couleurs
    \param x
         \~english X coordinate of the pixel to change
         \~french Coordonn�e X du pixel � changer
    \param y
         \~english Y coordinate of the pixel to change
         \~french Coordonn�e Y du pixel � changer
    \param color
         \~english New palette color to put
         \~french Nouvelle couleur de la palette � metrre
*/
extern inline void PA_DualSetSpritePixelEx(u8 sprite, u8 hsize, u8 n_colors, u8 x, u8 y, u8 color){
	PA_SetSpritePixelEx(0, sprite, hsize, n_colors, x, y, color);
	PA_SetSpritePixelEx(1, sprite, hsize, n_colors, x, y, color);	
}






/*! \fn extern inline void PA_DualSetSpritePixel(u8 sprite, s16 x, s16 y, bool color)
    \brief
         \~english Set a sprite's pixel to a given palette color. Like PA_SetSpritePixelEx, with less options, but a little slower
         \~french Mettre un pixel d'un sprite � une couleur donn�e. Comme PA_SetSpritePixelEx, avec moins d'options, mais un peu plus lent
    \param sprite
         \~english Sprite number in the sprite system
         \~french Num�ro du sprite dans le systeme de sprite
    \param x
         \~english X coordinate of the pixel to change
         \~french Coordonn�e X du pixel � changer
    \param y
         \~english Y coordinate of the pixel to change
         \~french Coordonn�e Y du pixel � changer
    \param color
         \~english New palette color to put
         \~french Nouvelle couleur de la palette � metrre
*/
extern inline void PA_DualSetSpritePixel(u8 sprite, s16 x, s16 y, bool color){
PA_DualSetSpritePixelEx(sprite, PA_GetSpriteLx(0, sprite), PA_GetSpriteColors(0, sprite), x, y, color);
}






/** @} */ // end of Sprite
















#endif


