/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]                 (main.cpp) *
********************************************************************************
|                                                                              |
|               Authors: Robert C. Tausworthe, David Nash, 2020                |
|                                                                              |
===============================================================================*

    Contents Description:  The WinMain() function for this application using
    the Win32++ framework.

    Programming Notes: The programming style roughly follows that established
    for the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "App.h"

#if defined (_MSC_VER) && (_MSC_VER >= 1920)      // VS2019 or higher          // dn ...
#pragma warning ( disable : 28251 ) // allow WinMain without annotations
#endif                                                                         // dn ...

/*******************************************************************************

    Windows API entry point

*=============================================================================*/
    int WINAPI
WinMain(HINSTANCE, HINSTANCE, LPSTR, int)                                   /*

    This function is called by the system as the initial entry point for
    a WinApi-based application. None of the passed parameters are used by
    this application, and are thus not named in the calling sequence above
    in order to suppress compiler messages.
*-----------------------------------------------------------------------------*/
{
      // semaphore name, instances, and  handle: make sure the name is
      // unique to this application
    const CString szSemaphoreName = _T("Unique name: TimeDemo");
       // number of allowed instances: here, 1 avoids archive file clashes
    const int nInstances = 1;
      // set default return value
    int rtn = -1;
      // Create and  check the semaphore that limits the number of
      // simultaneously executing instances of this application
      // to m_nInstances.
    HANDLE m_hSemaphore;
    if ((m_hSemaphore = CreateSemaphore(NULL, nInstances, nInstances,
        szSemaphoreName)) != NULL)
    {
        if (WaitForSingleObject(m_hSemaphore, 0) == WAIT_TIMEOUT)
        {
            ::MessageBox(NULL,
                _T("The allowed number of instances of this\n")
                _T("application are already running."),
                _T("Stop"), MB_OK | MB_ICONSTOP | MB_TASKMODAL);
                return 0;
        }
        try
        {
            CApp thisApp;

              // Run the application
            rtn = thisApp.Run();
        }

        catch(const CException& e)
        {
            CString msg;
            CString what(e.what());
            msg.Format(_T("%s\n%s\n%s"), e.GetText(), e.GetText(),
                e.GetErrorString(), _T("\nWinMain Goodbye..."));
            ::MessageBox(NULL, msg.c_str(), what.c_str(),
                MB_OK | MB_ICONSTOP | MB_TASKMODAL);
        }
        catch(...)      // catch all other exception events
        {
            ::MessageBox(NULL, _T("WinMain Goodbye..."),
                _T("Error"),
                MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
        }

              // release the semaphore
            ReleaseSemaphore(m_hSemaphore, 1, NULL);
            CloseHandle(m_hSemaphore);
        }
    return rtn;
}
