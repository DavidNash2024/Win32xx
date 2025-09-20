/* (27-Apr-2024) [Tab/Indent: 4/4][Line/Box: 80/74]                 (main.cpp) *
********************************************************************************
|                                                                              |
|                Authors: Robert Tausworthe, David Nash, 2020                  |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of WinMain for this sample program
    using the Win32++ Windows interface classes.

     Programming Notes: The programming style roughly follows that established
     got the 1995-1999 Jet Propulsion Laboratory Network Planning and
     Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"


/*******************************************************************************

    Windows application entry point.

*=============================================================================*/

#if defined (_MSC_VER) && (_MSC_VER >= 1920)      // VS2019 or higher
#pragma warning( suppress : 28251 )  // Ignore the annotation requirement for wWinMain.
#endif

    int WINAPI
wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int )                                /*

    This function is called by the system as the initial entry point for
    a WinApi-based application. None of the passed parameters are used by
    this application, and are thus not named in the calling sequence above
    in order to suppress compiler messages.
*-----------------------------------------------------------------------------*/
{
      // set default return value
    int rtn = -1;
      // Create and check the semaphore that limits the number of
      // simultaneously executing instances of this application
      // to m_nInstances.
    LPCWSTR semaphoreName = L"Win32++_TextFileReader";
    LONG     nInstances = 1; // number of allowed instances

    CSemaphore sf(nInstances, nInstances, semaphoreName, nullptr);
    if (WaitForSingleObject(sf, 0) == WAIT_TIMEOUT)
    {
        ::MessageBox(nullptr, L"The allowed number of instances of this\n"
        L"application are already running.", L"Stop",
        MB_OK | MB_ICONSTOP | MB_TASKMODAL);
        sf.ReleaseSemaphore(1, nullptr);
        return 0;  // before entering the message loop
    }
      // declare the CApp object and run the application
    try
    {
        CApp thisApp;
          // Run the application
        rtn = thisApp.Run();
    }
    catch (const CException& e)   // catch all CException events
    {
          // Process the exception and quit
        CString msg;
        msg << e.what() << L'\n' << e.GetText() << L"\nWinMain Goodbye...";
        ::MessageBox(nullptr, msg, L"Standard Exception", MB_OK |
            MB_ICONSTOP | MB_TASKMODAL);
    }
    catch(...)      // catch all other exception events
    {
        CString msg = "Unregistered exception event.\nWinMain Goodbye...";
        ::MessageBox(nullptr, msg, L"Unknown Exception", MB_OK |
            MB_ICONSTOP | MB_TASKMODAL);
    }
      // release the semaphore
    sf.ReleaseSemaphore(1, nullptr);
    return rtn;
}
