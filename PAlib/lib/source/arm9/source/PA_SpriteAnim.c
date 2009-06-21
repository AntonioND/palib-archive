
#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"

void PA_UpdateSpriteAnims(void);

extern fp VBLSpriteAnims;


void PA_StartSpriteAnimEx(u8 screen, u8 sprite, s16 firstframe, s16 lastframe, s16 speed, u8 type, s16 ncycles)
{
	// Activate animations
	VBLSpriteAnims = PA_UpdateSpriteAnims;
	
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
	
	if (!spriteanims[screen][sprite].play){ // Counts number of sprites playing...	
		
		nspriteanims += 1;
	}
		
	PA_SetSpriteAnimEx(screen, sprite, spriteanims[screen][sprite].lx, spriteanims[screen][sprite].ly, spriteanims[screen][sprite].colors, firstframe);
	spriteanims[screen][sprite].play = 1;	// playing...

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
							spriteanims[screen][currentsprite].currentframe+=spriteanims[screen][currentsprite].framechange;
							//spriteanims[screen][currentsprite].framechange; // stop on the correct animation
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




#ifdef __cplusplus
}
#endif






