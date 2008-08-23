/*
This example shows how to use the MikMod library for tracker music file
(MOD, S3M, IT, etc.) support instead of PAlib's built in MOD player which
only supports the original MOD format and has fewer playback options
compared to MikMod.

MikMod installation instructions:
As MikMod is a separate library, it is not contained in PALib. Compiling
this example or using MikMod in your own project will require first
installing the MikMod library itself, which can be found here:

http://blog.dev-scene.com/flatware/

or with the PAlib installer.

The mikmod folder should be extracted into your devkitPro installation
folder at the same level as the devkitARM, libnds and msys folders.

Be sure the Makefile for a project using MikMod has ARM7_MIKMOD_DSWIFI
as the selected ARM7 binary, too.
*/

// Include for PALib
#include <PA9.h>

// Include for MikMod
#include <mikmod9.h>

// Include the music
#include "music.h"

// called by the drivers in mikmod library
void MikMod9_SendCommand(u32 command)
{
	// this sends the value passed in as a parameter via the FIFO to the arm7
	while (REG_IPC_FIFO_CR & IPC_FIFO_SEND_FULL);
	REG_IPC_FIFO_TX = command;
}

// Needs to be called regularly
void TimerInterrupt()
{
	// player tick
	MikMod_Update();
	
	// the bpm can change in the middle of the song
	TIMER0_DATA = TIMER_FREQ_256(md_bpm * 50 / 125);
}

int main(int argc, char** argv)
{
	// PALib Initialization
	PA_Init();
	PA_InitVBL();
	PA_InitText(0, 0);
	
	// MikMod initialization ...
	
	// register hardware or software mixer - see the file "README.ds" in PAlib\mikmod for the details
	MikMod_RegisterDriver(&drv_nds_hw);
	//MikMod_RegisterDriver(&drv_nds_sw);
	
	// if we don't know what kind of module we're going to load we can register
	// all loaders, but that will result in a larger binary
	MikMod_RegisterAllLoaders();
	//MikMod_RegisterLoader(&load_mod);
	
	// Do the init call
	PA_OutputText(0, 1, 1, "Initializing mikmod ...\n");
	if (MikMod_Init(""))
	{
		PA_OutputText(0, 1, 2, "Could not initialize mikmod, reason:\n%s\n", MikMod_strerror(MikMod_errno));
		return 1;
	}
	
	// Load a module ...
	
	// Player_LoadMemory() loads a module directly from memory;
	// use Player_Load() to load it from FAT
	PA_OutputText(0, 1, 2, "Loading module ...\n");
	MODULE* module = Player_LoadMemory(music, (u32)music_size, 64, 0);
	if (!module)
	{
		PA_OutputText(0, 1, 2, "Could not load module, reason:\n%s\n", MikMod_strerror(MikMod_errno));
		return 1;
	}
	
	// Output some information
	PA_OutputText(0, 1, 3, "Title:    %s\n", module->songname);
	PA_OutputText(0, 1, 4, "Channels: %d\n", module->numchn);
	
	// call update with correct timing
	TIMER0_CR = 0;
	irqSet(IRQ_TIMER0, TimerInterrupt);
	irqEnable(IRQ_TIMER0);
	TIMER0_DATA = TIMER_FREQ_256(md_bpm * 50 / 125);
	TIMER0_CR = TIMER_DIV_256 | TIMER_IRQ_REQ | TIMER_ENABLE;

	PA_OutputText(0, 1, 6, "Starting module");
	Player_Start(module);
	
	while (Player_Active()) {
		// when using the software driver we could call update
		// here instead
		//MikMod_Update();
		PA_WaitForVBL();
	}

	PA_OutputText(0, 1, 10, "Stopping module");
	Player_Stop();
	Player_Free(module);
	
	PA_OutputText(0, 1, 11, "Program ended");
	
	return 0;
}
