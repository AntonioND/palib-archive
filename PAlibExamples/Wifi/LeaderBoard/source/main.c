// Includes
#include <PA9.h>

#define GAMEKEY "0123456789ABCDE" 

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
   PA_InitWifi();
    
	// Initialise the text system on the top screen
	PA_InitText(0,0);
	PA_InitText(1, 0);

	PA_OutputText(0,1,1,"Up/Down to change score");
	PA_OutputText(0,1,2,"Right Left to change level");
	PA_OutputText(0,1,3,"Press A to toggle response type");
	
	int rt =0;
	int score = 100;
	int level = 1;
	bool submitted = false;
	char *highscore = malloc(1000);
	char *levelname = malloc(20);

	while (1)
	{
	   if (submitted) PA_OutputText(0,1,10,"Change values to submit again!  ");
	   else PA_OutputText(0,1,10,          "    Press Start to Submit        ");

		//if anything changed - we will allow a resubmit (this is just a demo afterall!)
		if (Pad.Held.Up || Pad.Held.Down||Pad.Held.Right||Pad.Held.Left) submitted = false;
		
		if (Pad.Held.Up) score++;
		if (Pad.Held.Down) score--;

		if (Pad.Newpress.A)  {if (rt > 0) rt = 0; else rt = 1;}

		if (Pad.Newpress.Right) level++;
		if (Pad.Newpress.Left) level--;

		PA_OutputText(0,5,5,"Score = %d    ", score);
		PA_OutputText(0,5,6,"Level = Level %d    ", level);
		PA_OutputText(0,5,7,"Response Type = %d    ", rt);
		
		if (Pad.Newpress.Start)
		{ 
			sprintf(levelname, "Level_%d", level);			
		   submitted = PA_SubmitScore(	highscore, 	//the string the response will be copied to
												GAMEKEY,   	//your game key
												(char*)PA_UserInfo.Name, //player name (NO SPACES - use _ instead)
												score,  		//the players score
												levelname,  		//the name of the level (NO SPACES - use _ instead)
												rt 			//the response type = 0 : top 10 + position; = 1 : just the position number of the score.
											); 
			PA_ClearTextBg(1);
			if (submitted)
			{
		   	PA_BoxText(1,2,2,31,22, highscore, 1000);
		 	}  	
		 	else
		 	{
		 	 	PA_BoxText(1,2,2,31,22, "Your score was not transmitted succesfully.  Press Start to try again",100);  
		 	}	 	
		}			
		PA_WaitForVBL(); 
	}	
	return 0;
} // End of main()


