/* 
//	3D Sprite Moving example...
*/


// Includes
#include <PA9.h>       // Include for PA_Lib

#include "gfx/all_gfx.c"

#define SPC  120 //sprites per color
#define COLORS 7
//
#define RED  0
#define ORANGE  1
#define YELLOW  2
#define GREEN  3
#define BLUE  4
#define INDIGO 5
#define VIOLET 6




u16 gfx[COLORS];

typedef struct{
   s32 x, y, vx, vy;
   u16 angle;
   bool onscreen;
} spriteinfo;
spriteinfo sprites[SPC*COLORS];  // Let's manage the sprite positions ^^


void createFireworks(void);
void updateFireworks(void);


// Function: main()
int main(void)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_Init3D(); // Uses Bg0
	PA_Reset3DSprites();
	
	// Initialise the text system on the top screen
	PA_InitText(1, 1);
	PA_InitText(0, 1);  // Initialise the text system on the top screen	
						
	PA_OutputSimpleText(1, 1, 6, " 3d Sprite Example: Fireworks!");	
	
	// Just use one pallet and load 6 textures off it...
	PA_Load3DSpritePal(0, (void*)fw_Pal);	
	gfx[RED] = PA_3DCreateTex((void*)red_Texture, 8, 8, TEX_256COL);	
	gfx[ORANGE] = PA_3DCreateTex((void*)orange_Texture, 8, 8, TEX_256COL);
	gfx[YELLOW] = PA_3DCreateTex((void*)yellow_Texture, 8, 8, TEX_256COL);
	gfx[GREEN] = PA_3DCreateTex((void*)green_Texture, 8, 8, TEX_256COL);
	gfx[BLUE] = PA_3DCreateTex((void*)blue_Texture, 8, 8, TEX_256COL);
	gfx[INDIGO] = PA_3DCreateTex((void*)indigo_Texture, 8, 8, TEX_256COL);
	gfx[VIOLET] = PA_3DCreateTex((void*)violet_Texture, 8, 8, TEX_256COL);
	
	//create sprites for each color
	s32 i, j;
	for (i = 0; i < COLORS; i++)
	{
		for (j = 0; j < SPC; j++)
		{
		   sprites[j+(i*SPC)].x = -10;//10+15*i+i;
  		   sprites[j+(i*SPC)].y = j*8;
	   	PA_3DCreateSpriteFromTex(j+(i*SPC), gfx[i], 	8, 8, 	0,  sprites[j+(i*SPC)].x 	, sprites[j+(i*SPC)].y);
  		   sprites[j+(i*SPC)].angle = j*(512/SPC); 
			PA_3DSetSpriteRotate(j+(i*SPC), sprites[j+(i*SPC)].angle);
			sprites[j+(i*SPC)].vx = PA_Cos(sprites[j+(i*SPC)].angle)>>6;
			sprites[j+(i*SPC)].vy = -PA_Sin(sprites[j+(i*SPC)].angle)>>6;
			sprites[j+(i*SPC)].onscreen = false;
 
		}     
}		 
	   
	

	int countdown = PA_RandMinMax(10,30);

	while(1) {

		if (countdown == 0 || Stylus.Newpress) 
		{
		   countdown = PA_RandMinMax(10,30);
		   createFireworks();
		   
		}
		else
		{
		   countdown--;
		}		
		
		
		updateFireworks();
		PA_3DProcess();  // Update 3DSprites

		PA_WaitForVBL();
	}


	
	return 0;
} // End of main()


void createFireworks(void)
{
	int color =  PA_RandMinMax(0,COLORS);
	int maxtry = COLORS;
	while (sprites[color*SPC].onscreen && maxtry > 0){ if (color < COLORS) { color++;} else {color = 0;} maxtry--;}
	
	if (maxtry > 0) //don't run if we couldn't find a free group of sprites
	{
		int i, randx, randy;
		randx = PA_RandMinMax(30,230);
		randy = PA_RandMinMax(30,160);
	
		for(i = 0; i < SPC; i++)
		{
		   if (Stylus.Newpress)
			{
			   sprites[i+(color*SPC)].x= Stylus.X-10+PA_RandMax(20);
			   sprites[i+(color*SPC)].y=Stylus.Y-10+PA_RandMax(20);;
			}
			else
			{
			   	sprites[i+(color*SPC)].x=randx-10+PA_RandMax(20);;
			   	sprites[i+(color*SPC)].y=randy-10+PA_RandMax(20);;
			}
			   
			PA_3DSetSpriteXY(i+(color*SPC), sprites[i+(color*SPC)].x, sprites[i+(color*SPC)].y); 
			sprites[i+(color*SPC)].onscreen = true;
		}   
			
	}		
}   

void updateFireworks(void)
{
   int i;
  //PA_3DSetSpriteWidthHeight(i, 8, 8); // Resize to way smaller ^^
	for(i = 0; i < COLORS*SPC; i++)
	{
	   if (sprites[i].onscreen)
	   {
		   sprites[i].x+=sprites[i].vx;
	  	   sprites[i].y+=sprites[i].vy;
	  	   PA_3DSetSpriteXY(i, sprites[i].x, sprites[i].y); 
	  	   if (sprites[i].x > 256+8 || sprites[i].x < -8 || sprites[i].y > 192+8 || sprites[i].y < -8)
			{
			     sprites[i].onscreen = false;
			      PA_3DSetSpriteXY(i, -10, sprites[i].y); 
			   }   
	  	}   
  	}   
}   

