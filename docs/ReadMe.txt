PAlib documentation folder
==========================

Instructions for normal users
-----------------------------

This directory contains the PAlib documentation in two languages,
English and French. Just go to the subfolder of your choice to
pick a language.

Each language subfolder has three versions of the documentation:
  - PAlib.chm        - The CHM version for use with Windows.
  - PAlib.pdf        - The printable PDF version.
  - /html/ subfolder - The browsable HTML version.

Instructions for developers
---------------------------

The folder dev contains tools/scripts to generate the documentation.
By default it's set up for a Windows system with PAlib installed in
the folder C:\devkitPro\PAlib, but you can adapt it by:

  - Changing the STRIP_FROM_PATH and the HHC_LOCATION variable.
  - Disabling the .chm creation support (GENERATE_HTMLHELP = NO)
    because it depends on a Windows executable.

To generate the documentation you need the following installed:
  - Doxygen
  - MikTeX on Windows or pdfTeX on other OSes

Just run the appropiate batch file on Windows or follow these steps:
  - Run "doxygen Doxyfile_[Language]" where Language is the language
    you want to generate the documentation for.
  - Run make on the [Language]/latex folder.
