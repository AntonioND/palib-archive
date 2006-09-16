
#ifdef __cplusplus
extern "C" {
#endif

#include <nds.h>
#include <math.h>
#include "arm9/PA_3D.h"
#include "arm9/PA_Keys.h"
#include "palib_bin.h"





int textureID;
float angle3dX = 0.5,angle3dY = 0.0, viseeX = 0.0, viseeY = 0.0, viseeZ = 1.0;



int PA_InitCallList(PA_3DObject *obj);
int PA_VertexCallList(PA_3DObject *obj, float x, float y, float z);
int PA_TexCoordCallList(PA_3DObject *obj, int x, int y);
int PA_EndCallList(PA_3DObject *obj);
int PA_Draw3Dobject(PA_3DObject obj);
int PA_ScaleCallList(PA_3DObject *obj, float x, float y, float z);








int PA_InitCallList(PA_3DObject *obj){
	obj->callList[0]=2;
	obj->callList[1]=FIFO_COMMAND_PACK(FIFO_BEGIN, FIFO_NOP, FIFO_NOP, FIFO_NOP);
	obj->callList[2]=GL_TRIANGLE;
	obj->scalex=1.0;
	obj->scaley=1.0;
	obj->scalez=1.0;
	return 1;
}


int PA_VertexCallList(PA_3DObject *obj, float x, float y, float z){
	obj->callList[obj->callList[0]+1]=FIFO_COMMAND_PACK(FIFO_VERTEX16, FIFO_NOP, FIFO_NOP, FIFO_NOP);
	obj->callList[obj->callList[0]+2]=VERTEX_PACK(floattov16(x),floattov16(y));
	obj->callList[obj->callList[0]+3]=VERTEX_PACK(floattov16(z),0);
	obj->callList[0]+=3;
	if(x>obj->maxx)obj->maxx=x;
	else if(x<obj->minx)obj->minx=x;
	if(y>obj->maxy)obj->maxy=y;
	else if(y<obj->miny)obj->miny=y;
	if(z>obj->maxz)obj->maxz=z;
	else if(z<obj->minz)obj->minz=z;
	return 1;
}


int PA_TexCoordCallList(PA_3DObject *obj, int x, int y){
	obj->callList[obj->callList[0]+1]=FIFO_COMMAND_PACK(FIFO_TEX_COORD, FIFO_NOP, FIFO_NOP, FIFO_NOP);
	obj->callList[obj->callList[0]+2]=TEXTURE_PACK(floattot16(x),floattot16(y)); 
	obj->callList[0]+=2;
	return 1;
}


int PA_EndCallList(PA_3DObject *obj){
	obj->callList[obj->callList[0]+1]=FIFO_COMMAND_PACK(FIFO_END, FIFO_NOP, FIFO_NOP, FIFO_NOP);
	obj->callList[0]++;
	return 1;
}



int PA_ScaleCallList(PA_3DObject *obj, float x, float y, float z){
	obj->scalex = x;
	obj->scaley = y;
	obj->scalez = z;
	return 1;
}



int PA_Draw3Dobject(PA_3DObject obj){
	glPushMatrix(); 
	PA_Scale3D(obj.scalex/(obj.maxx-obj.minx),obj.scaley/(obj.maxy-obj.miny),obj.scalez/(obj.maxy-obj.miny));
	glCallList(obj.callList);
	glPopMatrix(1);
	return 1;
}


void PA_LoadSplash3D(){
	float rotatex = 0.0, rotatey = 0.0, rotatez = 0.0; 
	PA_Init3D();
	PA_LoadTexture(1,(u8*)palib_bin,texBIN); 
	while(1){ 
		//PA_UpdatePad();
		PA_Init3DDrawing(0.0,0.0,0.0,0.0,0.0,1.0);//initialisation de la perspective,... la camera est en position (0.0,0.0,2.0) et regarde vers (0.0,0.0,0.0) 
		PA_Texture(1);
		glPushMatrix();
		rotatex += 2.0;
		rotatey += 2.0;
		rotatez += 2.0;
		PA_3DBoxTexture(0.0,0.0,1.0,0.5,0.5,0.5,rotatex,rotatey,rotatez,255,0,0);
		glPopMatrix(1);
		glFlush();//afficher ce qu'on vient de dessiner 
		if(Pad.Held.Start)break;
		PA_WaitForVBL();
	}
}



void PA_VueKeys(){
	//PA_UpdatePad();
		if(Pad.Held.Up) angle3dY += 0.01;
		if(Pad.Held.Down)angle3dY -= 0.01;
		if(Pad.Held.Left)angle3dX -= 0.01;
		if(Pad.Held.Right)angle3dX += 0.01;
		viseeX = sin(angle3dX);
		viseeZ = cos(angle3dX);
		viseeY = tan(angle3dY);
}




void PA_glulookat(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) 
{ 
   f32 eyex = floattof32(x1), eyey = floattof32(y1), eyez = floattof32(z1), lookAtx = floattof32(x2);
   f32 lookAty = floattof32(y2), lookAtz = floattof32(z2), upx = floattof32(x3), upy = floattof32(y3), upz = floattof32(z3);
   f32 side[3], forward[3], up[3]; 

   forward[0] = lookAtx - eyex; 
   forward[1] = lookAty - eyey; 
   forward[2] = lookAtz - eyez; 

   normalizef32(forward); 

   up[0] = upx; 
   up[1] = upy; 
   up[2] = upz; 

   crossf32(forward, up, side); 
   normalizef32(side); 
   crossf32(side, forward, up); 
    
   glMatrixMode(GL_MODELVIEW); 

   MATRIX_LOAD4x3 =  side[0]; 
   MATRIX_LOAD4x3 =  up[0]; 
   MATRIX_LOAD4x3 = -forward[0]; 

   MATRIX_LOAD4x3 =  side[1]; 
   MATRIX_LOAD4x3 =  up[1]; 
   MATRIX_LOAD4x3 = -forward[1]; 

   MATRIX_LOAD4x3 =  side[2]; 
   MATRIX_LOAD4x3 =  up[2]; 
   MATRIX_LOAD4x3 = -forward[2]; 

   MATRIX_LOAD4x3 = 0; 
   MATRIX_LOAD4x3 = 0; 
   MATRIX_LOAD4x3 = floattof32(-1.0); 

   glTranslate3f32(-eyex, -eyey, -eyez); 
}



void PA_Init3D(void){
	videoSetMode(MODE_0_3D | DISPLAY_SPR_1D_LAYOUT | DISPLAY_SPR_ACTIVE|2<<20);//le mode 3d  PAS TESTE

	glViewPort(0,0,255,191);
	glClearColor(0,0,0);
	glClearDepth(0x7FFF);
	vramSetBankA(VRAM_A_TEXTURE);
	
	PA_InitSpriteExtPal(); // Init's sprite extended palettes
}

void PA_Init3DAndBg(void){
	videoSetMode(MODE_0_3D);
	
	glViewPort(0,0,255,191);
	glClearColor(0,0,0);
	glClearDepth(0x7FFF);
	vramSetBankA(VRAM_A_TEXTURE);
	vramSetBankB(VRAM_B_MAIN_BG_0x6000000);
	PA_InitBgExtPal();
}



void PA_Init3DDrawing(float x1, float y1, float z1, float x2, float y2, float z2){
	glReset();
	gluPerspective(35, 256.0 / 192.0, 0.1, 40);
			glLight(0, RGB15(31,31,31), 0, floattov10(-1.0), 0);
			glLight(1, RGB15(31,31,31), 0, 0, floattov10(-1.0));
			glLight(2, RGB15(31,31,31), 0, 0,	floattov10(1.0));
	PA_glulookat( x1, y1, z1,      //position de la camera
	x2, y2, z2,     //regarder vers
	0.0, 1.0, 0.0);//le haut
	glMatrixMode(GL_TEXTURE);
	glIdentity();
	glMatrixMode(GL_MODELVIEW);
	glMaterialf(GL_AMBIENT, RGB15(16,16,16));
	glMaterialf(GL_DIFFUSE, RGB15(16,16,16));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8,8,8));
	glMaterialf(GL_EMISSION, RGB15(16,16,16));
	glMaterialShinyness();
	//not a real gl function and will likely change
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE  | POLY_FORMAT_LIGHT0| POLY_FORMAT_LIGHT1| POLY_FORMAT_LIGHT2);

}


void PA_3DBox(float x, float y, float z, float longueur, float largeur, float hauteur,float rotatex, float rotatey, float rotatez, uint8 red, uint8 green, uint8 blue){
	glPushMatrix();
	PA_Translate3D(x,y,z);
	PA_Scale3D(longueur,largeur, hauteur);
	PA_Rotate3D(rotatex, rotatey, rotatez);
	glColor3b(red,green,blue);
	glBegin(GL_QUAD);
//1
	glVertex3v16(floattov16(1.0),floattov16(1.0),floattov16(1.0));
	glVertex3v16(floattov16(-1.0),floattov16(1.0),floattov16(1.0));
	glVertex3v16(floattov16(-1.0),floattov16(-1.0),floattov16(1.0));
	glVertex3v16(floattov16(1.0),floattov16(-1.0),floattov16(1.0));
//2
	glVertex3v16(floattov16(1.0),floattov16(1.0),floattov16(-1.0));
	glVertex3v16(floattov16(-1.0),floattov16(1.0),floattov16(-1.0));
	glVertex3v16(floattov16(-1.0),floattov16(-1.0),floattov16(-1.0));
	glVertex3v16(floattov16(1.0),floattov16(-1.0),floattov16(-1.0));
//3
	glVertex3v16(floattov16(-1.0),floattov16(1.0),floattov16(1.0));
	glVertex3v16(floattov16(-1.0),floattov16(1.0),floattov16(-1.0));
	glVertex3v16(floattov16(-1.0),floattov16(-1.0),floattov16(-1.0));
	glVertex3v16(floattov16(-1.0),floattov16(-1.0),floattov16(1.0));
//4
	glVertex3v16(floattov16(1.0),floattov16(1.0),floattov16(1.0));
	glVertex3v16(floattov16(1.0),floattov16(1.0),floattov16(-1.0));
	glVertex3v16(floattov16(1.0),floattov16(-1.0),floattov16(-1.0));
	glVertex3v16(floattov16(1.0),floattov16(-1.0),floattov16(1.0));
//5
	glVertex3v16(floattov16(1.0),floattov16(1.0),floattov16(1.0));
	glVertex3v16(floattov16(1.0),floattov16(1.0),floattov16(-1.0));
	glVertex3v16(floattov16(-1.0),floattov16(1.0),floattov16(-1.0));
	glVertex3v16(floattov16(-1.0),floattov16(1.0),floattov16(1.0));
//6
	glVertex3v16(floattov16(1.0),floattov16(-1.0),floattov16(1.0));
	glVertex3v16(floattov16(1.0),floattov16(-1.0),floattov16(-1.0));
	glVertex3v16(floattov16(-1.0),floattov16(-1.0),floattov16(-1.0));
	glVertex3v16(floattov16(-1.0),floattov16(-1.0),floattov16(1.0));
	glEnd();
	glPopMatrix(1);
}


void PA_3DBoxTexture(float x, float y, float z, float longueur, float largeur, float hauteur,float rotatex, float rotatey, float rotatez, uint8 red, uint8 green, uint8 blue){

	glPushMatrix();
	glTranslate3f32(floattof32(x),floattof32(y),floattof32(z));
	PA_Scale3D(longueur,largeur, hauteur);
	PA_Rotate3D(rotatex, rotatey, rotatez);
	glColor3b(red,green,blue);
	glBegin(GL_QUAD);

//1
	PA_TexCoord(128.0,128.0);
	glVertex3v16(floattov16(1.0),floattov16(1.0),floattov16(1.0));
	
	PA_TexCoord(128.0,0.0);
	glVertex3v16(floattov16(-1.0),floattov16(1.0),floattov16(1.0));
	
	PA_TexCoord(0.0,0.0);
	glVertex3v16(floattov16(-1.0),floattov16(-1.0),floattov16(1.0));
	
	PA_TexCoord(0.0,128.0);
	glVertex3v16(floattov16(1.0),floattov16(-1.0),floattov16(1.0));

//2
	PA_TexCoord(128.0,0.0);
	glVertex3v16(floattov16(1.0),floattov16(1.0),floattov16(-1.0));
	
	PA_TexCoord(128.0,128.0);
	glVertex3v16(floattov16(-1.0),floattov16(1.0),floattov16(-1.0));
	
	PA_TexCoord(0.0,128.0);
	glVertex3v16(floattov16(-1.0),floattov16(-1.0),floattov16(-1.0));
	
	PA_TexCoord(0.0,0.0);
	glVertex3v16(floattov16(1.0),floattov16(-1.0),floattov16(-1.0));

//3
	PA_TexCoord(128.0,128.0);
	glVertex3v16(floattov16(-1.0),floattov16(1.0),floattov16(1.0));
	
	PA_TexCoord(128.0,0.0);
	glVertex3v16(floattov16(-1.0),floattov16(1.0),floattov16(-1.0));
	
	PA_TexCoord(0.0,0.0);
	glVertex3v16(floattov16(-1.0),floattov16(-1.0),floattov16(-1.0));
	
	PA_TexCoord(0.0,128.0);
	glVertex3v16(floattov16(-1.0),floattov16(-1.0),floattov16(1.0));

//4
	PA_TexCoord(128.0,0.0);
	glVertex3v16(floattov16(1.0),floattov16(1.0),floattov16(1.0));
	
	PA_TexCoord(128.0,128.0);
	glVertex3v16(floattov16(1.0),floattov16(1.0),floattov16(-1.0));
	
	PA_TexCoord(0.0,128.0);
	glVertex3v16(floattov16(1.0),floattov16(-1.0),floattov16(-1.0));
	
	PA_TexCoord(0.0,0.0);
	glVertex3v16(floattov16(1.0),floattov16(-1.0),floattov16(1.0));

//5
	PA_TexCoord(0.0,128.0);
	glVertex3v16(floattov16(1.0),floattov16(1.0),floattov16(1.0));
	
	PA_TexCoord(128.0,128.0);
	glVertex3v16(floattov16(1.0),floattov16(1.0),floattov16(-1.0));
	
	PA_TexCoord(128.0,0.0);
	glVertex3v16(floattov16(-1.0),floattov16(1.0),floattov16(-1.0));
	
	PA_TexCoord(0.0,0.0);
	glVertex3v16(floattov16(-1.0),floattov16(1.0),floattov16(1.0));

//6
	PA_TexCoord(128.0,128.0);
	glVertex3v16(floattov16(1.0),floattov16(-1.0),floattov16(1.0));
	
	PA_TexCoord(0.0,128.0);
	glVertex3v16(floattov16(1.0),floattov16(-1.0),floattov16(-1.0));
	
	PA_TexCoord(0.0,0.0);
	glVertex3v16(floattov16(-1.0),floattov16(-1.0),floattov16(-1.0));
	
	PA_TexCoord(128.0,0.0);
	glVertex3v16(floattov16(-1.0),floattov16(-1.0),floattov16(1.0));
	
	
	glEnd();
	glPopMatrix(1);
}


#ifdef __cplusplus
}
#endif


