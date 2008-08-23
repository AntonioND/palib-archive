#!/bin/sh
cd $PAPATH
make clean
make
make archive
rm -rf lib\arm7_mikmod_dswifi\build
rm -rf lib\arm7_mod_dswifi\build
rm -rf lib\arm7_mod_liblobby\build
rm -rf lib\arm7_mp3\build
rm -rf lib\arm7_mp3_dswifi\build
rm -rf lib\arm7_mp3_liblobby\build
rm -rf lib\arm9\build

