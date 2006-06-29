//http://delfare.pizz.biz
#include <PA9.h> 

int main(){

	PA_Init3D();
	
	PA_3DObject object3D;
	
	object3D.callList = (u32*)malloc(sizeof(u32*)*255);
	
	PA_InitCallList(&object3D);
	PA_VertexCallList(&object3D, -1.0,0.0,0.0);
	PA_VertexCallList(&object3D,1.0,0.0,0.0);
	PA_VertexCallList(&object3D,1.0,1.0,0.0);
	
	PA_EndCallList(&object3D);
	
	while(1){
		PA_Init3DDrawing(0.0,0.0,1.0,0.0,0.0,0.0);
		glPushMatrix(); 
		glColor3b(255,0,0);
		PA_Draw3Dobject(object3D);
		glPopMatrix(1);
		glFlush();
	}
	return 1;
}

