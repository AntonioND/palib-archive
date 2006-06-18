#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"
/* 
   Thanks to Chris Double and Neimod
   http://www.double.co.nz/nintendo_ds
*/
void PA_MicInit() {
  memset(commandControl, 0, sizeof(CommandControl));  
}



void PA_PlaySample(int channel, int frequency, void* data, int length, int volume)
{
  struct Command* command = &commandControl->command[commandControl->currentCommand];
  struct PlaySampleSoundCommand* ps = &command->playSample;

  command->commandType = PLAY_ONE_SHOT_SAMPLE; 
  ps->channel = channel;
  ps->frequency = frequency;
  ps->data = data;
  ps->length = length;
  ps->volume = volume;

  commandControl->currentCommand++;
  commandControl->currentCommand &= MAX_COMMANDS-1;
}

void PA_StartRecording(u8* buffer, int length)
{
  struct Command* command = &commandControl->command[commandControl->currentCommand];
  struct StartRecordingCommand* sr = &command->startRecording;

  command->commandType = START_RECORDING; 
  sr->buffer = buffer;
  sr->length = length;

  commandControl->currentCommand++;
  commandControl->currentCommand &= MAX_COMMANDS-1;
}

int PA_StopRecording()
{
  struct Command* command = &commandControl->command[commandControl->currentCommand];
  command->commandType = STOP_RECORDING; 
  commandControl->return_data = -1;
  commandControl->currentCommand++;
  commandControl->currentCommand &= MAX_COMMANDS-1;

  /*while(commandControl->return_data == -1)
    swiDelay(1);*/
  return commandControl->return_data;
}

int PA_GetMicVol()
{
	return IPC->mailRead;
}

#ifdef __cplusplus
}
#endif



