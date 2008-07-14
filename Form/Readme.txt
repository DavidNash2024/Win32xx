Form Window
===========

This program has a frame window which includes a Tool Bar and 
Status Bar.  A modeless dialog, super-imposed over the client 
area of the frame window.

Classes derived from CWnd are used to create the windows.  The files
for this are located in the Win32++ directory, and the code
is lcoated in the Win32xx namespace.

The CMyDialog, CFormApp and CMainFrame classes are intended as 
templates for other programs.  The classes within the Win32xx 
namespace are intended to be used without modification.

Dialogs are easy to create, provided we have access to a resource 
editor to build the resource script file for us (often called
resource.rc). Commercial compilers usually include a resource editor
for this purpose, but free compilers generally don't. For a free 
resource editor, try resedit from here: http://www.resedit.net/

Generic Information about Win32++ Projects
==========================================

The various directories may contain the following types of files:

Extension | Description
----------|------------
cbp       | A project file used by CodeBlocks
dsp       | A project file used by Visual Studio 6
dsw       | A project file used by Visual Studio 6
sln       | A project file used by Visual Studio 2003, VS2005 or VS2008
vcproj    | A project file used by Visual Studio 2003, VS2005 or VS2008
bdsproj   | A project file used by Borland Developer Studio 2006 
bpf       | A project file used by Borland Developer Studio 2006 
dev       | A project file used by Dev-C++
cpp       | A C++ source file 
h         | A C++ header file
rc        | A C++ resouce file 
txt       | A text file

Supported Compilers and Integrated Development Environments (IDEs)
==================================================================
Win32++ supports the following:
* Borland C++ Compiler 5.5
* Borland Developer Studio 2006
* CodeBlocks
* Dev-C++
* MinGW GCC Compiler
* Visual Studio 6
* Visual Studio.net 2003
* Visual C++ Toolkit 2003
* Visual Studio.net 2005
* Visual Studio.net 2005 Express
* Visual Studio.net 2008
* Visual Studio.net 2008 Express

CodeBlocks is an IDE. The project files are configured for the following compilers
* Borland C++ Compiler 5.5
* MinGW GNU compiler
* Visual C++ Toolkit 2003

Dev-C++ is an IDE which supports the MinGW GNU compiler

Supported Operating Systems
===========================
The programs compiled with Win32++ can run on the following operating systems:
* Win95 (all versions, with or without Internet Explorer 4 installed)
* Win98 (both versions)
* WinME
* Windows NT 4
* Windows 2000
* Windows XP
* Windows Server 2003
* Windows Vista

Note: Programs compiled with Visual Studio.net 2008 and Visual Studio.net 2008 Express
will not run on operating systems earlier than Windows 2000.

Win32++ automatically detects if the operating system is capable of using rebars. If 
rebars are not supported by the OS, Win32++ produces a frame without rebars.

Win32++ is Unicode compliant and can therefore be used to develop Unicode applications.
Users are advised that only NT based operating systems (namely NT, Win2000, XP, Vista etc.)
support Unicode applications.

Win32++ is intended to be 64bit compliant, and should be capable of generating 64bit code.
This feature has not been tested as yet.

Directory Structure
===================
When extracting the files from the zip archive, be sure to preserve the directory structure.
The directory structure will typically look like this:

ParentFolder
ParentFolder\Program1
ParentFolder\Program2
....
ParentFolder\Win32++

The files which form the Win32++ library are contained in the Win32++ subdirectory.
It is located as a sibling directory to the various projects.

