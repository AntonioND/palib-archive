#ifndef _PA_3DSPRITES_
#define _PA_3DSPRITES_


#define PA_NMAXSPRITES 1024

#define TEX_A3I5 1
#define TEX_A5I3 6
#define TEX_4COL 2
#define TEX_16COL 3
#define TEX_256COL 4
#define TEX_16BITS 7

#define BLOCKSIZE_3D 16 // Minimal block size



 #define N_TEXTURES 1024
extern s16 obj_per_gfx3D[N_TEXTURES];

extern inline void PA_Set3D(bool texture_mapping, bool shader, bool alpha_test, bool alpha_blend, bool antialias, bool edgemark, bool fog_mode, bool fog, u8 fog_shift, bool rearplane)
{
 	*(u32*)0x4000060= texture_mapping|shader<<1|alpha_test<<2|alpha_blend<<3|antialias<<4|edgemark<<5|fog_mode<<6|fog<<7|fog_shift<<8|rearplane<<14;
}


void PA_Load3DSpritePal(u16 palette_number, void* palette);

  
typedef struct{
   s16 x, y;
} pa3dcorners;

typedef struct{
   u8 Alive;
   s32 X, Y;
   s32 ImgWidth, ImgHeight;
   s32 Width, Height, Tex;
   s32 RotX, RotY, RotZ;
   u8 Hflip, Vflip;
   u16 textureID;
   u16 palette; // Palette...
   u16 Priority;
   pa3dcorners corner[4]; // Corners...
   u8 alpha;
} pa3dsprites   ;
extern pa3dsprites pa_3dsprites[PA_NMAXSPRITES];

extern uint32 textures[MAX_TEXTURES];
extern void *texturesptr[MAX_TEXTURES]; // for updates...
extern uint32 activeTexture;

typedef struct{
	s32 firstframe, lastframe, currentframe; // frames...
	s8 framechange; // 1 by default, -1 to go back...
	s32 time; // Time...
	u8 type;
	s16 speed;
	u8 play;
	s32 ncycles;
}	type_3danims;
extern type_3danims sprite3danims[2048];
extern u16 n3Dspriteanims;


#define POLYGON_ATTR 		*(u32*)0x040004A4
#define PA_PLTT_BASE 		*(u32*)0x040004AC
#define PA_TEXIMAGE_PARAM 	*(u32*)0x040004A8


extern inline void PA_glTexParameter(	uint8 sizeX, uint8 sizeY,uint32* addr,uint8 mode, uint32 param) {
	textures[activeTexture] = param | (sizeX << 20) | (sizeY << 23) | (((uint32)addr >> 3) & 0xFFFF) | (mode << 26) | (1<<29);// 1 << 29 to set background color to blank
}


/*! \file PA_3DSprites.h
    \brief Sprites on one screen using the DS's 3D GPU

	Allows to have up to 2048 sprites of pretty much any size !
*/




/** @defgroup 3DSprites 3D Sprite System
 *  Sprites on one screen using the DS's 3D GPU
 *  @{
 */


void PA_Init3D(void);
void PA_3DProcess(void);
s16 PA_3DCreateTex(void* obj_data, u16 width, u16 height, u8 type);
/*u16 PA_3DCreate16bitGfx(void *gfx, s16 width, s16 height);
u16 PA_3DCreateGfx(void *gfx, s16 width, s16 height);*/
void PA_3DCreateSpriteFromTex(u16 sprite, u16 texture, u16 width, u16 height, u8 palette, s16 x, s16 y);
void PA_Reset3DSprites(void);


extern inline void PA_3DCreateSprite(u16 sprite, void *image, u16 width, u16 height, u8 type, u8 palette, s16 x, s16 y){
	u16 tex_gfx = PA_3DCreateTex(image, width, height, type);
   PA_3DCreateSpriteFromTex(sprite, tex_gfx, width, height, palette, x, y);
   obj_per_gfx3D[tex_gfx]--;
}   




void PA_3DDeleteTex(u32 tex_gfx); // Delete...


extern inline void PA_3DDeleteSprite(u16 sprite){
	pa_3dsprites[sprite].Alive = 0;	
	u16 tex_gfx = pa_3dsprites[sprite].textureID;
	obj_per_gfx3D[tex_gfx]--;
	if(obj_per_gfx3D[tex_gfx] == 0) PA_3DDeleteTex(tex_gfx);  // Delete if only sprite to use the gfx...
}   


extern inline void PA_3DSetSpriteX(u16 sprite, s16 x){
  pa_3dsprites[sprite].X = x;
}   
extern inline void PA_3DSetSpriteY(u16 sprite, s16 y){
  pa_3dsprites[sprite].Y = y;
}   
extern inline void PA_3DSetSpriteXY(u16 sprite, s16 x, s16 y){
  pa_3dsprites[sprite].X = x;
  pa_3dsprites[sprite].Y = y;	      
}   
extern inline void PA_3DSetSpriteRotateX(u16 sprite, s16 rotateX){
   pa_3dsprites[sprite].RotX = rotateX;
}   
extern inline void PA_3DSetSpriteRotateY(u16 sprite, s16 rotateY){
   pa_3dsprites[sprite].RotY = rotateY;
}   		
extern inline void PA_3DSetSpriteRotate(u16 sprite, s16 rotate){
   pa_3dsprites[sprite].RotZ = rotate;
}   		


extern inline void PA_3DSetSpriteRotateXYZ(u16 sprite, s16 rotateX, s16 rotateY, s16 rotateZ){
   pa_3dsprites[sprite].RotX = rotateX;  pa_3dsprites[sprite].RotY = rotateY;  pa_3dsprites[sprite].RotZ = rotateZ;  
}  

extern inline void PA_3DSetSpriteZoomX(u16 sprite, float zoomx){
   pa_3dsprites[sprite].Width = (s32)(pa_3dsprites[sprite].ImgWidth * zoomx);
}   
extern inline void PA_3DSetSpriteZoomY(u16 sprite, float zoomy){
   pa_3dsprites[sprite].Height = (s32)(pa_3dsprites[sprite].ImgHeight * zoomy);
}   
extern inline void PA_3DSetSpriteZoomXY(u16 sprite, float zoomx, float zoomy){
   pa_3dsprites[sprite].Width = (s32)(pa_3dsprites[sprite].ImgWidth * zoomx);
   pa_3dsprites[sprite].Height = (s32)(pa_3dsprites[sprite].ImgHeight * zoomy);   
}   

extern inline void PA_3DSetSpriteWidth(u16 sprite, u16 width){
   pa_3dsprites[sprite].Width = width;
//   PA_3DSetSpriteZoomX(sprite, (float)(width)/pa_3dsprites[sprite].ImgWidth);
}   
extern inline void PA_3DSetSpriteHeight(u16 sprite, u16 height){
   pa_3dsprites[sprite].Height = height;
//   PA_3DSetSpriteZoomY(sprite, (float)(height)/pa_3dsprites[sprite].ImgHeight);
}   
extern inline void PA_3DSetSpriteWidthHeight(u16 sprite, u16 width, u16 height){
   PA_3DSetSpriteWidth(sprite, width);
   PA_3DSetSpriteHeight(sprite, height);		   
}   
extern inline void PA_3DSetSpriteHflip(u16 sprite, u8 hflip){
   pa_3dsprites[sprite].Hflip = hflip;
}   
extern inline void PA_3DSetSpriteVflip(u16 sprite, u8 vflip){
   pa_3dsprites[sprite].Vflip = vflip;
}  


extern inline u8 PA_3DSpriteTouched(u16 sprite){
   return PA_StylusInZone((pa_3dsprites[sprite].X-(pa_3dsprites[sprite].Width>>1)), (pa_3dsprites[sprite].Y-(pa_3dsprites[sprite].Height>>1)),
   							(pa_3dsprites[sprite].X+(pa_3dsprites[sprite].Width>>1)), (pa_3dsprites[sprite].Y+(pa_3dsprites[sprite].Height>>1)));
}   

extern inline void PA_3DSetSpriteTex(u16 sprite, u16 texture){
   pa_3dsprites[sprite].textureID = texture;
}   

extern inline void PA_3DSetSpritePal(u16 sprite, u16 palette){
   pa_3dsprites[sprite].palette = palette;
}   

void PA_3DUpdateGfx(u16 texture, void *image);
void PA_3DSetSpriteFrame(u16 sprite, u16 frame);

extern inline void PA_3DSetSpriteTopLeft(u16 sprite, s16 x, s16 y){
   pa_3dsprites[sprite].corner[0].x = x; pa_3dsprites[sprite].corner[0].y = y;
}   
extern inline void PA_3DSetSpriteTopRight(u16 sprite, s16 x, s16 y){
   pa_3dsprites[sprite].corner[1].x = x; pa_3dsprites[sprite].corner[1].y = y;
}   
extern inline void PA_3DSetSpriteBottomLeft(u16 sprite, s16 x, s16 y){
   pa_3dsprites[sprite].corner[2].x = x; pa_3dsprites[sprite].corner[2].y = y;
}   
extern inline void PA_3DSetSpriteBottomRight(u16 sprite, s16 x, s16 y){
   pa_3dsprites[sprite].corner[3].x = x; pa_3dsprites[sprite].corner[3].y = y;
} 


/*! \fn void PA_StartSpriteAnimEx(u8 screen, u8 sprite, s16 firstframe, s16 lastframe, s16 speed, u8 type, s16 ncycles)
    \brief
         \~english Start a sprite animation. Once started, it continues on and on by itself until you stop it !
         \~french D�marre une animation de sprite. Une fois d�marr�e, elle continue tant qu'on ne l'arr�te pas !
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param sprite
         \~english sprite number in the sprite system
         \~french Num�ro du sprite dans le systeme de sprite	 
    \param firstframe
         \~english First frame of the animation sequence, most of the time 0...
         \~french Premi�res image de l'animation, g�n�ralement 0....
    \param lastframe
         \~english Last frame to be displayed. When it gets there, it loops back to the first frame
         \~french Derni�re image � afficher. Une fois atteinte, ca retourne � la premi�re
    \param speed
         \~english Speed, in frames per second. So speed 1 would mean 1 image per second, so 1 image every game frame
         \~french Vitesse, en frames par seconde (fps). 1 signifie donc 1 image par seconde... 
    \param type
         \~english Defines how you want it to loop. ANIM_LOOP (0) for a normal loop, ANIM_UPDOWN (1) for back and forth animation.
         \~french D�fini de quelle mani�re on veut boucler. ANIM_LOOP (0) pour normal, et ANIM_UPDOWN (1) pour d'avant en arri�re
    \param ncycles
         \~english Number of animation cycles before stopping. If using ANIM_UPDOWN, it takes 2 cycles to come back to the original image
         \~french Nombres de cycles d'animations avant l'arr�t. Si on utilise ANIM_UPDOWN, il faut 2 cycles pour que l'animation revienne � l'image de base
*/

void PA_3DStartSpriteAnimEx(u8 sprite, s16 firstframe, s16 lastframe, s16 speed, u8 type, s16 ncycles);


/*! \fn extern inline void PA_StartSpriteAnim(u8 screen, u8 sprite, s16 firstframe, s16 lastframe, s16 speed)
    \brief
         \~english Start a sprite animation. Once started, it continues on and on by itself until you stop it !
         \~french D�marre une animation de sprite. Une fois d�marr�e, elle continue tant qu'on ne l'arr�te pas !
    \param sprite
         \~english sprite number in the sprite system
         \~french Num�ro du sprite dans le systeme de sprite	 
    \param firstframe
         \~english First frame of the animation sequence, most of the time 0...
         \~french Premi�res image de l'animation, g�n�ralement 0....
    \param lastframe
         \~english Last frame to be displayed. When it gets there, it loops back to the first frame
         \~french Derni�re image � afficher. Une fois atteinte, ca retourne � la premi�re
    \param speed
         \~english Speed, in frames per second. So speed 1 would mean 1 image per second, so 1 image every game frame
         \~french Vitesse, en frames par seconde (fps). 1 signifie donc 1 image par seconde... 
*/
extern inline void PA_3DStartSpriteAnim(u8 sprite, s16 firstframe, s16 lastframe, s16 speed){
	PA_3DStartSpriteAnimEx(sprite, firstframe, lastframe, speed, ANIM_INFINITE);
}




/*! \fn extern inline void PA_StopSpriteAnim(u8 screen, u8 sprite)
    \brief
         \~english Stop a sprite animation
         \~french Arr�ter une animation de sprite
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param sprite
         \~english sprite number in the sprite system
         \~french Num�ro du sprite dans le systeme de sprite	
*/
extern inline void PA_3DStopSpriteAnim(u8 sprite)
{
	if (sprite3danims[sprite].play) n3Dspriteanims--;
	sprite3danims[sprite].play = 0;
}

/*! \fn extern inline void PA_SetSpriteAnimFrame(u8 screen, u8 sprite, u16 frame)
    \brief
         \~english Set the current animation frame number
         \~french Changer le num�ro actuel de la frame d'animation
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param sprite
         \~english sprite number in the sprite system
         \~french Num�ro du sprite dans le systeme de sprite	
    \param frame
         \~english Frame number to use...
         \~french Num�ro de frame...	 
*/
extern inline void PA_3DSetSpriteAnimFrame(u8 sprite, u16 frame)
{
	if(sprite3danims[sprite].currentframe != frame){
		PA_3DSetSpriteFrame(sprite, frame);
		sprite3danims[sprite].currentframe = frame;
	}
}


/*! \fn extern inline u16 PA_GetSpriteAnimFrame(u8 screen, u8 sprite)
    \brief
         \~english Returns the current animation frame number
         \~french Renvoie le num�ro actuel de la frame d'animation
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param sprite
         \~english sprite number in the sprite system
         \~french Num�ro du sprite dans le systeme de sprite	 
*/
extern inline u16 PA_3DGetSpriteAnimFrame(u8 sprite)
{
	return sprite3danims[sprite].currentframe;
}



/*! \fn extern inline void PA_SetSpriteAnimSpeed(u8 screen, u8 sprite, s16 speed)
    \brief
         \~english Set the current animation speed
         \~french Changer la vitesse de l'animation
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param sprite
         \~english sprite number in the sprite system
         \~french Num�ro du sprite dans le systeme de sprite	
    \param speed
         \~english Speed, in fps...
         \~french Vitesse, en fps...		 
*/
extern inline void PA_3DSetSpriteAnimSpeed(u8 sprite, s16 speed)
{
	sprite3danims[sprite].speed = speed;
}

/*! \fn extern inline u16 PA_GetSpriteAnimSpeed(u8 screen, u8 sprite)
    \brief
         \~english Returns the current animation speed
         \~french Renvoie la vitesse de l'animation
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param sprite
         \~english sprite number in the sprite system
         \~french Num�ro du sprite dans le systeme de sprite	
*/
extern inline u16 PA_3DGetSpriteAnimSpeed(u8 sprite)
{
	return sprite3danims[sprite].speed;
}


/*! \fn extern inline void PA_SetSpriteNCycles(u8 screen, u8 sprite, s16 NCycles)
    \brief
         \~english Set the current animation cycles left (-1 for inifinite loop)
         \~french Changer le nombre de cycles d'animation restant (-1 pour inifini)
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param sprite
         \~english sprite number in the sprite system
         \~french Num�ro du sprite dans le systeme de sprite	
    \param NCycles
         \~english Number of cycles
         \~french Nombre de cycles		 
*/
extern inline void PA_3DSetSpriteNCycles(u8 sprite, s16 NCycles)
{
	sprite3danims[sprite].ncycles = NCycles;
}



/*! \fn extern inline u16 PA_GetSpriteNCycles(u8 screen, u8 sprite)
    \brief
         \~english Returns the current number of animation cycles left
         \~french Renvoie le nombre de cycles d'animation restants
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param sprite
         \~english sprite number in the sprite system
         \~french Num�ro du sprite dans le systeme de sprite	
*/
extern inline u16 PA_3DGetSpriteNCycles(u8 sprite)
{
	return sprite3danims[sprite].speed;
}





/*! \fn extern inline u16 PA_SpriteAnimPause(u8 screen, u8 sprite, u8 pause)
    \brief
         \~english Pause or UnPause a sprite animation
         \~french Mettre en Pause en remettre en lecture une animation de sprite
    \param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \param sprite
         \~english sprite number in the sprite system
         \~french Num�ro du sprite dans le systeme de sprite	
     \param pause
         \~english 1 for pause, 0 for unpause
         \~french 1 pour pause, 0 pour reprendre la lecture...
*/
extern inline void PA_3DSpriteAnimPause(u8 sprite, u8 pause)
{
	if (pause&&sprite3danims[sprite].play) n3Dspriteanims--;
	else if ((!pause)&&(!sprite3danims[sprite].play)) n3Dspriteanims++;
	sprite3danims[sprite].play = !pause;
}



/** @} */ // end of 3DSprites




#endif


