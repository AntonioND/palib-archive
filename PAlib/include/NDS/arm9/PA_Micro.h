#if !defined(COMMAND_H)
#define COMMAND_H
/* 
   Thanks to Chris Double and Neimod
   http://www.double.co.nz/nintendo_ds
*/
/*
  Structures and functions to allow the ARM9 to send commands to the
  ARM7. Based on code from the MOD player example posted to the GBADEV
  forums.
*/

/* Enumeration of commands that the ARM9 can send to the ARM7 */
enum CommandType {PLAY_ONE_SHOT_SAMPLE,START_RECORDING,STOP_RECORDING};

/* Command parameters for playing a sound sample */
struct PlaySampleSoundCommand
{
  int channel;
  int frequency;
  void* data;
  int length;
  int volume;
};

/* Command parameters for starting to record from the microphone */
struct StartRecordingCommand
{
  u8* buffer;  
  int length;
};

/* The ARM9 fills out values in this structure to tell the ARM7 what
   to do. */
struct Command {
  enum CommandType commandType;
  union {
    void* data;  
     struct PlaySampleSoundCommand playSample;    
     struct StartRecordingCommand  startRecording;
  };
};

/* Maximum number of commands */
#define MAX_COMMANDS 20

/* A structure shared between the ARM7 and ARM9. The ARM9
   places commands here and the ARM7 reads and acts upon them.
*/
typedef struct CommandControl {
  struct Command command[MAX_COMMANDS];
  int currentCommand;
  int return_data;
}CommandControl;

/* Address of the shared CommandControl structure */
#define commandControl ((CommandControl*)((uint32)(IPC) + sizeof(TransferRegion)))

#if defined(ARM9)
void PA_MicInit();
void PA_PlaySample(int channel, int frequency, void* data, int length, int volume);
void PA_StartRecording(u8* buffer, int length);
int PA_StopRecording();
int PA_GetMicVol();
#endif

#if defined(ARM7)
void CommandProcessCommands();
#endif

#endif
