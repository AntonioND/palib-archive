//////////////////////////////////////////////////
// PAlib C++ project template                   //
// MyApp.h                                      //
// Main project header.                         //
//////////////////////////////////////////////////

#pragma once

// Include the PAlib header
#include <PA9.h>

// Include our graphics
#include "all_gfx.h"

// Using statements
using PA::Application;
using PA::Sprite;
using PA::HandleProvider;

// Main application class
class MyApp: public Application{
	// Put your variables here
	// Also don't forget to declare them
	// in the MyApp.cpp file!
	static Sprite* sprites;
	static HandleProvider<128> hnd;

	// Main methods (needed by run())
	void init();
	void render();
	bool update();

	// Our own methods
	u16 loadrocket();
};
