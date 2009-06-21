# Microsoft Developer Studio Project File - Name="DocViewForm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DocViewForm - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DocViewForm.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DocViewForm.mak" CFG="DocViewForm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DocViewForm - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DocViewForm - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DocViewForm - Win32 Release"

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

!ELSEIF  "$(CFG)" == "DocViewForm - Win32 Debug"

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

# Name "DocViewForm - Win32 Release"
# Name "DocViewForm - Win32 Debug"
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

SOURCE="..\Win32++\Toolbar.h"
# End Source File
# Begin Source File

SOURCE="..\Win32++\WinCore.h"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\src\Resource.rc
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=..\src\FormApp.cpp
# End Source File
# Begin Source File

SOURCE=..\src\FormDoc.cpp
# End Source File
# Begin Source File

SOURCE=..\src\FormView.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Mainfrm.cpp
# End Source File
# Begin Source File

SOURCE=..\src\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=..\src\FormApp.h
# End Source File
# Begin Source File

SOURCE=..\src\FormDoc.h
# End Source File
# Begin Source File

SOURCE=..\src\FormView.h
# End Source File
# Begin Source File

SOURCE=..\src\Mainfrm.h
# End Source File
# Begin Source File

SOURCE=..\src\resource.h
# End Source File
# Begin Source File

SOURCE=..\src\stdafx.h
# End Source File
# End Group
# End Target
# End Project
