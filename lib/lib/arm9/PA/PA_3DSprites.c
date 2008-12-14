
#ifdef __cplusplus
extern "C" {
#endif

#include <PA9.h>

pa3dsprites pa_3dsprites[PA_NMAXSPRITES];  // 3D Sprite infos


s16 pa_freetextures;
//u32 *pa_3dnextblock = (u32*);

#define N_TEXTURES 1024

// VRAM registers
#define SUB_BG2_CR     (*(vuint16*)0x0400100C)
#define SUB_BG2_XDX    (*(vuint16*)0x04001020)
#define SUB_BG2_XDY    (*(vuint16*)0x04001022)
#define SUB_BG2_YDX    (*(vuint16*)0x04001024)
#define SUB_BG2_YDY    (*(vuint16*)0x04001026)
#define SUB_BG2_CX     (*(vuint32*)0x04001028)
#define SUB_BG2_CY     (*(vuint32*)0x0400102C)


u16 n_free_mem3D; // nombre d'emplacements libres
u16 used_mem3D[16384]; // Note la quantité de mémoire utilisée en chaque point de la mémoire pour pouvoir effacer les gfx...
s16 obj_per_gfx3D[N_TEXTURES]; // Nombre de sprites utilisant un gfx donné...
mem_usage free_mem3D[16384];

type_3danims sprite3danims[PA_NMAXSPRITES];

u16 n3Dspriteanims;

u16 freetexslots[N_TEXTURES]; // Free Texture Slots...


u8 pa_3Dbanks;

uint32 textures[MAX_TEXTURES];
void *texturesptr[MAX_TEXTURES]; // for updates...
uint32 activeTexture;


void PA_3DUpdateSpriteAnims(void);
void PA_3DCapture(void);


u8 pa_3Ddual = 0;
u8 pa_3Dscreen = 0;

void PA_SetRegCapture(bool enable, uint8 srcBlend, uint8 destBlend, uint8 bank, uint8 offset, uint8 size, uint8 source, uint8 srcOffset)
{
   uint32 value = 0;
   if (enable)
      value |= 1 << 31; // 31 is enable
   value |= 3 << 29; // 29-30 seems to have something to do with the blending
   value |= (srcOffset & 0x3) << 26; // capture source offset is 26-27
   value |= (source & 0x3) << 24; // capture source is 24-25
   value |= (size & 0x3) << 20; // capture data write size is 20-21
   value |= (offset & 0x3) << 18; // write offset is 18-19 
   value |= (bank & 0x3) << 16; // vram bank select is 16-17 
   value |= (srcBlend & 0xFF) << 8; // graphics blend evb is 8..12
   value |= (destBlend & 0xFF) << 0; // ram blend EVA is bits 0..4 
   
   REG_DISPCAPCNT = value;
}



/* 40004A4h - Cmd 29h - POLYGON_ATTR
  0-3   Light 0..3 Enable Flags (each bit: 0=Disable, 1=Enable)
  4-5   Polygon Mode  (0=Modulation,1=Decal,2=Toon/Highlight Shading,3=Shadow)
  6     Polygon Back Surface   (0=Hide, 1=Render)  ;Line-segments are always
  7     Polygon Front Surface  (0=Hide, 1=Render)  ;rendered (no front/back)
  8-10  Not used
  11    Depth-value for Translucent Polygons  (0=Keep Old, 1=Set New Depth)
  12    Far-plane intersecting polygons       (0=Hide, 1=Render/clipped)
  13    1-Dot polygons behind DISP_1DOT_DEPTH (0=Hide, 1=Render)
  14    Depth Test, Draw Pixels with Depth    (0=Less, 1=Equal) (usually 0)
  15    Fog Enable                            (0=Disable, 1=Enable)
  16-20 Alpha      (0=Wire-Frame, 1..30=Translucent, 31=Solid)
  21-23 Not used
  24-29 Polygon ID (00h..3Fh, used for translucent, shadow, and edge-marking)
  30-31 Not used
  */
//   	POLYGON_ATTR = (0<<3)|(0<<4)|(1<<6)|(1<<7)|(0<<11)|(0<<12)|(15<<16);
   	  

#define PA_glRotateX glRotateXi
/*
extern inline void PA_glRotateX(u32 angle) {	
   angle = (((angle&511) * LUT_SIZE)>>9);

  	int32 sine = SIN[angle &  LUT_MASK];
	int32 cosine = COS[angle & LUT_MASK];

	MATRIX_MULT3x3 = inttof32(1);
	MATRIX_MULT3x3 = 0;
	MATRIX_MULT3x3 = 0;

	MATRIX_MULT3x3 = 0;
	MATRIX_MULT3x3 = cosine;
	MATRIX_MULT3x3 = sine;
  
	MATRIX_MULT3x3 = 0;
	MATRIX_MULT3x3 = -sine;
	MATRIX_MULT3x3 = cosine;	
	
}
*/

#define PA_glRotateY glRotateYi
/*
extern inline void PA_glRotateY(u32 angle) {	
   angle = (((angle&511) * LUT_SIZE)>>9);

	int32 sine = SIN[angle &  LUT_MASK];
	int32 cosine = COS[angle & LUT_MASK];

	MATRIX_MULT3x3 = cosine;
	MATRIX_MULT3x3 = 0;
	MATRIX_MULT3x3 = -sine;
  
	MATRIX_MULT3x3 = 0;
	MATRIX_MULT3x3 = inttof32(1);
	MATRIX_MULT3x3 = 0;
  
	MATRIX_MULT3x3 = sine;
	MATRIX_MULT3x3 = 0;
	MATRIX_MULT3x3 = cosine;	
}
*/

#define PA_glRotateZ glRotateZi
/*
extern inline void PA_glRotateZ(u32 angle) {	
   angle = (((angle&511) * LUT_SIZE)>>9);
	
	int32 sine = SIN[angle &  LUT_MASK];
	int32 cosine = COS[angle & LUT_MASK];

	MATRIX_MULT3x3 = cosine;
	MATRIX_MULT3x3 = sine;
	MATRIX_MULT3x3 = 0;

	MATRIX_MULT3x3 = - sine;
	MATRIX_MULT3x3 = cosine;
	MATRIX_MULT3x3 = 0;
  
	MATRIX_MULT3x3 = 0;
	MATRIX_MULT3x3 = 0;
	MATRIX_MULT3x3 = inttof32(1);		
}   
*/
  
 
extern inline void PA_glVertex(u16 x, u16 y, u16 z){
	GFX_VERTEX16 = (y << 16) | (x & 0xFFFF);
	GFX_VERTEX16 = ((uint32)z);
}	




extern inline void PA_glTranslate(s32 x, s32 y, s32 z) {
  MATRIX_TRANSLATE = (x);
  MATRIX_TRANSLATE = (y);
  MATRIX_TRANSLATE = (z);
}

#define PA_TEXTURE_PACK(u,v)    (((u) & 0xFFFF) | ((v) << 16))
  
void PA_3DSpriteToScreen(u16 sprite, s16 ydecal){
    
   MATRIX_PUSH = 0;
  
	PA_glTranslate(pa_3dsprites[sprite].X, (192-(pa_3dsprites[sprite].Y+ydecal)), pa_3dsprites[sprite].Priority);

	GFX_TEX_FORMAT = textures[pa_3dsprites[sprite].textureID]; 
	activeTexture = pa_3dsprites[sprite].textureID; 
	
	PA_glRotateX(pa_3dsprites[sprite].RotX);	
	PA_glRotateY(pa_3dsprites[sprite].RotY); 
	PA_glRotateZ(pa_3dsprites[sprite].RotZ); 
   	
   POLYGON_ATTR = (0<<3)|(0<<4)|(1<<6)|(1<<7)|(0<<11)|(0<<12)|(pa_3dsprites[sprite].alpha<<16)|(pa_3dsprites[sprite].polyID<<24);
   PA_PLTT_BASE = pa_3dsprites[sprite].palette<<5;

	
	u32 x1, x2, y1, y2;
	u8 hflip = pa_3dsprites[sprite].Hflip;	u8 vflip = pa_3dsprites[sprite].Vflip; 
	  	
	x1 = inttot16(pa_3dsprites[sprite].ImgWidth)*(hflip);	
	x2 = inttot16(pa_3dsprites[sprite].ImgWidth)*(!hflip);
	y1 = inttot16(pa_3dsprites[sprite].ImgHeight)*(!vflip);	
	y2 = inttot16(pa_3dsprites[sprite].ImgHeight)*(vflip);  
		
	s32 zoomx0 =  	pa_3dsprites[sprite].Width>>1;	s32 zoomx1 =  	(pa_3dsprites[sprite].Width+1)>>1;	
	s32 zoomy0 =  	pa_3dsprites[sprite].Height>>1;	s32 zoomy1 =  	(pa_3dsprites[sprite].Height+1)>>1;					

	// Start drawing texture
	GFX_BEGIN = GL_QUAD;
	
		GFX_NORMAL = NORMAL_PACK(0,inttov10(-1),0);

		GFX_TEX_COORD = (PA_TEXTURE_PACK(x1,y1));
		PA_glVertex(-zoomx1+pa_3dsprites[sprite].corner[2].x,	-zoomy1-pa_3dsprites[sprite].corner[2].y, 0 );			

		
		GFX_TEX_COORD = (PA_TEXTURE_PACK(x2, y1));
		PA_glVertex(zoomx0+pa_3dsprites[sprite].corner[3].x,	-zoomy1-pa_3dsprites[sprite].corner[3].y, 0 );	

		GFX_TEX_COORD = (PA_TEXTURE_PACK(x2,y2));				
		PA_glVertex(zoomx0+pa_3dsprites[sprite].corner[1].x,	zoomy0-pa_3dsprites[sprite].corner[1].y, 0 );		
		
		GFX_TEX_COORD = (PA_TEXTURE_PACK(x1, y2));
		PA_glVertex(-zoomx1+pa_3dsprites[sprite].corner[0].x,	zoomy0-pa_3dsprites[sprite].corner[0].y, 0 );		

	GFX_END = 0;
   
   MATRIX_POP = 1;

}  

//---------------------------------------------------------------------------------
void PA_glReset(void) {
//---------------------------------------------------------------------------------
	while (GFX_STATUS & (1<<27)); // wait till gfx engine is not busy
  
	// Clear the FIFO
	GFX_STATUS |= (1<<29);

	// Clear overflows for list memory
	GFX_CONTROL = ((1<<12) | (1<<13)) | GL_TEXTURE_2D;
	glResetMatrixStack();

	GFX_TEX_FORMAT = 0;
	GFX_POLY_FORMAT = 0;
  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
} 


void PA_3DProcess(void){


	if(pa_3Ddual) PA_3DCapture();
	PA_3DUpdateSpriteAnims(); // Update animations...
	PA_glReset();
	glEnable(GL_BLEND);
	glAlphaFunc(BLEND_ALPHA);

//	PA_Set3D(0, 0, 0, 1, 1, 0, 0, 0, 0, 1);

	MATRIX_CONTROL = GL_PROJECTION;
	glLoadIdentity();

	glOrthof32 (0, 255, 0, 191, 1, 8192);

	gluLookAt(	0.0, 0.0, 1.0,		//camera position 
				0.0, 0.0, 0.0,		//look at
				0.0, 1.0, 0.0);		//up

	MATRIX_CONTROL = GL_TEXTURE;
	glLoadIdentity();
	
	MATRIX_CONTROL = GL_MODELVIEW;

	glMaterialf(GL_AMBIENT, RGB15(31,31,31));
	glMaterialf(GL_DIFFUSE, RGB15(31,31,31));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(15,15,15));
	glMaterialf(GL_EMISSION, RGB15(31,31,31));   

	//ds uses a table for shinyness..this generates a half-ass one
	glMaterialShinyness(); 
	
	PA_TEXIMAGE_PARAM |= (1<<29); // Palette index 0 = transparent

	s32 i;

	if(pa_3Ddual){
		if(pa_3Dscreen == 1)	{
		   for(i = 0; i < PA_NMAXSPRITES; i++) {
			   if(pa_3dsprites[i].Alive)  PA_3DSpriteToScreen(i, 0);
			}
	
		}
			   
		else	{
		   for(i = 0; i < PA_NMAXSPRITES; i++) {
			   if(pa_3dsprites[i].Alive)  PA_3DSpriteToScreen(i, -PA_ScreenSpace);	
			}
	
		}	
	   PA_SwitchScreens(); 
		pa_3Dscreen = !pa_3Dscreen;
	}		
	else { // Normal
	   for(i = 0; i < PA_NMAXSPRITES; i++) {
		   if(pa_3dsprites[i].Alive)  PA_3DSpriteToScreen(i, 0);	
		}
	}			
	
	GFX_FLUSH = 0;

		
}   
 




void PA_3DDeleteTex(u32 tex_gfx) {
u16 i;
u16 j;
u8 exit = 0;
s8 decal = 0; // Décalage que l'on aura à faire pour classer le tableau...


   obj_per_gfx3D[tex_gfx] = 0;
   
    freetexslots[pa_freetextures] = tex_gfx;
    pa_freetextures++; 
   
    tex_gfx = (textures[tex_gfx]&(0xFFFF))<<1;
   

  // Gestion dynamique de la mémoire...
  for (i = 0; ((i < n_free_mem3D) & !exit); i++) { // On regarde les différents emplacements mémoire pour ajouter le morceau libre dans le tas...
	 if (tex_gfx < free_mem3D[i].mem_block) { // Si on a un trou qui précède le premier trou dispo, on va devoir tout décaler...peut-etre !
		exit = 1; // On va pourvoir sortir, après avir ordonné le tableau...
		decal = 1;
		if ((tex_gfx > 0) & (free_mem3D[i - 1].mem_block + free_mem3D[i - 1].free == tex_gfx)) { // On a 2 cases côtes à côtes, donc pas besoin de décaler, on ajouter la mem dispo...
		   free_mem3D[i-1].free += used_mem3D[tex_gfx];
		   tex_gfx = free_mem3D[i-1].mem_block;
		   used_mem3D[tex_gfx] = free_mem3D[i-1].free;
		   decal = 0;
		}
		if (tex_gfx + used_mem3D[tex_gfx] == free_mem3D[i].mem_block) { // Si le bloc d'après suit parfaitement le bloc qu'on vient d'ajouter...
		   -- decal;
		   free_mem3D[i].mem_block = tex_gfx;
		   free_mem3D[i].free += used_mem3D[tex_gfx];
		}
		// Si le décalage est de 0, on touche à rien
		if (decal == -1) { // On doit décaler vers la gauche... les données sont déjà dans le tableau
		   for (j = i - 1; j < n_free_mem3D; j++) {// On recopie la liste plus loin...
			  free_mem3D[j] = free_mem3D[j + 1];
		   }
		   -- n_free_mem3D;
		}
		else if (decal == 1) { // On doit tout décaler pour faire rentrer la case vide...
		   ++n_free_mem3D;
		   for (j = n_free_mem3D; j > i; j--) {// On recopie la liste plus loin...
			  free_mem3D[j] = free_mem3D[j - 1];
		   }
		   free_mem3D[i].mem_block = tex_gfx;
		   free_mem3D[i].free = used_mem3D[tex_gfx];
		}
	 }
  }

  // Effacage de la mémoire
//      DMA_Copy((void*)Blank, (void*)(SPRITE_GFX1 + (0x200000 *  screen) + (tex_gfx << NUMBER_DECAL)), (used_mem3D[tex_gfx] << MEM_DECAL), DMA_32NOW);
  used_mem3D[tex_gfx] = 0;


}






s16 PA_3DCreateTex(void* obj_data, u16 width, u16 height, u8 type) {
   
   
   pa_freetextures--; // One less texture slot
	s16 currenttex = freetexslots[pa_freetextures];
	GFX_TEX_FORMAT = textures[currenttex]; 
	activeTexture = currenttex;   
   
   s32 i;
   s16 texwidth = -1;
   s16 texheight = -1;
   for(i = 0; i < 7; i++) { // Get height...
	   if(width == (1<<(i+3))) texwidth = i;
	   if(height == (1<<(i+3))) texheight = i;
	}			   
	   
	
	u32 mem_size = width*height;
	// Correct size
	switch (type) {
	 case TEX_16BITS:
	   mem_size = mem_size << 1;
	   break;
	 case TEX_4COL:
	   mem_size = mem_size >> 2;
	   break;
	 case TEX_16COL:
	   mem_size = mem_size >> 1;
	   break;
	 default:
	   break;
	}	

	
	u32 block_size = mem_size >> 4; // block size
	
	

	u8 exit = 0;
	u16 n_mem = 0;
	u32 truenumber;
	
	for (i = 0; (i < n_free_mem3D) & !exit; i++) {
	   if (block_size <= free_mem3D[i].free) {
		  n_mem = i;
		  exit = 1;
	   }
	}
	
   i = free_mem3D[n_mem].mem_block; // On met la valeur de coté pour la renvoyer...
   truenumber = 0x06800000  + (i << 4);
   used_mem3D[i] = block_size;   // Nombre de blocks  
   
   // Start Copy...
   vramSetBankA(VRAM_A_LCD);  
   if(pa_3Dbanks == 2) vramSetBankB(VRAM_B_LCD);  
   DMA_Copy(obj_data, (void*)(truenumber), (mem_size>>1), DMA_16NOW);
   if(pa_3Dbanks == 2) vramSetBankB(VRAM_B_TEXTURE);  
   vramSetBankA(VRAM_A_TEXTURE);
   
   PA_glTexParameter(texwidth, texheight, (u32*)truenumber, type, TEXGEN_TEXCOORD);
   
   free_mem3D[n_mem].free -= block_size;
   if (free_mem3D[n_mem].free > 0) free_mem3D[n_mem].mem_block += block_size; // S'il reste un bout libre, on garde...
   else {// On doit tout décaler d'un cran... vers la gauche
	  for (i = n_mem; i < n_free_mem3D; i++) {// On recopie la liste plus loin...
		 free_mem3D[i] = free_mem3D[i + 1];
	  }
	  --n_free_mem3D;
   }
   texturesptr[currenttex] = (void*)obj_data; // Save pointer for animations
   obj_per_gfx3D[currenttex] = 1; // Nombre d'objets sur ce gfx...
    
   return currenttex;
}

void PA_3DCreateSpriteFromTex(u16 sprite, u16 texture, u16 width, u16 height, u8 palette, s16 x, s16 y){
	pa_3dsprites[sprite].Alive = 1;	
	pa_3dsprites[sprite].X = x; pa_3dsprites[sprite].Y = y;
	pa_3dsprites[sprite].ImgWidth = width; pa_3dsprites[sprite].ImgHeight = height;
	pa_3dsprites[sprite].Width = width; pa_3dsprites[sprite].Height = height;		
	pa_3dsprites[sprite].Hflip = 0; pa_3dsprites[sprite].Vflip = 0;
	pa_3dsprites[sprite].RotX = 0; pa_3dsprites[sprite].RotY = 0; pa_3dsprites[sprite].RotZ = 0;
	pa_3dsprites[sprite].textureID = texture;
	pa_3dsprites[sprite].Priority = 1024;
	pa_3dsprites[sprite].alpha = 31; // Solid
	pa_3dsprites[sprite].polyID = 0; // 0 polyID by default
	
	u8 i;
	for(i = 0; i < 4; i++){
		pa_3dsprites[sprite].corner[i].x = 0;
		pa_3dsprites[sprite].corner[i].y = 0;	
	}	
	
	if(((textures[texture]>>26)&7) == TEX_4COL){
	   pa_3dsprites[sprite].palette = palette<<1; // 1 more...
	}
	else{
	   pa_3dsprites[sprite].palette = palette; 
	}		   
	
	obj_per_gfx3D[texture]++;
}   



void PA_Reset3DSprites(void){
   n_free_mem3D = 1;
	free_mem3D[0].mem_block = 0; free_mem3D[0].free = 8192; // Number of free blocks in memory   
	s32 i;
	for(i = 0; i < PA_NMAXSPRITES; i++) {
		pa_3dsprites[i].Alive = 0; // Delete sprite
		sprite3danims[i].play = 0; // No animation
	}
	for(i = 0; i < 1024; i++) freetexslots[i] = 1023-i; // free slots...
	pa_freetextures = 1024;
	n3Dspriteanims = 0;
	pa_3Dbanks = 1;
}   


void PA_Reset3DSprites2Banks(void){
   n_free_mem3D = 1;
	free_mem3D[0].mem_block = 0; free_mem3D[0].free = 16384; // Number of free blocks in memory   
	s32 i;
	for(i = 0; i < PA_NMAXSPRITES; i++) {
		pa_3dsprites[i].Alive = 0; // Delete sprite
		sprite3danims[i].play = 0; // No animation
	}
	for(i = 0; i < 1024; i++) freetexslots[i] = 1023-i; // free slots...
	pa_freetextures = 1024;
	n3Dspriteanims = 0;
	pa_3Dbanks = 2; 
}   




void PA_3DUpdateGfx(u16 texture, void *image) {
   s32 mem_size = 1 <<(6+((textures[texture]>>20)&7)+((textures[texture]>>23)&7));
   u8 type = ((textures[texture]>>26)&7);
   switch (type) {
	 case TEX_16BITS:
	   mem_size = mem_size << 1;
	   break;
	 case TEX_4COL:
	   mem_size = mem_size >> 2;
	   break;
	 case TEX_16COL:
	   mem_size = mem_size >> 1;
	   break;
	 default:
	   break;
	}	
   vramSetBankA(VRAM_A_LCD);  
   if(pa_3Dbanks == 2) vramSetBankB(VRAM_B_LCD); 
	DMA_Copy((image), (void*)(0x06800000 + ((textures[texture]&0xFFFF)<<3)), (mem_size>>1), DMA_16NOW);
   if(pa_3Dbanks == 2) vramSetBankB(VRAM_B_TEXTURE);  
   vramSetBankA(VRAM_A_TEXTURE);
}


void PA_3DSetSpriteFrame(u16 sprite, u16 frame){
   
   u16 texture = pa_3dsprites[sprite].textureID;
   s16 mem_size = pa_3dsprites[sprite].ImgWidth * pa_3dsprites[sprite].ImgHeight;
   u8 type = ((textures[texture]>>26)&7);
   switch (type) {
	 case TEX_16BITS:
	   mem_size = mem_size << 1;
	   break;
	 case TEX_4COL:
	   mem_size = mem_size >> 2;
	   break;
	 case TEX_16COL:
	   mem_size = mem_size >> 1;
	   break;
	 default:
	   break;
	}	   
   PA_3DUpdateGfx(texture, texturesptr[texture] + mem_size*frame);
   
   
}   



void PA_3DUpdateSpriteAnims(void)
{
u16 anims = n3Dspriteanims;
u16 currentsprite = 0;

while((anims > 0) && (currentsprite < PA_NMAXSPRITES))
{
	if (sprite3danims[currentsprite].play)
	{
		//++nanim;
		sprite3danims[currentsprite].time += sprite3danims[currentsprite].speed;
		if (sprite3danims[currentsprite].time >= 60) 
		{
			while (sprite3danims[currentsprite].time >= 60)
			{
				sprite3danims[currentsprite].time -= 60;
				sprite3danims[currentsprite].currentframe+=sprite3danims[currentsprite].framechange;
				if (((sprite3danims[currentsprite].framechange > 0) && (sprite3danims[currentsprite].currentframe > sprite3danims[currentsprite].lastframe))||((sprite3danims[currentsprite].framechange < 0) && (sprite3danims[currentsprite].currentframe < sprite3danims[currentsprite].firstframe))){
					if (sprite3danims[currentsprite].type == ANIM_LOOP){ // Loop
						sprite3danims[currentsprite].currentframe = sprite3danims[currentsprite].firstframe;	
					}
					else { // Don't loop, go back -> switch speed and first/last frames
						sprite3danims[currentsprite].framechange = -sprite3danims[currentsprite].framechange;
						if (sprite3danims[currentsprite].ncycles == 1)// It was the last one
							sprite3danims[currentsprite].currentframe+=sprite3danims[currentsprite].framechange;
							//sprite3danims[currentsprite].framechange; // stop on the correct animation
						else sprite3danims[currentsprite].currentframe+=sprite3danims[currentsprite].framechange<<1; // continue going back and forth   
						
					}	

					// In all cases :
					sprite3danims[currentsprite].ncycles--; // 1 less to do
					if (sprite3danims[currentsprite].ncycles == 0) {
						PA_3DStopSpriteAnim(currentsprite);
					}
					else if (sprite3danims[currentsprite].ncycles<0) sprite3danims[currentsprite].ncycles = -1; // Inifinite
				}
			}
			//PA_OutputText(1, 0, nanim+18, "%d : %d, %d %d  ", currentsprite, PA_GetSpriteX(0, currentsprite), PA_GetSpriteY(0, currentsprite), PA_GetSpriteGfx(0, currentsprite));
			PA_3DSetSpriteFrame(currentsprite, sprite3danims[currentsprite].currentframe);
			
		}
		anims--; // Une de faite !
	}
currentsprite++; // next sprite...
}

}



void PA_3DStartSpriteAnimEx(u16 sprite, s16 firstframe, s16 lastframe, s16 speed, u8 type, s16 ncycles)
{
	sprite3danims[sprite].currentframe = sprite3danims[sprite].firstframe = firstframe;
	sprite3danims[sprite].lastframe = lastframe;
	sprite3danims[sprite].speed = speed;	
	sprite3danims[sprite].time = 0;
	sprite3danims[sprite].type = type;
	sprite3danims[sprite].ncycles = ncycles;
	sprite3danims[sprite].framechange = 1; // normal change to start
	
	if (!sprite3danims[sprite].play){ // Counts number of sprites playing...	
		
		n3Dspriteanims += 1;
	}
	
	PA_3DSetSpriteFrame(sprite, firstframe); // Set first animation frame
	sprite3danims[sprite].play = 1;	// playing...

	//PA_OutputText(1, 0, nanim+12, "%d : %d, %d   ", sprite, PA_GetSpriteX(0, sprite), PA_GetSpriteY(0, sprite));
	//nanim++;
}



void PA_Init3DDual(void)		
{
	PA_Init3D2Banks();
	PA_Reset3DSprites2Banks();
	
	// Init the Sub screen (test if eli can set this up correctly)
	// Set the Sub screen for 15 bit 2D
	videoSetModeSub(MODE_5_2D | DISPLAY_BG2_ACTIVE | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_2D_BMP_256);
	SUB_BG2_CR = BG_BMP16_256x256;
	SUB_BG2_XDX = 256;
	SUB_BG2_XDY = 0;
	SUB_BG2_YDX = 0;
	SUB_BG2_YDY = 256;
	SUB_BG2_CY = 0;
	SUB_BG2_CX = 0;
	
	vramSetBankC(VRAM_C_SUB_BG);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	s32 x, y;

	PA_ResetSpriteSysScreen(1);

   for (y = 0; y < 3; y++){
	   for (x = 0; x < 4; x++)	   {
	      PA_Create16bitSpriteFromGfx(1, x+y*4, x*8+(y*256), OBJ_SIZE_64X64, x*64, y*64);
	   }
	} 
	PA_UpdateOAM1();

	pa_3Ddual = 1;
	pa_3Dscreen = 0;	  
}

void PA_3DCapture(void){
	if(pa_3Dscreen == 0)	{
	   vramSetBankC(VRAM_C_SUB_BG);
	   vramSetBankD(VRAM_D_LCD);
	   PA_SetRegCapture(true, 0, 31, 3, 0, 3, 0, 0);
	}
		   
	else	{
      vramSetBankC(VRAM_C_LCD);
      vramSetBankD(VRAM_D_SUB_SPRITE);
      PA_SetRegCapture(true, 0, 31, 2, 0, 3, 0, 0);
	}	   
}   




#ifdef __cplusplus
}
#endif

