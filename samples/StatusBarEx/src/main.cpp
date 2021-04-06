/* [Tab/Indent: 8/8][Line/Box: 80/74]                               (main.cpp) *
********************************************************************************

    Contents Description: Implementation of the Windows interface for a sample
    program demonstrating the use of a custom status bar within the Win32++
    Windows framework.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "App.h"

/*******************************************************************************

    Implementation of the Windows execution interface

*=============================================================================*/
    int APIENTRY
WinMain(HINSTANCE, HINSTANCE, LPSTR, int)                                   /*

    This function is called by the system as the initial entry point for
    a WinApi-based application. None of the passed parameters are used by
    this application, and are thus not named in the calling sequence above
    in order to suppress compiler messages.
*-----------------------------------------------------------------------------*/
{
    try
    {
          // Start Win32++
        CApp theApp;

          // Run the application and the message loop
        return theApp.Run();
    }

      // catch all unhandled CException types
    catch (const CException &e)
    {
          // Display the exception and quit
        MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);
        return -1;
    }
}
