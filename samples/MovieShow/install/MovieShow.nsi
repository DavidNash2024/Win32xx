

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

  Unicode true

;--------------------------------
;General

  ;Name and file
  Name "MovieShow"
  OutFile "SetupMovieShow.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES64\MovieShow"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKLM "Software\Win32++\MovieShow" "Install_Dir"

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin

;--------------------------------
;Interface Configuration

  !define MUI_ICON "..\src\res\Curtains.ico"
  !define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\win-uninstall.ico"
  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "MovieShow (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  File "files\MovieShow.exe"
  File "files\MediaInfo.dll"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\NSIS_Example2 "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MovieShow" "DisplayName" "MovieShow"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MovieShow" "Publisher" "David Nash"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MovieShow" "DisplayIcon" "$INSTDIR\MovieShow.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MovieShow" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MovieShow" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MovieShow" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\MovieShow"
  CreateShortcut "$SMPROGRAMS\MovieShow\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortcut "$SMPROGRAMS\MovieShow\MovieShow.lnk" "$INSTDIR\MovieShow.exe" "" "$INSTDIR\MovieShow.exe" 0
  CreateShortCut "$DESKTOP\Movie Show.lnk" "$INSTDIR\MovieShow.exe" ""
  
SectionEnd

 
;--------------------------------
;Uninstaller Section

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MovieShow"
  DeleteRegKey HKLM "Software\Win32++\MovieShow"

  ; Remove files and uninstaller
  Delete $INSTDIR\MovieShow.exe
  Delete $INSTDIR\MediaInfo.dll
  Delete $INSTDIR\uninstall.exe
  Delete $APPDATA\Win32++\MovieShow\MovieData.bin

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\MovieShow\*.*"
  Delete "$DESKTOP\Movie Show.lnk"

  ; Remove directories used
  RMDir "$SMPROGRAMS\MovieShow"
  RMDir "$INSTDIR"
  RMDir "$APPDATA\Win32++\MovieShow"

SectionEnd