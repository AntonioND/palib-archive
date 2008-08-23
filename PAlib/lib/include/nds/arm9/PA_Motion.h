#ifndef PA_MOTIOn
#define PA_MOTIOn

#include <nds/arm9/ndsmotion.h>


typedef struct {
   u8 Up, Down, Right, Left;
} pamotionpad;


typedef struct{
   s32 X, Y, Z, AccelX, AccelY, AccelZ, Zrot, Angle, Force;
   s32 Vx, Vy, Vz, Vzrot;
   pamotionpad Newpress, Held, Released;
} motion_struct;
extern motion_struct Motion;

void PA_MotionVBL(void);

// turn on the accelerometer
extern inline void PA_MotionInit(void) {
	motion_init();
	MotionVBL = PA_MotionVBL;
}

// checks whether a DS Motion Card is plugged in
extern inline u8 PA_CheckDSMotion(){
    return motion_init();
}



extern u8 pamotiontopad;

extern inline void PA_MotionToPad(u8 enable){
	pamotiontopad = enable;
}



/** @defgroup Motion DS Motion functions
 *  Easy enable and play around with your DS Motion !
 *  @{
 */




/*! \fn extern inline void PA_Mode7MoveLeftRight(s16 x_deplac)
    \brief
         \~english Move lateraly, so left or right...
         \~french Se déplacer latéralement
    \~\param x_deplac
         \~english Number of pixels to move left or right
         \~french De combien de pixels se déplacer à gauche ou à droite

*/





/** @} */ // end of Motion




#endif


