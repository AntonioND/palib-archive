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
	$(DEVKITPRO)/devkitARM/arm-elf/bin/ar -cr lib/libpa7.a  lib/arm7/build/command7.o lib/arm7/build/microphone7.o  lib/arm7/build/ModFile.o lib/arm7/build/pa.o  lib/arm7/build/Sound7.o
	$(DEVKITPRO)/devkitARM/arm-elf/bin/ar -cr lib/libpa9.a  lib/arm9/build/gba-jpeg-decode.o lib/arm9/build/pa.o  lib/arm9/build/palib.o lib/arm9/build/Sound9.o