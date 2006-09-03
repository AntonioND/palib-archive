PATH := $(DEVKITARM)/bin:$(PATH)
SUBDIRS:= `ls | egrep -v '^(CVS|tools)$$'`

######################################################################

all:
	make -C lib/arm7
	make -C lib/arm9

######################################################################

clean:
	make clean -C lib/arm7
	make clean -C lib/arm9
 
######################################################################

archive:
	$(DEVKITPRO)/devkitARM/arm-eabi/bin/ar -cr lib/libpa7.a  lib/arm7/build/command7.o lib/arm7/build/microphone7.o  lib/arm7/build/ModFile.o lib/arm7/build/PA.o  lib/arm7/build/Sound7.o
	$(DEVKITPRO)/devkitARM/arm-eabi/bin/ar -cr lib/libpa9.a  lib/arm9/build/gba-jpeg-decode.o lib/arm9/build/PA.o lib/arm9/build/PA_Draw.o lib/arm9/build/PA_Keys.o lib/arm9/build/PA_Math.o  lib/arm9/build/PA_Reco.o lib/arm9/build/PA_Sprite.o lib/arm9/build/PA_BgTiles.o lib/arm9/build/PA_BgLargeMap.o lib/arm9/build/PA_Mode7.o lib/arm9/build/PA_Text.o lib/arm9/build/PA_Text8bit.o  lib/arm9/build/PA_Interrupt.o lib/arm9/build/PA_Palette.o lib/arm9/build/palib.o lib/arm9/build/Sound9.o lib/arm9/build/PA_16c.o lib/arm9/build/disc_io.o lib/arm9/build/gba_nds_fat.o lib/arm9/build/io_efa2.o lib/arm9/build/io_fcsr.o lib/arm9/build/io_m3cf.o lib/arm9/build/io_m3sd.o lib/arm9/build/io_mpcf.o lib/arm9/build/io_nmmc.o lib/arm9/build/io_sccf.o lib/arm9/build/io_scsd.o lib/arm9/build/io_sccf.o lib/arm9/build/PA_API.o lib/arm9/build/PA_GBFS.o lib/arm9/build/PA_Gif.o lib/arm9/build/PA_3D.o lib/arm9/build/palib.o lib/arm9/build/keyboard.raw.o lib/arm9/build/keyboard.map.o lib/arm9/build/keyboard.pal.o lib/arm9/build/PA_Keyboard.o lib/arm9/build/PA_IA.o lib/arm9/build/PA_IA.o lib/arm9/build/PA_FS.o lib/arm9/build/PA_Sound.o lib/arm9/build/PA_Timer.o lib/arm9/build/PA_Wifi.o lib/arm9/build/pa_splashEyes.o lib/arm9/build/pa_splashBlue.o