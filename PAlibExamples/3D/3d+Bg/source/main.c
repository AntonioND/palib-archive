//copyright delfare <http://delfare.pizz.biz>
#include <PA9.h> 

#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"



int main()
{
	PA_Init();
	PA_InitVBL();

	PA_Init3DAndBg();//init the 3d

	PA_EasyBgLoad(0,3, Large);  // Warning : bg number 0 NOT available in 3D mode...
	PA_EasyBgLoad(1, 3, Large);
	
	s32 scrollx = 0; 
	s32 scrolly = 0;

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
		
		scrollx += (Pad.Held.Right - Pad.Held.Left)*4; // scroll 4 pixels at a time
		scrolly += (Pad.Held.Down - Pad.Held.Up)*4; // Same thing
	
		PA_EasyBgScrollXY(0, 3, scrollx, scrolly); 
		PA_EasyBgScrollXY(1, 3, scrollx, scrolly); 		
					
		PA_WaitForVBL();
		
	}
	
	return 0;
}


	

