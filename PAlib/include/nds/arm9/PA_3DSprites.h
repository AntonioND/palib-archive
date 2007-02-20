#ifndef _PA_3DSPRITES_
#define _PA_3DSPRITES_


#define PA_NMAXSPRITES 2048

#define TEX_A3I5 1
#define TEX_A5I3 6
#define TEX_4COL 2
#define TEX_16COL 3
#define TEX_256COL 4
#define TEX_16BITS 7

#define BLOCKSIZE_3D 16 // Minimal block size



 


extern inline void PA_glTranslate(s32 x, s32 y, s32 z) {
  MATRIX_TRANSLATE = (x);
  MATRIX_TRANSLATE = (y);
  MATRIX_TRANSLATE = (z);
}

extern inline void PA_Set3D(bool texture_mapping, bool shader, bool alpha_test, bool alpha_blend, bool antialias, bool edgemark, bool fog_mode, bool fog, u8 fog_shift, bool rearplane)
{
 	*(u32*)0x4000060= texture_mapping|shader<<1|alpha_test<<2|alpha_blend<<3|antialias<<4|edgemark<<5|fog_mode<<6|fog<<7|fog_shift<<8|rearplane<<14;
}


void PA_Load3DSpritePal(u16 palette_number, void* palette);


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
} pa3dsprites   ;
extern pa3dsprites pa_3dsprites[PA_NMAXSPRITES];

extern uint32 textures[MAX_TEXTURES];
extern uint32 activeTexture;

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

/*
extern inline void PA_3DCreateSprite16bitFromGfx(u16 sprite, u16 texture, u16 width, u16 height, s16 x, s16 y){
	pa_3dsprites[sprite].Alive = 1;	
	pa_3dsprites[sprite].X = x; pa_3dsprites[sprite].Y = y;
	pa_3dsprites[sprite].ImgWidth = width; pa_3dsprites[sprite].ImgHeight = height;
	pa_3dsprites[sprite].Width = width; pa_3dsprites[sprite].Height = height;		
	pa_3dsprites[sprite].Hflip = 0; pa_3dsprites[sprite].Vflip = 0;
	pa_3dsprites[sprite].RotX = 0; pa_3dsprites[sprite].RotY = 0; pa_3dsprites[sprite].RotZ = 0;
	pa_3dsprites[sprite].textureID = texture;
	pa_3dsprites[sprite].palette = 0;
}   

extern inline void PA_3DCreateSpriteFromGfx(u16 sprite, u16 texture, u8 palette, u16 width, u16 height, s16 x, s16 y){
	pa_3dsprites[sprite].Alive = 1;	
	pa_3dsprites[sprite].X = x; pa_3dsprites[sprite].Y = y;
	pa_3dsprites[sprite].ImgWidth = width; pa_3dsprites[sprite].ImgHeight = height;
	pa_3dsprites[sprite].Width = width; pa_3dsprites[sprite].Height = height;		
	pa_3dsprites[sprite].Hflip = 0; pa_3dsprites[sprite].Vflip = 0;
	pa_3dsprites[sprite].RotX = 0; pa_3dsprites[sprite].RotY = 0; pa_3dsprites[sprite].RotZ = 0;
	pa_3dsprites[sprite].textureID = texture;
	pa_3dsprites[sprite].palette = palette;
}     


extern inline void PA_3DCreateSprite4colFromGfx(u16 sprite, u16 texture, u8 palette, u16 width, u16 height, s16 x, s16 y){
	pa_3dsprites[sprite].Alive = 1;	
	pa_3dsprites[sprite].X = x; pa_3dsprites[sprite].Y = y;
	pa_3dsprites[sprite].ImgWidth = width; pa_3dsprites[sprite].ImgHeight = height;
	pa_3dsprites[sprite].Width = width; pa_3dsprites[sprite].Height = height;		
	pa_3dsprites[sprite].Hflip = 0; pa_3dsprites[sprite].Vflip = 0;
	pa_3dsprites[sprite].RotX = 0; pa_3dsprites[sprite].RotY = 0; pa_3dsprites[sprite].RotZ = 0;
	pa_3dsprites[sprite].textureID = texture;
	pa_3dsprites[sprite].palette = palette<<1;
}    */ 





extern inline void PA_3DDeleteSprite(u16 sprite){
	pa_3dsprites[sprite].Alive = 0;	
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
   pa_3dsprites[sprite].Width = pa_3dsprites[sprite].ImgWidth * zoomx;
}   
extern inline void PA_3DSetSpriteZoomY(u16 sprite, float zoomy){
   pa_3dsprites[sprite].Height = pa_3dsprites[sprite].ImgHeight * zoomy;
}   
extern inline void PA_3DSetSpriteZoomXY(u16 sprite, float zoomx, float zoomy){
   pa_3dsprites[sprite].Width = pa_3dsprites[sprite].ImgWidth * zoomx;
   pa_3dsprites[sprite].Height = pa_3dsprites[sprite].ImgHeight * zoomy;   
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



/** @} */ // end of 3DSprites




#endif


