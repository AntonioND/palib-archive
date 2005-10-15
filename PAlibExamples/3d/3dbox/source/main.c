//copyright delfare <http://delfare.pizz.biz>
#include <PA9.h> 
int main()
{
PA_Init3D();//init the 3d
while(1) 
{ 
PA_Init3DDrawing(0.0,0.0,1.0,//x,y,z
0.0,0.0,0.0);//eyex, eyey, eyez
glPushMatrix(); 
PA_3DBox(0.0,0.0,0.0,//x,y,z
0.5,0.1,1.0,//width, height, lenght 
0.0,0.0,0.0,//rotation x, y, z
255,0,0);//color
glPopMatrix(1);
glFlush();//show
}

return 0;
}
