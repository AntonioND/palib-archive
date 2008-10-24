PATH := $(DEVKITARM)/bin:$(PATH)
SUBDIRS:= `ls | egrep -v '^(CVS|tools)$$'`

######################################################################

all:
	make -C lib/arm7_mikmod_dswifi
	make -C lib/arm7_mp3
	make -C lib/arm7_mp3_liblobby
	make -C lib/arm7_mod_liblobby
	make -C lib/arm7_mp3_dswifi
	make -C lib/arm7_mod_dswifi
	make -C lib/arm9

######################################################################

clean:
	make clean -C lib/arm7_mikmod_dswifi
	make clean -C lib/arm7_mp3
	make clean -C lib/arm7_mp3_liblobby
	make clean -C lib/arm7_mp3_dswifi
	make clean -C lib/arm7_mod_dswifi
	make clean -C lib/arm7_mod_liblobby
	make clean -C lib/arm9
 
######################################################################

archive:
	$(DEVKITPRO)/devkitARM/arm-eabi/bin/ar -cr lib/libpa9.a lib/arm9/build/gba-jpeg-decode.o lib/arm9/build/PA.o lib/arm9/build/PA_Draw.o lib/arm9/build/PA_Draw8bit.o lib/arm9/build/PA_Draw8bitEx.o lib/arm9/build/PA_Draw16bit.o lib/arm9/build/PA_Draw16bitEx.o lib/arm9/build/PA_Keys.o lib/arm9/build/PA_IO.o lib/arm9/build/PA_Math.o  lib/arm9/build/PA_Reco.o lib/arm9/build/PA_Sprite.o lib/arm9/build/PA_SpriteAnim.o lib/arm9/build/PA_BgTiles.o lib/arm9/build/PA_BgEasy.o lib/arm9/build/PA_BgRot.o lib/arm9/build/PA_BgGetPixel.o lib/arm9/build/PA_BgLargeMap.o lib/arm9/build/PA_Mode7.o lib/arm9/build/PA_Text.o lib/arm9/build/PA_TextBits.o lib/arm9/build/PA_TextSpecial.o lib/arm9/build/PA_TextPrint.o lib/arm9/build/PA_Text8bit.o lib/arm9/build/PA_Text16bit.o lib/arm9/build/PA_Interrupt.o lib/arm9/build/PA_Palette.o lib/arm9/build/PA_PaletteEx.o lib/arm9/build/PA_16c.o lib/arm9/build/PA_16cTextRot.o lib/arm9/build/PA_GBFS.o lib/arm9/build/PA_Gif.o lib/arm9/build/PA_GifToTex.o lib/arm9/build/PA_GifToTiles.o lib/arm9/build/keyboard.raw.o lib/arm9/build/keyboard.map.o lib/arm9/build/keyboard.pal.o lib/arm9/build/PA_Keyboard.o lib/arm9/build/PA_IA.o lib/arm9/build/PA_IA.o lib/arm9/build/PA_FS.o lib/arm9/build/PA_Video.o lib/arm9/build/PA_Motion.o lib/arm9/build/PA_Timer.o lib/arm9/build/PA_Wifi.o lib/arm9/build/PA_LeaderBoard.o lib/arm9/build/PA_SplashEyes.o lib/arm9/build/PA_SplashBlue.o lib/arm9/build/PA_BgTrans.o lib/arm9/build/PA_3D.o lib/arm9/build/PA_3DSprites.o lib/arm9/build/PA_Stuff.o lib/arm9/build/PA_RTC.o lib/arm9/build/PA_2D.o lib/arm9/build/PA_SpriteStylus.o lib/arm9/build/PA_Inits.o lib/arm9/build/PA_FATLoad.o lib/arm9/build/dgif_lib.o lib/arm9/build/gif_err.o lib/arm9/build/gif_font.o lib/arm9/build/gifalloc.o lib/arm9/build/quantize.o lib/arm9/build/as_lib9.o lib/arm9/build/PA_Sound.o lib/arm9/build/PA_FATLoad.o lib/arm9/build/PA_KeysSpecial.o lib/arm9/build/PA_Fake16bitBg.o 