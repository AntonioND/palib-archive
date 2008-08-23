#ifndef __HEADER1__
#define __HEADER1__

#include <PA9.h>

#include "../source/gfx/all_gfx.h" // Gfx include for the header (.h)


// Macros in the header file
#define FRISBEE 10 // Sprite number...
#define SCREENHOLE 48 // Size of the space between the screens... This is what looked the best

// Typedefs go in header file too...
typedef struct{
	s16 x, y; // This is for the frisbee's position
	s16 vx, vy; // Frisbee speed
	s16 angle; // To make the frisbee turn on itself
}frisinfos;

extern frisinfos frisbee[10]; // Variables go in .c files. 
// If you want them to be accessible in other files, declare as extern in a header


// Function declarations
void GameInit(void);  // Game Init...
void PlayGame(void);  // Game Play...






#endif


