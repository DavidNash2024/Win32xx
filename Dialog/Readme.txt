Dialog Example
==============

This program has a modal dialog as its main window.

A class derived from CDialog is used to create the dialog.  The files
for this are located in the WinDev++ directory, and the code
is lcoated in the WinDevxx namespace.

The CMyDialog and CDialogApp classes are intended as templates for other 
programs.  The classes within the WinDevxx namespace are intended to 
be used without modification.

Dialog applications are easy to create, provided we have access to a 
resource editor to build the resource script file for us (often called
resource.rc). Commercial compilers usually include a resource editor
for this purpose, but free compilers generally don't. For a free 
resource editor, try resedit from here: http://www.resedit.net/


Generic Information about WinDev++ Projects
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
WinDev++ supports the following:
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
The programs compiled with WinDev++ can run on the following operating systems:
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

WinDev++ automatically detects if the operating system is capable of using rebars. If 
rebars are not supported by the OS, WinDev++ produces a frame without rebars.

WinDev++ is Unicode compliant and can therefore be used to develop Unicode applications.
Users are advised that only NT based operating systems (namely NT, Win2000, XP, Vista etc.)
support Unicode applications.

WinDev++ is intended to be 64bit compliant, and should be capable of generating 64bit code.
This feature has not been tested as yet.

Directory Structure
===================
When extracting the files from the zip archive, be sure to preserve the directory structure.
The directory structure will typically look like this:

ParentFolder
ParentFolder\Program1
ParentFolder\Program2
....
ParentFolder\WinDev++

The files which form the WinDev++ library are contained in the WinDev++ subdirectory.
It is located as a sibling directory to the various projects.

