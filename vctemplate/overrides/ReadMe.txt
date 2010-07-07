/----------------------------------------------\
|/--------------------------------------------\|
|| Welcome to the PAlib VC++ Project Template ||
|\--------------------------------------------/|
\----------------------------------------------/

You must get familiar with the structure of a PAlib project:

Folders:
  data    - Contains data files that you want to include in your project.
  include - Contains header files (.h).
  source  - Contains source files (.c, .cpp and .s). Those are your program.
  gfx     - Contains PAGfx (a graphics converter) and the graphics for your program.
  audio   - Contains module files (mod, s3m, xm and it) and sound effects (wav) for use with Maxmod.
  filesystem - Contains files which will be embedded in the .nds file. Use libfilesystem to read them.

Files:
  Makefile            - This is a special file that tells your system how to convert your project files into a NDS ROM (how to compile your project).
  (Optional) logo.bmp - This is a custom logo that will be shown in the application loader of your Nintendo DS instead of the default PAlib one.
  ReadMe.txt          - The file you are reading! :P
  source/main.c       - This contains your program.

=================================
| Quick start guide for newbies |
=================================

- When you want to compile your project press F7. A pane on the bottom will appear with text indicating the status of the compilation.
    /---------------------------------------------------------------------------------------------------------------------------\
    | TIP: Watch out for the warnings and errors! If you don't know what they mean, ask in the PAlib forums (forum.palib.info). |
    \---------------------------------------------------------------------------------------------------------------------------/
- When you want to run your project, press F5 to launch the .nds file. You must associate .nds files with an emulator for this
    to work! A good one is no$gba, which you can find at c:\devkitPro\PAlib\emulators\no$gba.
    If you haven't compiled your project yet and press F5 it will automatically build it before running it.
- If you're using libfilesystem and your program doesn't work in your card use HomebrewMenu by devkitPro (go to devkitPro.org/hbmenu).

======================================
| Preparing your program for release |
======================================

You should replace the project description in the Makefile with something more appropriate. The default is:
TEXT1           := PAlib Project
TEXT2           := Change this text
TEXT3           := for your project!

As the last step, consider creating a custom logo.bmp for your project even if your card doesn't show this icon.
Its size has to be 32x32 and it can have up to 16 colors (color index 0 is transparent).
If your card doesn't show the internal logo in NDS ROMs, you can use DSOrganize to view it on the DS and confirm that it looks OK.

==============================
| Section for advanced users |
==============================

To use features like Maxmod you have to select an ARM7 core for your project.
You can do this in the Makefile - open it with an editor and read the first few lines.

The following features are available:

Audio:
 ASlib  - RAW and MP3 file playback using Noda's ASlib (examples\Sound\ASlib\ASlib_General) - this is the default
 Maxmod - module (mod, s3m, xm and it) and sound effect (wav) playback using eKid's Maxmod (it comes with libnds).

Networking:
 dswifi - dswifi for DS<->AccessPoint connections (examples\Wifi\dswifi)

/-----------------------------------------------------------------------------------------------------------------------------\
| IMPORTANT: Your project will only work on the G6 and M3 Real cards if you choose an ARM7 core which is smaller than 64 KBs. |
| Currently the only cores that can be used on those cards are the ARM7_MP3 and ARM7_MAXMOD_DSWIFI cores.                     |
\-----------------------------------------------------------------------------------------------------------------------------/
