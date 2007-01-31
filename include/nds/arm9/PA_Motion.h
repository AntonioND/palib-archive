#ifndef PA_MOTIOn
#define PA_MOTIOn

typedef struct {
   u8 Up, Down, Right, Left;
} pamotionpad;


typedef struct{
   s32 X, Y, Z, Zrot, Angle, Force;
   s32 Vx, Vy, Vz, Vzrot;
   pamotionpad Newpress, Held, Released;
} motion_struct;
extern motion_struct Motion;


u8 PA_MotionInit(void);

u8 PA_CheckDSMotion(void);

extern u8 pamotiontopad;

extern inline void PA_MotionToPad(u8 enable){
	pamotiontopad = enable;
}



/** @defgroup Motion DS Motion Commands !!!
 *  Easy enable and play around with your DS Motion !
 *  @{
 */




/*! \fn extern inline void PA_Mode7MoveLeftRight(s16 x_deplac)
    \brief
         \~english Move lateraly, so left or right...
         \~french Se déplacer latéralement
    \param x_deplac
         \~english Number of pixels to move left or right
         \~french De combien de pixels se déplacer à gauche ou à droite

*/





/** @} */ // end of Motion




#endif


