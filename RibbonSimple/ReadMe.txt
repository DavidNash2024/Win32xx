SimpleRibbon Example
====================
This project demonstrates how to add a ribbon to a simple window. The 
application uses TRACE to send text to the output window when a ribbon button
is pressed. It also detects changes to the ribbon height and adjusts the 
view window accordingly.

The ribbon buttons are defined in the Ribbon.xml. This file must be compiled 
using uuic. To compile Ribbon.xml, add it to the set of files to be compiled,
and modify its properties, specifying the "Custom Build Step" parameters as
described below.

Prerequisites:
==============
1. Windows 7 operating system
2. Microsoft Windows SDK v7.0 
3. Visual Studio 2005 or 2008 (full or Express Edition)

Compiling Instructions
======================
To compile Ribbon.xml, specify it's custom build as follows:

Command Line  uicc.exe ..\src\Ribbon.xml ..\src\Ribbon.bml /header:..\src\RibbonUI.h /res:..\src\RibbonUI.rc
Outputs       Ribbon.bml;RibbonUI.rc;RibbonUI.h 


