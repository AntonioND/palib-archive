//////////////////////////////////////////////////
// PAlib C++ project template                   //
// MyApp.cpp                                    //
// Main project source code file.               //
//////////////////////////////////////////////////

// Include the application header
#include "MyApp.h"

// Declare variables
Sprite* MyApp::sprites;
HandleProvider<128> MyApp::hnd; // max number of handles

// Initialization function
void MyApp::init(){
	// Initialize the text system
	PA_LoadDefaultText(1, 0);

	// Load the graphics
	u16 rocketgfx = loadrocket();

	// Create the sprites
	sprites = new Sprite[8];
	
	for(int i = 0; i < 8; i ++){
		// The init() method behaves the same way as the constructor.
		sprites[i].init(1, hnd.newhandle());
		sprites[i].create(rocketgfx, OBJ_SIZE_32X32, 0); // also accepts a gfx id

		// Print the sprite ID
		PA_OutputText(1, 1, 1+i, "%d", sprites[i].sprid);

		// Move the sprite
		sprites[i].move(i<<5, i<<3);
		sprites[i].priority(1);
	}
}

// Rocket graphic loading function
u16 MyApp::loadrocket(){
	PA_LoadSpritePal(1, 0, (void*)rocket_Pal);
	return PA_CreateGfx(1, (void*)rocket_Sprite, OBJ_SIZE_32X32, 1);
}

// Render function: called after each VBlank
void MyApp::render(){
	// Render the sprites
	for(int i = 0; i < 8; i ++)
		sprites[i].render();
}

// Update function
bool MyApp::update(){

	// Keep going
	return true;
}
