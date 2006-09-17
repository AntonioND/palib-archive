//copyright delfare <http://delfare.pizz.biz>
#include <PA9.h> 

#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"



int main()
{
	PA_Init();
	PA_InitVBL();
	
	PA_Init3D();//init the 3d
	
	PA_SetBgPalCol(0, 0, PA_RGB(31, 31, 31));
	
	PA_LoadSpritePal(1, 0, (void*)sprite0_Pal);		
	PA_CreateSprite(1, 0, (void*)vaisseau_Sprite,OBJ_SIZE_32X32, 1, 0, 50, 50); 
		
	PA_LoadSpritePal(0, 0, (void*)sprite0_Pal);					
	PA_CreateSprite(0, 0, (void*)vaisseau_Sprite,OBJ_SIZE_32X32, 1, 0, 50, 50);
	
	int rot=0;
	
	while(1) 
	{ 
	  	PA_Init3DDrawing(0.0,0.0,1.0,//x,y,z
		0.0,0.0,0.0);//eyex, eyey, eyez
		
		glPushMatrix(); 
		
		rot++;
		
		PA_3DBox(0.0,0.0,0.0,//x,y,z
		0.3,0.3,0.3,//width, height, lenght 
		rot,rot,rot,//rotation x, y, z
		0,0,255);//color
		
		glPopMatrix(1);
		glFlush();//show	*/
					
		PA_WaitForVBL();
		
	}

return 0;
}


	

