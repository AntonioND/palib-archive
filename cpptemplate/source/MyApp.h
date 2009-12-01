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
using PA::Fixed;

// Main application class
class MyApp: public Application{
	// Put your variables here
	// Also don't forget to declare them
	// in the MyApp.cpp file!
	static int nframe;
	static Sprite rocket;

	// Main methods (needed by run())
	void init();
	void render();
	bool update();

	// Our own methods
	void loadgraphics();
};
