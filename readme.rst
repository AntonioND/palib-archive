PAlib archive
=============

PAlib (Programmer's Arsenal library) is a library to develop programs for the
Nintendo DS. It was developed on top of libnds, and it gives the users a simpler
API to interact with the hardware (graphics, audio, touchscreen, etc).

There are far better options nowadays, so it shouldn't be used. This repository
is only available to preserve the history of this library.

PAlib was initially hosted at SourceForge (2006 - 2007):

https://sourceforge.net/projects/pands/files/

It was later moved to Google Code (2008), which has also survived until 2022.

https://code.google.com/archive/p/palibds/

The last few releases were made in a website called www.palib-dev.com, but they
can no longer be downloaded because the website is dead, and the web archive
didn't preserve them:

http://web.archive.org/web/20110902055959if_/http://www.palib-dev.com/index.php

This repository contains an archive of all released versions of PAlib, as well
as all of the Google Code SVN repository history (converted to GIT). It also
contains version 100707.

**Important note**: This repository is missing the following versions mentioned
in the changelog:

- PAlib 0912XX STABLE beta
- PAlib 090801 beta
- PAlib 090621 beta
- PAlib 090613 beta
- PAlib 090601 beta
- PAlib 090509 beta
- PAlib beta 090408
- PAlib 080313
- PALib_CommunityUpdate_BETA-080203

If anyone knows where to find them, please, raise an issue in this repository,
or send them to me directly.

Branches
--------

There are a few branches available in this repository. Note that they don't
share an initial commit, so the history of this repository is a bit weird.

- ``releases/sourceforge``

  All releases available in SourceForge. They have been taken from the 7z, zip
  or installer versions, as the authors were a bit inconsistent and it isn't
  possible to find all versions of PAlib in all of the three formats.

  They have been uncompressed and left as they are, including files like
  ``Thumbs.db`` or copies of files, as the purpose of this repository is to
  preserve its history.

  The releases can all be found here:

  https://sourceforge.net/projects/pands/files/

- ``releases/googlecode``

  The only available release in Google Code. This branch is on top of
  ``releases/sourceforge``. You can find the Google Code releases here:

  https://code.google.com/archive/p/palibds/downloads

- ``releases/all``

  It combines ``releases/sourceforge``, ``releases/googlecode`` and it adds
  version 100707 on top.

- ``svn/trunk``, ``svn/PAGfx``, ``svn/dkAr24_Compatibility_noobey``

  All the branches of the Google Code SVN repository (the ones that actually
  had code).

  https://code.google.com/archive/p/palibds/source

Other files
-----------

The archived repository https://code.google.com/archive/p/palib-tutorial-pdf/
contains the PAlib wiki tutorial preserved in PDF format. You can find it in
this repository in folder ``palib-tutorial-pdf``.
