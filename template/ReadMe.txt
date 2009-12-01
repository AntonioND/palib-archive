/-----------------------------------------\
|/---------------------------------------\|
|| Welcome to the PAlib Project Template ||
|\---------------------------------------/|
\-----------------------------------------/

Using this template you can get a PAlib NDS application running in almost no time.
You must get familiar with the structure of a PAlib project:

Folders:
  data    - Contains data files that you want to include in your project.
  include - Contains header files (.h).
  source  - Contains source files (.c, .cpp and .s). Those are your program.
  gfx     - Contains PAGfx (a graphics converter) and the graphics for your program.
  audio   - Contains module files (mod, s3m, xm and it) and sound effects (wav) for use with Maxmod.
  efsroot - Contains files which you will be able to read using EFS.

Files:
  build.bat and clean.bat - These are used to compile and clean your project (on Windows), respectively.
  Makefile                - This is a special file that tells your system how to convert your project files into a NDS ROM (how to compile your project).
  (Optional) logo.bmp     - This is a custom logo that will be shown in the application loader of your Nintendo DS instead of the default PAlib one.
  Template.pnproj         - This is a project file that you can use with Programmers' Notepad, a source code editor.
  ReadMe.txt              - The file you are reading! :P
  source/main.c           - This contains your program.

=================================
| Quick start guide for newbies |
=================================

- When you want to compile your project launch build.bat. Now a command prompt should open and you will see
   the status of the compiling process.
    /---------------------------------------------------------------------------------------------------------------------------\
    | TIP: Watch out for the warnings and errors! If you don't know what they mean, ask in the PAlib forums (forum.palib.info). |
    \---------------------------------------------------------------------------------------------------------------------------/
- When you want to run your project, launch your favorite emulator (for example: PAlib/emulators/no$gba/no$gba.exe) and open the
   NDS file that has been generated.
- If you want to use Programmers' Notepad, launch it and open the Template.pnproj file.

============================================
| Linux/Mac/Other users, please read this! |
============================================

If you want to use PAGfx copy the Mono-ready PAGfx binaries from PAlib\tools\PAGfx\Mono to the gfx folder.

======================================
| Preparing your program for release |
======================================

You should replace the project description in the Makefile with something more appropriate. The default is:
TEXT1 := PAlib Project
TEXT2 := Change this text
TEXT3 := for your project!

As the last step, consider creating a custom logo.bmp for your project even if your card doesn't show this icon.
Its size has to be 32x32 and it can have up to 16 colors (color index 0 is transparent).
If your card doesn't show the internal logo from NDS ROMs, you can use DSOrganize to view it on the DS and confirm that it looks OK.

==============================
| Section for advanced users |
==============================

To use special features like Maxmod you have to select an ARM7 core for your project.
You can do this in the Makefile - open it with an editor and read the first few lines.

The following features are available:

Sound/Music:
 ASlib  - RAW and MP3 file playback using Noda's ASlib (examples\Sound\ASlib\ASlib_General) - this is the default
 Maxmod - module playback (mod, s3m, xm and it) using eKid's Maxmod (it comes with libnds)

Networking:
 dswifi - dswifi for DS<->AccessPoint connections (examples\Wifi\dswifi)

Filesystem:
 EFSlib - This library appends a filesystem to the NDS ROM (examples\FAT\EFS) - if you choose this, copy the patcher and the files
           source/efs_lib.c and source/efs_lib.h from the example.

/-----------------------------------------------------------------------------------------------------------------------------\
| IMPORTANT: Your project will only work on the G6 and M3 Real cards if you choose an ARM7 core which is smaller than 64 KBs. |
| Currently the only cores that can be used on those cards are the ARM7_MP3 and ARM7_MAXMOD_DSWIFI cores.                     |
\-----------------------------------------------------------------------------------------------------------------------------/
