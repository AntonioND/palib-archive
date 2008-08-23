
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
/*
const u16 PA_16bit_sizes[4][3] = {
   {64*16, 128*8, 128*16},
   {256*8, 256*4, 256*16},
   {1024*4, 512*4, 512*8},
   {4096*2, 2048*2, 2048*4}
};*/

obj_inf PA_obj[2][128] __attribute__ ((aligned (4)));  // Les 128 premiers pour l'écran du haut, et encore 128 pour l'écran du bas...


s16 DualSpriteX[128]; // memorize 128 pixel positions for dual sprites


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
	s16 i;
	
   n_free_mem[screen] = 1;
   free_mem[screen][0].mem_block = 0; free_mem[screen][0].free = 1024;
   
   for(i = 0; i < 1024; i++){
		used_mem[screen][i] = 0;
		obj_per_gfx[screen][i] = 0;
   }

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
	
//	nspriteanims = 0; // No animations...
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
	//if (color_mode == 2) mem_size = PA_16bit_sizes[obj_size][obj_shape] >> 6;
	
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
	if(exit == 0){
		PA_ResetBgSysScreen(1);
		PA_InitText(1, 0);
		PA_OutputSimpleText(1, 0, 8, "Sorry, there just seems to not be enough place to put all your sprites !! Load less sprites, or use PA_CloneSprite or PA_CreateGfx and PA_CreateSpriteFromGfx to optimise the space used. You can also use 16color sprites...");
		PA_OutputSimpleText(1, 0, 16, "Je suis désolé mon vieux, mais là je n'ai plus assez de place pour charger les sprites en mémoire ! Utilises moins de fonds, ou alors sers-toi de PA_CloneSprite ou PA_CreateGfx et PA_CreateSpriteFromGfx pour optimiser la place nécessaire... Tu peux aussi utiliser des sprites en 16 couleurs...");
		
		PA_WaitFor(Stylus.Newpress);
	
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
	  --n_free_mem[screen];
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




void PA_InitSpriteDraw(u8 screen, u8 sprite) {
	spriteanims[screen][sprite].lx = PA_GetSpriteLx(screen, sprite);
	spriteanims[screen][sprite].ly = PA_GetSpriteLy(screen, sprite);
	spriteanims[screen][sprite].colors = PA_GetSpriteColors(screen, sprite);
	spriteanims[screen][sprite].gfx = (u16*)(SPRITE_GFX1 + (0x200000 *  screen) + (PA_GetSpriteGfx(screen, sprite) << 7));
}







void PA_UpdateOAM(void){
// Update OAM
//PA_UpdateOAM0();
//PA_UpdateOAM1();
s16 i;
s32 value = 0;
s32 value2 = 512;
if (!PA_SpriteExtPrio){
	DMA_CopyEx(0, PA_obj, OAM, 512, DMA_32NOW); // DMA0 Copy 
	//dmaCopy(PA_obj, OAM, 128 * sizeof(SpriteEntry));
/*
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
	}*/
}
else{ // Use the extended priorities
	value += 3;
	value2 += 3;
	for (i = 0; i < 128; i++){ // copy rotsets
		OAM[value] = PA_obj[0][i].atr3;	
		OAM[value2] = PA_obj[1][i].atr3;		
		value += 4;
		value2 += 4;		
	}
	
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






extern inline void PA_UpdateMoveSprite(void) {
	PA_MovedSprite.Time++;
	if ((PA_MovedSprite.Time > 2) || Stylus.Released) {
		PA_MovedSprite.Moving = 0;
		PA_MovedSprite.Time = 0;
	}
}

fp VBLSpriteAnims = PA_Nothing;

void PA_UpdateSpriteVBL(void){
	PA_UpdateMoveSprite(); // Met à jour les infos sur les déplacements de sprites
	PA_UpdateOAM(); // Updates the Object on screen
	VBLSpriteAnims();
	//PA_UpdateSpriteAnims(); // Update the sprite animations... done last because less important...
}


#ifdef __cplusplus
}
#endif






