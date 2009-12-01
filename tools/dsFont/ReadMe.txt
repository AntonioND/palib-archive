-----------------------------------------------------------------
                dsFont for PAlib by Thomas Schöps
-----------------------------------------------------------------

INFO:
-----

   dsFont is a Tool which creates a Bitmap (*.bmp) file from an arbitrary font installed on your PC.
   Credits go to me and to David Scherfgen, as the general process is based on his CreateBitmapFont tool
   for the TriBase engine. dsFont uses FreeImage to save the output file.
   Don't expect a perfect font output from it, it should rather give you templates to edit with
   a graphics program, for example The Gimp.

USAGE:
------

 * To create a new font, first set the settings at the left side of the program.
   It's most important to keep the font small so that it fits into the grid (this is sized 7x7 if you create a 8x8 tiled font;
   7x7 because 2 borders of the 8x8 grid should remain transparent so that neighboring characters don't merge).

 * If you specify only the width or only the height of the font, the other value will be chosen by Windows
   so that the proportions remain original.

 * With "@Arial Unicode MS", for example, it's ok to set the height to 12 and leave the width field blank
   if you want to create an 8x8 tiled font.

 * You can also try the Autosize feature, which tries to find the best size for the selected font.
   "Autosize (proportional)" selects the best height and leaves the width field blank (-> original proportions),
   "Autosize (full)" selects both height and width.

 * If the option "Automatic size overflow correction" is set, the program tries to cut letters
   which are just a little too big. This can save you some work, but may screw some letters totally up at the borders.

 * You can choose between 5 different coloring modes:
   - Color gradient: Apply a top->down color gradient
   - 3D Appearance: Works only right with bold fonts (or with fonts bigger than 8x8)
   - Image: Take the colors for the font from an image. If it is too small, it will wrap around.
   - Color gradient over image: Same as Image, but with a color gradient on top of it. Specify the gradient colors relative,
     i.e. RGB(0, 0, 0) means the color of the image and RGB(100, 100, 100) is brighter, RGB(-100, -100, -100) is darker.
   - 3D Appearance over image: Same as "Color gradient over image", but with 3D.

 * For the last 3 color modes, you have to use a background image. Choose it with the "Browse ..." button.

 * You can set 3 color values in the program:
   - the background color
   - top color: the first color in the color gradient OR the upper left color for 3D
   - sub color: the last color in the color gradient (set it to the same as the top color to use only a single color)
                OR the lower right color for 3D

 * With "color steps", you can adjust in how many steps the sub color should be reached (only for color gradients).
   Enter how many different color shades you want.
   Set it to the tile size to get a smooth transition, to 1 to take only the top color.

 * You can save and load the settings with the two bottom-right buttons.

 * To get a preview of the output, you can use the "image preview (once)" button.
   To take a closer look at the preview, use the 2x and 4x Zoom and drag the preview around with the mouse.

 * After you saved the font with "Save bitmap", dsFont outputs the characters which didn't fit into the grid
   in the "Log" window (can be quite much if you export all characters).
   You should look with a graphics program, e.g. The Gimp, if you can correct these.
   You should also take a look at the other characters, for example y, g, p and q,
   as the algorithm to fit them into the grid often sets them too high so that they look like capital letters.
   Then you can use the bitmap as a font on the DS.

 * If you can't decide which font to use, you can try the "Save all" button, which loops through all fonts,
   applies Autosize and saves them with the active color and tile settings. Then you can decide by the output
   which font to use, but be careful: this might take several minutes to complete!

CHANGELOG:
----------

version 0.3a:
-------------

- Added: 2 examples
- Added: 4th export mode (export chars from a defined range)
- Added: Possibility to export all fonts with the same settings into a folder. Use this with caution.
- Changed: Autosize runs faster.
- Fixed: Autosize suggests reasonable sizes for 16x16 fonts

version 0.3:
------------

- Added: The output tile size can now be set from 1 (senseless) to 16. Set it to 8 for a tiled font or small 8bit font, or to 16 for a big 8bit font.
- Added: Option for another horizontal alignment: Characters can now be placed in the middle of the tile. I think this must be turned off if you want to use the font as an 8bit font.
- Added: If using the "All chars" or "Normal chars" export option, characters can be excluded by typing them into the exclude field (or copy-and-paste them from the log window).
- Changed: The output image now has a variable height depending on the tile size.
- Changed: UI changed again (bigger preview window, all setting blocks are in one column)
- Changed: The default save format is now png instead of bmp, as it is much smaller.
- Fixed: The calculation of the difference between real and optimal width for the log window actually used the real height, not the real width.

version 0.2:
------------

- Added: Other image formats as output supported (most important: png, tga).
- Added: Other coloring modes than only Color gradient: 3D, Image, Color gradient over image, 3D over image.
- Added: Information output by the Autosize features.
- Added: As the number of settings grew and grew, the settings can now be saved and loaded.
- Added: Standard Color Picker dialog to choose the colors.
- Added: The Font Preview window can be used to preview the output image.
- Added: 2x and 4x Zoom for the preview window.
- Changed: "Color Step" was changed to "Color Steps" and now means the number of shades in one tile.
- Changed: Double Buffering for the preview window - no more flickering.
- Changed: UI rearranged, much cleaner now.
- Fixed: Some bugfixes.

version 0.1:
------------

- Initial release in the PAlib Forums.