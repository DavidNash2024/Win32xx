/* (11-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                  (App.CPP) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CApp class for the
    FilePrintPreview demo program and the WinMain interface with the Windows
    operating system using the Win32++ Windows interface classes.

    Programming Notes: The programming style roughly follows that established
    for the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "App.h"

  // the one and only CApp object, not on the stack for efficiency
CApp theApp;

/*============================================================================*/
    BOOL CApp::
InitInstance()                                                          /*

    Called by the framework to create the frame.
*-----------------------------------------------------------------------------*/
{
    m_frame.Create();   // throws a CWinException on failure
    return TRUE;
}

/*******************************************************************************

    Windows program interface

*=============================================================================*/
    int APIENTRY
WinMain(HINSTANCE, HINSTANCE, LPSTR, int)                                   /*

    Launch the application. The instances, command line, and show state are
    unused in this program.
*-----------------------------------------------------------------------------*/
{
    try
    {     // Start Win32++ and run the application: invoke theApp::InitInstance()
          // and start the thread's message loop
        return theApp.Run();
    }

      // catch all unhandled CException types
    catch (const CException &e)
    {     // Display the exception and quit
        MessageBox(NULL, e.GetText(), AtoT(e.what()), MB_ICONERROR);
        return -1;
    }
}
/*----------------------------------------------------------------------------*/