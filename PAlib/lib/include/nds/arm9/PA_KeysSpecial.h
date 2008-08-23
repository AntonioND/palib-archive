#ifndef _GH_KEYS
#define _GH_KEYS

#ifdef __cplusplus
extern "C" {
#endif

#include "PA9.h"

/*! \file PA_KeysSpecial.h
    \brief Support for special DS controllers

    Macros, variables, and prototypes needed for DS controller accessory (Guitar Hero Grip, Taito Paddle, ...) support.
*/

/** @defgroup KeysSpecial Special controllers
 *      Macros, variables, and prototypes needed for DS controller accessory (Guitar Hero Grip, Taito Paddle, ...) support.

 *  @{
 */


#ifdef ARM9
#define WAIT_CR	REG_EXMEMCNT
#else
#define WAIT_CR	REG_EXMEMSTAT
#endif


#define GH_POLL (*(vuint8*)0x0A000000)
#define BUTTON_BLUE 8
#define BUTTON_YELLOW 16
#define BUTTON_RED 32
#define BUTTON_GREEN 64

#define UPDATEGHPAD(type, pad)  type.Green = (pad & BUTTON_GREEN)>>6;\
   type.Red = (pad & BUTTON_RED) >> 5;\
   type.Yellow = (pad & BUTTON_YELLOW) >> 4;\
   type.Blue = (pad & BUTTON_BLUE) >> 3;\
   type.Anykey = (!(!((pad&120))));

typedef struct {
   u8 Green, Red, Yellow, Blue, Anykey;
} GH_Buttons;

typedef struct {
   GH_Buttons Held, Released, Newpress; // Pressed si on appuye, Held si on garde appuyé, et Released si on relache
} GH_Pad;
extern GH_Pad GHPad;
   
extern u16 GHCompletePad, GHExPad, GHTempPad;


/*! \fn bool PA_DetectGHPad(void)
    \brief
         \~english Check to see if there's a Guitar Hero pad inserted in slot-2.  Returns 1 if there is or 0 if there isn't.
         \~french 
*/
bool PA_DetectGHPad(void);

/*! \fn bool PA_InitGHPad(void)
    \brief
         \~english Set up the Guitar Hero pad for use. Returns a 1 if initialization was successful, or a 0 if it wasn't.
         \~french 
*/
bool PA_InitGHPad(void);

/*! \fn void PA_DeInitGHPad(void)
    \brief
         \~english De-initialize the Guitar Hero pad. It's recommended to call this when you won't be using the GH pad anymore.
         \~french 
*/
void PA_DeInitGHPad(void);

/*! \fn void PA_UpdateGHPad(void)
    \brief
         \~english Update the values of GHPad. But NOTE: you won't need it if you used PA_InitGHPad as it's done automatically every Vblank.
         \~french 
*/
void PA_UpdateGHPad(void);



#define PADDLE_LOW (*(vuint8*)0x0A000000)
#define PADDLE_HIGH (*(vuint8*)0x0A000001)

typedef struct {
  u16 Count, OldCount;
  s16 Speed;
  u8 Low, High, OldLow, OldHigh;
} PaddleInfo;
extern PaddleInfo Paddle;

/*! \fn bool PA_DetectPaddle(void)
    \brief
         \~english Check to see if there's a Taito Paddle inserted in slot-2.  Return 1 if there is or 0 if there isn't.
         \~french 
*/
bool PA_DetectPaddle(void);

/*! \fn bool PA_InitPaddle(void)
    \brief
         \~english Set up the Taito Paddle for use. Returns a 1 if initialization was successful, or a 0 if it wasn't.
         \~french 
*/
bool PA_InitPaddle(void);

/*! \fn void PA_DeInitPaddle(void)
    \brief
         \~english De-initialize the Taito Paddle. It's recommended to call this when you won't be using the paddle anymore.
         \~french 
*/
void PA_DeInitPaddle(void);

/*! \fn void PA_UpdatePaddle(void)
    \brief
         \~english Update the values of Paddle. But NOTE: you won't need it if you used PA_InitPaddle as it's done automatically every Vblank.
         \~french 
*/
void PA_UpdatePaddle(void);


/** @} */ // end of Fat Loading group

#ifdef __cplusplus
}
#endif
#endif
