;PAlib Installer !
;Multilingual
;Written by Mollusk, based on Joost Verburg's multilanguage

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"
  !include "WordFunc.nsh"
;!insertmacro WordFind
!insertmacro WordReplace
;--------------------------------
;General

  ;Name and file
  Name "PAlib Installer"
  OutFile "PAlib.exe"

  ;Default installation folder
  InstallDir "c:\devkitPro"
  
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
 
    
Section "PAlib" section_1
SetOutPath "$INSTDIR\PAlib\"
  FILE /r "C:\devkitPro\PAlib\*.*"
  Push PAPATH
var /GLOBAL INSTDIR2
strcpy $INSTDIR2 $INSTDIR
${WordReplace} "$INSTDIR" "\" "/" "+" $INSTDIR2
  Push "$INSTDIR2/PAlib/"
  ;Push "/c/devkitPro/Palib"
  Call WriteEnvStr
SetOutPath "$INSTDIR\PAlibTemplate"
  FILE /r "C:\devkitPro\PAlibTemplate\*.*"
SectionEnd

Section "Examples" section_2
SetOutPath "$INSTDIR\PAlibExamples\"
  FILE /r "C:\devkitPro\PAlibExamples\*.*"
SectionEnd

Section "English Documentation" section_3
SetOutPath "$INSTDIR\PAlibDoc Eng"
  FILE /r "C:\devkitPro\PAlibDoc Eng\*.*"
SectionEnd

Section "French Documentation" section_4
SetOutPath "$INSTDIR\PAlibDoc Fr"
  FILE /r "C:\devkitPro\PAlibDoc Fr\*.*"
SectionEnd

Section "Shortcuts" shortcuts
WriteIniStr "$INSTDIR\PAlib.url" "InternetShortcut" "URL" "http://www.palib.com"
SectionEnd








  ;Descriptions
 LangString DESC_Section_1 ${LANG_English} "Palib and the default Template, and sets the environnement variable"
 LangString DESC_Section_2 ${LANG_English} "Examples on how to use sprites, backgrounds, etc..."
 LangString DESC_Section_3 ${LANG_English} "English Doc..."
 LangString DESC_Section_4 ${LANG_English} "French Doc..."
 LangString DESC_Shortcuts ${LANG_English} "Shortcut to PAlib.com"
 LangString DESC_Section_1 ${LANG_French} "Palib et le Template par défaut, et ajoute la variable d'environnement"
 LangString DESC_Section_2 ${LANG_French} "Exemples pour l'utilisation des sprites, fonds, etc..."
 LangString DESC_Section_3 ${LANG_French} "Doc en Anglais..."
 LangString DESC_Section_4 ${LANG_French} "Doc Française..."
 LangString DESC_Shortcuts ${LANG_French} "Raccourci vers PAlib.com"

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${section_1} $(DESC_section_1)
  !insertmacro MUI_DESCRIPTION_TEXT ${section_2} $(DESC_section_2)
  !insertmacro MUI_DESCRIPTION_TEXT ${section_3} $(DESC_section_3)
  !insertmacro MUI_DESCRIPTION_TEXT ${section_4} $(DESC_section_4)
  !insertmacro MUI_DESCRIPTION_TEXT ${shortcuts} $(DESC_Shortcuts)
!insertmacro MUI_FUNCTION_DESCRIPTION_END 

;--------------------------------
;Installer Functions

; Language choice...
Function .onInit

  !insertmacro MUI_LANGDLL_DISPLAY

FunctionEnd

; Autoexecute install.bat
Function .onInstSuccess
   SetOutPath "$INSTDIR\PAlib"
   ExecShell open "$INSTDIR\PAlib\install.bat"
   
FunctionEnd
















