#ifndef _PA_3D
#define _PA_3D
#include <nds.h>
#include <malloc.h>
#include <nds/arm9/image.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const u8 PAlib[];
extern const u32 PAlib_size[];

#define texPCX 1
#define texBIN 2
/*! \file PA_3D.h
    \brief
	\~english 3D functions tuto : http://delfare.pizz.biz/
	\~french les fonctions 3D tuto : http://delfare.pizz.biz/
*/



extern int textureID;
extern float angle3dX, angle3dY;
extern float viseeX, viseeY, viseeZ;

typedef struct{
	u32 *callList;
	float maxx, maxy, maxz, minx, miny, minz;
	float scalex,scaley,scalez;
}PA_3DObject;


/** @defgroup 3D 3D functions
 *  Say thanks to Delfare for all these ! And check out http://delfare.pizz.biz for how to use them...
 *  @{
 */




/*! \fn void PA_Init3D(void)
    \brief
         \~english Initialise the 3d !!! Big thanks to Delfare for all the 3d functions !
         \~french Initialise la 3d !! Un grand merci à Delfare pour toutes les fonctions 3d ! 
*/



void PA_Init3D(void);
/*! \fn void PA_LoadSplash3D(void);
    \brief
        \~english Load a PlayerAdvance splash screen... It's always nice to give some credit ;)
        \~french Affiche un splash screen PlayerAdvance... C'est toujours sympa de remercier ;)
*/
void PA_LoadSplash3D(void);
/*! \fn void PA_Init3DDrawing(float x1, float y1, float z1, float x2, float y2, float z2)
    \brief
         \~english takes care of the perspective,  (x1,y1,z1) is the position of the camera, (x2,y2,z2) is where the camera looks at
         \~french gere la perspective, (x1,y1,z1) est la position de la camera, (x2,y2,z2) est la position regardee par la camera,...
    \param x1
         \~english X position of the camera
         \~french Position X de la caméra
    \param y1
         \~english Y position of the camera
         \~french Position Y de la caméra		 
    \param z1
         \~english Z position of the camera
         \~french Position Z de la caméra			 
    \param x2
         \~english X position to look at
         \~french Position X à regarder
    \param y2
         \~english Y position to look at
         \~french Position Y à regarder 
    \param z2
         \~english Z position to look at
         \~french Position Z à regarder
*/

void PA_Init3DDrawing(float x1, float y1, float z1, float x2, float y2, float z2);




/*! \fn void PA_glulookat(float eyex, float eyey, float eyez, float lookAtx, float lookAty, float lookAtz, float upx, float upy, float upz)
    \brief
         \~english takes care of the camera, (eyex,eyey,eyez) is the position of the camera, (lookAtx,lookAty,lookAtz) is where the camera looks at, (upx,upy,upz) is the coord of the up
         \~french gere la position de la camera : (eyex,eyey,eyez) est la position de la camera, (lookAtx,lookAty,lookAtz) est la position regardee par la camera, (upx,upy,upz) est la position du haut
    \param eyex
         \~english X position of the camera
         \~french Position X de la caméra
    \param eyey
         \~english Y position of the camera
         \~french Position Y de la caméra		 
    \param eyez
         \~english Z position of the camera
         \~french Position Z de la caméra			 
    \param lookAtx
         \~english X position to look at
         \~french Position X à regarder
    \param lookAty
         \~english Y position to look at
         \~french Position Y à regarder 
    \param lookAtz
         \~english Z position to look at
         \~french Position Z à regarder
     \param upx
         \~english X position of the top
         \~french Position X du haut
    \param upy
         \~english Y position of the top
         \~french Position Y du haut
    \param upz
         \~english Z position of the top
         \~french Position Z du haut 
         
         
*/
void PA_glulookat(float eyex, float eyey, float eyez, float lookAtx, float lookAty, float lookAtz, float upx, float upy, float upz);
/*! \fn void PA_VueKeys(void)
    \brief
         \~english takes care of the sight with the keys, (viseeX,viseeY,viseeZ) is where the camera looks at, you must use PA_glulookat with viseeX,viseeY and viseeZ
         \~french gere la vue grace aux fleches, (viseeX,viseeY,viseeZ) est la position a regarder, il vous faut faire un PA_glulookat avec ces valeurs pour positionner la vue 
*/

extern inline void PA_image8to16(sImage* img)
 {
	 int i;

	 u16* temp = (u16*)malloc(img->height*img->width*2);
	 
	 for(i = 0; i < img->height * img->width; i++)
		 temp[i] = img->palette[img->data8[i]] | (1<<15);

	 img->data16 = temp;
 }


void PA_VueKeys(void);
/*! \fn extern inline void PA_LoadTexture(int numtexture, u8* nomtexture, int mode)
    \brief
         \~english load a texture
         \~french charge une texture
    \param numtexture
         \~english number of texture to load...
         \~french numero de la texture à charger...
    \param nomtexture
         \~english Texture to load...
         \~french Texture à charger...
    \param mode
         \~english texBIN or texPCX
         \~french texBIN ou texPCX		 
*/

extern inline void PA_LoadTexture(int numtexture, u8* nomtexture, int mode){
if(mode == texBIN){
glBindTexture(0, numtexture);
glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0,TEXGEN_TEXCOORD, (u8*)nomtexture);
}
else if(mode == texPCX){
sImage pcx;
loadPCX((u8*)nomtexture, &pcx);
PA_image8to16(&pcx);
glBindTexture(0, numtexture);
glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, pcx.data8);
}
}
/*! \fn extern inline void PA_Texture(int numtexture)
    \brief
         \~english use a texture
         \~french utilise une texture
    \param numtexture
         \~english number of texture to use...
         \~french numero de la texture à utiliser...
*/
extern inline void PA_Texture(int numtexture){
glBindTexture(0, numtexture);
}
/*! \fn void PA_3DBox(float x, float y, float z, float longueur, float largeur, float hauteur,float rotatex, float rotatey, float rotatez,uint8 red, uint8 green, uint8 blue)
    \brief
         \~english create a 3D box 
         \~french cree un box en 3d, position : (x,y,z), de taille longueur, largeur et hauteur, et de couleur (red,green,blue)
    \param x
         \~english Box X position
         \~french Position X du cube
    \param y
         \~english Box Y position
         \~french Position Y du cube
    \param z
         \~english Box Z position
         \~french Position Z du cube
    \param longueur
         \~english Box length
         \~french Longueur du cube
    \param largeur
         \~english Box width
         \~french Largeur de la boite
    \param hauteur
         \~english Box height
         \~french Hauteur de la boite
    \param rotatex
         \~english rotate x
         \~french rotation sur l'axe x
    \param rotatey
         \~english rotate y
         \~french rotation sur l'axe y
    \param rotatez
         \~english rotate z
         \~french rotation sur l'axe z
    \param red
         \~english red color(max 31)
         \~french couleur rouge(31 est le maximum) 
    \param green
         \~english green color(max 31)
         \~french couleur verte(31 est le maximum) 
    \param blue
         \~english blue color(max 31)
         \~french couleur bleue(31 est le maximum) 
*/
void PA_3DBox(float x, float y, float z, float longueur, float largeur, float hauteur,float rotatex, float rotatey, float rotatez, uint8 red, uint8 green, uint8 blue);
/*! \fn void PA_3DBoxTexture(float x, float y, float z, float longueur, float largeur, float hauteur,float rotatex, float rotatey, float rotatez,uint8 red, uint8 green, uint8 blue)
    \brief
         \~english Create a 3D Box, but with a nice texture... 
         \~french cree un box en 3d, position : (x,y,z), de taille longueur, largeur et hauteur, et de couleur (red,green,blue) avec coordonnees de texture
    \param x
         \~english Box X position
         \~french Position X du cube
    \param y
         \~english Box Y position
         \~french Position Y du cube
    \param z
         \~english Box Z position
         \~french Position Z du cube
    \param longueur
         \~english Box length
         \~french Longueur du cube
    \param largeur
         \~english Box width
         \~french Largeur de la boite
    \param hauteur
         \~english Box height
         \~french Hauteur de la boite
    \param rotatex
         \~english rotate x
         \~french rotation sur l'axe x
    \param rotatey
         \~english rotate y
         \~french rotation sur l'axe y
    \param rotatez
         \~english rotate z
         \~french rotation sur l'axe z
    \param red
         \~english red color(max 31)
         \~french couleur rouge(31 est le maximum) 
    \param green
         \~english green color(max 31)
         \~french couleur verte(31 est le maximum) 
    \param blue
         \~english blue color(max 31)
         \~french couleur bleue(31 est le maximum) 
*/
void PA_3DBoxTexture(float x, float y, float z, float longueur, float largeur, float hauteur,float rotatex, float rotatey, float rotatez,uint8 red, uint8 green, uint8 blue);



/*! \fn void PA_Rotate3D(float x,float y,float z)
    \brief
         \~english rotate
         \~french faire une rotation 
    \param x
         \~english rotate x
         \~french rotation sur l'axe x
    \param y
         \~english rotate y
         \~french rotation sur l'axe y
    \param z
         \~english rotate z
         \~french rotation sur l'axe z
*/
extern inline void PA_Rotate3D(float x,float y,float z){
	glRotateX(x);
    glRotateY(y);
	glRotateZ(z);
}
/*! \fn void PA_Translate3D(float x,float y,float z)
    \brief
         \~english translate
         \~french faire une translation 
    \param x
         \~english translate x
         \~french translation sur l'axe x
    \param y
         \~english translate y
         \~french translation sur l'axe y
    \param z
         \~english translate z
         \~french translation sur l'axe z
*/
extern inline void PA_Translate3D(float x, float y, float z){
glTranslate3f32(floattof32(x),floattof32(y),floattof32(z));
}
/*! \fn void PA_Scale3D(float x,float y,float z)
    \brief
         \~english scale
         \~french changer la taille d'un objet 
    \param x
         \~english scale x
         \~french redimentionnement sur l'axe x
    \param y
         \~english scale y
         \~french redimentionnement sur l'axe y
    \param z
         \~english scale z
         \~french redimentionnement sur l'axe z
*/
extern inline void PA_Scale3D(float x, float y, float z){
	if(x==0.0)x=1.0;
	if(y==0.0)y=1.0;
	if(z==0.0)z=1.0;
	MATRIX_SCALE = floattof32(x);
	MATRIX_SCALE = floattof32(y);
	MATRIX_SCALE = floattof32(z);
}
/*! \fn extern inline void PA_Vertex3D(float x,float y,float z)
    \brief
         \~english create a vertex without floattov16
         \~french creer un vertex sans floattov16
    \param x
         \~english  X position
         \~french Position X 
    \param y
         \~english  Y position
         \~french Position Y
    \param z
         \~english Z position
         \~french Position Z
*/
extern inline void PA_Vertex3D(float x, float y, float z){
glVertex3v16(floattov16(x),floattov16(y),floattov16(z));
}
/*! \fn extern inline void PA_3Dto2D(void)
    \brief
         \~english you must use it for use PA_Vertex2D 
         \~french vous devez utiliser ceci pour utiliser PA_Vertex2D
*/
extern inline void PA_3Dto2D(){
MATRIX_IDENTITY = 0;
}


/*! \fn extern inline void PA_Vertex2D(int x, int y)
    \brief
         \~english create a vertex in 2D
         \~french creer un vertex 2D(il faut avoir fait PA_3Dto2D() avant pour que ca marche correctement
    \param x
         \~english  X position
         \~french Position X 
    \param y
         \~english  Y position
         \~french Position Y
*/
extern inline void PA_Vertex2D(int x, int y){
	float x1 = (float)((x*0.36)/256)-0.18;
	float y1 = (float)((y*0.28)/192)-0.14;
	glVertex3v16(floattov16(x1),floattov16(y1),floattov16(-0.2));
}
/*! \fn void PA_TexCoord(float x,float y)
    \brief
         \~english use the tex coord
         \~french utilise les coordonnees de texture en reglant un probleme de ndslib
    \param x
         \~english  X position
         \~french Position X 
    \param y
         \~english  Y position
         \~french Position Y
*/
extern inline void PA_TexCoord(float x, float y){
glTexCoord1i(TEXTURE_PACK(inttot16((int)x),inttot16((int)y)));
}







int PA_InitCallList(PA_3DObject *obj);
int PA_VertexCallList(PA_3DObject *obj, float x, float y, float z);
int PA_TexCoordCallList(PA_3DObject *obj, int x, int y);
int PA_EndCallList(PA_3DObject *obj);
int PA_Draw3Dobject(PA_3DObject obj);
int PA_ScaleCallList(PA_3DObject *obj, float x, float y, float z);
/** @} */ // end of 3D

#ifdef __cplusplus
}
#endif

#endif
