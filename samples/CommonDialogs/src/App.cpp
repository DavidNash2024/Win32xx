/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                  (App.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

    Contents Description:  This file contains the WinMain() function and
    CApp Class implementation for the CommonDialogs demonstration sample
    application using the Win32++ Windows interface classes, Copyright
    (c) 2005-2016 David Nash, under permissions granted therein.

        Caveats: The copyright displayed above extends only to the author's
    original contributions to the subject class, and to the alterations,
    additions, deletions, and other treatments of materials that may have
    been extracted from the cited sources.  Unaltered portions of those
    materials retain their original copyright status. The author hereby
    grants permission to any person obtaining a copy of this treatment
    of the subject class and any associated documentation composed by
    the author, to utilize this material, free of charge and without
    restriction or limitation, subject to the following conditions:

        The above copyright notice, as well as that of David Nash
        and Win32++, together with the respective permission
        conditions shall be included in all copies or substantial
        portions of this material so copied, modified, merged,
        published, distributed, or otherwise held by others.

    These materials are provided "as is", without warranty of any kind,
    express or implied, including but not limited to: warranties of
    merchantability, fitness for a particular purpose, and non-infringement.
    In no event shall the authors or copyright holders be liable for any
    claim, damages, or other liability, whether in an action of contract,
    tort or otherwise, arising from, out of, or in connection with, these
    materials, the use thereof, or any other other dealings therewith.

    Special Conventions:

    Programming Notes: The architectural members of this program set,
    viz., CApp, CDoc, CMainFrame, and CView, are meant to be edited and
    adapted to conform with the needs of each new application to which they
    are applied. The code provided in this sample is meant to serve as the
    template for applications that require full color selection of frame,
    controls, and client area, font selection for controls, standard file
    open and save-as dialogs, and most-recently used list (MRU), with
    persistent data stored in and retrieved from archive files.
    
    Acknowledgement:
        The author would like to thank and acknowledge the advice,
        critical review, insight, and assistance provided by David Nash
        in the development of this work.

    Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
        Planning and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"

#include <sys\stat.h>
#include <io.h>
#include "StdApp.h"

/*******************************************************************************

    Implementation of the CApp class

********************************************************************************

    Local extern and (static) default constants         */


/*******************************************************************************

    Implementation of the CApp class

*=============================================================================*/
    BOOL CApp::
InitInstance()                              /*

    This method is immediately called from the base class (CWinApp) Run()
    method to create the frame, perform initialization of the app, and
    return TRUE on success. Returning FALSE terminates the program.
    
    Here, the About box information, app path, app directory, app name,
    app exe name, archive file name, and other constants are generated and
    saved as public data members of this object via the mere declaration of
    the CAppGlobal m_AppGlobal object in App.h.
*-----------------------------------------------------------------------------*/
{
      //Create the Frame Window
    m_Frame.Create();   // throws a CWinException on failure  

    return TRUE;
}

/*******************************************************************************

    Static Members                                                  */

static const CString months = "Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec";

/*============================================================================*/
    ULONG CApp::
DatInt(const CString &date)                                             /*

    Convert the date, of form mmm dd yyyy, to a long integer of the form
    0xyyyymodd, where mmm is character based month, and mo is 0 (Jan) to
    11 (Dec).
*-----------------------------------------------------------------------------*/
{
    int     yyyy = _ttoi(date.Mid(7, 4));
    int     dd = _ttoi(date.Mid(4, 2));
    int     mo   = months.Find(date.Mid(0, 3)) / 4;
    ULONG   ans  = ((yyyy * 100 + mo) * 100) + dd;
    return  ans;
}

/*============================================================================*/
    CString CApp::
IntDat(ULONG hexdate)                                               /*

    Convert the hex date, of form 0xyyyymodd, to a CString date of the form
    mmm dd yyyy,  where mmm is character based month, and mo is 0 (Jan) to
    11 (Dec).
*-----------------------------------------------------------------------------*/
{
    UINT dd = hexdate % 100;
    UINT mo = (hexdate / 100) % 100;
    UINT yyyy = (hexdate / 10000);
    CString ans;
    ans.Format(_T("%s %02d, %u"),  months.Mid(4 * mo, 3).c_str(), dd, yyyy);
    return ans;
}

/*******************************************************************************

    The WinMain() function invoked by the system to launch the application.

*=============================================================================*/
    int APIENTRY
WinMain(HINSTANCE , HINSTANCE , LPSTR , int nCmdShow)                         /*

    Application entry point: hInstance is the handle to the current
    instance of the application; hPrevInstance is always 0; lpCmdLine
    is a pointer to a null-terminated string specifying the command line
    for the application, excluding the program name; nCmdShow specifies
    how the window is to be shown (consult WinMain() documentation for
    parameter values).
*-----------------------------------------------------------------------------*/
{
      // set default return value
    int rtn = -1;
      // Create and check the semaphore that limits the number of
      // simultaneously executing instances of this application
      // to m_nInstances.
    static  const   LPCTSTR semaphoreName = _T("Win32++_CommonDialogsDemo");
    static  const   int     nInstances = 1; // number of allowed instances
    static  HANDLE  m_hSem;

    m_hSem = CreateSemaphore(NULL, nInstances, nInstances, semaphoreName);
    if (WaitForSingleObject(m_hSem, 0) == WAIT_TIMEOUT)
    {
        ::MessageBox(NULL, _T("The allowed number of instances of this\n")
        _T("application are already running."), _T("Stop"), MB_OK | 
        MB_ICONSTOP | MB_TASKMODAL);
        CloseHandle(m_hSem);
        return 0;  // before entering the message loop
    }
      // declare the CApp object and run the application
    try
    {
        CApp thisApp;
          // save entry parameters
        thisApp.SetCmdShow(nCmdShow);
          // Run the application
        rtn = thisApp.Run();
    }
    catch (CException &e)   // catch all CException events
    {
          // Process the exception and quit
        CString msg = e.what() + (CString)_T("\n") + e.GetText() + 
        (CString)_T("\nWinMain Goodbye...");
        ::MessageBox(NULL, msg, _T("Standard Exception"), MB_OK |
            MB_ICONSTOP | MB_TASKMODAL);
    }
    catch(CString &s)   // catch CString events
    {
        CString msg = s + (CString)"\nWinMain Goodbye...";
        ::MessageBox(NULL, msg, _T("Registered Exception"), MB_OK |
            MB_ICONSTOP | MB_TASKMODAL);
    }
    catch(LPCTSTR s)        // catch C string events
    {
        CString msg = s + (CString)"\nWinMain Goodbye...";
        ::MessageBox(NULL, msg, _T("Registered Exception"), MB_OK |
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
    ReleaseSemaphore(m_hSem, 1, NULL);
    CloseHandle(m_hSem);
    return rtn;
}

