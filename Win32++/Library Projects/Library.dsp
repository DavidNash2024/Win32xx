# Microsoft Developer Studio Project File - Name="Library" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Library - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Library.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Library.mak" CFG="Library - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Library - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Library - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Library - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\VS6\Win32xxR.lib"

!ELSEIF  "$(CFG)" == "Library - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\VS6\Win32xxD.lib"

!ENDIF 

# Begin Target

# Name "Library - Win32 Release"
# Name "Library - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE="..\..\Win32++\src\dialog.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\docking.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\frame.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\gdi.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\listview.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\mdi.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\propertysheet.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\rebar.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\socket.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\splitter.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\statusbar.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\tab.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\toolbar.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\treeview.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\wincore.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE="..\..\Win32++\src\dialog.h"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\docking.h"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\frame.h"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\gdi.h"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\listview.h"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\mdi.h"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\propertysheet.h"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\rebar.h"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\socket.h"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\splitter.h"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\statusbar.h"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\tab.h"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\toolbar.h"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\treeview.h"
# End Source File
# Begin Source File

SOURCE="..\..\Win32++\src\wincore.h"
# End Source File
# End Group
# End Target
# End Project
