Dialog Example
==============

This program has a modal dialog as its main window.

A class derived from CDialog is used to create the dialog.  The files
for this are located in the Win32++ directory, and the code
is lcoated in the Win32xx namespace.

The CMyDialog and CDialogApp classes are intended as templates for other 
programs.  The classes within the Win32xx namespace are intended to 
be used without modification.

Dialog applications are easy to create, provided we have access to a 
resource editor to build the resource script file for us (often called
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

