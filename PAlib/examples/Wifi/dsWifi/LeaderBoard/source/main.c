// Leaderboards using PAlib and dswifi lib example
//
// Make sure you have selected an ARM7 binary with DSWIFI support in your project Makefile!
//

// Includes
#include <PA9.h>

#define GAMEKEY1 2345
#define GAMEKEY2 45678
#define GAMEKEY3 "ABCDE" 

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
	char *readhighscore = malloc(1001);
	char *levelname = malloc(20);
	char *gamekey = malloc(15);

	//do some trickery to hide the gamekey....yours should be more elaborate than this
	sprintf(gamekey, "0%04d%05d%s", GAMEKEY1-1111, GAMEKEY2+11111, GAMEKEY3);

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

		PA_OutputText(0,2,5,"Score = %d    ", score);
		PA_OutputText(0,2,6,"Level = Level %d    ", level);
		PA_OutputText(0,2,7,"Response Type = %d    ", rt);
		PA_OutputText(0,2,8,"Secret Key = %s    ", gamekey);
		
		if (Pad.Newpress.Start)
		{ 
			sprintf(levelname, "Level_%d", level);			
		   submitted = PA_SubmitScore(	highscore, 	//the string the response will be copied to
												gamekey,   	//your game key
												(char*)PA_UserInfo.Name, //player name (NO SPACES - use _ instead)
												score,  		//the players score
												levelname,  		//the name of the level (NO SPACES - use _ instead)
												rt 			//the response type = 0 : top 10 + position; = 1 : just the position number of the score.
											); 
			PA_ClearTextBg(1);
			if (submitted)
			{
			   if (sprintf(readhighscore, "%s", highscore))
  				{
  				   //return string is readable!
		   		PA_OutputText(1,2,2, highscore); //don't use box text here...it freezes sometimes
		  		}
		  		else
		  		{
		  		   //score submitted but response no good
		  		   PA_BoxText(1,2,2,31,22, "Your score was sent to the online leaderboard but there was a problem with the response.\n\nCheck out your score online: \nleaderboard.\n      palib.com", 1000); 
    			}			
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


