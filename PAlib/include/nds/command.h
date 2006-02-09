#if !defined(COMMAND_H)
#define COMMAND_H

/*
  Structures and functions to allow the ARM9 to send commands to the
  ARM7. Based on code from the MOD player example posted to the GBADEV
  forums.
*/
#ifdef __cplusplus
extern "C" {
#endif

/* Enumeration of commands that the ARM9 can send to the ARM7 */
enum CommandType {
  PLAY_ONE_SHOT_SAMPLE,
  START_RECORDING,
  STOP_RECORDING
};

/* Command parameters for playing a sound sample */
typedef struct
{
  int channel;
  int frequency;
  void* data;
  int length;
  int volume;
}PlaySampleSoundCommand;

/* Command parameters for starting to record from the microphone */
typedef struct 
{
  u8* buffer;  
  int length;
}StartRecordingCommand;

/* The ARM9 fills out values in this structure to tell the ARM7 what
   to do. */
typedef struct  {
  enum CommandType commandType;
  union {
    void* data;  
    PlaySampleSoundCommand playSample;    
    StartRecordingCommand  startRecording;
  };
}Command;

/* Maximum number of commands */
#define MAX_COMMANDS 20

/* A structure shared between the ARM7 and ARM9. The ARM9
   places commands here and the ARM7 reads and acts upon them.
*/
typedef struct  {
  Command command[MAX_COMMANDS];
  int currentCommand;
  int return_data;
}CommandControl;

/* Address of the shared CommandControl structure */
#define commandControl ((CommandControl*)((uint32)(IPC) + sizeof(TransferRegion)))

#if defined(ARM9)
void CommandInit();
void CommandPlayOneShotSample(int channel, int frequency, void* data, int length, int volume);
void CommandStartRecording(u8* buffer, int length);
int CommandStopRecording();
#endif

#if defined(ARM7)
void CommandProcessCommands();
#endif


#ifdef __cplusplus
}
#endif


#endif
