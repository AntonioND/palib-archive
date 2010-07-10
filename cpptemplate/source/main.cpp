//////////////////////////////////////////////////
// PAlib C++ project template                   //
// main.cpp                                     //
// Contains the entry point of the program.     //
//////////////////////////////////////////////////

// Include the application header
#include "MyApp.h"

// Entry point
int main(){
	// Initialize PAlib
	PA_Init();

	// Run the application
	MyApp app;
	app.run();

	return 0;
}
