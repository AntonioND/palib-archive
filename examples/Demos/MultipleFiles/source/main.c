// Multiple files !

#include <PA9.h>

// Include the sprites :
#include "header.h" // Include header file, that's all !



int main(void)
{

	// Initialise the lib...
	PA_Init();
	PA_InitVBL(); 

	GameInit(); // Game Init from game.c

	PlayGame(); // Playing the game !!

return 0;
}

// That's all folks, easy, right ? :p

