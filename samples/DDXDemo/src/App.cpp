/* (12-Jun-2015) [Tab/Indent: 8/8][Line/Box: 80/74]                  (App.cpp) *
********************************************************************************
|                                                                              |
|                      Author: Robert C. Tausworthe, 2020                      |
|                                                                              |
===============================================================================*

    Contents Description: The DDXDemo CApp class implementation. This class
    governs the overall actions of the DDX/DDV Demonstration program. This
    class is a modified version of that found in the FormDocView sample
    distributed with the Win32++ Windows interface classes. The modified sample
    program was based on code provided by Lynn Allan. This demo extends the
    given sample by application of Dialog Data Exchange and Validation (DDX/DDV).

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

    Acknowledgement: The author would like to thank and acknowledge the advice,
    critical review, insight, and assistance provided by David Nash in the
    development of this work.

*******************************************************************************/

#include "stdafx.h"
#include "App.h"

/******************************************************************************

        The WINAPI entry point for a graphical Windows-based application.

*============================================================================*/
    int WINAPI
WinMain(HINSTANCE, HINSTANCE, LPSTR, int)                               /*

    This function is called by the system as the initial entry point for
    a WinApi-based application. None of the passed parameters are used by
    this application, and are thus not named in the calling sequence above
    in order to suppress compiler messages.
*-----------------------------------------------------------------------------*/
{
    try
    {
          // declare the application object
        CApp theApp;

          // run the application
        return theApp.Run();
    }

    // catch all unhandled CException types
    catch (const CException& e)
    {
        // Display the exception and quit
        MessageBox(NULL, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return -1;
    }

    // catch all unhandled exceptions inherited from std::exception
    catch (const std::exception &e)
    {
          // Process the exception and quit
        MessageBox(NULL, _T("An unhandled std::exception was thrown"), AtoT(e.what()), MB_ICONERROR);

        return -1;
    }

}

/*******************************************************************************

        CApp Methods

*============================================================================*/
    BOOL CApp::
InitInstance()                                                          /*

    Called by theApp.Run() execution in WinMain to create the application
    frame, which controls the program.
*-----------------------------------------------------------------------------*/
{
      // Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure
    return TRUE;
}

