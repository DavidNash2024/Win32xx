/* [Tab/Indent: 8/8][Line/Box: 80/74]                                (App.cpp) *
********************************************************************************

    Contents Description: Implementation of the Windows interface andCApp
    class for a sample program demonstrating the use of a custom status bar
    within the Win32++ Windows framework.

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
        MessageBox(NULL, e.GetText(), AtoT(e.what()), MB_ICONERROR);
        return -1;
    }
}

/*******************************************************************************

    Implementation of the CApp class

*=============================================================================*/
    BOOL CApp::
InitInstance()                                                              /*

    Create the Frame Window.
*-----------------------------------------------------------------------------*/
{
    try
    {
        m_frame.Create(); // throws a CWinException on failue
        return TRUE;
    }
    catch (...)
    {     // We get here if the Frame creation fails
        ::MessageBox(NULL, _T("Failed to create Frame window"),
            _T("ERROR"), MB_ICONERROR);
    }
    return FALSE; // returning FALSE ends the application
}
/*----------------------------------------------------------------------------*/
