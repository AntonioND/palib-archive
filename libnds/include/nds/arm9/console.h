/*---------------------------------------------------------------------------------
	$Id: console.h,v 1.11 2006/02/23 00:14:02 wntrmute Exp $

	console functions

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

	$Log: console.h,v $
	Revision 1.11  2006/02/23 00:14:02  wntrmute
	add types header
	
	Revision 1.10  2006/01/10 05:45:02  dovoto
	Added a consoleClear because i can....
	
	Revision 1.9  2005/11/14 11:57:16  wntrmute
	Corrected doxygen comments
	
	Revision 1.8  2005/11/07 04:11:53  dovoto
	Added consoleDemoInit for prototyping
	
	Revision 1.7  2005/10/20 20:54:44  wntrmute
	doxygenation
	use siscanf
	
	Revision 1.6  2005/09/12 06:50:23  wntrmute
	removed *printAt
	added ansi escape sequences
	
	Revision 1.5  2005/09/04 00:24:44  wntrmute
	exposed consoleSetPos
	move iprintAt and printAt to separate files

	Revision 1.4  2005/08/31 01:10:33  wntrmute
	reworked console into stdio

	Revision 1.3  2005/08/23 17:06:10  wntrmute
	converted all endings to unix

	Revision 1.2  2005/08/01 23:18:22  wntrmute
	adjusted headers for logging


---------------------------------------------------------------------------------*/

/*! \file console.h
    \brief nds stdio support.
*/

#ifndef CONSOLE_H
#define CONSOLE_H

#define CONSOLE_USE_COLOR255 16

#include <nds/jtypes.h>

#ifdef __cplusplus
extern "C" {
#endif
/*! \fn void consoleInit(u16* font, u16* charBase, u16 numCharacters, u8 charStart, u16* map, u8 pal, u8 bitDepth)
	\brief Initialise the console.
	\param font	base address of the 16 color font to use
	\param charBase	VRAM address to load the font
	\param numCharacters number of characters in the font
	\param charStart ascii code of the first character in the font
	\param map base address of the map to use for printing
	\param pal 16 color palette index to use
	\param bitDepth 256/16 color tile flag. 
	
	Initializes the console with the given parameters. When pal is greater than 15 and
	bitDepth is 16 then	all non zero entries in the font are set to index 255. When bitDepth
	is not 16 then the font tiles are created as 8bit (256 color).
 
*/
void consoleInit(u16* font, u16* charBase, u16 numCharacters, u8 charStart, u16* map, u8 pal, u8 bitDepth);
/*! \fn void consoleInitDefault(u16* map, u16* charBase, u8 bitDepth)
	\brief Initialize the console with some default parameters.
	\param charBase	VRAM address to load the font
	\param map base address of the map to use for printing
	\param bitDepth 256/16 color tile flag

	This function calls consoleInit() with the default built in font and character ranges, the parameters given
	are as for that function. 
*/
void consoleInitDefault(u16* map, u16* charBase, u8 bitDepth);

/*! \fn void consoleDemoInit(void)
\brief Initialize the console to a default state for prototyping.
This function sets the console to use sub display, VRAM_C, and BG0 and enables MODE_0_2D on the
sub display.  It is intended for use in prototyping applications which need print ability and not actual
game use.  Print functionality can be utilized with just this call.
*/
void consoleDemoInit(void);
/*! \fn void consoleClear(void)
\brief Clears the screan by iprintf("\x1b[2J");
*/
void consoleClear(void);
#ifdef __cplusplus
}
#endif

#endif
