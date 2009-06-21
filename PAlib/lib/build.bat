@echo off
cd source

cd arm7_maxmod_dswifi
make clean
make
if %ERRORLEVEL% NEQ 0 goto ERROROREXIT
rd /S /Q build
cd ..

cd arm7_mp3
make clean
make
if %ERRORLEVEL% NEQ 0 goto ERROROREXIT
rd /S /Q build
cd ..

cd arm7_mp3_dswifi
make clean
make
if %ERRORLEVEL% NEQ 0 goto ERROROREXIT
rd /S /Q build
cd ..

cd arm9
make clean
make
if %ERRORLEVEL% NEQ 0 goto ERROROREXIT
rd /S /Q build
cd ..

cd..

:ERROROREXIT
pause