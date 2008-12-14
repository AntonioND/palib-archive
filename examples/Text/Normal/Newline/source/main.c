/* The text is drawn on a background (you can chose which background you want, 
and can display information on the screen, now supporting newlines with "\n"
*/


// Includes
#include <PA9.h>       // Include for PA_Lib


// Function: main()
int main(int argc, char ** argv)
{
  PA_Init();    // Initializes PA_Lib
  PA_InitVBL(); // Initializes a standard VBL

// Initialise the text system on the top screen
  PA_InitText(1, 0);

// This'll put a green color on the bottom screen... Color component (red, green, blue) range from 0 to 31
  PA_SetTextCol(0, 0, 31, 0);

// The same old PA_OutputSimpleText, but now with newline support
  PA_OutputSimpleText(1, 1, 1, "          ATTENTION:\n\nPA_OutputSimpleText now has\nsupport for line breaking in\nthe body of the text. So it's\nmuch easier to place a lot of\ntext on the screen.\n\nThis should be really useful\nif you need to parse a text\nfile loaded from FAT!");

// It works with PA_OutputText, as well
  PA_OutputText(1, 1, 13, "But wait, that's not all! It\nalso works with variables in\nPA_OutputText.\n\nWhat do you think of that,\n%s?\n\nIt's %d:%02d, do you know\nwhere your text is?", PA_UserInfo.Name, PA_RTC.Hour, PA_RTC.Minutes);

  while (1) {PA_WaitForVBL();}

  return 0;
} // End of main()
