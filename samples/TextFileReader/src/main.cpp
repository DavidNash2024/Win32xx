/* (02-Aug-2016) [Tab/Indent: 4/4][Line/Box: 80/74]                 (main.cpp) *
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

#if defined (_MSC_VER) && (_MSC_VER >= 1920)      // VS2019 or higher
#pragma warning ( disable : 28251 )     // do not require WinMain annotations
#endif

/*******************************************************************************

    Windows application entry point.

*=============================================================================*/
    int WINAPI
WinMain(HINSTANCE, HINSTANCE, LPSTR, int )                                  /*

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
    LPCTSTR semaphoreName = _T("Win32++_CommonDialogsDemo");
    LONG     nInstances = 1; // number of allowed instances

    CSemaphore sf(nInstances, nInstances, semaphoreName, NULL);
    if (WaitForSingleObject(sf, 0) == WAIT_TIMEOUT)
    {
        ::MessageBox(0, _T("The allowed number of instances of this\n")
        _T("application are already running."), _T("Stop"),
        MB_OK | MB_ICONSTOP | MB_TASKMODAL);
        sf.ReleaseSemaphore(1, NULL);
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
        CString msg = e.what() + (CString)_T("\n") +
            e.GetText() + (CString)_T("\nWinMain Goodbye...");
        ::MessageBox(NULL, msg, _T("Standard Exception"), MB_OK |
            MB_ICONSTOP | MB_TASKMODAL);
    }
    catch(...)      // catch all other exception events
    {
        CString msg = _T("Unregistered exception event.\n")
            _T("WinMain Goodbye...");
        ::MessageBox(NULL, msg, _T("Unknown Exception"), MB_OK |
            MB_ICONSTOP | MB_TASKMODAL);
    }
      // release the semaphore
    sf.ReleaseSemaphore(1, NULL);
    return rtn;
}
