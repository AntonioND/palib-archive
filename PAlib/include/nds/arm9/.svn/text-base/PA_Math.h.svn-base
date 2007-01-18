#ifndef _PA_Math
#define _PA_Math

#ifdef __cplusplus
extern "C" {
#endif

#include "PA9.h"

#include "PA_Interrupt.h"



typedef struct{
	u8 Screen;
	float Time;
	s8 Functions;
	s32 Runs[16];
}PA_TestInfos;
extern PA_TestInfos PA_TestInfo;

extern inline void PA_TestInit(u8 screen, float time){
	PA_TestInfo.Screen = screen;
	PA_TestInfo.Time = time*60; // Convert seconds to VBLs
	PA_TestInfo.Functions = 0; // Functions tested
}

#define PA_TESTSTART  { PA_OutputText(PA_TestInfo.Screen, 0, 4+PA_TestInfo.Functions, "Testing function %02d", PA_TestInfo.Functions+1);\
				PA_WaitForVBL();\
				PA_TestVBLs = 0;\
				while( PA_TestVBLs < PA_TestInfo.Time){


#define PA_TESTEND  PA_TestInfo.Runs[PA_TestInfo.Functions] ++; }\
					PA_TestInfo.Functions++;} // 1 more function tested
								

void PA_TestResults(void);













/** @defgroup Math Some math functions...
 *  Adjust angles, get random values...
 *  @{
 */

/*! \def PA_Cos(angle)
    \brief
         \~english Returns the Cos value for an angle. The value goes from -256 to 256... Watch out though : the angle is not in 360 degrees, but in 512 !
         \~french Renvoie la valeur Cosinus d'un angle. Cette valeur est comprise entre -256 et 256... Attention : l'angle n'est pas en 360 degrés, mais en 512 !
*/
#define PA_Cos(angle) PA_SIN[((angle) + 128)&511]


/*! \def PA_Sin(angle)
    \brief
         \~english Returns the Sin value for an angle. The value goes from -256 to 256... Watch out though : the angle is not in 360 degrees, but in 256 !
         \~french Renvoie la valeur Sinus d'un angle. Cette valeur est comprise entre -256 et 256... Attention : l'angle n'est pas en 360 degrés, mais en 256 !
*/
#define PA_Sin(angle) PA_SIN[((angle))&511]




extern u16 RandomValue;  // Seed pour la fonction random

/*! \fn extern inline u32 PA_Rand(void)
    \brief
         \~english Gives a random number, taken from Ham... This is taken from Ham, I have no credit. 
         \~french Donne un numéro aléatoire... Ceci est pris de Ham, je n'ai aucun mérite. 
*/
extern inline u32 PA_Rand(void) {
int i;
   RandomValue+=0x9248;

   for(i=0;i<3;i++)   {
       if(RandomValue&0x1) {
           RandomValue>>=1;
           RandomValue|=0x8000;
       }
       else  RandomValue>>=1;
   }
   return(RandomValue);
}


/*! \fn extern inline void PA_InitRand(void)
    \brief
         \~english Auto-seeds the Rand function based on the clock !
         \~french Initialisation automatique du rand, basé sur l'horloge interne...
*/
// Les fonctions randoms sont prises de Ham, je n'ai strictement AUCUN mérite ! Désolé ! Je l'ai un peu modifiée, elle doit etre plus rapide
extern inline void PA_InitRand(void) {
   RandomValue = PA_RTC.Minutes*60 + PA_RTC.Seconds + PA_RTC.Hour*3600 + PA_RTC.Day*3600*24+PA_RTC.Month*3600*24*30;
   PA_Rand(); PA_Rand(); PA_Rand(); // On fait plusieurs fois de base
}

/*! \fn void PA_SRand(s32 r)
    \brief
         \~english Set the random's seed. This is taken from Ham, I have no credit. I just made it a little shorter/faster (maybe)
         \~french Initialiser le random avec un seed. Ceci est pris de Ham, je n'ai aucun mérite. J'ai juste raccourci/accéléré un peu le tout
    \param r
         \~english Seed value
         \~french Seed...

*/
// Les fonctions randoms sont prises de Ham, je n'ai strictement AUCUN mérite ! Désolé ! Je l'ai un peu modifiée, elle doit etre plus rapide
extern inline void PA_SRand(s32 r) {
   RandomValue = r;
}





/*! \fn extern inline u32 PA_RandMax(u32 max)
    \brief
         \~english Gives a random number, between 0 and the given number (included)
         \~french Donne un numéro aléatoire... Entre 0 et le nombre donné (inclus)
    \param max
         \~english Maximum included value
         \~french Valeur maximale incluse
*/
extern inline u32 PA_RandMax(u32 max)
{
    return PA_Rand()%(max + 1);
}



/*! \fn extern inline u32 PA_RandMinMax(u32 min,u32 max)
    \brief
         \~english Gives a random number, between the 2 given numbers (included)
         \~french Donne un numéro aléatoire... Entre les 2 nombres donnés (inclus)
   \param min
         \~english Minimum included value
         \~french Valeur minimale incluse 
   \param max
         \~english Maximum included value
         \~french Valeur maximale incluse
*/
extern inline u32 PA_RandMinMax(u32 min,u32 max)
{
    return ((PA_Rand()%((max + 1)-min)) + min);
} 


/*! \fn extern inline u32 PA_Distance(s32 x1, s32 y1, s32 x2, s32 y2)
    \brief
         \~english Calculate the distance (squared) between 2 points
         \~french Calculer la distance (au carré) entre 2 points
    \param x1
         \~english X coordinate of the fist point
         \~french Coordonnée X du premier point
    \param y1
         \~english Y coordinate of the first point
         \~french Coordonnée Y du premier point
    \param x2
         \~english X coordinate of the second point
         \~french Coordonnée X du deuxième point
    \param y2
         \~english Y coordinate of the second point
         \~french Coordonnée Y du deuxième point
*/
extern inline u64 PA_Distance(s32 x1, s32 y1, s32 x2, s32 y2) {
   s64 h = x1 - x2;
   s64 v = y1 - y2;
   return(h*h + v*v);
}

/*! \fn extern inline u32 PA_TrueDistance(s32 x1, s32 y1, s32 x2, s32 y2)
    \brief
         \~english Calculate the real distance between 2 points. Much slower than PA_Distance
         \~french Calculer la vraie distance entre 2 points. Beaucoup plus lent que PA_Distance
    \param x1
         \~english X coordinate of the fist point
         \~french Coordonnée X du premier point
    \param y1
         \~english Y coordinate of the first point
         \~french Coordonnée Y du premier point
    \param x2
         \~english X coordinate of the second point
         \~french Coordonnée X du deuxième point
    \param y2
         \~english Y coordinate of the second point
         \~french Coordonnée Y du deuxième point
*/
extern inline u64 PA_TrueDistance(s32 x1, s32 y1, s32 x2, s32 y2) {
   s64 h = x1 - x2;
   s64 v = y1 - y2;
   return(swiSqrt(h*h + v*v));
}



/*! \fn u16 PA_AdjustAngle(u16 angle, s16 anglerot, s32 startx, s32 starty, s32 targetx, s32 targety)
    \brief
         \~english Adjust an angle, for example to calculate in which direction an object shoudl turn
         \~french Ajuster un angle, par exemple pour calculer la direction à prendre par un vaisseau
     \param angle
         \~english Base angle, from 0 to 511
         \~french Angle de base, de 0 à 511
    \param anglerot
         \~english For how much to turn...
         \~french De combien tourner...
    \param startx
         \~english Initial X position
         \~french Coordonnée X de base
    \param starty
         \~english Initial Y position
         \~french Coordonnée Y de base
    \param targetx
         \~english Target X position
         \~french Coordonnée X de la cible
    \param targety
         \~english Target Y position
         \~french Coordonnée Y de la cible
*/

u16 PA_AdjustAngle(u16 angle, s16 anglerot, s32 startx, s32 starty, s32 targetx, s32 targety);



/*! \fn extern inline u16 PA_GetAngle(s32 startx, s32 starty, s32 targetx, s32 targety)
    \brief
         \~english Get the angle, from 0 to 511, formed between the horizontal and the line
         \~french Récupérer l'angle, de 0 à 511, par rapport à l'horizontale...
    \param startx
         \~english Initial X position
         \~french Coordonnée X de base
    \param starty
         \~english Initial Y position
         \~french Coordonnée Y de base
    \param targetx
         \~english Target X position
         \~french Coordonnée X de la cible
    \param targety
         \~english Target Y position
         \~french Coordonnée Y de la cible
*/
extern inline u16 PA_GetAngle(s32 startx, s32 starty, s32 targetx, s32 targety) {
u16 angle = 0;
u16 anglerot = 180;


while(anglerot > 5) {
	angle = PA_AdjustAngle(angle, anglerot, startx, starty, targetx, targety);
	anglerot = (anglerot - ((3 * anglerot) >> 3)); // On diminue petit à petit la rotation...
}

// Ajustement encore plus précis...
anglerot = 4;
angle = PA_AdjustAngle(angle, anglerot, startx, starty, targetx, targety);
anglerot = 2;
angle = PA_AdjustAngle(angle, anglerot, startx, starty, targetx, targety);
anglerot = 1;
angle = PA_AdjustAngle(angle, anglerot, startx, starty, targetx, targety);

return angle;
}

/** @} */ // end of Math


extern inline s32 PA_Modulo(s32 var, s32 modulo){
while(var < 0) var += modulo;
return (var%modulo);
}

#ifdef __cplusplus
}
#endif

#endif


