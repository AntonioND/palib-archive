;PAlib Installer !
;Multilingual
;Written by Mollusk and Delfare, based on Joost Verburg's multilanguage
;Updated by Scognito and Sweater Fish

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"
  !include "WordFunc.nsh"
;  !insertmacro WordFind
  !insertmacro WordReplace

;--------------------------------
;General

  ;Name and file
  Name "PAlib_080823 Installer"
  OutFile "PAlib_080823 Installer.exe"

  ;Default installation folder
  InstallDir "C:\devkitPro"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\PAlib" ""

  ;Compression -> 7z
  SetCompressor /SOLID lzma 

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Language Selection Dialog Settings

  ;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\PAlib" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English" # first language is the default language
  !insertmacro MUI_LANGUAGE "French"
  !insertmacro MUI_LANGUAGE "German"
  !insertmacro MUI_LANGUAGE "Spanish"
  !insertmacro MUI_LANGUAGE "Polish"

;--------------------------------
;Reserve Files
  
  ;These files should be inserted before other files in the data block
  ;Keep these lines before any File command
  ;Only for solid compression (by default, solid compression is enabled for BZIP2 and LZMA)
  
  !insertmacro MUI_RESERVEFILE_LANGDLL
  ;!include "WordFunc.nsh"
  !include WriteEnvStr.nsh # or the name you chose


;--------------------------------
 ;Language Strings
 
    
Section "PAlib" PAlib
SetOutPath "$INSTDIR\palib\"
  FILE /r "C:\DS\dev\Installer\palib\PAlib ChangeLog.txt"
SetOutPath "$INSTDIR\palib\lib\"
  FILE /r "C:\DS\dev\Installer\palib\lib\*.*"
  Push PAPATH
var /GLOBAL INSTDIR2
strcpy $INSTDIR2 $INSTDIR
${WordReplace} "$INSTDIR" "\" "/" "+" $INSTDIR2
  Push "$INSTDIR2/palib/lib"
  Call WriteEnvStr

SetOutPath "$INSTDIR\palib\template"
  FILE /r "C:\DS\dev\Installer\palib\template\*.*"
SectionEnd

Section "Examples" Examples
SetOutPath "$INSTDIR\palib\examples\"
  FILE /r "C:\DS\dev\Installer\palib\examples\*.*"
SectionEnd

SectionGroup "PAlib Documentation" Docs
Section "English Doc" Docs_En
SetOutPath "$INSTDIR\palib\tools\PAlibDocs\eng"
  FILE /r "C:\DS\dev\Installer\palib\tools\PAlibDocs\eng\*.*"

CreateShortCut "$INSTDIR\palib\PAlib Docs (English).lnk" "$INSTDIR\palib\tools\PAlibDocs\eng\index.html"

;SetOutPath "$INSTDIR\palib"
;  FILE /r "C:\DS\dev\Installer\palib\PAlib Docs (English).html"
SectionEnd

Section "French Doc" Docs_Fr
SetOutPath "$INSTDIR\palib\tools\PAlibDocs\fr"
  FILE /r "C:\DS\dev\Installer\palib\tools\PAlibDocs\fr\*.*"

CreateShortCut "$INSTDIR\palib\PAlib Docs (French).lnk" "$INSTDIR\palib\tools\PAlibDocs\fr\index.html"

;SetOutPath "$INSTDIR\palib"
;  FILE /r "C:\DS\dev\Installer\palib\PAlib Docs (French).html"
SectionEnd
SectionGroupEnd

SectionGroup "Emulators" Emulators
Section "No$GBA" NoCash
SetOutPath "$INSTDIR\palib\emulators\no$gba"
  FILE /r "C:\DS\dev\Installer\palib\emulators\no$gba\*.*"
SectionEnd

Section "iDeaS (Windows)" Ideas_Win
SetOutPath "$INSTDIR\palib\emulators\ideas-win32"
  FILE /r "C:\DS\dev\Installer\palib\emulators\ideas-win32\*.*"
SectionEnd

Section "iDeaS (Linux)" Ideas_Lin
SetOutPath "$INSTDIR\palib\emulators\ideas-linux"
  FILE /r "C:\DS\dev\Installer\palib\emulators\ideas-linux\*.*"
SectionEnd

Section "DeSmuME (Windows)" Desmume_Win
SetOutPath "$INSTDIR\palib\emulators\desmume-win32"
  FILE /r "C:\DS\dev\Installer\palib\emulators\desmume-win32\*.*"
SectionEnd

Section "DeSmuME (Linux)" Desmume_Lin
SetOutPath "$INSTDIR\palib\emulators\desmume-linux"
  FILE /r "C:\DS\dev\Installer\palib\emulators\desmume-linux\*.*"
SectionEnd

Section "NeonDS" NeonDS
SetOutPath "$INSTDIR\palib\emulators\neonds"
  FILE /r "C:\DS\dev\Installer\palib\emulators\neonds\*.*"
SectionEnd
SectionGroupEnd

Section "VisualHAM" Vham
SetOutPath "$INSTDIR\palib\tools\vham"
  FILE /r "C:\DS\dev\Installer\palib\tools\vham\*.*"

  ; back up old value of .vhw
!define Index "Line${__LINE__}"
  ReadRegStr $1 HKCR ".vhw" ""
  StrCmp $1 "" "${Index}-NoBackup"
    StrCmp $1 "OptionsFile" "${Index}-NoBackup"
    WriteRegStr HKCR ".vhw" "backup_val" $1
"${Index}-NoBackup:"
  WriteRegStr HKCR ".vhw" "" "OptionsFile"
  ReadRegStr $0 HKCR "OptionsFile" ""
  StrCmp $0 "" 0 "${Index}-Skip"
	WriteRegStr HKCR "OptionsFile" "" "Program Options File"
	WriteRegStr HKCR "OptionsFile\shell" "" "open"
	WriteRegStr HKCR "OptionsFile\DefaultIcon" "" "$INSTDIR\palib\tools\vham\vham.exe,0"
"${Index}-Skip:"
  WriteRegStr HKCR "OptionsFile\shell\open\command" "" \
    '$INSTDIR\palib\tools\vham\vham.exe "%1"'
  WriteRegStr HKCR "OptionsFile\shell\edit" "" "Edit Options File"
  WriteRegStr HKCR "OptionsFile\shell\edit\command" "" \
    '$INSTDIR\palib\tools\vham\vham.exe "%1"'
!undef Index
  ; Rest of script 

System::Call 'Kernel32::SetEnvironmentVariableA(t, t) i("PAPATH", "$INSTDIR2/palib/lib/").r0'
SetOutPath "$INSTDIR\palib\tools\vham\config"
  ExecShell open "$INSTDIR\palib\tools\vham\config\VhamInit.exe"

SectionEnd

SectionGroup "Other Tools" Other_Tools
Section "dsFont" dsFont
SetOutPath "$INSTDIR\palib\tools\dsFont"
  FILE /r "C:\DS\dev\Installer\palib\tools\dsFont\*.*"
SectionEnd

Section "GIF2Frames" Gif2frames
SetOutPath "$INSTDIR\palib\tools\Gif2Frames"
  FILE /r "C:\DS\dev\Installer\palib\tools\Gif2Frames\*.*"
SectionEnd

Section "ImageSplitter" ImageSplitter
SetOutPath "$INSTDIR\palib\tools\ImageSplitter"
  FILE /r "C:\DS\dev\Installer\palib\tools\ImageSplitter\*.*"
SectionEnd

Section "PAGfx (Windows)" PA_Gfx
SetOutPath "$INSTDIR\palib\tools\PAGfx"
  FILE /r "C:\DS\dev\Installer\palib\tools\PAGfx\*.*"
SectionEnd

Section "PAGfx (Linux)" PA_Gfx_Lin
SetOutPath "$INSTDIR\palib\tools\PAGfx-Linux"
  FILE /r "C:\DS\dev\Installer\palib\tools\PAGfx-Linux\*.*"
SectionEnd

Section "viDeoconverterS" Video
SetOutPath "$INSTDIR\palib\tools\video"
  FILE /r "C:\DS\dev\Installer\palib\tools\video\*.*"
SectionEnd
SectionGroupEnd

SectionGroup "Additional Libs" Add_Libs
Section "EFS Lib" EFS_Lib
SetOutPath "$INSTDIR\Other Libs"
  FILE /r "C:\DS\dev\Installer\Other Libs\efs_lib_v2.zip"
SectionEnd
Section "liblobby" liblobby
SetOutPath "$INSTDIR\Other Libs"
  FILE /r "C:\DS\dev\Installer\Other Libs\liblobby-svn-2007-12-03.zip"
SectionEnd
Section "mikmod" mikmod
SetOutPath "$INSTDIR\Other Libs"
  FILE /r "C:\DS\dev\Installer\Other Libs\mikmod-3.2.0-ds1.zip"
SectionEnd
SectionGroupEnd

Section "Shortcuts" Shortcuts
WriteIniStr "$INSTDIR\palib\PAlib Website.url" "InternetShortcut" "URL" "http://www.palib.info"
SectionEnd






  ;Descriptions
LangString DESC_PAlib ${LANG_English} "Palib with source code and project template"
LangString DESC_Examples ${LANG_English} "Example code showing how to use sprites, backgrounds and much more with PAlib"
LangString DESC_Docs ${LANG_English} "PAlib documentation generated by Doxygen"
 LangString DESC_Docs_En ${LANG_English} "Docs in English"
 LangString DESC_Docs_Fr ${LANG_English} "Docs in French"
LangString DESC_Emulators ${LANG_English} "For testing your project on a computer"
 LangString DESC_NoCash ${LANG_English} "No$GBA v2.6 (freeware version)"
 LangString DESC_Ideas_Win ${LANG_English} "iDeaS 1.0.2.4 for Windows"
 LangString DESC_Ideas_Lin ${LANG_English} "iDeaS 1.0.2.4 for Linux"
 LangString DESC_Desmume_Win ${LANG_English} "DeSmuME 0.7.2 for Windows"
 LangString DESC_Desmume_Lin ${LANG_English} "DeSmuME 0.7.0 for Linux"
 LangString DESC_NeonDS ${LANG_English} "NeonDS 0.1.1"
LangString DESC_Vham ${LANG_English} "VisualHAM development environment"
LangString DESC_Other_Tools ${LANG_English} "Other tools to help you"
 LangString DESC_dsFont ${LANG_English} "Create and convert custom fonts for easy importing into your project"
 LangString DESC_Gif2frames ${LANG_English} "Extract individual frames from an animated GIF"
 LangString DESC_ImageSplitter ${LANG_English} "Split larger images up into smaller pieces for creating large sprites"
 LangString DESC_PA_Gfx ${LANG_English} "Create sprite, background and texture files ready for easy importing into your project"
 LangString DESC_PA_Gfx_Lin ${LANG_English} "Linux version of PAGfx"
 LangString DESC_Video ${LANG_English} "Convert small videos for easy (?) importing into your project"
LangString DESC_Add_Libs ${LANG_English} "Additional Libs"
 LangString DESC_EFS_Lib ${LANG_English} "EFS Lib 2.0"
 LangString DESC_liblobby ${LANG_English} "liblobby 2007-12-03"
 LangString DESC_mikmod ${LANG_English} "mikmod 3.2.0-ds1"
LangString DESC_Shortcuts ${LANG_English} "Shortcut to PAlib.info website and forums"


LangString DESC_PAlib ${LANG_French} "PAlib avec code source et template"
LangString DESC_Examples ${LANG_French} "Exemples de code montrant comment utiliser les sprites, les fonds et beaucoup d'autres choses avec PAlib"
LangString DESC_Docs ${LANG_French} "Documentation PAlib générée avec Doxygen"
 LangString DESC_Docs_En ${LANG_French} "Documentation en Anglais"
 LangString DESC_Docs_Fr ${LANG_French} "Documentation en Français"
LangString DESC_Emulators ${LANG_French} "Testez vos projets sur ordinateur"
 LangString DESC_NoCash ${LANG_French} "No$GBA v2.6 (version freeware)"
 LangString DESC_Ideas_Win ${LANG_French} "iDeaS 1.0.2.4 pour Windows"
 LangString DESC_Ideas_Lin ${LANG_French} "iDeaS 1.0.2.4 pour Linux"
 LangString DESC_Desmume_Win ${LANG_French} "DeSmuME 0.7.2 pour Windows"
 LangString DESC_Desmume_Lin ${LANG_French} "DeSmuME 0.7.0 pour Linux"
 LangString DESC_NeonDS ${LANG_French} "NeonDS 0.1.1"
LangString DESC_Vham ${LANG_French} "Environnement de développement VisualHAM "
LangString DESC_Other_Tools ${LANG_French} "Autres outils pour vous aider"
 LangString DESC_dsFont ${LANG_French} "Créer et convertir des polices personnalisées pour un import facile dans vos projets"
 LangString DESC_Gif2frames ${LANG_French} "Extrait les images d'un GIF animé"
 LangString DESC_ImageSplitter ${LANG_French} "Découpe de grandes images en petits morceaux pour faire de grands sprites"
 LangString DESC_PA_Gfx ${LANG_French} "Créer des fichiers de sprite, fond et texture prêts à être facilement importés dans votre projet"
 LangString DESC_PA_Gfx_Lin ${LANG_French} "Version linux de PAGfx"
 LangString DESC_Video ${LANG_French} "Convertir de petites vidéos pour un import facile (?) dans votre projet"
LangString DESC_Add_Libs ${LANG_French} "Additional Libs"
 LangString DESC_EFS_Lib ${LANG_French} "EFS Lib 2.0"
 LangString DESC_liblobby ${LANG_French} "liblobby 2007-12-03"
 LangString DESC_mikmod ${LANG_French} "mikmod 3.2.0-ds1"
LangString DESC_Shortcuts ${LANG_French} "Raccourci vers le site PAlib.info et ses forums"


LangString DESC_PAlib ${LANG_German} "PAlib mit Quelltext und Vorlage"
LangString DESC_Examples ${LANG_German} "PAlib Beispiel Quelltext zur nutzung von Hintergründen, Sprites und noch vielem mehr"
LangString DESC_Docs ${LANG_German} "Doxygen generierte PAlib Dokumentation"
 LangString DESC_Docs_En ${LANG_German} "Dokumentation in English"
 LangString DESC_Docs_Fr ${LANG_German} "Dokumentation in French"
LangString DESC_Emulators ${LANG_German} "Zum testen deiner Projekte auf deinem Computer"
 LangString DESC_NoCash ${LANG_German} "No$GBA v2.6 (freeware version) für Windows"
 LangString DESC_Ideas_Win ${LANG_German} "iDeaS 1.0.2.4 für Windows"
 LangString DESC_Ideas_Lin ${LANG_German} "iDeaS 1.0.2.4 für Linux"
 LangString DESC_Desmume_Win ${LANG_German} "DeSmuME 0.7.2 für Windows"
 LangString DESC_Desmume_Lin ${LANG_German} "DeSmuME 0.7.0 für Linux"
 LangString DESC_NeonDS ${LANG_German} "NeonDS 0.1.1"
LangString DESC_Vham ${LANG_German} "VisualHAM Entwicklungsumgebung (Editor)"
LangString DESC_Other_Tools ${LANG_German} "andere nützliche Programme"
 LangString DESC_dsFont ${LANG_German} "Erstelle und konventiere eigene Schriftarten zum einen verwenden in deinem Projekt"
 LangString DESC_Gif2frames ${LANG_German} "Extrahieren einzelner Bilder aus animierten GIF-Dateien"
 LangString DESC_ImageSplitter ${LANG_German} "Zerteile große Bilder in kleine Stücke damit du große Bilder verwenden kannst"
 LangString DESC_PA_Gfx ${LANG_German} "Erstelle Hintergrund, Texturen und Sprites zur einfachen verwendung in deinem Projekt"
 LangString DESC_PA_Gfx_Lin ${LANG_German} "Linux version von PAGfx"
 LangString DESC_Video ${LANG_German} "Konventiere kleine Filme um sie in deinem Projekt verwenden zu können"
LangString DESC_Add_Libs ${LANG_German} "Additional Libs"
 LangString DESC_EFS_Lib ${LANG_German} "EFS Lib 2.0"
 LangString DESC_liblobby ${LANG_German} "liblobby 2007-12-03"
 LangString DESC_mikmod ${LANG_German} "mikmod 3.2.0-ds1"
LangString DESC_Shortcuts ${LANG_German} "Verknüpfung zur PALib Webseite und dem PAlib-Forum"


LangString DESC_PAlib ${LANG_Spanish} "PAlib con código fuente y plantilla"
LangString DESC_Examples ${LANG_Spanish} "Código de ejemplo que muestra cómo usar los sprites, los fondos y muchas cosas más"
LangString DESC_Docs ${LANG_Spanish} "Documentación de PAlib"
 LangString DESC_Docs_En ${LANG_Spanish} "Documentación en inglés"
 LangString DESC_Docs_Fr ${LANG_Spanish} "Documentación en francés"
LangString DESC_Emulators ${LANG_Spanish} "Emuladores"
 LangString DESC_NoCash ${LANG_Spanish} "No$GBA v2.6 (versión freeware) para Windows"
 LangString DESC_Ideas_Win ${LANG_Spanish} "iDeaS 1.0.2.4 para Windows"
 LangString DESC_Ideas_Lin ${LANG_Spanish} "iDeaS 1.0.2.4 para Linux"
 LangString DESC_Desmume_Win ${LANG_Spanish} "DeSmuME 0.7.2 para Windows"
 LangString DESC_Desmume_Lin ${LANG_Spanish} "DeSmuME 0.7.0 para Linux"
 LangString DESC_NeonDS ${LANG_Spanish} "NeonDS 0.1.1"
LangString DESC_Vham ${LANG_Spanish} "Entorno de desarrollo VisualHAM"
LangString DESC_Other_Tools ${LANG_Spanish} "Otras utilidades"
 LangString DESC_dsFont ${LANG_Spanish} "Crea y convierte fuentes para fácilmente integrarlas en tu proyecto"
 LangString DESC_Gif2frames ${LANG_Spanish} "Extrae los diferentes fotogramas de un archivo GIF"
 LangString DESC_ImageSplitter ${LANG_Spanish} "Separa grandes images en pequeñas partes para poder hacer sprites grandes"
 LangString DESC_PA_Gfx ${LANG_Spanish} "Convierte sprites, fondos y texturas al formato de PAlib para fácilmente incluirlos en tu proyecto"
 LangString DESC_PA_Gfx_Lin ${LANG_Spanish} "Versión para Linux de PAGfx"
 LangString DESC_Video ${LANG_Spanish} "Convierte vídeos pequeños para fácil inclusión en tu proyecto"
LangString DESC_Add_Libs ${LANG_Spanish} "Additional Libs"
 LangString DESC_EFS_Lib ${LANG_Spanish} "EFS Lib 2.0"
 LangString DESC_liblobby ${LANG_Spanish} "liblobby 2007-12-03"
 LangString DESC_mikmod ${LANG_Spanish} "mikmod 3.2.0-ds1"
LangString DESC_Shortcuts ${LANG_Spanish} "Acceso directo a PAlib.info y a los foros"


LangString DESC_PAlib ${LANG_Polish} "Palib z zródlo kodu, plus Szablon"
LangString DESC_Examples ${LANG_Polish} "Example code showing how to use sprites, backgrounds, and lots more with PAlib"
LangString DESC_Docs ${LANG_Polish} "PAlib  documentacja wyprodukowal przez Doxygen"
 LangString DESC_Docs_En ${LANG_Polish} "Documentacja W Jezyku angielskim"
 LangString DESC_Docs_Fr ${LANG_Polish} "Documentacja W Jezyku francuskim"
LangString DESC_Emulators ${LANG_Polish} "Próbuja wasz projekt na komputerze"
 LangString DESC_NoCash ${LANG_Polish} "No$GBA v2.6 (freeware version) dla Windows"
 LangString DESC_Ideas_Win ${LANG_Polish} "iDeaS 1.0.2.4 dla Windows"
 LangString DESC_Ideas_Lin ${LANG_Polish} "iDeaS 1.0.2.4 dla Linux"
 LangString DESC_Desmume_Win ${LANG_Polish} "DeSmuME 0.7.2 for Windows"
 LangString DESC_Desmume_Lin ${LANG_Polish} "DeSmuME 0.7.0 for Linux"
 LangString DESC_NeonDS ${LANG_Polish} "NeonDS 0.1.1"
LangString DESC_Vham ${LANG_Polish} "VisualHAM Srodowisko rozwoju (opracowywanie)"
LangString DESC_Other_Tools ${LANG_Polish} "Inne narzedzia pomagac wam poza"
 LangString DESC_dsFont ${LANG_Polish} "Tworza i konwertuja celne asortymenty czcionek dla prostego importowania do waszych projektów"
 LangString DESC_Gif2frames ${LANG_Polish} "Wyciagaja poszczególna osoba (indywidualny) ksztaltuje (struktura) od zywego GIF"
 LangString DESC_ImageSplitter ${LANG_Polish} "Rozbijane wieksze obrazy do mniejszych czesci dla stworzenia wielkiego sprites"
 LangString DESC_PA_Gfx ${LANG_Polish} "Tworza *sprite*, tlo i struktura rejestruje gotowy dla prostego importowania do waszego projekt (projektowy)"
 LangString DESC_PA_Gfx_Lin ${LANG_Polish} "Linux Wersja PAGfx"
 LangString DESC_Video ${LANG_Polish} "Konwertuja maly wideomagnetofon dla prostego (?) importujacy (importowanie) do waszego projekt (projektowy)"
LangString DESC_Add_Libs ${LANG_Polish} "Additional Libs"
 LangString DESC_EFS_Lib ${LANG_Polish} "EFS Lib 2.0"
 LangString DESC_liblobby ${LANG_Polish} "liblobby 2007-12-03"
 LangString DESC_mikmod ${LANG_Polish} "mikmod 3.2.0-ds1"
LangString DESC_Shortcuts ${LANG_Polish} "Skrócenie PAlib.info website i fora"


!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${PAlib} $(DESC_PAlib)
  !insertmacro MUI_DESCRIPTION_TEXT ${Examples} $(DESC_Examples)
  !insertmacro MUI_DESCRIPTION_TEXT ${Docs} $(DESC_Docs)
  !insertmacro MUI_DESCRIPTION_TEXT ${Docs_En} $(DESC_Docs_En)
  !insertmacro MUI_DESCRIPTION_TEXT ${Docs_Fr} $(DESC_Docs_Fr)
  !insertmacro MUI_DESCRIPTION_TEXT ${Emulators} $(DESC_Emulators)
  !insertmacro MUI_DESCRIPTION_TEXT ${NoCash} $(DESC_NoCash)
  !insertmacro MUI_DESCRIPTION_TEXT ${Ideas_Win} $(DESC_Ideas_Win)
  !insertmacro MUI_DESCRIPTION_TEXT ${Ideas_Lin} $(DESC_Ideas_Lin)
  !insertmacro MUI_DESCRIPTION_TEXT ${Desmume_Win} $(DESC_Desmume_Win)
  !insertmacro MUI_DESCRIPTION_TEXT ${Desmume_Lin} $(DESC_Desmume_Lin)
  !insertmacro MUI_DESCRIPTION_TEXT ${NeonDS} $(DESC_NeonDS)
  !insertmacro MUI_DESCRIPTION_TEXT ${Vham} $(DESC_Vham)
  !insertmacro MUI_DESCRIPTION_TEXT ${Other_Tools} $(DESC_Other_Tools)
  !insertmacro MUI_DESCRIPTION_TEXT ${dsFont} $(DESC_dsFont)
  !insertmacro MUI_DESCRIPTION_TEXT ${Gif2frames} $(DESC_Gif2frames)
  !insertmacro MUI_DESCRIPTION_TEXT ${ImageSplitter} $(DESC_ImageSplitter)
  !insertmacro MUI_DESCRIPTION_TEXT ${PA_Gfx} $(DESC_PA_Gfx)
  !insertmacro MUI_DESCRIPTION_TEXT ${PA_Gfx_Lin} $(DESC_PA_Gfx_Lin)
  !insertmacro MUI_DESCRIPTION_TEXT ${Video} $(DESC_Video)
  !insertmacro MUI_DESCRIPTION_TEXT ${Add_Libs} $(DESC_Add_Libs)
  !insertmacro MUI_DESCRIPTION_TEXT ${EFS_Lib} $(DESC_EFS_Lib)
  !insertmacro MUI_DESCRIPTION_TEXT ${liblobby} $(DESC_liblobby)
  !insertmacro MUI_DESCRIPTION_TEXT ${mikmod} $(DESC_mikmod)
  !insertmacro MUI_DESCRIPTION_TEXT ${Shortcuts} $(DESC_Shortcuts)
!insertmacro MUI_FUNCTION_DESCRIPTION_END 

;--------------------------------
;Installer Functions

; Language choice...
Function .onInit

  !insertmacro MUI_LANGDLL_DISPLAY

FunctionEnd

; Autoexecute install.bat
;Function .onInstSuccess
;
;   SetOutPath "$INSTDIR\PAlib"
;   ExecShell open "$INSTDIR\PAlib\install.bat"
;   
;FunctionEnd
















