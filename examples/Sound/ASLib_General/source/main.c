// ASLib example - by Noda
// PALib version by Tom

// use libfat with a "test.mp3" file at the root, else load default mp3 from ram
//#define USE_FAT_TEST

// Includes
#include <PA9.h>

// the sounds
#include "test.h"
#include "sfxa.h"
#include "sfxb.h"

int main(int argc, char** argv)
{
	// variables
    int i, rate = 0, newrate = 0;
    bool mp3_mode = true, autopan = false;
    u8 lastsfx = 0, usage = 0, pan = 0, fx_mode = AS_SURROUND;

	// PALib Initialization
	PA_Init();
	PA_InitVBL();
	PA_InitText(0, 0);

	// Initialize FAT if selected
#ifdef USE_FAT_TEST
    if(!fatInitDefault()) {
        PA_OutputText(0, 1, 1, "Fat init error !!!");
        return 1;
    }
#endif

	// Init AS_Lib for mp3
	PA_VBLFunctionInit(AS_SoundVBL);
	AS_Init(AS_MODE_MP3 | AS_MODE_SURROUND | AS_MODE_16CH);
	AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND);

	// Infinite loop to keep the program running
	while (1)
	{
		// display info text
		PA_OutputText(0, 0, 0, "    ASlib example - by Noda\n    -----------------------");
	
        if (mp3_mode) {

            // show some infos
            PA_OutputText(0, 0, 3, "       MP3 Testing mode\n\n\n"
								   "A: play MP3   B: pause MP3\n\n"
								   "X: stop MP3   Y: surround ON/OFF\n\n"
								   "Stylus : < > pan   ^ v pitch\n\n"
								   "--------------------------------\n");

            PA_OutputText(0, 0, 22, "  [press select for SFX mode]\n");

            // show sample rate & panning
            PA_OutputText(0, 0, 14, "Sample rate : %d   pan : %d  \n", newrate, IPC_Sound->chan[IPC_Sound->mp3.channelL].snd.pan);
    
            // show surround status
            PA_OutputText(0, 0, 16, "Surround : %s\n", (IPC_Sound->mp3.delay ? "on " : "off"));
            PA_OutputText(0, 0, 17, "(restart play to take effect)");
    
            // show playback status
            PA_OutputText(0, 0, 19, "MP3 Status : ");    // clear status field
            switch (AS_GetMP3Status()) {
			    case (MP3ST_STOPPED) :      PA_OutputText(0, 13, 19, "stopped       "); break;
	            case (MP3ST_PAUSED) :       PA_OutputText(0, 13, 19, "paused        "); break;
	            case (MP3ST_PLAYING) :      PA_OutputText(0, 13, 19, "playing       "); break;
	            case (MP3ST_OUT_OF_DATA) :  PA_OutputText(0, 13, 19, "out of data   "); break;
	            case (MP3ST_DECODE_ERROR) : PA_OutputText(0, 13, 19, "decoding error"); break;
	            case (MP3ST_INITFAILED) :   PA_OutputText(0, 13, 19, "init failed   "); break;
            }
    
            // controls
            if (Pad.Newpress.A) {
            
                if (AS_GetMP3Status() & MP3ST_PAUSED) {
                
                    AS_MP3Unpause();
                    
                } else {
                
#ifdef USE_FAT_TEST
                    AS_MP3StreamPlay("test.mp3");
#else
                    AS_MP3DirectPlay((u8*)test, (u32)test_size);
#endif
                    AS_SetMP3Loop(true);
                    rate = 0;
                    newrate = 0;
                }
            }
            if (Pad.Newpress.B) {
    
                AS_MP3Pause();
            }
            if (Pad.Newpress.X) {
            
                AS_MP3Stop();
            }        
            if (Pad.Newpress.Y) {
    
                if(IPC_Sound->mp3.delay)
                    AS_SetMP3Delay(AS_NO_DELAY);
                else
                    AS_SetMP3Delay(AS_SURROUND);
            }        

            // change panning & sample rate using the stylus
            if((Stylus.Held) && rate && (AS_GetMP3Status() & MP3ST_PLAYING)) {
            
                newrate = (rate + rate/4) - (rate * Stylus.Y / 96 / 4);
                AS_SetMP3Rate(newrate);
                AS_SetMP3Pan(Stylus.X / 2);
            }
            // set sample rate
            if(rate == 0 && IPC_Sound->mp3.rate && (AS_GetMP3Status() & MP3ST_PLAYING)) {
            
                rate = IPC_Sound->mp3.rate;
                newrate = rate;
            }
        
        } else {
        
            // show some infos
            PA_OutputText(0, 0, 3, "       SFX Testing mode\n\n\n"
								   "A: play SFX_A     B: play SFX_B\n\n"
								   "X: autopan ON/OFF Y: FX mode\n\n"
								   "--------------------------------\n");
			PA_OutputText(0, 0, 22, "  [press select for MP3 mode]\n");

            // show channels usage, autopan status & panning of last sound played
            PA_OutputText(0, 0, 12, "Chan usage : %d / %d  \n\n", usage, IPC_Sound->num_chan - 1);
            PA_OutputText(0, 0, 14, "Autopan : %s\n\n", (autopan ? "on " : "off"));
            PA_OutputText(0, 0, 16, "Last SFX pan : %d  \n", IPC_Sound->chan[lastsfx].snd.pan);
    
            // show fx mode
            PA_OutputText(0, 0, 18, "SFX effect : %s\n", (fx_mode == AS_SURROUND ? "surround" : ( fx_mode == AS_REVERB ? "reverb  " : "no fx   ")));
            PA_OutputText(0, 0, 19, "(start new SFX to take effect)");

            // controls
            if (Pad.Newpress.A) {
            
                lastsfx = AS_SoundQuickPlay(sfxa);
            }
            if (Pad.Newpress.B) {

                lastsfx = AS_SoundQuickPlay(sfxb);
            }
            if (Pad.Newpress.X) {
                autopan = !autopan;
            }        
            if (Pad.Newpress.Y) {
            
                if(fx_mode == AS_SURROUND)
                    fx_mode = AS_REVERB;
                else if(fx_mode == AS_REVERB)
                    fx_mode = AS_NO_DELAY;
                else
                    fx_mode = AS_SURROUND;

                // set new default sound settings
                AS_SetDefaultSettings(AS_PCM_8BIT, 11025, fx_mode);
            }        
            
            // autopan
            if(autopan)
                AS_SetSoundPan(lastsfx, (++pan > 127 ? 255 - pan : pan));
    
            // get the number of busy channels
            usage = 0;
            for(i = 1; i < IPC_Sound->num_chan; i++)
                if(IPC_Sound->chan[i].busy)
                    usage++;
                    
        }

        // switch testing mode
        if ((Pad.Newpress.Select)) {

            if(mp3_mode)
                AS_MP3Stop();
				
			PA_ClearTextBg(0);
            
            mp3_mode = !mp3_mode;
        }

        PA_WaitForVBL();
    }
	
	return 0;
}
