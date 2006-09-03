// ----- Change log -----
// 05/08/05 - Created.
// ----------------------

#include <nds.h>
#include <string.h>

#include "ModFile.h"
#include "Sound7.h"

// ----- Constants -----

	// This is to deal with a quirk in the MOD format, that allows loop 
	// lengths to be non-zero, but still treats them as non-looping. 
	// By experimentation, the cutoff seems to be 4.
#define SMP_LOOPLENGTH_THRESHOLD	4
	// For patterns to specify that there is no note. We have 5 octaves, 
	// so only notes 0-59 are used, and 63 is the highest that still fits 
	// in the same number of bits
#define MOD_NO_NOTE			63
	// Only 31 samples, which we refer to as 0-30, so use 31 for invalid
#define MOD_NO_SAMPLE		31


// ----- Local structures -----

typedef struct _MODFILE_LOAD_VARS
{
	MOD *dest;
	const u8 *src;
	u8 *memPool;
	u32 memPoolSize;
	u32 memPoolUsed;

} MODFILE_LOAD_VARS;


// ----- Local function prototypes -----

static void LoadSamples		(MODFILE_LOAD_VARS *vars);
static void LoadOrders		(MODFILE_LOAD_VARS *vars);
static void LoadPatterns	(MODFILE_LOAD_VARS *vars);
static void LoadSampleDatas	(MODFILE_LOAD_VARS *vars);
u32 NoteBinarySearch		(u32 period, u32 min, u32 max) CODE_IN_IWRAM;
u32 FindClosestNote			(u32 period) CODE_IN_IWRAM;


// ----- Global functions -----

	// Returns the amount of memory in memPool used
u32 ModFileLoad(const void *modFile, MOD *dest, void *memPool, u32 memPoolSize)
{
	s32 i;
	MODFILE_LOAD_VARS vars;

	vars.dest			= dest;
	vars.src			= (const u8*)modFile;
	vars.memPool		= (u8*)memPool;
	vars.memPoolSize	= memPoolSize;
	vars.memPoolUsed	= 0;

	vars.src = (const u8*)modFile + 1080;
	if(!memcmp(vars.src, "M.K.", 4))
	{
		vars.dest->channelCount = 4;
	}
	else if(!memcmp(vars.src + 1, "CHN", 3))
	{
		ASSERT(src[0] >= '0' && vars.src[0] <= '9');
		vars.dest->channelCount = vars.src[0] - '0';
	}
	else if(!memcmp(vars.src + 2, "CH", 2))
	{
		ASSERT(vars.src[0] >= '0' && vars.src[0] <= '9');
		ASSERT(vars.src[1] >= '0' && vars.src[1] <= '9');
		vars.dest->channelCount = (vars.src[0] - '0') * 10 + (vars.src[1] - '0');
	}
	else
	{
		ASSERT(0);
	}

	vars.src = (const u8*)modFile;
	for (i = 0; i < 20; i++)
		vars.dest->name[i] = *vars.src++;
	vars.dest->name[19] = '\0';	// Replace last char with null, just incase

	LoadSamples		(&vars);
	LoadOrders		(&vars);
	LoadPatterns	(&vars);
	LoadSampleDatas	(&vars);

	return vars.memPoolUsed;
}

static void LoadSamples(MODFILE_LOAD_VARS *vars)
{
	s32 i;

	for(i = 0; i < 31; i++)
	{
		SAMPLE_HEADER *smp = &vars->dest->sample[i];

		vars->src += 22;			// Skip 22-byte name
		((u16*)smp)[0] = ((u16*)vars->src)[0];	// Load 8 bytes that line 
		((u16*)smp)[1] = ((u16*)vars->src)[1];	// up with struct members
		((u16*)smp)[2] = ((u16*)vars->src)[2];	// (2 bytes at a time since 
		((u16*)smp)[3] = ((u16*)vars->src)[3];	// they're not word aligned)
		vars->src += 8;

		smp->smpData = NULL;	// Nothing for now, will load later

		smp->length		= ( ((smp->length & 0xff) << 8) | 
							 (smp->length >> 8) );

		smp->loopStart	= ( ((smp->loopStart & 0xff) << 8) | 
							 (smp->loopStart >> 8) );

		smp->loopLength	= ( ((smp->loopLength & 0xff) << 8) | 
							 (smp->loopLength >> 8) );

		if(smp->loopLength <= SMP_LOOPLENGTH_THRESHOLD)
			smp->loopLength = 0;	// Deal with a funky quirk in the MOD format
	}

}	// LoadSamples

static void LoadOrders(MODFILE_LOAD_VARS *vars)
{
	vars->dest->orderCount = *vars->src++;
	vars->src++;	// Unused byte...

		// Read the orders
	dmaCopyWords(3, vars->src, vars->dest->order, 128);
	vars->src += 128;

		// Back to sig, already handled it so jump over
	vars->src += 4;

}	// LoadOrders

static void LoadPatterns(MODFILE_LOAD_VARS *vars)
{
	s32 i;
	u8 highestOrder = 0;
	u8 curPattern, row, column;
	u32 patternCount;

	for(i = 0; i < vars->dest->orderCount; i++)
	{
		if(vars->dest->order[i] > highestOrder)
			highestOrder = vars->dest->order[i];
	}

		// +1 because pattern 0 is included
	patternCount = highestOrder + 1;

		// Allocate the pointers for the patterns
	vars->dest->pattern = (u8**)(vars->memPool + vars->memPoolUsed);
	vars->memPoolUsed += patternCount*sizeof(u8*);
	ASSERT(vars->memPoolUsed < vars->memPoolSize);
	   // Initialize the memory to 0
	memset( vars->dest->pattern, 0, patternCount*sizeof(u8*) );

		// And load them
	for(curPattern = 0; curPattern < patternCount; curPattern++)
	{
		u32 patternSize = 4*64*sndMod.channelCount;

			// Allocate the memory for the current pattern (they are always 1K)
		vars->dest->pattern[curPattern] = (u8*)(vars->memPool + vars->memPoolUsed);
		vars->memPoolUsed += patternSize;
		ASSERT(vars->memPoolUsed < vars->memPoolSize);

		for(row = 0; row < 64; row++)
		{
			for(column = 0; column < sndMod.channelCount; column++)
			{
				u8 cell[4];
				u8 sample;
				u16 period;
				u8 effect;
				u8 param;
				u8 closestNote;

				*((u32*)cell) = *((u32*)vars->src);
				vars->src += 4;

				sample = (cell[0] & 0xF0) | (cell[2] >> 4);
				period = cell[1] | ((cell[0] & 0xF) << 8);
				effect = cell[2] & 0xF;
				param  = cell[3];

				if(period == 0)
					closestNote = MOD_NO_NOTE;
				else
					closestNote = FindClosestNote(period);

				if(sample == 0)
					sample = MOD_NO_SAMPLE;
				else
					sample -= 1;

					// Now that we have our note, we can store the data in our new pattern
					// Calculate the address of the cell to output to
					// rowOffset = row * channelCount columns per row * 4 bytes per cell
					// columnOffset = column * 4 bytes per cell
				u32 *outCell = (u32*)&vars->dest->pattern[curPattern][row*vars->dest->channelCount*4 + column*4];
				*outCell = closestNote | (sample << 8) | 
								(effect << 16) | (param << 24);
			}
		}
	}

}	// LoadPatterns

static void LoadSampleDatas(MODFILE_LOAD_VARS *vars)
{
	s32 curSmp;

	for(curSmp = 0; curSmp < 31; curSmp++)
	{
		SAMPLE_HEADER *smp = &vars->dest->sample[curSmp];

		if(smp->length > 0)
		{
				// Length is stored as half
			smp->smpData = (s8*)(vars->memPool + vars->memPoolUsed);
			vars->memPoolUsed += ((smp->length+1) & ~1)*2*sizeof(s8);
			ASSERT(vars->memPoolUsed < vars->memPoolSize);

			dmaCopyWords(3, vars->src, smp->smpData, (smp->length+1)*2 & ~3);
			vars->src += smp->length*2;

				// Handle DS requiring word-aligned length. 
				// Since length is stored as half, it's already
				// guaranteed to be a multiple of 2, but if it's not 
				// a multiple of 4, copy the last sample twice
			if(smp->length & 1)
			{
				smp->smpData[smp->length*2 + 1] = 
					smp->smpData[smp->length*2] = 
					smp->smpData[smp->length*2 - 1];
			}

				// Convert to words, since that's what the sound regs take
			smp->length = (smp->length + 1) >> 1;
			smp->loopStart = smp->loopStart >> 1;
			smp->loopLength = (smp->loopLength + 1) >> 1;

		}
		// Else leave this sample's data as NULL
	}

}	// LoadSampleDatas



// ----- Helpers -----

u32 CODE_IN_IWRAM NoteBinarySearch(u32 period, u32 min, u32 max)
{
	if(min == max - 1)
		return min;
	else if(period > notePeriodTable[(min + max) >> 1])
		return NoteBinarySearch(period, min, (min + max) >> 1);
	else
		return NoteBinarySearch(period, (min + max) >> 1, max);
}

u32 CODE_IN_IWRAM FindClosestNote(u32 period)
{
	u32 note = NoteBinarySearch(period, 0, 12*5);

	if(note == 12*5 - 1)
	{
		return note;
	}
	else
	{
		s32 dist1 = notePeriodTable[note] - period;
		s32 dist2 = notePeriodTable[note + 1] - period;

		if(dist1*dist1 < dist2*dist2)
			return note;
		else
			return note + 1;
	}
}
