/*---------------------------------------------------------------------------------
	$Id: touch.h,v 1.8 2005/10/03 21:19:34 wntrmute Exp $

	Microphone control for the ARM7

	Copyright (C) 2005
		Michael Noland (joat)
		Jason Rogers (dovoto)
		Dave Murphy (WinterMute)

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any
	damages arising from the use of this software.

	Permission is granted to anyone to use this software for any
	purpose, including commercial applications, and to alter it and
	redistribute it freely, subject to the following restrictions:

	1.	The origin of this software must not be misrepresented; you
		must not claim that you wrote the original software. If you use
		this software in a product, an acknowledgment in the product
		documentation would be appreciated but is not required.
	2.	Altered source versions must be plainly marked as such, and
		must not be misrepresented as being the original software.
	3.	This notice may not be removed or altered from any source
		distribution.

	$Log: touch.h,v $
	Revision 1.8  2005/10/03 21:19:34  wntrmute
	use ratiometric mode
	lock touchscreen on and average several readings
	
	Revision 1.7  2005/09/12 06:51:58  wntrmute
	tidied touch code
	
	Revision 1.6  2005/08/23 17:06:10  wntrmute
	converted all endings to unix

	Revision 1.5  2005/08/03 17:36:23  wntrmute
	moved touch struct to ipc

	Revision 1.4  2005/08/01 23:12:17  wntrmute
	extended touchReadXY to return touchscreen co-ordinates as well as screen co-ordinates

	Revision 1.3  2005/07/29 00:57:40  wntrmute
	updated file headers
	added touchReadXY function
	made header C++ compatible


---------------------------------------------------------------------------------*/
#ifndef ARM7_TOUCH_INCLUDE
#define ARM7_TOUCH_INCLUDE
//---------------------------------------------------------------------------------


#ifndef ARM7
#error Touch screen is only available on the ARM7
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <nds/arm7/serial.h>


#define SCREEN_WIDTH	256
#define SCREEN_HEIGHT	192


#define TSC_MEASURE_TEMP1    0x84
#define TSC_MEASURE_Y        0x90
#define TSC_MEASURE_BATTERY  0xA4
#define TSC_MEASURE_Z1       0xB4
#define TSC_MEASURE_Z2       0xC4
#define TSC_MEASURE_X        0xD0
#define TSC_MEASURE_AUX      0xE4
#define TSC_MEASURE_TEMP2    0xF4



touchPosition touchReadXY();

uint16 touchRead(uint32 command);
uint32 touchReadTemperature(int * t1, int * t2);

#ifdef __cplusplus
}
#endif


//---------------------------------------------------------------------------------
#endif	// ARM7_TOUCH_INCLUDE
//---------------------------------------------------------------------------------

