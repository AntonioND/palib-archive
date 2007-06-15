/*---------------------------------------------------------------------------------
	$Id: rumble.h,v 1.4 2007/02/07 16:38:11 wntrmute Exp $
 
	Copyright (C) 2005
		Michael Noland (joat)
		Jason Rogers (dovoto)
		Dave Murphy (WinterMute)
		Mike Parks (BigRedPimp)
	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any
	damages arising from the use of this software.
	Permission is granted to anyone to use this software for any
	purpose, including commercial applications, and to alter it and
	redistribute it freely, subject to the following restrictions:
 
  1. The origin of this software must not be misrepresented; you
     must not claim that you wrote the original software. If you use
     this software in a product, an acknowledgment in the product
     documentation would be appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and
     must not be misrepresented as being the original software.
  3. This notice may not be removed or altered from any source
     distribution.
	$Log: rumble.h,v $
	Revision 1.4  2007/02/07 16:38:11  wntrmute
	add warioware rumble support
	
	Revision 1.3  2005/11/27 07:47:07  joatski
	Fixed multiple linebreaks.
	Moved cart bus control functions and GBA header to memory.h
	
	Revision 1.2  2005/11/02 07:06:20  bigredpimp
	Added revision comment block

---------------------------------------------------------------------------------*/
/*! \file rumble.h
    \brief nds rumble option pak support.
*/
#ifndef RUMBLE_HEADER_INCLUDE
#define RUMBLE_HEADER_INCLUDE

#ifdef __cplusplus
extern "C" {
#endif

#define RUMBLE_PAK			(*(vuint16 *)0x08000000)
#define WARIOWARE_PAK		(*(vuint16 *)0x080000C4)
#define WARIOWARE_ENABLE	(*(vuint16 *)0x080000C6)
 
typedef enum {
   RUMBLE,
   WARIOWARE
}RUMBLE_TYPE;

/*! \fn bool isRumbleInserted(void);
	\brief Check for rumble option pak.
	Returns true if the cart in the GBA slot is a Rumble option pak.
*/
bool isRumbleInserted(void);

/*! \fn void setRumble(bool position);
	\param position Alternates position of the actuator in the pak
	\brief Fires the rumble actuator.
*/
void setRumble(bool position);

#ifdef __cplusplus
}
#endif

#endif
