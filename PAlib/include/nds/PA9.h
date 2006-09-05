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

#include "gbfs.h"
#include "gba-jpeg-decode.h"
#include "Sound9.h"


#include "arm9/PA_General.h"

#include "arm9/PA_Text.h"
#include "arm9/PA_BgTiles.h"

#include "arm9/PA_BgLargeMap.h"




#include "arm9/vidfile.h"
#include "arm9/PA_Sprite.h"
#include "arm9/PA_SpriteDual.h"
#include "arm9/PA_Sound.h"
#include "arm9/PA_Micro.h"
#include "arm9/PA_Draw.h"
#include "arm9/PA_Interrupt.h"
#include "arm9/PA_Keys.h"

#include "arm9/PA_BgRot.h"

#include "arm9/PA_Timer.h"

#include "arm9/PA_API.h"  // API only if has text...

#include "arm9/PA_Math.h"
#include "arm9/PA_Palette.h"
#include "arm9/PA_PaletteDual.h"
#include "arm9/PA_Save.h"
#include "arm9/PA_FS.h"
#include "arm9/PA_GBFS.h"
#include "arm9/PA_SpecialFx.h"

#include "arm9/PA_TileDual.h"

#include "arm9/PA_FAT.h"  // fat driver by chrishm

#include "arm9/PA_Keyboard.h"

#include "arm9/PA_Window.h"

#include "arm9/PA_Mode7.h"

#include "arm9/PA_3D.h"


#include "arm9/PA_IA.h"
#include "arm9/PA_Reco.h"

#include "arm9/PA_16c.h"

#include "arm9/PA_Wifi.h"

#include "arm9/PA_Splash.h"

#include "arm9/PA_Gif.h"

#ifdef __cplusplus
}
#endif


#endif



