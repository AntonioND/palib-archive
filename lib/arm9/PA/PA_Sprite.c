#include "PA9.h"
//#include "PA_Sprite.h"

//////////////////////////////////////////////////////////////////////
// Système de sprites
//////////////////////////////////////////////////////////////////////

u16 n_free_mem[2]; // nombre d'emplacements libres
u8 used_mem[2][1024]; // Note la quantité de mémoire utilisée en chaque point de la mémoire pour pouvoir effacer les gfx...
u8 obj_per_gfx[2][1024]; // Nombre de sprites utilisant un gfx donné...
mem_usage free_mem[2][1024];
u16 FirstGfx[2] = {0, 0};


const PA_sizes PA_size[3][4] = {{{8, 8}, {16, 16}, {32, 32}, {64, 64}}, {{16, }, {32, 8}, {32, 16}, {64, 32}}, {{8, 16}, {8, 32}, {16, 32}, {32, 64}}};


// Utilisé pour déterminer la taille en mémoire en fonction de la taille du sprite
const u16 PA_obj_sizes[4][3] = {
   {64, 128, 128},
   {256, 256, 256},
   {1024, 512, 512},
   {4096, 2048, 2048}
};

obj_inf PA_obj[2][128]EWRAM_BSS;  // Les 128 premiers pour l'écran du haut, et encore 128 pour l'écran du bas...


PA_DrawSprites PA_DrawSprite[MAX_DRAW]EWRAM_BSS;  // Infos pour dessiner sur les sprites

unsigned char *PA_SpriteBuffer[MAX_DRAW]EWRAM_BSS; // Pointeurs vers les buffers  que l'on créera à la volée


u16 *PA_SpriteAnimP[2][128]EWRAM_BSS; // Pointer towards the sprite's graphics, allows animation...









void PA_ResetSpriteSys(void) {
u8 n;
u8 i;

   n_free_mem[0] = 1;
   free_mem[0][0].mem_block = 0; free_mem[0][0].free = 1024;
   n_free_mem[1] = 1;
   free_mem[1][0].mem_block = 0; free_mem[1][0].free = 1024;
	
   DMA_Copy((void*)Blank, (void*)used_mem, 256, DMA_32NOW);
   DMA_Copy((void*)Blank, (void*)obj_per_gfx, 256, DMA_32NOW);

   // Tous les sprites sont déplacés hors écran, et tout est initialisé...
for (i = 0; i < 2; i++) {
	for(n = 0; n < 128; n++) {
		PA_obj[i][n].atr0 = 192;
		PA_obj[i][n].atr1 = 256;
		PA_obj[i][n].atr2 = 0;
		PA_obj[i][n].atr3 = 0;
	}
	for(n = 0; n < 128; n++) {
		PA_SetRotset(i,n,0,256,256);  // Pad de zoom ou de rotation par défaut
	}
}
FirstGfx[0] = 0;
FirstGfx[1] = 0;

}




u16 PA_CreateGfx(bool screen, void* obj_data, u8 obj_shape, u8 obj_size, u8 color_mode) {
u16 mem_size = PA_obj_sizes[obj_size][obj_shape] >> (7 + !color_mode);
if (mem_size == 0) mem_size++; // Peut faire 0 si 8x8...
u8 exit = 0;
u16 n_mem = 0;
u16 i, truenumber;

for (i = 0; (i < n_free_mem[screen]) & !exit; i++) {
   if (mem_size <= free_mem[screen][i].free) {
      n_mem = i;
      exit = 1;
   }
}

   i = free_mem[screen][n_mem].mem_block; // On met la valeur de coté pour la renvoyer...
   truenumber = i + FirstGfx[screen];
   DMA_Copy(obj_data, (void*)(SPRITE_GFX1 + (0x200000 *  screen) + (truenumber << NUMBER_DECAL)), (mem_size << MEM_DECAL), DMA_32NOW);
   used_mem[screen][i] = mem_size;   // Nombre de blocks
   obj_per_gfx[screen][i] = 1; // Nombre d'objets sur ce gfx...

   free_mem[screen][n_mem].free -= mem_size;
   if (free_mem[screen][n_mem].free > 0) free_mem[screen][n_mem].mem_block += mem_size; // S'il reste un bout libre, on garde...
   else {// On doit tout décaler d'un cran... vers la gauche
      for (i = n_mem; i < n_free_mem[screen]; i++) {// On recopie la liste plus loin...
         free_mem[screen][i] = free_mem[screen][i + 1];
      }
      -- n_free_mem[screen];
   }
   
   return truenumber;
}


void PA_DeleteSprite(bool screen, u8 obj_number){
u16 obj_gfx = PA_GetSpriteGfx(screen, obj_number);

// Si aucun objet n'utilise cet emplacement, on libère la mémoire... Sinon on signale qu'un de moins s'en sert
   if (obj_per_gfx[screen][obj_gfx] == 0) PA_DeleteGfx(screen, obj_gfx);
   else --obj_per_gfx[screen][obj_gfx];

	PA_obj[screen][obj_number].atr0 = 192;
	PA_obj[screen][obj_number].atr1 = 256;
	PA_obj[screen][obj_number].atr2 = 0;
   

}



void PA_DeleteGfx(bool screen, u16 obj_gfx) {
u16 i;
u16 j;
u8 exit = 0;
s8 decal = 0; // Décalage que l'on aura à faire pour classer le tableau...

obj_gfx -= FirstGfx[screen];

   obj_per_gfx[screen][obj_gfx] = 0;

      // Gestion dynamique de la mémoire...
      for (i = 0; ((i < n_free_mem[screen]) & !exit); i++) { // On regarde les différents emplacements mémoire pour ajouter le morceau libre dans le tas...
         if (obj_gfx < free_mem[screen][i].mem_block) { // Si on a un trou qui précède le premier trou dispo, on va devoir tout décaler...peut-etre !
            exit = 1; // On va pourvoir sortir, après avir ordonné le tableau...
            decal = 1;
            if ((obj_gfx > 0) & (free_mem[screen][i - 1].mem_block + free_mem[screen][i - 1].free == obj_gfx)) { // On a 2 cases côtes à côtes, donc pas besoin de décaler, on ajouter la mem dispo...
               free_mem[screen][i-1].free += used_mem[screen][obj_gfx];
               obj_gfx = free_mem[screen][i-1].mem_block;
               used_mem[screen][obj_gfx] = free_mem[screen][i-1].free;
               decal = 0;
            }
            if (obj_gfx + used_mem[screen][obj_gfx] == free_mem[screen][i].mem_block) { // Si le bloc d'après suit parfaitement le bloc qu'on vient d'ajouter...
               -- decal;
               free_mem[screen][i].mem_block = obj_gfx;
               free_mem[screen][i].free += used_mem[screen][obj_gfx];
            }
            // Si le décalage est de 0, on touche à rien
            if (decal == -1) { // On doit décaler vers la gauche... les données sont déjà dans le tableau
               for (j = i - 1; j < n_free_mem[screen]; j++) {// On recopie la liste plus loin...
                  free_mem[screen][j] = free_mem[screen][j + 1];
               }
               -- n_free_mem[screen];
            }
            else if (decal == 1) { // On doit tout décaler pour faire rentrer la case vide...
               ++n_free_mem[screen];
               for (j = n_free_mem[screen]; j > i; j--) {// On recopie la liste plus loin...
                  free_mem[screen][j] = free_mem[screen][j - 1];
               }
               free_mem[screen][i].mem_block = obj_gfx;
               free_mem[screen][i].free = used_mem[screen][obj_gfx];
            }
         }
      }

      // Effacage de la mémoire
      DMA_Copy((void*)Blank, (void*)(SPRITE_GFX1 + (0x200000 *  screen) + (obj_gfx << NUMBER_DECAL)), (used_mem[screen][obj_gfx] << MEM_DECAL), DMA_32NOW);
      used_mem[screen][obj_gfx] = 0;
 


}



void PA_SetSpritePixelEx(bool screen, u8 sprite, u8 hsize, u8 n_colors, u8 x, u8 y, u8 color) {
hsize = hsize >> 3;
//		PA_SetSpritePixelEx(PA_DrawSprite[draw_number].screen, PA_DrawSprite[draw_number].sprite, PA_DrawSprite[draw_number].hsize, PA_DrawSprite[draw_number].n_colors, x, y, color);

	u16 ncase = (x >> 3) + ((y >> 3) * hsize);
	x&=7; y&=7;
	u32 *gfx = (u32*)(SPRITE_GFX1 + (0x200000 *  screen) + (PA_GetSpriteGfx(screen, sprite) << 5));

	if (n_colors) { // 256 couleurs
		ncase = (ncase << 4) + (x >> 2) + (y << 1);
//			PA_DrawSprite[draw_number].gfxpointer[ncase] = color;
		x &=3; x <<= 3;
		gfx[ncase] &= ~(255 << x);
		gfx[ncase] |= color << x;
	}
	else {
		ncase = (ncase << 3) + (x >> 3) + y;
		x &=3; x <<= 2;
		gfx[ncase] &= ~(15 << x);
		gfx[ncase] |= color << x;
	}
}


u8 PA_GetSpritePixelEx(bool screen, u8 sprite, u8 hsize, u8 n_colors, u8 x, u8 y) {
hsize = hsize >> 3;
//		PA_SetSpritePixelEx(PA_DrawSprite[draw_number].screen, PA_DrawSprite[draw_number].sprite, PA_DrawSprite[draw_number].hsize, PA_DrawSprite[draw_number].n_colors, x, y, color);

	u16 ncase = (x >> 3) + ((y >> 3) * hsize);
	x&=7; y&=7;
	u32 *gfx = (u32*)(SPRITE_GFX1 + (0x200000 *  screen) + (PA_GetSpriteGfx(screen, sprite) << 5));

	if (n_colors) { // 256 couleurs
		ncase = (ncase << 4) + (x >> 2) + (y << 1);
//			PA_DrawSprite[draw_number].gfxpointer[ncase] = color;
		x &=3; x <<= 3;
		return ((gfx[ncase] >> x) &255);
	}
	else {
		ncase = (ncase << 3) + (x >> 3) + y;
		x &=3; x <<= 2;
		return ((gfx[ncase] >> x) & 15);

	}
}

/*
void PA_SetBufferPixelEx(u8 drawsprite, u8 hsize, u8 n_colors, u8 x, u8 y, u8 color) {
hsize = hsize >> 3;
u8 ncase = (x >> 3) + ((y >> 3) * hsize);

	if (n_colors) { // 256 couleurs
		PA_SpriteBuffer[drawsprite][[(ncase << 6) + x + (y << 3)] = color];
	}
	else {
		ncase = (ncase << 5) + (x >> 1) + (y << 2);
		u8 npair = x&1; // impair
		spritegfx[ncase] &= 15 << (npair << 2);
		spritegfx[ncase] |= color;
	}
}
*/
/*
typedef struct {
	bool screen; // Ecran
	u8 sprite;		// Numéro du sprite
	bool wasdrawing; // Si on dessinait dessus ou pas avant
	u8 x, y;  // Position x et y d'avant
	u8 hsize, vsize; // Tailles
	u8 n_colors; // Nombre de couleurs
	u8 drawsize; // Taille du trait
} PA_DrawSprites;
*/


void PA_DrawSpritePixel(u8 draw_number, s16 x, s16 y, u8 color){
	if ((x > -1) && (x < PA_DrawSprite[draw_number].hsize) && (y > -1) && (y < PA_DrawSprite[draw_number].vsize)) {
//		PA_SetSpritePixelEx(PA_DrawSprite[draw_number].screen, PA_DrawSprite[draw_number].sprite, PA_DrawSprite[draw_number].hsize, PA_DrawSprite[draw_number].n_colors, x, y, color);
		u8 hsize = (PA_DrawSprite[draw_number].hsize >> 3);
		u16 ncase = (x >> 3) + ((y >> 3) * hsize);
		x&=7; y&=7;

		if (PA_DrawSprite[draw_number].n_colors) { // 256 couleurs
			ncase = (ncase << 4) + (x >> 2) + (y << 1);
//			PA_DrawSprite[draw_number].gfxpointer[ncase] = color;
			x &=3; x <<= 3;
			PA_DrawSprite[draw_number].gfxpointer[ncase] &= ~(255 << x);
			PA_DrawSprite[draw_number].gfxpointer[ncase] |= color << x;
		}
		else {
			ncase = (ncase << 3) + (x >> 3) + y;
			x &=3; x <<= 2;
			PA_DrawSprite[draw_number].gfxpointer[ncase] &= ~(15 << x);
			PA_DrawSprite[draw_number].gfxpointer[ncase] |= color << x;
		}
	}

}


void PA_DrawSpriteLine(u8 draw_number, s16 x1, s16 y1, s16 x2, s16 y2, u16 color){
/*if (x1 < 0) x1 = 0;
if (y1 < 0) y1 = 0;
if (x2 < 0) x2 = 0;
if (y2 < 0) y2 = 0;*/

  int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;

  dx=x2-x1;      /* the horizontal distance of the line */
  dy=y2-y1;      /* the vertical distance of the line */
  dxabs = dx;
  sdx = 1;
  if (dx < 0) {
	  dxabs = -dx;
	  sdx = -1;
  }
  dyabs = dy;
  sdy = 1;
  if (dy < 0) {
	  dyabs = -dy;
	  sdy = -1;
  }

  x=dyabs>>1;
  y=dxabs>>1;
  px=x1;
  py=y1;

     PA_DrawSpritePixel(draw_number, px, py, color);

  if (dxabs>=dyabs) {
    for(i=0;i<dxabs;i++)  {
      y+=dyabs;
      if (y>=dxabs)  {
        y-=dxabs;
        py+=sdy;
      }
      px+=sdx;
     PA_DrawSpritePixel(draw_number, px, py, color);
    }
  }
  else {
    for(i=0;i<dyabs;i++) {
      x+=dxabs;
      if (x>=dyabs)  {
        x-=dyabs;
        px+=sdx;
      }
      py+=sdy;
     PA_DrawSpritePixel(draw_number, px, py, color);
    }
  }
}

void PA_InitSpriteDraw(bool screen, u8 sprite, u8 draw_number, u8 drawsize) {
	PA_DrawSprite[draw_number].screen = screen;
	PA_DrawSprite[draw_number].sprite = sprite;
	PA_DrawSprite[draw_number].wasdrawing = 0;
	PA_DrawSprite[draw_number].hsize = PA_GetSpriteLx(screen, sprite);
	PA_DrawSprite[draw_number].vsize = PA_GetSpriteLy(screen, sprite);
	PA_DrawSprite[draw_number].n_colors = PA_GetSpriteColors(screen, sprite);
	PA_DrawSprite[draw_number].drawsize = drawsize;
	PA_DrawSprite[draw_number].gfxpointer = (u32*)(SPRITE_GFX1 + (0x200000 *  screen) + (PA_GetSpriteGfx(screen, sprite) << 5));

//u16 size = PA_DrawSprite[draw_number].hsize * PA_DrawSprite[draw_number].vsize;
//PA_SpriteBuffer[draw_number] = (unsigned char*) malloc (size);
//DMA_Copy(Blank, PA_SpriteBuffer[draw_number], size >> 2, DMA_32NOW);

}



void PA_SpriteDraw(u8 draw_number, s16 x, s16 y, u16 color){
s8 i, j, low, high;

low = -(PA_DrawSprite[draw_number].drawsize >> 1);
high = (PA_DrawSprite[draw_number].drawsize + 1) >> 1;

	if (!PA_DrawSprite[draw_number].wasdrawing) {
		for (i = low; i < high; i++)
			for (j = low; j < high; j++)
				PA_DrawSpritePixel(draw_number, x+i, y+j, color);
	}
	else {
		for (i = low; i < high; i++)
			for (j = low; j < high; j++)
				PA_DrawSpriteLine(draw_number, PA_DrawSprite[draw_number].x+i, PA_DrawSprite[draw_number].y+j, x+i, y+j, color);
	}


PA_DrawSprite[draw_number].wasdrawing = 1;
PA_DrawSprite[draw_number].x = x;
PA_DrawSprite[draw_number].y = y;
//PA_UpdateSpriteGfx(PA_DrawSprite[draw_number].screen, PA_DrawSprite[draw_number].sprite, PA_SpriteBuffer[draw_number]);
}





const s16 PA_SIN[512] = {
	0x0000,0x0003,0x0006,0x0009,0x000D,0x0010,0x0013,0x0016,	0x0019,0x001C,0x001F,0x0022,0x0026,0x0029,0x002C,0x002F,
	0x0032,0x0035,0x0038,0x003B,0x003E,0x0041,0x0044,0x0047,	0x004A,0x004D,0x0050,0x0053,0x0056,0x0059,0x005C,0x005F,
	0x0062,0x0065,0x0068,0x006B,0x006D,0x0070,0x0073,0x0076,	0x0079,0x007B,0x007E,0x0081,0x0084,0x0086,0x0089,0x008C,
	0x008E,0x0091,0x0093,0x0096,0x0098,0x009B,0x009D,0x00A0,	0x00A2,0x00A5,0x00A7,0x00AA,0x00AC,0x00AE,0x00B1,0x00B3,

	0x00B5,0x00B7,0x00B9,0x00BC,0x00BE,0x00C0,0x00C2,0x00C4,	0x00C6,0x00C8,0x00CA,0x00CC,0x00CE,0x00CF,0x00D1,0x00D3,
	0x00D5,0x00D7,0x00D8,0x00DA,0x00DC,0x00DD,0x00DF,0x00E0,	0x00E2,0x00E3,0x00E5,0x00E6,0x00E7,0x00E9,0x00EA,0x00EB,
	0x00ED,0x00EE,0x00EF,0x00F0,0x00F1,0x00F2,0x00F3,0x00F4,	0x00F5,0x00F6,0x00F7,0x00F8,0x00F8,0x00F9,0x00FA,0x00FA,
	0x00FB,0x00FC,0x00FC,0x00FD,0x00FD,0x00FE,0x00FE,0x00FE,	0x00FF,0x00FF,0x00FF,0x0100,0x0100,0x0100,0x0100,0x0100,

	0x0100,0x0100,0x0100,0x0100,0x0100,0x0100,0x00FF,0x00FF,	0x00FF,0x00FE,0x00FE,0x00FE,0x00FD,0x00FD,0x00FC,0x00FC,
	0x00FB,0x00FA,0x00FA,0x00F9,0x00F8,0x00F8,0x00F7,0x00F6,	0x00F5,0x00F4,0x00F3,0x00F2,0x00F1,0x00F0,0x00EF,0x00EE,
	0x00ED,0x00EB,0x00EA,0x00E9,0x00E7,0x00E6,0x00E5,0x00E3,	0x00E2,0x00E0,0x00DF,0x00DD,0x00DC,0x00DA,0x00D8,0x00D7,
	0x00D5,0x00D3,0x00D1,0x00CF,0x00CE,0x00CC,0x00CA,0x00C8,	0x00C6,0x00C4,0x00C2,0x00C0,0x00BE,0x00BC,0x00B9,0x00B7,

	0x00B5,0x00B3,0x00B1,0x00AE,0x00AC,0x00AA,0x00A7,0x00A5,	0x00A2,0x00A0,0x009D,0x009B,0x0098,0x0096,0x0093,0x0091,
	0x008E,0x008C,0x0089,0x0086,0x0084,0x0081,0x007E,0x007B,	0x0079,0x0076,0x0073,0x0070,0x006D,0x006B,0x0068,0x0065,
	0x0062,0x005F,0x005C,0x0059,0x0056,0x0053,0x0050,0x004D,	0x004A,0x0047,0x0044,0x0041,0x003E,0x003B,0x0038,0x0035,
	0x0032,0x002F,0x002C,0x0029,0x0026,0x0022,0x001F,0x001C,	0x0019,0x0016,0x0013,0x0010,0x000D,0x0009,0x0006,0x0003,

	0x0000,0xFFFD,0xFFFA,0xFFF7,0xFFF3,0xFFF0,0xFFED,0xFFEA,	0xFFE7,0xFFE4,0xFFE1,0xFFDE,0xFFDA,0xFFD7,0xFFD4,0xFFD1,
	0xFFCE,0xFFCB,0xFFC8,0xFFC5,0xFFC2,0xFFBF,0xFFBC,0xFFB9,	0xFFB6,0xFFB3,0xFFB0,0xFFAD,0xFFAA,0xFFA7,0xFFA4,0xFFA1,
	0xFF9E,0xFF9B,0xFF98,0xFF95,0xFF93,0xFF90,0xFF8D,0xFF8A,	0xFF87,0xFF85,0xFF82,0xFF7F,0xFF7C,0xFF7A,0xFF77,0xFF74,
	0xFF72,0xFF6F,0xFF6D,0xFF6A,0xFF68,0xFF65,0xFF63,0xFF60,	0xFF5E,0xFF5B,0xFF59,0xFF56,0xFF54,0xFF52,0xFF4F,0xFF4D,

	0xFF4B,0xFF49,0xFF47,0xFF44,0xFF42,0xFF40,0xFF3E,0xFF3C,	0xFF3A,0xFF38,0xFF36,0xFF34,0xFF32,0xFF31,0xFF2F,0xFF2D,
	0xFF2B,0xFF29,0xFF28,0xFF26,0xFF24,0xFF23,0xFF21,0xFF20,	0xFF1E,0xFF1D,0xFF1B,0xFF1A,0xFF19,0xFF17,0xFF16,0xFF15,
	0xFF13,0xFF12,0xFF11,0xFF10,0xFF0F,0xFF0E,0xFF0D,0xFF0C,	0xFF0B,0xFF0A,0xFF09,0xFF08,0xFF08,0xFF07,0xFF06,0xFF06,
	0xFF05,0xFF04,0xFF04,0xFF03,0xFF03,0xFF02,0xFF02,0xFF02,	0xFF01,0xFF01,0xFF01,0xFF00,0xFF00,0xFF00,0xFF00,0xFF00,

	0xFF00,0xFF00,0xFF00,0xFF00,0xFF00,0xFF00,0xFF01,0xFF01,	0xFF01,0xFF02,0xFF02,0xFF02,0xFF03,0xFF03,0xFF04,0xFF04,
	0xFF05,0xFF06,0xFF06,0xFF07,0xFF08,0xFF08,0xFF09,0xFF0A,	0xFF0B,0xFF0C,0xFF0D,0xFF0E,0xFF0F,0xFF10,0xFF11,0xFF12,
	0xFF13,0xFF15,0xFF16,0xFF17,0xFF19,0xFF1A,0xFF1B,0xFF1D,	0xFF1E,0xFF20,0xFF21,0xFF23,0xFF24,0xFF26,0xFF28,0xFF29,
	0xFF2B,0xFF2D,0xFF2F,0xFF31,0xFF32,0xFF34,0xFF36,0xFF38,	0xFF3A,0xFF3C,0xFF3E,0xFF40,0xFF42,0xFF44,0xFF47,0xFF49,

	0xFF4B,0xFF4D,0xFF4F,0xFF52,0xFF54,0xFF56,0xFF59,0xFF5B,	0xFF5E,0xFF60,0xFF63,0xFF65,0xFF68,0xFF6A,0xFF6D,0xFF6F,
	0xFF72,0xFF74,0xFF77,0xFF7A,0xFF7C,0xFF7F,0xFF82,0xFF85,	0xFF87,0xFF8A,0xFF8D,0xFF90,0xFF93,0xFF95,0xFF98,0xFF9B,
	0xFF9E,0xFFA1,0xFFA4,0xFFA7,0xFFAA,0xFFAD,0xFFB0,0xFFB3,	0xFFB6,0xFFB9,0xFFBC,0xFFBF,0xFFC2,0xFFC5,0xFFC8,0xFFCB,
	0xFFCE,0xFFD1,0xFFD4,0xFFD7,0xFFDA,0xFFDE,0xFFE1,0xFFE4,	0xFFE7,0xFFEA,0xFFED,0xFFF0,0xFFF3,0xFFF7,0xFFFA,0xFFFD};




