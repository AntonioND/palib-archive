
#ifdef __cplusplus
extern "C" {
#endif

#include "PA_Timer.h"
#include "PA_Sound.h"

#define REGENTIME 1100

PA_SoundOptions PA_SoundOption; // Default sound formats...
u32 *FS_mod;
u32 *FS_wav[16];



//stream vars
u32 *Stream_Gap[16];
u32 *Stream_End[16];
u32 *Stream_Datas[16];
u32 *Stream_Regen_Ptr[16];
s64	Stream_Length[16];
u32	Stream_Last_Tick[16];
s32	Stream_Repeat[16];
u8	Stream_Timer[16];
bool StreamStarted=false;

void PA_InitStreamSound(void) 
{
	
	if(StreamStarted==true) {
	
		return;
	}
	
	StreamStarted=true;
	u8 i;
	//to prevent errors setting Stream vars
	for (i = 0; i < 16; i++)
	{
		Stream_Gap[i] = NULL;
		Stream_Length[i] = 0;
		Stream_Repeat[i] = -1;
	}
	StartTime(true);
	NewTimer(true);
}

void PA_SetDefaultSound(u8 volume, int freq, s16 format){
	PA_SoundOption.volume = volume;
	PA_SoundOption.freq = freq;
	PA_SoundOption.format = format;
}
	 
void PA_PlaySoundEx2(u8 PA_Channel, const void* data, s32 length, u8 volume, int freq, s16 format, BOOL repeat, int repeatPoint){

	
	
	
   /*	SndChannel[PA_Channel].timer = freq;
	SndChannel[PA_Channel].vol = volume;
	SndChannel[PA_Channel].pan = 64;
	SndChannel[PA_Channel].format = format;
   	SndChannel[PA_Channel].loopStart = repeatPoint;
   	SndChannel[PA_Channel].length = length;*/

    DC_FlushAll();
    IPC->soundData = &snd;
	PA_IPC.Sound[PA_Channel].Busy = 1; // now busy
	
	if(!repeat){ // use PA system for normal sounds
		// Sound commands
		PA_IPC.Sound[PA_Channel].Command |= (1<<PAIPC_PLAY); // play
		PA_IPC.Sound[PA_Channel].Data = (u32*)data;
		PA_IPC.Sound[PA_Channel].Volume = volume;  
		PA_IPC.Sound[PA_Channel].Pan = 64; 
		PA_IPC.Sound[PA_Channel].Rate = freq;
		PA_IPC.Sound[PA_Channel].Length = length-4;
		PA_IPC.Sound[PA_Channel].Format = format;	
		PA_IPC.Sound[PA_Channel].Repeat = 0;
	}
	else { // Use special system if repeating data involved
		snd.data[PA_Channel].data = data;
		snd.data[PA_Channel].len = length-4;
		snd.data[PA_Channel].rate = freq;
		snd.data[PA_Channel].pan = 64;
		snd.data[PA_Channel].vol = volume;
		snd.data[PA_Channel].format = format;	
		SndPlay(PA_Channel, (void*)FS_wav[PA_Channel], length, volume, freq, format, repeat, repeatPoint);
		PA_IPC.Sound[PA_Channel].Repeat = 1;
	}
	
}



void PA_StopSound(u8 PA_Channel){
	if(PA_IPC.Sound[PA_Channel].Repeat==0) PA_IPC.Sound[PA_Channel].Command |= (1<<PAIPC_STOP); 
	else SndStop(PA_Channel);
}




void PA_PlayFSStreamSoundEx2(u8 PA_Channel, u16 FS_wav_number, u8 volume, int freq, s16 format, BOOL repeat, int repeatPoint)
{
	u32 Alloc;
	PA_InitStreamSound();
	
	free(FS_wav[PA_Channel]);

	if((freq == 11025) || ((freq == 22050) && (format == 0))) // Ptr have to be divided by 4
	{
		Alloc = freq*4;
		if(format == 1)
			Alloc *=2;
		FS_wav[PA_Channel] = (u32*)malloc(Alloc+4);//3s allocation
		Stream_Gap[PA_Channel] = (u32*)(((u32)(FS_wav[PA_Channel])) + (Alloc/4)*3);
		Stream_Regen_Ptr[PA_Channel] = (u32*)(((u32)(PA_PAFSFile(FS_wav_number))) + (Alloc/4)*3);
	}
	else
	{
        Alloc = freq*3;
		if(format == 1)
			Alloc *=2;
		FS_wav[PA_Channel] = (u32*)malloc(Alloc+4);//3s allocation
		Stream_Gap[PA_Channel] = (u32*)(((u32)(FS_wav[PA_Channel])) + (Alloc/3)*2);
		Stream_Regen_Ptr[PA_Channel] = (u32*)(((u32)(PA_PAFSFile(FS_wav_number))) + (Alloc/3)*2);
	}

	Stream_Length[PA_Channel] = PA_FSFile[FS_wav_number].Length;//Real Length
	Stream_End[PA_Channel] = (u32*)(((u32)(FS_wav[PA_Channel])) + Alloc);
	Stream_Datas[PA_Channel] =  (u32*)PA_PAFSFile(FS_wav_number);
	if(repeat == true)
		Stream_Repeat[PA_Channel] = repeatPoint;//Repeat if we can
	else
		Stream_Repeat[PA_Channel] = -1;//No repeat

	DMA_Copy(PA_PAFSFile(FS_wav_number), FS_wav[PA_Channel], Alloc / 4, DMA_32NOW);
	Stream_Last_Tick[PA_Channel] = 1000;

	//sending datas
	PA_PlaySoundEx2(PA_Channel, (void*)FS_wav[PA_Channel], Alloc, volume, freq, format, true, 0);

	//launch timer
	Stream_Timer[PA_Channel] = NewTimer(true);
}

void PA_RegenStream()
{
	u8 i,j;
	u32 Regen, Regen2;
	s32 RemainTillEnd;
	s16 time;

	for(i = 0; i < 16; ++i)
	{
		if(Stream_Length[i] > 0)
		{
			time = Tick(Stream_Timer[i]) - Stream_Last_Tick[i];
			//test if we need regen
			if(time >= REGENTIME)
			{
				Stream_Last_Tick[i] = (Tick(Stream_Timer[i])/1000)*1000;
				if((Stream_Last_Tick[i]%45000) == 0) //slow timer to prevent pbs
					Stream_Last_Tick[i] += 1000;
				//Regen time !
				//calculate regen
				Regen = (time/1000);
				RemainTillEnd = Stream_Length[i] - ((u32)(Stream_Regen_Ptr[i]) - (u32)(Stream_Datas[i]));
				if(SndChannel[i].format == 1) //16bit = 2*more datas
				{
					Regen *= (SndChannel[i].timer) * 2;
					RemainTillEnd /= 2;
				}
				else
					Regen *= (SndChannel[i].timer);
				//test
	
				if(RemainTillEnd <= SndChannel[i].timer)//we have reach the end
				{
					if(Stream_Repeat[i] == -1)//NO_LOOP
					{
						//loop or not if not end stream
						//first step finish current file before looping
						//Filling last datas
						FillTheGap(i, RemainTillEnd);
						SndChgRepeat(i,false , 0);
						SndChgLength(i,(u32)(Stream_Gap[i] - FS_wav[i]));
						Stream_Length[i] = 0;
					}
					else//LOOP
					{
						//Filling last datas
						if((RemainTillEnd %4) != 0)
						{
							FillTheGap(i, RemainTillEnd - (RemainTillEnd %4));
							for(j = 0; j < (RemainTillEnd %4);++j)
								*Stream_Gap[i] = *Stream_Regen_Ptr[i];
						}
						else
						{
							FillTheGap(i, RemainTillEnd);
						}

						//Set Up timer and data ptr
						Stream_Regen_Ptr[i] = (u32*)(((u32)(Stream_Datas[i])) + Stream_Repeat[i]);
                        ResetTimer(Stream_Timer[i]);
						if((Stream_Last_Tick[i]%45000) > 35000)
							Stream_Last_Tick[i] = 1000;
						else
							Stream_Last_Tick[i] = 0;

						//second step fill the gap with start content and put good valued to timer and ptr
						Regen2 = Regen - RemainTillEnd;
						//regen the loop
						FillTheGap(i, Regen2);
					}
				}
				else
				{
					FillTheGap(i, Regen);
				}
			}
		}
	}

}

//Fill gap with an amount of datas
void FillTheGap(u8 PA_Channel, u32 size)
{
	u32 Remain;
	Remain = (u32)(Stream_End[PA_Channel]) - (u32)(Stream_Gap[PA_Channel]);
	if(Remain <= size)
	{
		//regen and come back to beginning
		if(Remain > 3)
		{
			DMA_Copy(Stream_Regen_Ptr[PA_Channel], Stream_Gap[PA_Channel], Remain /4, DMA_32NOW);
			Stream_Regen_Ptr[PA_Channel] = (u32*)(((u32)(Stream_Regen_Ptr[PA_Channel])) + Remain);
		}
		Stream_Gap[PA_Channel] = FS_wav[PA_Channel];
		if((size - Remain) > 3)
		{
			DMA_Copy(Stream_Regen_Ptr[PA_Channel], Stream_Gap[PA_Channel], (size-Remain) /4, DMA_32NOW);
			Stream_Gap[PA_Channel] = (u32*)(((u32)(FS_wav[PA_Channel])) + (size - Remain));
			Stream_Regen_Ptr[PA_Channel] = (u32*)(((u32)(Stream_Regen_Ptr[PA_Channel])) + (size - Remain));
		}
	}
	else
	{
		//no special case
		DMA_Copy(Stream_Regen_Ptr[PA_Channel], Stream_Gap[PA_Channel], size /4, DMA_32NOW);
		Stream_Gap[PA_Channel] = (u32*)(((u32)(Stream_Gap[PA_Channel])) + size);
		Stream_Regen_Ptr[PA_Channel] = (u32*)(((u32)(Stream_Regen_Ptr[PA_Channel])) + size);
	}
}

void PA_StopStream(u8 PA_Channel)
{
	StreamStarted=false;
	Stream_Length[PA_Channel] = 0;
	SndStop(PA_Channel);
}

void PA_PauseStream(u8 PA_Channel)
{
	Stream_Length[PA_Channel] = -(Stream_Length[PA_Channel]);
	PauseTimer(Stream_Timer[PA_Channel]);
	SndPause(PA_Channel);
}

void PA_UnpauseStream(u8 PA_Channel)
{
	Stream_Length[PA_Channel] = -(Stream_Length[PA_Channel]);
	UnpauseTimer(Stream_Timer[PA_Channel]);
	SndUnpause(PA_Channel);
}


#ifdef __cplusplus
}
#endif

