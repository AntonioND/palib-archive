/*
   microphone code based on neimod's microphone example.
   See: http://neimod.com/dstek/ 
   Chris Double (chris.double@double.co.nz)
   http://www.double.co.nz/nintendo_ds
*//*
#include <nds.h>
#include "microphone7.h"

#define PA_SERIAL_ENABLE   0x8000
#define PA_SERIAL_BUSY     0x80

#define SPI_DEVICE_POWER  (0 << 8)
#define SPI_DEVICE_TOUCH  (2 << 8)
#define SPI_BAUDRATE_2Mhz 1
#define SPI_BAUDRATE_1Mhz 2

#ifndef SPI_CONTINUOUS
	#define SPI_CONTINUOUS    (1<<11)
#endif

#define PM_AMP_OFFSET     2
#define PM_AMP_ON         1
#define PM_AMP_OFF    	  0

static s8* PA_microphone_buffer = 0;
static int PA_microphone_buffer_length = 0;
static int PA_current_length = 0;

void TurnOnMicrophone()
{
  PM_SetAmp(PM_AMP_ON);
}

void TurnOffMicrophone()
{
  PM_SetAmp(PM_AMP_OFF);
}


void PA_StartRecording(s8* buffer, int length)
{
  PA_microphone_buffer = buffer;
  PA_microphone_buffer_length = length;
  PA_current_length = 0;

  TurnOnMicrophone();

  // Setup a 16kHz timer
  TIMER3_DATA = 0xF7CF;
  TIMER3_CR = TIMER_ENABLE | TIMER_DIV_1 | TIMER_IRQ_REQ;
}

int PA_StopRecording()
{
  TIMER3_CR &= ~TIMER_ENABLE;
  TurnOffMicrophone();
  PA_microphone_buffer = 0;
  return PA_current_length;
}

void  PA_ProcessMicrophoneTimerIRQ()
{
  if(PA_microphone_buffer && PA_microphone_buffer_length > 0) {
    // Read data from the microphone. Data from the Mic is unsigned, flipping
    // the highest bit makes it signed.
    *PA_microphone_buffer++ = MIC_ReadData() - 128;
    --PA_microphone_buffer_length;
    PA_current_length++;
  }
  VBLANK_INTR_WAIT_FLAGS |= IRQ_TIMER3;
}*/
