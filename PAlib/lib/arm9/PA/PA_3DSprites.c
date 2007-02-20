
#ifdef __cplusplus
extern "C" {
#endif

#include <PA9.h>

pa3dsprites pa_3dsprites[PA_NMAXSPRITES];  // 3D Sprite infos


s16 pa_ntextures = 0;
u32 *pa_3dnextblock = (u32*)0x06800000;


u16 n_free_mem3D; // nombre d'emplacements libres
u8 used_mem3D[8192]; // Note la quantité de mémoire utilisée en chaque point de la mémoire pour pouvoir effacer les gfx...
u8 obj_per_gfx3D[8192]; // Nombre de sprites utilisant un gfx donné...
mem_usage free_mem3D[8192];




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
   	  
  
void PA_3DSpriteToScreen(u16 sprite){
   
 
   	glPushMatrix();		
  
	PA_glTranslate(pa_3dsprites[sprite].X, (191-pa_3dsprites[sprite].Y), pa_3dsprites[sprite].Priority);
//   	glBindTexture(0, textureID);
	GFX_TEX_FORMAT = textures[pa_3dsprites[sprite].textureID]; 
	activeTexture = pa_3dsprites[sprite].textureID; 
	
	glRotateX((pa_3dsprites[sprite].RotX*360)>>9);	
	glRotateY((pa_3dsprites[sprite].RotY*360)>>9); 
	glRotateZ((pa_3dsprites[sprite].RotZ*360)>>9); // Convert from 512 angle
   	
//	PA_glTranslate((pa_3dsprites[sprite].X-128)*58, (96-pa_3dsprites[sprite].Y)*58, 0);

   	POLYGON_ATTR = (0<<3)|(0<<4)|(1<<6)|(1<<7)|(0<<11)|(0<<12)|(31<<16);
   	PA_PLTT_BASE = pa_3dsprites[sprite].palette<<5;

   	
   	u32 x1, x2, y1, y2;
   	u8 hflip = pa_3dsprites[sprite].Hflip;
   	u8 vflip = pa_3dsprites[sprite].Vflip; 
		  	
   	x1 = inttot16(pa_3dsprites[sprite].ImgWidth)*(!vflip);
   	x2 = inttot16(pa_3dsprites[sprite].ImgWidth)*(vflip);
   	y1 = inttot16(pa_3dsprites[sprite].ImgHeight)*(!hflip);
   	y2 = inttot16(pa_3dsprites[sprite].ImgHeight)*(hflip);  
		
		s32 zoomx =  	pa_3dsprites[sprite].Width>>1;
		s32 zoomy =  	pa_3dsprites[sprite].Height>>1;					
   	
		//draw the obj
		glBegin(GL_QUAD);
		
			glNormal(NORMAL_PACK(0,inttov10(-1),0));

			glTexCoord1i(TEXTURE_PACK(x1, y2));
			glVertex3v16(-zoomx,	-zoomy, 0 );
	
			glTexCoord1i(TEXTURE_PACK(x1,y1));
			glVertex3v16(zoomx,	-zoomy, 0 );
	
			glTexCoord1i(TEXTURE_PACK(x2, y1));
			glVertex3v16(zoomx,	zoomy, 0 );

			glTexCoord1i(TEXTURE_PACK(x2,y2));
			glVertex3v16(-zoomx,	zoomy, 0 );
		glEnd();   
   
   	glPopMatrix(1);
}   


void PA_3DProcess(void){

	glReset();
//		glPushMatrix();
//	PA_Set3D(0, 0, 0, 1, 1, 0, 0, 0, 0, 1);
		
	//any floating point gl call is being converted to fixed prior to being implemented
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//		gluPerspectivef32(50, 5461, 4096, 204800);
	glOrthof32 (0, 256, 0, 192, 1, 204800);

	gluLookAt(	0.0, 0.0, 1.0,		//camera possition 
				0.0, 0.0, 0.0,		//look at
				0.0, 1.0, 0.0);		//up

	//move it away from the camera
//		glTranslate3f32(0, 0, floattof32(-1));
	
	glMatrixMode(GL_TEXTURE);
	glIdentity();
	
	glMatrixMode(GL_MODELVIEW);

	glMaterialf(GL_AMBIENT, RGB15(31,31,31));
	glMaterialf(GL_DIFFUSE, RGB15(31,31,31));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(15,15,15));
	glMaterialf(GL_EMISSION, RGB15(31,31,31));   

	//ds uses a table for shinyness..this generates a half-ass one
	glMaterialShinyness();

	//not a real gl function and will likely change
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK); 

	s32 i;
	PA_TEXIMAGE_PARAM |= (1<<29); // Palette index 0 = transparent
	for(i = 0; i < PA_NMAXSPRITES; i++) if(pa_3dsprites[i].Alive)  PA_3DSpriteToScreen(i);

//	glFlush();   
	GFX_FLUSH = 0;
   
}   



void PA_3DDeleteTex(u16 tex_gfx) {
u16 i;
u16 j;
u8 exit = 0;
s8 decal = 0; // Décalage que l'on aura à faire pour classer le tableau...


   obj_per_gfx3D[tex_gfx] = 0;

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
   
	s16 currenttex = pa_ntextures;
	pa_ntextures++;
	GFX_TEX_FORMAT = textures[currenttex]; 
	activeTexture = currenttex;   
   
   s32 i;
   s16 texwidth = -1;
   s16 texheight = -1;
   for(i = 0; i < 7; i++) { // Get height...
	   if(width == (1<<(i+3))) texwidth = i;
	   if(height == (1<<(i+3))) texheight = i;
	}			   
	   
	
	u16 mem_size = width*height;
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

	
	u16 block_size = mem_size >> 4; // block size
	
	

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
   
   // Start Copy...
   vramSetBankA(VRAM_A_LCD);  
   DMA_Copy(obj_data, (void*)(truenumber), (mem_size>>1), DMA_16NOW);
   vramSetBankA(VRAM_A_TEXTURE);
   
   PA_glTexParameter(texwidth, texheight, (u32*)truenumber, type, TEXGEN_TEXCOORD);
   
   // Remove from free Mem
   used_mem3D[i] = block_size;   // Nombre de blocks
   obj_per_gfx3D[i] = 1; // Nombre d'objets sur ce gfx...

   free_mem3D[n_mem].free -= block_size;
   if (free_mem3D[n_mem].free > 0) free_mem3D[n_mem].mem_block += block_size; // S'il reste un bout libre, on garde...
   else {// On doit tout décaler d'un cran... vers la gauche
	  for (i = n_mem; i < n_free_mem3D; i++) {// On recopie la liste plus loin...
		 free_mem3D[i] = free_mem3D[i + 1];
	  }
	  --n_free_mem3D;
   }
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
	if(((textures[texture]>>26)&7) == TEX_4COL){
	   pa_3dsprites[sprite].palette = palette<<1; // 1 more...
	}
	else{
	   pa_3dsprites[sprite].palette = palette; // 1 more...
	}		   
}   


void PA_Reset3DSprites(void){
   n_free_mem3D = 1;
	free_mem3D[0].mem_block = 0; free_mem3D[0].free = 8192; // Number of free blocks in memory   
	s32 i;
	for(i = 0; i < PA_NMAXSPRITES; i++) pa_3dsprites[i].Alive = 0; // Delete sprite
}   




/*
int PAglTexImage2D(int target, int empty1, int type, int sizeX, int sizeY, int empty2, int param, uint8* texture) ;

u16 PA_3DCreate16bitGfx(void *gfx, s16 width, s16 height){
//	glBindTexture(0, pa_ntextures);
	s16 currenttex = pa_ntextures;
	pa_ntextures++;
	GFX_TEX_FORMAT = textures[currenttex]; 
	activeTexture = currenttex;
	
	PAglTexImage2D(0, 0, GL_RGBA, width , height, 0, TEXGEN_TEXCOORD, (u8*)gfx); 

   return currenttex;
}   

u16 PA_3DCreateGfx(void *gfx, s16 width, s16 height){
//	glBindTexture(0, pa_ntextures);
	s16 currenttex = pa_ntextures;
	pa_ntextures++;
	GFX_TEX_FORMAT = textures[currenttex]; 
	activeTexture = currenttex;
	
	PAglTexImage2D(0, 0, GL_RGB256, width , height, 0, TEXGEN_TEXCOORD, (u8*)gfx); 

   return currenttex;
}   



u16 PA_3DCreate16colGfx(void *gfx, s16 width, s16 height){
//	glBindTexture(0, pa_ntextures);
	s16 currenttex = pa_ntextures;
	pa_ntextures++;
	GFX_TEX_FORMAT = textures[currenttex]; 
	activeTexture = currenttex;
	
	PAglTexImage2D(0, 0, GL_RGB16, width , height, 0, TEXGEN_TEXCOORD, (u8*)gfx); 

   return currenttex;
}   

u16 PA_3DCreate4colGfx(void *gfx, s16 width, s16 height){
//	glBindTexture(0, pa_ntextures);
	s16 currenttex = pa_ntextures;
	pa_ntextures++;
	GFX_TEX_FORMAT = textures[currenttex]; 
	activeTexture = currenttex;
	
	PAglTexImage2D(0, 0, GL_RGB4, width , height, 0, TEXGEN_TEXCOORD, (u8*)gfx); 

   return currenttex;
}   




int PAglTexImage2D(int target, int empty1, int type, int sizeX, int sizeY, int empty2, int param, uint8* texture) {
  uint32 size = 0;
  uint32* addr;

  size = 1 << (sizeX + sizeY + 6);

  switch (type) {
    case GL_RGB:
    case GL_RGBA:
      size = size << 1;
      break;
    case GL_RGB4:
      size = size >> 2;
      break;
    case GL_RGB16:
      size = size >> 1;
      break;

    default:
      break;
  }
  
  addr = pa_3dnextblock;
	pa_3dnextblock += size >> 2;
  
  //addr = getNextTextureSlot(size);
  
  if(!addr)
    return 0;

  // unlock texture memory
//  vramTemp = vramSetMainBanks(VRAM_A_LCD,VRAM_B_LCD,VRAM_C_LCD,VRAM_D_LCD);
	vramSetBankA(VRAM_A_LCD);  
	
  if (type == GL_RGB) {
    // We do GL_RGB as GL_RGBA, but we set each alpha bit to 1 during the copy
    u16 * src = (u16*)texture;
    u16 * dest = (u16*)addr;
    
    PA_glTexParameter(sizeX, sizeY, addr, GL_RGBA, param);
    
    while (size--) {
      *dest++ = *src | (1 << 15);
      src++;
    }
  } else {
    // For everything else, we do a straight copy
    PA_glTexParameter(sizeX, sizeY, addr, type, param);
    swiCopy((uint32*)texture, addr , size / 4 | COPY_MODE_WORD);
  }

  vramSetBankA(VRAM_A_TEXTURE);
  

  return 1;
}
*/



#ifdef __cplusplus
}
#endif

