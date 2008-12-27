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


extern u16 GHCompletePad, GHExPad;
extern u8 gh_update_padtimes;

typedef struct {
	u8 Green, Red, Yellow, Blue, Anykey;
} GH_Buttons_TruthValues_Struct;

typedef struct {
	s32 Green, Red, Yellow, Blue, Anykey;
} GH_Buttons_IntegerValues_Struct;

typedef struct {
	GH_Buttons_TruthValues_Struct Held, Released, Newpress, DblClick;
	GH_Buttons_IntegerValues_Struct Uptime, Downtime;
} GH_Pad_Struct;

extern GH_Pad_Struct GHPad;



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

/*! \fn extern inline void PA_SetAutoUpdateGHPadTimes(u8 on)
    \brief
         \~english Decides if the number of frames each key is pressed/not pressed is automatically evaluated. You can then retrieve the number of frames each key is pressed with Pad.Downtime.Green (or Red...), the number of frames each key is not pressed with Pad.Uptime.Green (...), and if any key was pressed two times fast enough with Pad.DblClick.Green (...)
    \~\param on
         \~english 1 for on, 0 for off
 */
extern inline void PA_SetAutoUpdateGHPadTimes(u8 on)
{
	gh_update_padtimes = on;
}


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
