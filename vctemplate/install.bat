@echo off

:: PAlibAppTemplate Installation Launcher
:: This script requires administrator privileges!

:: Get the path to devkitPro

set D_DRIVE=%DEVKITPRO:~1,1%
set D_PATH=%DEVKITPRO:~3%

:: Go to the drive
%D_DRIVE%:

:: Go to the directory
cd "%D_DRIVE%:/%D_PATH%/PAlib/vctemplate/install"

:: Banner
type banner.txt
pause

:: Launch the installation script (with admin privileges :))
start install.js
exit
