/* (20-Oct-2024) [Tab/Indent: 8/8][Line/Box: 80/74]                 (main.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description:  This file contains the WinMain() function for the
    CommonDialogs demonstration sample application using the Win32++ Windows
    interface classes.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"

#if defined (_MSC_VER) && (_MSC_VER >= 1920)      // VS2019 or higher
#pragma warning( disable : 28251 )  // disable annotation warnings for wWinMain()
#endif

/*******************************************************************************

    The WinMain() function invoked by the system to launch the application.

*=============================================================================*/
    int WINAPI
wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)                                   /*

    Application entry point. The normal current and previous instance handles,
    as well as the command line string and window show parameter, are unused
    in this application.
*-----------------------------------------------------------------------------*/
{
      // set default return value
    int rtn = -1;

      // Run the application
    CApp thisApp;
    try
    {
        rtn = thisApp.Run();
    }
    catch (const CException &e)   // catch all CException events
    {
          // Process the exception and quit
        CString msg;
        msg << e.what() << '\n' << e.GetText() << "\nWinMain Goodbye...";
        ::MessageBox(nullptr, msg, L"Standard Exception", MB_OK |
            MB_ICONSTOP | MB_TASKMODAL);
    }
    catch(...)      // catch all other exception events
    {
        CString msg = L"Unregistered exception event.\nWinMain Goodbye...";
        ::MessageBox(nullptr, msg, L"Unknown Exception", MB_OK |
            MB_ICONSTOP | MB_TASKMODAL);
    }
    return rtn;
}
