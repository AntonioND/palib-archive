
#ifdef __cplusplus
extern "C" {
#endif


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


s16 nspriteanims = 0; // Number of animated sprites currently...
spriteanim spriteanims[2][128]; // Init the array on PAlib init...


const PA_sizes PA_size[3][4] = {{{8, 8}, {16, 16}, {32, 32}, {64, 64}}, {{16, 8}, {32, 8}, {32, 16}, {64, 32}}, {{8, 16}, {8, 32}, {16, 32}, {32, 64}}};


// Utilisé pour déterminer la taille en mémoire en fonction de la taille du sprite
const u16 PA_obj_sizes[4][3] = {
   {64, 128, 128},
   {256, 256, 256},
   {1024, 512, 512},
   {4096, 2048, 2048}
};

const u16 PA_16bit_sizes[4][3] = {
   {64*16, 128*8, 128*16},
   {256*8, 256*4, 256*16},
   {1024*4, 512*4, 512*8},
   {4096*2, 2048*2, 2048*4}
};

obj_inf PA_obj[2][128];  // Les 128 premiers pour l'écran du haut, et encore 128 pour l'écran du bas...


//PA_DrawSprites PA_DrawSprite[MAX_DRAW];  // Infos pour dessiner sur les sprites

//unsigned char *PA_SpriteBuffer[MAX_DRAW]; // Pointeurs vers les buffers  que l'on créera à la volée


u16 *PA_SpriteAnimP[2][1024]; // Pointer towards the sprite's graphics, allows animation




u8 PA_SpriteExtPrio = 0;
u8 PA_SpritePrio[2][128]; // Set the sprite priorities...


	s8 PA_next[2][128];
	s8 PA_first[2][256]; // Sprite at given priority...


void PA_InitSpriteExtPrio(u8 SpritePrio){
	PA_SpriteExtPrio = SpritePrio;
	u8 i, n;
	if (SpritePrio == 1){
		for (i = 0; i < 2; i++) {
			for(n = 0; n < 128; n++) {
				PA_SpritePrio[i][n] = 0;
			}
		}
	}
}


void PA_ResetSpriteSysScreen(u8 screen) {
	u8 n;
	
	   n_free_mem[screen] = 1;
	   free_mem[screen][0].mem_block = 0; free_mem[screen][0].free = 1024;
		
	   DMA_Copy((void*)Blank, (void*)used_mem[screen], 256, DMA_32NOW);
	   DMA_Copy((void*)Blank, (void*)obj_per_gfx[screen], 256, DMA_32NOW);
	
		for(n = 0; n < 128; n++) {
			PA_obj[screen][n].atr0 = 192;
			PA_obj[screen][n].atr1 = 256;
			PA_obj[screen][n].atr2 = 0;
			PA_obj[screen][n].atr3 = 0;
		}
		for(n = 0; n < 32; n++) {
			PA_SetRotset(screen,n,0,256,256);  // Pas de zoom ou de rotation par défaut
		}
	
	FirstGfx[screen] = 0;
	
	nspriteanims = 0; // No animations...
	for(n = 0; n < 128; n++) {
		nspriteanims -= spriteanims[screen][n].play; // remove sprites from sprite to animate list
		spriteanims[screen][n].play = 0;
	}
	
	
	if (screen == 0) PA_MoveSpriteType = 0; 

}




void PA_ResetSpriteSys(void) {

	PA_ResetSpriteSysScreen(0);
	PA_ResetSpriteSysScreen(1);
	
	nspriteanims = 0; // No animations...
	PA_InitSpriteExtPrio(0);// normal priority system by default
}




/*
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
	for(n = 0; n < 32; n++) {
		PA_SetRotset(i,n,0,256,256);  // Pas de zoom ou de rotation par défaut
	}
}
FirstGfx[0] = 0;
FirstGfx[1] = 0;

nspriteanims = 0; // No animations...
for (i = 0; i < 2; i++) {
	for(n = 0; n < 128; n++) {
		spriteanims[i][n].play = 0;
	}
}

//nspriteanims = 0; // No animations...

PA_MoveSpriteType = 0; 

PA_InitSpriteExtPrio(0);// normal priority system by default

}

*/


u16 PA_CreateGfx(u8 screen, void* obj_data, u8 obj_shape, u8 obj_size, u8 color_mode) {
u16 mem_size = PA_obj_sizes[obj_size][obj_shape] >> (8 - color_mode);
if (color_mode == 2) mem_size = PA_16bit_sizes[obj_size][obj_shape] >> 6;

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
   
   PA_SpriteAnimP[screen][truenumber] = (u16*)obj_data; // mémorise la source de l'image...
   return truenumber;
}


void PA_DeleteSprite(u8 screen, u8 obj_number){
u16 obj_gfx = PA_GetSpriteGfx(screen, obj_number);

// Si aucun objet n'utilise cet emplacement, on libère la mémoire... Sinon on signale qu'un de moins s'en sert
	if (obj_per_gfx[screen][obj_gfx] > 0) {
		--obj_per_gfx[screen][obj_gfx];
		if (obj_per_gfx[screen][obj_gfx] == 0) PA_DeleteGfx(screen, obj_gfx);
	}

	PA_obj[screen][obj_number].atr0 = 192;
	PA_obj[screen][obj_number].atr1 = 256;
	PA_obj[screen][obj_number].atr2 = 0;
}



void PA_DeleteGfx(u8 screen, u16 obj_gfx) {
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
	u8 screen; // Ecran
	u8 sprite;		// Numéro du sprite
	u8 wasdrawing; // Si on dessinait dessus ou pas avant
	u8 x, y;  // Position x et y d'avant
	u8 hsize, vsize; // Tailles
	u8 n_colors; // Nombre de couleurs
	u8 drawsize; // Taille du trait
} PA_DrawSprites;
*/

/*
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

}*/

/*
void PA_DrawSpriteLine(u8 draw_number, s16 x1, s16 y1, s16 x2, s16 y2, u16 color){

  int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;

  dx=x2-x1;   
  dy=y2-y1;   
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
*/

void PA_InitSpriteDraw(u8 screen, u8 sprite) {
	spriteanims[screen][sprite].lx = PA_GetSpriteLx(screen, sprite);
	spriteanims[screen][sprite].ly = PA_GetSpriteLy(screen, sprite);
	spriteanims[screen][sprite].colors = PA_GetSpriteColors(screen, sprite);
	spriteanims[screen][sprite].gfx = (u16*)(SPRITE_GFX1 + (0x200000 *  screen) + (PA_GetSpriteGfx(screen, sprite) << 7));
}


/*
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
}*/








void PA_StartSpriteAnimEx(u8 screen, u8 sprite, s16 firstframe, s16 lastframe, s16 speed, u8 type, s16 ncycles)
{
	spriteanims[screen][sprite].lx = PA_GetSpriteLx(screen, sprite);
	spriteanims[screen][sprite].ly = PA_GetSpriteLy(screen, sprite);
	spriteanims[screen][sprite].colors = PA_GetSpriteColors(screen, sprite);	
	spriteanims[screen][sprite].currentframe = spriteanims[screen][sprite].firstframe = firstframe;
	spriteanims[screen][sprite].lastframe = lastframe;
	spriteanims[screen][sprite].speed = speed;	
	spriteanims[screen][sprite].time = 0;
	spriteanims[screen][sprite].type = type;
	spriteanims[screen][sprite].ncycles = ncycles;
	spriteanims[screen][sprite].framechange = 1; // normal change to start
	if (!spriteanims[screen][sprite].play){ // If wasn't playing, say to play and display the first image
		PA_SetSpriteAnimEx(screen, sprite, spriteanims[screen][sprite].lx, spriteanims[screen][sprite].ly, spriteanims[screen][sprite].colors, spriteanims[screen][sprite].currentframe);
		spriteanims[screen][sprite].play = 1;	// playing...
		nspriteanims += 1;
	}
	
	//PA_OutputText(1, 0, nanim+12, "%d : %d, %d   ", sprite, PA_GetSpriteX(0, sprite), PA_GetSpriteY(0, sprite));
	//nanim++;
}




void PA_UpdateSpriteAnims(void)
{
u16 anims = nspriteanims;
u8 currentsprite = 0;
u8 screen = 0;
//	PA_SetSpriteAnimEx(screen, sprite, , PA_GetSpriteLy(screen, sprite), //PA_GetSpriteColors(screen, sprite), animframe);

//nanim = 0;
/*
u8 i;
for (i = 0; i < 128; i++) {
	if(PA_SpriteTouched(i))
		PA_OutputText(1, 0, 23, "%d : %d, %d, gfx : %d   ", i, PA_GetSpriteX(0, i), PA_GetSpriteY(0, i), PA_GetSpriteGfx(0, i));
}*/

while((anims > 0) && (currentsprite < 128))
{
for (screen = 0; screen < 2; screen++){
	if (spriteanims[screen][currentsprite].play)
	{
		
		//++nanim;
		spriteanims[screen][currentsprite].time += spriteanims[screen][currentsprite].speed;
		if (spriteanims[screen][currentsprite].time >= 60) 
		{
			while (spriteanims[screen][currentsprite].time >= 60)
			{
				spriteanims[screen][currentsprite].time -= 60;
				spriteanims[screen][currentsprite].currentframe+=spriteanims[screen][currentsprite].framechange;
				if (((spriteanims[screen][currentsprite].framechange > 0) && (spriteanims[screen][currentsprite].currentframe > spriteanims[screen][currentsprite].lastframe))||((spriteanims[screen][currentsprite].framechange < 0) && (spriteanims[screen][currentsprite].currentframe < spriteanims[screen][currentsprite].firstframe))){
					if (spriteanims[screen][currentsprite].type == ANIM_LOOP){ // Loop
						spriteanims[screen][currentsprite].currentframe = spriteanims[screen][currentsprite].firstframe;	
					}
					else { // Don't loop, go back -> switch speed and first/last frames
						spriteanims[screen][currentsprite].framechange = -spriteanims[screen][currentsprite].framechange;
						if (spriteanims[screen][currentsprite].ncycles == 1)// It was the last one
							spriteanims[screen][currentsprite].currentframe+=spriteanims[screen][currentsprite].framechange; // stop on the correct animation
						else spriteanims[screen][currentsprite].currentframe+=spriteanims[screen][currentsprite].framechange<<1; // continue going back and forth
					}	

					// In all cases :
					spriteanims[screen][currentsprite].ncycles--; // 1 less to do
					if (spriteanims[screen][currentsprite].ncycles == 0) {
						PA_StopSpriteAnim(screen, currentsprite);
					}
					else if (spriteanims[screen][currentsprite].ncycles<0) spriteanims[screen][currentsprite].ncycles = -1; // Inifinite
				}
			}
			//PA_OutputText(1, 0, nanim+18, "%d : %d, %d %d  ", currentsprite, PA_GetSpriteX(0, currentsprite), PA_GetSpriteY(0, currentsprite), PA_GetSpriteGfx(0, currentsprite));
			PA_SetSpriteAnimEx(screen, currentsprite, spriteanims[screen][currentsprite].lx, spriteanims[screen][currentsprite].ly, spriteanims[screen][currentsprite].colors, spriteanims[screen][currentsprite].currentframe);
			
		}
		anims--; // Une de faite !
	}
}
currentsprite++; // next sprite...
}

}



void PA_UpdateOAM(void){
// Update OAM
//PA_UpdateOAM0();
//PA_UpdateOAM1();
s16 i;
s32 value = 0;
s32 value2 = 512;
	for (i = 0; i < 128; i++){ // copy
		OAM[value] = PA_obj[0][i].atr0;
		OAM[value + 1] = PA_obj[0][i].atr1;
		OAM[value + 2] = PA_obj[0][i].atr2;
		OAM[value + 3] = PA_obj[0][i].atr3;	
		OAM[value2] = PA_obj[1][i].atr0;
		OAM[value2 + 1] = PA_obj[1][i].atr1;
		OAM[value2 + 2] = PA_obj[1][i].atr2;
		OAM[value2 + 3] = PA_obj[1][i].atr3;		
		value += 4;
		value2 += 4;		
	}
	

if (PA_SpriteExtPrio){ // Use the extended priorities

	
	value = 0;
	s8 sprite;

	for (i = 0; i < 256; i+=2) {
		PA_first[0][i] = PA_first[1][i] = -1;
		PA_first[0][i+1] = PA_first[1][i+1] = -1;
	}
//	for (i = 0; i < 128; i++) next[0][i] = next[1][i] = -1;	

	for (i = 127; i > -1; i--){ // sort
		PA_next[0][i] = PA_first[0][PA_SpritePrio[0][i]];
		PA_first[0][PA_SpritePrio[0][i]] = i;
		PA_next[1][i] = PA_first[1][PA_SpritePrio[1][i]];
		PA_first[1][PA_SpritePrio[1][i]] = i;		
	}
	
	u8 screen;
	for (screen = 0; screen < 2; screen++){
		//s16 temp = 0;
		//value = screen << 9; // 512 start for the top screen
		for (i = 0; i < 256; i++){ // copy
			sprite = PA_first[screen][i];
			while(sprite != -1){
				//PA_OutputText(screen, temp, 0, "%04d", sprite);
				//temp += 4;
				//PA_OutputText(1, value, 0, "%d   ", sprite);
				OAM[value] = PA_obj[screen][sprite].atr0;
				OAM[value + 1] = PA_obj[screen][sprite].atr1;
				OAM[value + 2] = PA_obj[screen][sprite].atr2;
				value += 4;
				sprite = PA_next[screen][sprite];
			}
		}
	}
}

}


#ifdef __cplusplus
}
#endif






