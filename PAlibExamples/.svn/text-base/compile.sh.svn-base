
#! /bin/tcsh -f


#The following line removes every .nds file from a folder where i copy the .ds.gba with changed extension to put them directly on SuperCard...
if ($#argv == 1) then
    if ($argv[1] == 'clean') then
         rm -f ~/NDS/devkitPro/SD/*.nds
    else
        echo Wrong parameter. Type \'clean\' to, guest what, clean!.
   exit
    endif
endif

set SUBDIRS = `ls | egrep -v 'makeall'`

   foreach i ($SUBDIRS)
      echo +++++++++++++++++++++++++++++++++++++++++ENTERING FOLDER  $i+++++++++++++++++++++++++++++++++++++++++
      cd $i
      set SUBDIRS2 = `ls`
      foreach j ($SUBDIRS2)
         echo -----------------------------------------------Entering folder $j-----------------------------------------------
         cd $j
         if (-e Makefile) then
            if ($#argv == 1) then
               if ($argv[1] == 'clean') then
                  make clean
               else
                  echo Wrong parameter. Type \'clean\' to, guest what, clean!.
                  exit
               endif
            else
               make
#The following line copies .ds.gba files to another folder and changes its extension to .nds making it easier to transfer them to a SuperCard.
               cp *.ds.gba ~/NDS/devkitPro/SD/$j.nds
            endif
         endif
         echo Exiting $j
         cd ..
      end
      echo Exiting $i
      cd ..
   end 
   
   