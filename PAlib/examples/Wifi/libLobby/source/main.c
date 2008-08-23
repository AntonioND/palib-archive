/* Basic liblobby example using PAlib

An example showing use of liblobby with PAlib for "ad-hoc" DS-to-DS communications.

Make sure you have selected an ARM7 binary with LIBLOBBY support in your project's
Makefile!

See the examples included with liblobby for further usage.

liblobby installation instructions:
As liblobby is an external lib, it requires separate installation. Get the liblobby
installation files from the PAlib installer or see:

http://forum.gbadev.org/viewtopic.php?t=14198

Extract the files into a directory called "liblobby" in your DevkitPro directory
(so that in the "liblobby" directory there are include, lib, etc.) alongside the 
devkitARM, libnds, msys folders.
*/



// Includes
#include <PA9.h>       		// Include for PA_Lib

#include "MessageQueue.h"	// Includes for liblobby
#include "802.11.h"
#include "lobby.h"

// We need a custom VBL function which is called every VBlank to update liblobby; if you want to play mp3's simultaneously, add a AS_SoundVBL(); in this function
// and call PA_VBLFunctionInit(customVBL) instead of PA_VBLFunctionInit(AS_SoundVBL) when initializing aslib
void customVBL(void)
{
	IPC_RcvCompleteCheck();
	LOBBY_Update();
}

// The receive handler - it is called every time something is received on the stream which with it is registered
void Receive(unsigned char *data, int length, LPLOBBY_USER from)
{
	PA_OutputText(0, 1, 20, "Received: %s       ", (char*)data);
}

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	PA_InitText(0, 0);
	PA_OutputText(0, 1, 1, "Hello WIFI!");
	
	// Initialize libLobby and output some information ...
	PA_VBLFunctionInit(customVBL);
	if (!IPC_Init())
	{
		PA_OutputText(0, 1, 3, "Couldn't IPC_Init()!");
		return 1;
	}
	IPC_SetChannelCallback(0, &LWIFI_IPC_Callback);
	PA_OutputText(0, 1, 2, "LOBBY_Init() ...");
	LOBBY_Init();
	
	// Register a stream handler for stream number 1;
	// Streams below number 0x8000 are "acknowledged streams", that means liblobby will make sure that everything you send reaches its destination.
	LOBBY_SetStreamHandler(0x0001, &Receive);
	
	// main loop
	int selected = 0;
	PA_OutputText(0, 1, 3, "Entering main loop ...");
	while (1)
	{
		// Print the number of connections
		int max = LOBBY_GetNumberOfKnownUsers();
		PA_OutputText(0, 1, 22, "Users: %d  ", max);
		
		// Allow the selection of the destination
		if (Pad.Newpress.Down)
			selected++;
		if (Pad.Newpress.Up)
			selected--;
		if (max > 0)
			selected %= max ;
		
		// On button presses, send text to the selected connection
		if (Pad.Newpress.A)
		{
			LOBBY_SendToUser(LOBBY_GetUserByID(selected),0x0001,(unsigned char *)"Key [A] pressed.",17) ;
		}
		if (Pad.Newpress.B)
		{
			LOBBY_SendToUser(LOBBY_GetUserByID(selected),0x0001,(unsigned char *)"Key [B] pressed.",17) ;
		}
		if (Pad.Newpress.X)
		{
			LOBBY_SendToUser(LOBBY_GetUserByID(selected),0x0001,(unsigned char *)"Key [X] pressed.",17) ;
		}
		if (Pad.Newpress.Y)
		{
			LOBBY_SendToUser(LOBBY_GetUserByID(selected),0x0001,(unsigned char *)"Key [Y] pressed.",17) ;
		}
		
		// Write all connections to the screen
		int i;
		for (i = 0; i < max; i++)
		{
			LPLOBBY_USER user = LOBBY_GetUserByID(i) ;
			if (i == selected)
			{
				PA_OutputText(0, 1, 5+i, "->%s (%s)       ", LOBBY_GetUserName(user), LOBBY_IsTimedOut(user) ? "TIMEOUT" : "OK");
			}
			else
			{
				PA_OutputText(0, 1, 5+i, "  %s (%s)       ", LOBBY_GetUserName(user), LOBBY_IsTimedOut(user) ? "TIMEOUT" : "OK");
			}
		}
		
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
