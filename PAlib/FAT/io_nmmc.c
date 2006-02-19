/*
	io_nmmc.c

	Hardware Routines for reading an SD or MMC card using
	a Neoflash MK2 or MK3.

	Written by www.neoflash.com

	Submit bug reports for this device to the NeoFlash forums

	This software is completely free. No warranty is provided.
	If you use it, please give me credit and email me about your
	project at chishm@hotmail.com

	See gba_nds_fat.txt for help and license details.
*/

#include "io_nmmc.h"

#ifdef SUPPORT_NMMC

#include <nds/card.h>

#define SPI_FREQ	3

#define MK2_CONFIG_ZIP_RAM_CLOSE		((1 << 5) | (1 << 1))
#define MK2_CONFIG_GAME_FLASH_CLOSE		(1 << 4)

// SPI functions

static inline void Neo_OpenSPI( u8 frequency )
{
	CARD_CR1 = 0x0000A040 | frequency;
}

static inline u8 Neo_SPI( u8 dataByte )
{
	CARD_EEPDATA = dataByte;
	while (CARD_CR1 & 0x80);		// card busy
	return CARD_EEPDATA;
}

static inline void Neo_CloseSPI ( void )
{
	CARD_CR1 = 0;
}

static inline void Neo_MK2GameMode()	{
	Neo_OpenSPI(SPI_FREQ);				// Enable DS Card's SPI port
	Neo_SPI(0xF1);				// Switch to game mode
	Neo_CloseSPI();				// Disable DS Card's SPI port
}

static inline void Neo_EnableEEPROM( u32 enable )	{
	Neo_OpenSPI(SPI_FREQ);
	if(enable)	Neo_SPI(0x06);
	else 		Neo_SPI(0x0E);
	Neo_CloseSPI();
}

void Neo_WriteMK2Config(u16 config) {
	Neo_EnableEEPROM(1);
	Neo_OpenSPI(SPI_FREQ);
	Neo_SPI(0xFA);					// Send mem conf write command
	Neo_SPI(((config>>8)&255));		// Send high byte
	Neo_SPI(config&255);			// Send low byte
	Neo_CloseSPI();
	Neo_EnableEEPROM(0);
}

// Low level functions

u8 selectMMC_command [8] = {0xFF, 0x00, 0x6A, 0xDF, 0x37, 0x59, 0x33, 0xA3};

void Neo_SelectMMC (u8 dataByte) 
{
	selectMMC_command[1] = dataByte;	// Set enable / disable byte
	cardWriteCommand (selectMMC_command);	// Send "5. Use the EEPROM CS to access the MK2 MMC/SD card"
	CARD_CR2 = CARD_ACTIVATE | CARD_nRESET;
	while (CARD_CR2 & CARD_BUSY);
	return;
}

void Neo_EnableMMC( bool enable )
{
	if ( enable == false) {
		Neo_CloseSPI ();
		Neo_SelectMMC (0);
		Neo_SelectMMC (0);
	} else {
		Neo_SelectMMC (1);
		Neo_SelectMMC (1);
		Neo_OpenSPI (SPI_FREQ);
	}
	return;
}

void Neo_SendMMCCommand( u8 command, u8 *argument )
{
	Neo_SPI (0xFF);
	Neo_SPI (command | 0x40);
	Neo_SPI (argument[3]);
	Neo_SPI (argument[2]);
	Neo_SPI (argument[1]);
	Neo_SPI (argument[0]);
	Neo_SPI (0x95);
	Neo_SPI (0xFF);
	return;
}

bool Neo_CheckMMCResponse( u8 response, u8 mask )	{
	u32 i;
	for(i=0;i<256;i++)	{
		if( ( Neo_SPI( 0xFF ) & mask ) == response )
			return true;
	}
	return false;
}

// Neo MMC functions

bool Neo_InitMMC()	{
	u32 i;
	u32 argument;

	argument = 0;

	Neo_MK2GameMode();
	Neo_WriteMK2Config( MK2_CONFIG_ZIP_RAM_CLOSE | MK2_CONFIG_GAME_FLASH_CLOSE );

	Neo_EnableMMC( true );		// Open SPI port to MMC card
	for (i = 0; i < 10; i++) {
		Neo_SPI(0xFF);			// Send 10 0xFF bytes to MMC card
	}
	Neo_SendMMCCommand(0,(u8*)&argument);	// Send GO_IDLE_STATE command
	if( Neo_CheckMMCResponse( 0x01, 0xFF ) == false )	{		// Check that it replied with 0x01 (not idle, no other error)
		Neo_EnableMMC( false );
		return false;
	}
	for(i=0;i<256;i++)	{
		Neo_SendMMCCommand(1,(u8*)&argument);	// Poll with SEND_OP_COND
		if( Neo_CheckMMCResponse( 0x00, 0x01 ) == true ) {	// Check for idle state
			Neo_EnableMMC( false );				// Close SPI port to MMC card
			return true;						// Valid interface found

		}
	}
	Neo_EnableMMC( false );
	return false;
}

bool Neo_ReadMMCSector( void *buffer, u32 address )	{
	u32 i;
	u8 *p=buffer;
	Neo_EnableMMC( true );											// Open SPI port to MMC card
	Neo_SendMMCCommand( 17, (u8*)&address );						// Send READ_SINGLE_BLOCK command
	if( Neo_CheckMMCResponse( 0x00, 0xFF ) == false )	{			// Make sure no errors occured
		Neo_EnableMMC( false );
		return false;
	}
	if( Neo_CheckMMCResponse( 0xFE, 0xFF ) == false )	{			// Check for Start Block token
		Neo_EnableMMC( false );
		return false;
	}
	for( i = 0; i < 512; i++ )										// Read in the block of data
  		p[ i ] = Neo_SPI( 0xFF );
	Neo_SPI( 0xFF );												// Ignore CRC16
	Neo_SPI( 0xFF );												// Ignore CRC16
	Neo_EnableMMC( false );											// Close SPI port to MMC card
	return true;
}

bool Neo_WriteMMCSector( void *buffer, u32 address )	{
	u32 i;
	u8 *p=buffer;
	Neo_EnableMMC( true );											// Open SPI port to MMC card
	Neo_SendMMCCommand( 24, (u8*)&address );						// Send WRITE_BLOCK command
	if( Neo_CheckMMCResponse( 0x00, 0xFF ) == false )	{			// Make sure no errors occured
		Neo_EnableMMC( false );
		return false;
	}
	Neo_SPI( 0xFE );												// Send Start Block token
	for( i = 0; i < 512; i++ )										// Send the block of data
  		Neo_SPI( p[ i ] );
	Neo_SPI( 0xFF );												// Send CRC16 ignore bytes
	Neo_SPI( 0xFF );												// Send CRC16 ignore bytes
	if( ( Neo_SPI( 0xFF ) & 0x0F ) != 0x05 )	{					// Make sure the block was accepted
		Neo_EnableMMC( false );
		return false;
	}
	while( Neo_SPI( 0xFF ) == 0x00 );								// Wait for the block to be written
	Neo_EnableMMC( false );											// Close SPI port to MMC card
	return true;
}

// Neo MMC driver functions

bool NMMC_IsInserted(void)	{
	return true;
}

bool NMMC_ClearStatus (void) {
	return true;
}

bool NMMC_Shutdown(void) {
	return true;
}

bool NMMC_StartUp(void) {
	return Neo_InitMMC();
} 

bool NMMC_WriteSectors (u32 sector, u8 numSecs, void* buffer)	{
	int inumSecs=numSecs;
	int i;
	u8 *p=(u8*)buffer;
	if( numSecs == 0 )	inumSecs = 256;
	for( i=0; i<inumSecs; i++ )		{
		if(	Neo_WriteMMCSector( p, sector * 512 ) == false )
			return false;
		p+=512;
		sector++;
	}
	return true;
}

bool NMMC_ReadSectors (u32 sector, u8 numSecs, void* buffer)	{
	int inumSecs=numSecs;
	int i;
	u8 *p=(u8*)buffer;
	if( numSecs == 0 )	inumSecs = 256;
	for( i=0; i<inumSecs; i++ )		{
		if(	Neo_ReadMMCSector( p, sector * 512 ) == false )
			return false;
		p+=512;
		sector++;
	}
	return true;
}

IO_INTERFACE io_nmmc = {
	0x434D4D4E,											// 'NMMC'
	FEATURE_MEDIUM_CANREAD | FEATURE_MEDIUM_CANWRITE,
	(FN_MEDIUM_STARTUP)&NMMC_StartUp,
	(FN_MEDIUM_ISINSERTED)&NMMC_IsInserted,
	(FN_MEDIUM_READSECTORS)&NMMC_ReadSectors,
	(FN_MEDIUM_WRITESECTORS)&NMMC_WriteSectors,
	(FN_MEDIUM_CLEARSTATUS)&NMMC_ClearStatus,
	(FN_MEDIUM_SHUTDOWN)&NMMC_Shutdown
} ;


LPIO_INTERFACE NMMC_GetInterface(void) {
	return &io_nmmc ;
}

#else // #ifdef NDS

bool NMMC_InvalidFunction(void)	
{
	return false;
}

bool NMMC_InvalidDiscFunction (u32 sector, u8 numSecs, void* buffer)	
{
	return false;
}

IO_INTERFACE io_nmmc = {
	DEVICE_TYPE_NMMC,
	FEATURE_MEDIUM_CANREAD | FEATURE_MEDIUM_CANWRITE | FEATURE_SLOT_NDS,
	(FN_MEDIUM_STARTUP)&NMMC_InvalidFunction,
	(FN_MEDIUM_ISINSERTED)&NMMC_InvalidFunction,
	(FN_MEDIUM_READSECTORS)&NMMC_InvalidDiscFunction,
	(FN_MEDIUM_WRITESECTORS)&NMMC_InvalidDiscFunction,
	(FN_MEDIUM_CLEARSTATUS)&NMMC_InvalidFunction,
	(FN_MEDIUM_SHUTDOWN)&NMMC_InvalidFunction
} ;


LPIO_INTERFACE NMMC_GetInterface(void) {
	return &io_nmmc ;
}

#endif	// #ifdef SUPPORT_NMMC
