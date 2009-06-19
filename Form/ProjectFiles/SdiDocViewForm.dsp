# Microsoft Developer Studio Project File - Name="SdiDocViewForm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SdiDocViewForm - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SdiDocViewForm.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SdiDocViewForm.mak" CFG="SdiDocViewForm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SdiDocViewForm - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SdiDocViewForm - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SdiDocViewForm - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "..\include" /I "..\..\Win32++" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /i "..\..\Win32++" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /map

!ELSEIF  "$(CFG)" == "SdiDocViewForm - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\..\Win32++" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /i "..\\" /i "\Win32++" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /map

!ENDIF 

# Begin Target

# Name "SdiDocViewForm - Win32 Release"
# Name "SdiDocViewForm - Win32 Debug"
# Begin Group "GuiSourceFiles"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\GuiFiles\Mainfrm.cpp
# End Source File
# Begin Source File

SOURCE=..\GuiFiles\SdiApp.cpp
# End Source File
# Begin Source File

SOURCE=..\GuiFiles\SdiDoc.cpp
# End Source File
# Begin Source File

SOURCE=..\GuiFiles\SdiView.cpp
# End Source File
# End Group
# Begin Group "GuiHeaderFiles"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE="..\..\Win32++\default_resource.h"
# End Source File
# Begin Source File

SOURCE=..\GuiFiles\Mainfrm.h
# End Source File
# Begin Source File

SOURCE=..\GuiFiles\resource.h
# End Source File
# Begin Source File

SOURCE=..\GuiFiles\SdiApp.h
# End Source File
# Begin Source File

SOURCE=..\GuiFiles\SdiDoc.h
# End Source File
# Begin Source File

SOURCE=..\GuiFiles\SdiView.h
# End Source File
# End Group
# Begin Group "Win32++"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\Win32++\Default_Resource.h"
# End Source File
# Begin Source File

SOURCE="..\Win32++\Dialog.h"
# End Source File
# Begin Source File

SOURCE="..\Win32++\Frame.h"
# End Source File
# Begin Source File

SOURCE="..\Win32++\GDI.h"
# End Source File
# Begin Source File

SOURCE="..\Win32++\Rebar.h"
# End Source File
# Begin Source File

SOURCE="..\Win32++\Statusbar.h"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\StdAfx.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\stdafx.h"
# End Source File
# Begin Source File

SOURCE="..\Win32++\Toolbar.h"
# End Source File
# Begin Source File

SOURCE="..\Win32++\WinCore.h"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\GuiFiles\Resource.rc
# End Source File
# End Group
# Begin Group "NonGuiSourceFiles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\document.cpp
# End Source File
# End Group
# Begin Group "NonGuiHeaderFiles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\document.h
# End Source File
# End Group
# End Target
# End Project
