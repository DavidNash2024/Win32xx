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
3. Visual Studio 2005, 2008 or 2010 (full or Express Edition)

Compiling Instructions
======================
To compile Ribbon.xml, specify it's custom build properties as follows:

Command Line  uicc.exe ..\src\Ribbon.xml ..\src\Ribbon.bml /header:..\src\RibbonUI.h /res:..\src\RibbonUI.rc
Outputs       Ribbon.bml;RibbonUI.rc;RibbonUI.h 

Note: Ensure that the following VC++ Directories are configured:
Executables directory:  C:\Program Files\Microsoft SDKs\Windows\v7.0\Bin
Include directory:      C:\Program Files\Microsoft SDKs\Windows\v7.0\Include


How the ribbon works
====================
Microsoft's RibbonUI framework uses COM to implement the ribbon. The COM 
interfaces involved are IUIApplication and IUICommandHandler. The Win32++ 
CRibbon class inherits from both IUIApplication and IUICommandHandler. 

To add a ribbon to your frame, inherit CMainFrame from CRibbonFrame, and add
the Ribbon.xml file containing the definitions for your ribbon to the project. 
CRibbonFrame inherits from CRibbonFrameT<CFrame>. To create and
interact with the ribbon, we override the relevant functions from CRibbon.

The functions you may wish to override are:
OnCreateUICommand 
  Called for each Command specified in the Ribbon markup to bind the Command 
  to an IUICommandHandler.  
OnDestroyUICommand 
  Called for each Command specified in the Ribbon markup when the Ribbon host 
  application window is destroyed.  
OnViewChanged 
  Called when the state of a View changes.
Execute 
  Executes or previews the Commands bound to the Command handler.  
UpdateProperty
  Sets a property value for a bound Command, for example, setting a Command to 
  enabled or disabled depending on the state of a View. 
  
This example demonstrates how to override Execute to respond to button clicks 
on the ribbon. 