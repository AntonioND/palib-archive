--------------
August 23, 2008
--------------
This is the readme file for the PAlib_080823 plain archive package. It contains three items, please
read them all before installing.


---------------------------
ITEM!  Installing devkitARM
---------------------------
PAlib_080823 works best with the r21 version of the devkitARM toolchain.

For Windows users, the recommended installation procedure is to install the newest version of the
toolchain using the devkitPro installer which is available here:

http://sourceforge.net/project/showfiles.php?group_id=114505&package_id=160396

After installing, navigate to the C:\devkitPro directory and manually delete the entire "devkitARM"
directory from within C:\devkitPro (but leave the others).

Now download devkitARM r21 and install it in place of the one you just deleted. You can get r21
here:

http://sourceforge.net/project/showfiles.php?group_id=114505&package_id=124207&release_id=549080


Your devkitARM toolchain installation is now complete and ready for use with PAlib_080823.



------------------------
ITEM!  PAlib Directories
------------------------
The PAlib default directory structure has been changed, so it's especially important that you
completely remove your old PAlib installation to avoid conflicts or confusion. If you want to
save a backup of the old version, making a zip archive and then completely deleting the original
folder is recommended.

The new directory structure now looks like this:

%INSTALLDIR%                 // C:\devkitPro is recommended, but it's up to you*
   |
   +-PAlib
       |
       +-emulators           // all the emulators go in here
       |   *...
       |
       +-examples            // the usual examples (and some unusual ones!)
       |
       +-lib
       |   +-include         // PAlib header files
       |   +-lib             // the precompiled PAlib library + source
       |
       +-template            // project template ***NEW USERS SHOULD LOOK HERE FIRST***
       |
       +-tools               // extra tools
           +-docs
           +-dsFont
           +-Gif2Frames
           +-ImageSplitter
           +-PAGfx
           +-pagfx-linux
           +-Vham
           +-Video


Because you're not using the Windows installer package, you must set your PAPATH environment
variable manually. In Windows, go to System Properties>>Advanced>>Environment Variables and
either edit the existing PAPATH variable (if there is one) or add a new one (if this is your
first stime installing PAlib). The PAPATH variable should point to the "lib" directory within the
"PAlib" directory. For instance, if you chose to install to C:\devkitPro, your PAPATH environment
variable would be "/c/devkitPro/PAlib/lib" (Windows users: note the Unix-style forward-slashes).

*If you choose to install PAlib to a folder other than C:\devkitPro: the devkitARM toolchain can
remain in its original C:\devkitPro location, in which case external libraries such as MikMod and
liblobby should be installed there as well (NOT IN THE PALIB INSTALL DIRECTORY). Alternately, the
contents of the devkitPro directory (devkitARM, libnds, msys) can be copied to your chosen PAlib
installation folder, but you will have to manually change the devkitPro environment variables
following the same method described above. Change the DEVKITARM and DEVKITPRO paths and also the
to point to the new directory and also edit the general PATH variable where you should change
"C:\devkitPro\msys\bin" to point to the new directory.


-----------------------
ITEM!  Add-on Libraries
-----------------------
In addition to PAlib itself, installation packages ar provided for additional libraries: mikmod,
liblobby and efs_lib. Brief installation instructions for each of them will be given here.

---

mikmod:
To install, extract the "mikmod" directory into your DevkitPro directory (C:\devkitPro or wherever
the DEVKITPRO environment variable points, see the comment above).

For a discussion of mikmod see this gbadev.org topic:

http://forum.gbadev.org/viewtopic.php?t=14710

Also be sure to check the PAlib with mikmod example code in /examples/Sound/mikmod/ for important
information about how to use mikmod in your PAlib project.


---

liblobby:
To install, extract the files into a directory called "liblobby" in your DevkitPro directory
(so that in the "liblobby" directory there are include, lib, etc.).

For a discussion of liblobby see this gbadev.org topic:

http://forum.gbadev.org/viewtopic.php?t=14198

Also be sure to check the liblobby with PAlib example in /examples/Wifi/liblobby for more info.


---

efs_lib:
The efs_lib.c/.h files should be put into your project's source folder alongside your main.c/.cpp
file and the line #include "efs_lib.h" placed after #include <PA9.h> in your source.

For a discussion of efs_lib v2.0 see this gbadev.org topic:

http://forum.gbadev.org/viewtopic.php?t=15726

Also be sure to check the /examples/FAT/EFS_Lib/ example for important information about using
efs_lib with PAlib and see /examples/Sound/ASlib_with_EFSlib for directions on storing MP3
files in EFS and playing them with AS_Lib.


---

Many other development libraries are available for the DS to help with various tasks, as well.
Each should be installed as per its own instructions, keeping in mind any changes you have made to
your environment paths.



-------------------------
ITEM!  Secret Fourth Item
-------------------------
Thanks for reading!  If you're a new user, I hope you find PAlib as useful as I have.  If you're a
long time user just updating, I think you will find the many fixes and additions in this version
worthwhile. If you have any questions or contructive comments or if you just want to discuss DS
development with your fellow PAlib users, please come to:

http://forum.palib.info/index.php?action=forum

Have fun!
