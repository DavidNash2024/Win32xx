SimpleRibbon Example
====================
This project demonstrates how to add a ribbon to a simple window. The ribbon
buttons are defined in an Ribbon.xml. 

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


