//////////////////////////////////////////////////
// PAlib C++ project template                   //
// MyApp.cpp                                    //
// Main project source code file.               //
//////////////////////////////////////////////////

// Include the application header
#include "MyApp.h"

// Declare variables
int MyApp::nframe = 0;
Sprite MyApp::rocket(1, 0); // screen, sprite number

// Initialization function
void MyApp::init(){
	// Initialize the text system
	PA_InitText(1, 0);

	// Load our graphics
	loadgraphics();

	// Set the rocket's starting position
	rocket.move(128, 192-64);

	// Set the rocket's background priority
	rocket.priority(1); // on top of BG1 = below BG0 = below text background

	Fixed a(4.5f);
	Fixed b(79);
	Fixed c(a);
	int d = 40;

	PA_OutputText(1, 1, 10, "Arith1:    %d %d %f3", int(b + 1), int(b + c), float(a + 0.25f));

	c = 45;
	c = (c * 2) - a;
	d *= Fixed(0.5f); // fake division
	PA_OutputText(1, 1, 11, "Arith2:    %d %d %f3", int(c), int(b * 2), float(b / 2));
	PA_OutputText(1, 1, 12, "Modulo:    %d %d", int(Fixed(4) % Fixed(2)), int(Fixed(414) % Fixed(141)));
	PA_OutputText(1, 1, 13, "FixedTest: %d %f3 %f3", int(a * 2), float(a * 0.5f), float(a / Fixed(2)));
	PA_OutputText(1, 1, 14, "Sqrt:      %d %f4", int(Fixed(25).sqrt()), float(Fixed(2).sqrt()));
	PA_OutputText(1, 1, 15, "Extra:     %d %d 0x%x %f3", 4 + Fixed(1.2f), d, a.raw(), float(Fixed::r2f(0x800)));

	if(c == 85.5f) PA_OutputSimpleText(1, 1, 16, "CmpTest1:  true");
	if(c > a) PA_OutputSimpleText(1, 1, 17, "CmpTest2:  true");
}

// Graphics loading function
void MyApp::loadgraphics(){
	// Load our rocket graphic
	PA_LoadSpritePal(1, 0, (void*) rocket_Pal);
	rocket.create((void*)rocket_Sprite, OBJ_SIZE_32X32, 0);

	// Rotate it
	rocket.bindrotset(0);
	rocket.dblsize(true);
	rocket.rotozoom(-64, 128, 128);
}

// Render function: called after each VBlank
void MyApp::render(){
	// Output the value of our variable
	PA_OutputText(1, 1, 1, "Frame counter: %d", nframe);
	PA_OutputText(1, 1, 2, "X: %d  ", int(rocket.pos.x));
	PA_OutputText(1, 1, 3, "Y: %d"  , int(rocket.pos.y));

	// Render the rocket
	rocket.render();
}

// Update function
bool MyApp::update(){
	// Increment the counter
	nframe ++;

	// Move the rocket
	rocket.pos.x ++; if(rocket.pos.x == 256) rocket.pos.x = -64;
	rocket.pos.y --; if(rocket.pos.y == -64) rocket.pos.y = 192;

	// Keep going
	return true;
}
