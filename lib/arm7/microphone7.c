/*
   microphone code based on neimod's microphone example.
   See: http://neimod.com/dstek/ 
   Chris Double (chris.double@double.co.nz)
   http://www.double.co.nz/nintendo_ds
*/
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

// Turn on the Microphone Amp. Code based on neimod's example.
void PM_SetAmp(u8 control)
{
  while(SIO_CR & PA_SERIAL_BUSY)
    swiDelay(1);

  SIO_CR = PA_SERIAL_ENABLE | SPI_DEVICE_POWER | SPI_BAUDRATE_1Mhz | SPI_CONTINUOUS;
  REG_SPIDATA = PM_AMP_OFFSET;

  while(SIO_CR & PA_SERIAL_BUSY)
    swiDelay(1);

  SIO_CR = PA_SERIAL_ENABLE | SPI_DEVICE_POWER | SPI_BAUDRATE_1Mhz;
  REG_SPIDATA = control;
}

// Read a byte from the microphone. Code based on neimod's example.
u8 MIC_GetData8()
{
  u16 result, result2;
  
  while(SIO_CR & PA_SERIAL_BUSY)
    swiDelay(1);
 
  SIO_CR = PA_SERIAL_ENABLE | SPI_DEVICE_TOUCH | SPI_BAUDRATE_2Mhz | SPI_CONTINUOUS;
  REG_SPIDATA = 0xEC;  // Touchscreen command format for AUX
  
  while(SIO_CR & PA_SERIAL_BUSY)
    swiDelay(1);

  REG_SPIDATA = 0x00;

  while(SIO_CR & PA_SERIAL_BUSY)
    swiDelay(1);

  result = REG_SPIDATA;
  SIO_CR = PA_SERIAL_ENABLE | SPI_DEVICE_TOUCH | SPI_BAUDRATE_2Mhz;
  REG_SPIDATA = 0x00; 

  while(SIO_CR & PA_SERIAL_BUSY)
    swiDelay(1);

  result2 = REG_SPIDATA;
  
  IPC->mailRead=result;

  return (((result & 0x7F) << 1) | ((result2>>7)&1));
}

static u8* microphone_buffer = 0;
static int microphone_buffer_length = 0;
static int current_length = 0;

void TurnOnMicrophone()
{
  PM_SetAmp(PM_AMP_ON);
}

void TurnOffMicrophone()
{
  PM_SetAmp(PM_AMP_OFF);
}

void StartRecording(u8* buffer, int length)
{
  microphone_buffer = buffer;
  microphone_buffer_length = length;
  current_length = 0;

  TurnOnMicrophone();

  // Setup a 16kHz timer
  TIMER0_DATA = 0xF7CF;
  TIMER0_CR = TIMER_ENABLE | TIMER_DIV_1 | TIMER_IRQ_REQ;
}

int StopRecording()
{
  TIMER0_CR &= ~TIMER_ENABLE;
  TurnOffMicrophone();
  microphone_buffer = 0;
  return current_length;
}

void  ProcessMicrophoneTimerIRQ()
{
  if(microphone_buffer && microphone_buffer_length > 0) {
    // Read data from the microphone. Data from the Mic is unsigned, flipping
    // the highest bit makes it signed.
    *microphone_buffer++ = MIC_GetData8() ^ 0x80;
    --microphone_buffer_length;
    current_length++;
  }
}
