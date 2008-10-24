#ifndef _PA_MAIN
#define _PA_MAIN

#ifdef __cplusplus
extern "C" {
#endif



#include <nds.h>
#include <nds/memory.h>
#include <nds/bios.h>
#include <malloc.h>

#include <stdarg.h> // Pour le système de text
#include <string.h>

#include "arm9/PA_IO.h"
#include "arm9/PA_GBFS.h"
#include "arm9/PA_FS.h"
#include "gbfs.h"
#include "gba-jpeg-decode.h"
#include "Sound9.h"
#include "fat.h"
#include "PA_IPC.h"

#include "arm9/PA_General.h"

#include "arm9/PA_Text.h"
#include "arm9/PA_TextBits.h"
#include "arm9/PA_BgTiles.h"

#include "arm9/PA_BgLargeMap.h"


#include "arm9/PA_Video.h"
#include "arm9/PA_Sprite.h"
#include "arm9/PA_SpriteDual.h"
#include "arm9/PA_Sound.h"
#include "arm9/PA_Micro.h"
#include "arm9/PA_Draw.h"
#include "arm9/PA_Interrupt.h"
#include "arm9/PA_Keys.h"
#include "arm9/PA_Motion.h"
#include "arm9/PA_BgRot.h"

#include "arm9/PA_Timer.h"

#include "arm9/PA_Math.h"
#include "arm9/PA_Palette.h"
#include "arm9/PA_PaletteDual.h"
#include "arm9/PA_Save.h"
#include "arm9/PA_SpecialFx.h"

#include "arm9/PA_TileDual.h"


#include "arm9/PA_Keyboard.h"

#include "arm9/PA_Window.h"

#include "arm9/PA_Mode7.h"

#include "arm9/PA_3DSprites.h"
//#include "arm9/PA_3D.h"


#include "arm9/PA_IA.h"
#include "arm9/PA_Reco.h"

#include "arm9/PA_16c.h"

#include "arm9/PA_Wifi.h"

#include "arm9/PA_Splash.h"

#include "arm9/PA_Gif.h"

#include "arm9/PA_BgTrans.h"

#include "arm9/as_lib9.h"

#include "arm9/PA_FATLoad.h"
#include "arm9/PA_KeysSpecial.h"
#include "arm9/PA_Fake16bit.h"

#ifdef __cplusplus
}
#endif


#endif



