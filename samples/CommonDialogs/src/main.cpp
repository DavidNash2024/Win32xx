/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                 (main.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description:  This file contains the WinMain() function for the
    CommonDialogs demonstration sample application using the Win32++ Windows
    interface classes.

    Programming Notes: The architectural members of this program set,
    viz., CApp, CDoc, CMainFrame, and CView, are meant to be edited and
    adapted to conform with the needs of each new application to which they
    are applied. The code provided in this sample is meant to serve as the
    template for applications that require full color selection of frame,
    controls, and client area, font selection for controls, standard file
    open and save-as dialogs, and most-recently used list (MRU), with
    persistent data stored in and retrieved from archive files.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"

#if defined (_MSC_VER) && (_MSC_VER >= 1920)      // VS2019 or higher
#pragma warning( disable : 28251 )  // do not require annotation of WinMain()
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
      // Create and check the semaphore that limits the number of
      // simultaneously executing instances of this application
      // to m_nInstances.

    const LPCWSTR semaphoreName = L"Win32++_CommonDialogsDemo";
    const int instances = 1; // number of allowed instances
    HANDLE semaphore = CreateSemaphore(nullptr, instances, instances, semaphoreName);
    if (WaitForSingleObject(semaphore, 0) == WAIT_TIMEOUT)
    {
        ::MessageBox(nullptr, L"The allowed number of instances of this\n"
        L"application are already running.", L"Stop", MB_OK |
        MB_ICONSTOP | MB_TASKMODAL);
        CloseHandle(semaphore);
        return 0;  // before entering the message loop
    }
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
      // release the semaphore
    ReleaseSemaphore(semaphore, 1, nullptr);
    CloseHandle(semaphore);
    return rtn;
}
