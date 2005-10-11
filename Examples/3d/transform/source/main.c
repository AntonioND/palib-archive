//copyright delfare <http://delfare.pizz.biz>
#include <PA9.h> 
int main()
{ 
PA_Init3D();//initialisation du mode, textures,...
while(1) 
{ 
PA_Init3DDrawing(0.0,0.0,2.0,0.0,0.0,0.0);//initialisation de la perspective,... la camera est en position (0.0,0.0,2.0) et regarde vers (0.0,0.0,0.0)	
glPushMatrix();
glPushMatrix();
PA_Scale3D(2.0,4.0,2.0);
PA_Translate3D(1.0,0.0,0.0);
PA_Rotate3D(10.0,0.0,0.0); 
glBegin(GL_QUAD);
glColor3b(255,0,0);
PA_Vertex3D(-1.0,0.0,0.0);
PA_Vertex3D(1.0,0.0,0.0);
PA_Vertex3D(1.0,1.0,0.0);
PA_Vertex3D(-1.0,1.0,0.0);
glEnd();
glPopMatrix(1); 
glPopMatrix(1);
glFlush();//afficher ce qu'on vient de dessiner
}

return 0;
}//end main 
