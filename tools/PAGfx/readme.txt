PAGfx - a background/sprite/texture converter for the Nintendo DS! 

If you don't have the .NET Framework (>= 2.0), please download it here:
http://www.microsoft.com/downloads/details.aspx?FamilyID=0856eacb-4362-4b0d-8edd-aab15c5e04f5&DisplayLang=en

If you have Mono (>= 2.4.2.2), alternative binaries are located in the Mono subfolder.

How does it work? 
Using an ini file, named PAGfx.ini 
In it, you have to put the following information:

#TransColor Black (the color you want to be transparent: White, Black, Green, or Magenta, or an Hex color such as FF00FF or 00FFFF)
  Pretty basic, to choose which color is the transparent color.
   That way the colors doesn't need to be the first color in the palette index,
   the program will do it for you when converting

#Sprites : the current format is spritefile colormode (256colors or 16bit) palettename (multiple files can have the same palette...)
 firstsprite.bmp 256colors sprite0
 test.jpg 256colors sprite1
 ship.jpg 16bit

 In this example, you see how to use 256colors or 16bit sprites. Just give the filename, the color mode, and then the palette number (from 0 to the last palette number)

#Backgrounds : backgroundfile bgmode (for bitmaps, 8bit or 16bit; TileBg, RotBg, LargeMap) palettename 
 bg0.bmp TileBg bg0
 bitmap.bmp 8bit bg0

 Works the same way as the sprites, but with different backgrounds modes...

#Textures : texturefile colormode (4colors, 16colors, 256colors, 16bit) palettename 
 floor.bmp 256colors floor
 water.bmp 4colors

 Works the same way as the sprites, but with different texture modes

Changelog
=========

====================
| v0.10 (by fincs) |
====================

PAGfx
-----

[Output] The outputted header is now even cleaner.

[Custom transparent color] Fixed :s

[Backgrounds] Now outputting a background structure instead of pointers to the files.

[Backgrounds] Renamed InfiniteMap to UnlimitedBg.

[Backgrounds] The TiledBg and LargeMap modes were finally obsoleted by EasyBg.

[Backgrounds] EasyBg is now the default.

PAGfx Frontend
--------------

[Custom transparent color] Fixed :s

[INI load] Marking TiledBg and LargeMap as obsoleted -> EasyBg.

[UI] The UI was made less noisy (no more annoying msgboxes!).

===================
| v0.9 (by fincs) |
===================

General
-------

[Build system] Now maintaining two versions: the .NET version for Windows and the Mono version for Linux/Mac OS X/others.

PAGfx
-----

[Export] Exporting support for .c files has been removed as the #include "gfx/all_gfx.c" method has been deprecated.

[Transparent colors] Now supporting Hex colors as transparent colors.

[Command line] When specifying another PAGfx.ini it outputs to the folder where the file is in.

[all_gfx.h] The output file is now more clear and uses #pragma once instead of an #include guard.

[Palettes] Now PAGfx doesn't truncate the palette when it has less than 256 colors.

[Invalid C identifiers] PAGfx now replaces all invalid characters in the C identifiers with an underscore ("_").

PAGfx Frontend
--------------

[Color/Background Modes] Now supporting the whole color and background modes.

[Textures] Texture loading from PAGfx.ini was fixed, it now works.

[Transparent color] Now supports specifying a custom transparent color.

[Color mode warnings] Replaced the annoying color mode warning msgboxes by a status "bar".

[Toolbar] The "Update Row" button has been removed as it only crashed PAGfx.

[INI exporting] Graphic files that are on the same folder as PAGfx.exe or in a subfolder are exported without the full path.

[Mono-version] The PAGfx Frontend was ported to Mono, albeit with some GUI glitches.

[Mono-version] Colormode warnings have been disabled under Mono due to lack of support.

=========
| v0.82 |
=========

PAGfx
-----

[FIX] Better declarations in all_gfx.h and .c, with #ifndef and C++ compatible stuff :)

[FIX] Now works correctly when you don't precise the Sprite/Bg mode (256colors, etc...) Will take the most common mode by default :)


PAGfx Frontend
--------------

[FIX] Now works correctly when you don't precise the Sprite/Bg mode (256colors, etc...) Will take the most common mode by default :) (when importing PAGfx.ini files)

===========================
| v0.81 (thanks benualdo) |
===========================

PAGfx Frontend
--------------

[FIX] Exporting 8bit backgrounds should now work

==================================
| v0.8 (100% by Mario, thanks !) |
==================================

PAGfx
-----

[NEW] Accepts a filename parameter

[FIX] Speed up color conversion

[FIX] Exception if can't open background file 

PAGfx Frontend
--------------

[NEW] Warning if incompatible color depth

[FIX] Exception if saved without palette name

[NEW] Message after loaded

[FIX] Preview updated after load

[FIX] Preview updated after tab change

[NEW] New button 'Update Row'

[FIX] No more minitemp.png

[FIX] Correct aspect ratio in preview

[NEW] Error handling in file access 

=========
| v0.7b |
=========

- Small Bugfixes, shouldn't give errors anymore
- Frontend updated with the correct Parsing code
- Corrected error when first moving some sprites to background view

========
| v0.7 |
========

- Bugfixes

========
| v0.6 |
========

- Fixed a typo : a 16bit bitmap was called Sprite in all_gfx.h
- Added bg_Width and bg_Heigth to the background files
- Removed window resizing, for compatibility issues
- Removed the press key at the end, added a Log generation instead
- Better interaction with PAlib, to use the new PAlib palette functions, DO NOT put a palette name for backgrounds, or give it the background's name...
- PAGC Frontend now deletes the image preview

========
| v0.5 |
========

- Better Text output
- New Frontend, by Kleevah !!
- Completely rewrote lots of the code, new ini format and parser, 
- Added support for RotScale backgrounds, though untested it should work :)
- Faster sprite and background conversions, but tile optimising remains desperately slow for backgrounds of more than 1024x1024
- Simplified the code, reduced it's size (alot)
- Fixed a bug for TileBgs (used to write tons of line breaks for nothing)

========
| v0.3 |
========

- Added support for tiled backgrounds, with tile optimisations (HFlip/VFlip), and DMA Copy for TiledBg (LargeMap is a tilebg without DMACopy optimisation)
- Added support for 8bit bitmaps...
- Added support for 16bit bitmaps ! (with BIT15 set on, not like gfx2gba, so you can do a nice and quick DMA_Copy...)
- Better all_gfx.c and .h files, added palettes in them, organized a little more with comments
- You can now put Magenta or Fuschia, same thing
- Speed up the whole program (like 4 times faster when converting)
- Added error codes for basic ini typos : wrong palette number, wrong file name... That way you know why it crashes :)
- Slightly bigger window size
- Added #ifndef...#endif to the header file

========
| v0.2 |
========

- Adds support for 16bit sprites, tested on emulator + hardware, works :-)
- Now exports all_gfx.c and all_gfx.h, which are a C file and a Header with the declaration of your gfxs, way easier to integrate :op
- Fixed a bug for non-square sprites...

========
| v0.1 |
========

- First versions, converts 256 colors sprites to DS/gba format !