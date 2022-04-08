/* (11-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                  (main.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the WinMain interface with the
    Windows operating system using the Win32++ Windows interface classes.

    Programming Notes: The programming style roughly follows that established
    for the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "App.h"

/*******************************************************************************

    Windows program interface

*=============================================================================*/
    int WINAPI
WinMain(HINSTANCE, HINSTANCE, LPSTR, int)                                   /*

    Launch the application. The instances, command line, and show state are
    unused in this program.
*-----------------------------------------------------------------------------*/
{
     try
    {
          // Start Win32++
        CApp theApp;

          // run the application: invoke theApp::InitInstance()
          // and start the thread's message loop
        return theApp.Run();
    }

      // catch all unhandled CException types
    catch (const CException &e)
    {     // Display the exception and quit
        MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);
        return -1;
    }
}
/*----------------------------------------------------------------------------*/