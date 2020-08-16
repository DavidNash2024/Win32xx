/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                  (App.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description:  This file contains the WinMain() function and
    CApp Class implementation for the CommonDialogs demonstration sample
    application using the Win32++ Windows interface classes.

    Programming Notes: The architectural members of this program set,
    viz., CApp, CDoc, CMainFrame, and CView, are meant to be edited and
    adapted to conform with the needs of each new application to which they
    are applied. The code provided in this sample is meant to serve as the
    template for applications that require full color selection of frame,
    controls, and client area, font selection for controls, standard file
    open and save-as dialogs, and most-recently used list (MRU), with
    persistent data stored in and retrieved from archive files.
    
    Programming Notes: The programming standards roughly follow those 
    established by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
    Planning and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"

  // the one and only CApp object, put here to reduce the stack size
static CApp thisApp;

/*******************************************************************************

    The WinMain() function invoked by the system to launch the application.

*=============================================================================*/
    int APIENTRY
WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int show)/*

    Application entry point. The normal current and previous instance handles, 
    as well as the command line string and window show parameter, are unused 
    in this application.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(instance);
    UNREFERENCED_PARAMETER(prevInstance);
    UNREFERENCED_PARAMETER(cmdLine);
    UNREFERENCED_PARAMETER(show);
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
      // Run the application
    try
    {
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

/*******************************************************************************

    Implementation of the CApp class

*=============================================================================*/
    BOOL CApp::
InitInstance()                                                              /*

    This method is immediately called from the base class (CWinApp) Run()
    method to create the frame, perform initialization of the app and
    return TRUE on success. Returning FALSE terminates the program.
*-----------------------------------------------------------------------------*/
{
      // Initialize data strings containing the AboutBox information, the 
      // app path, the app directory, the app name, and the archive path.
    CString appPath;
    ::GetModuleFileName(NULL, appPath.GetBuffer(FILENAME_MAX), FILENAME_MAX);
    appPath.ReleaseBuffer();
    CFile f; // no file opened here, just using the name parsing parts
    f.SetFilePath(appPath);
    CString appName  = f.GetFileNameWOExt();
      // locate the archive file
    CString archiveDir = MakeAppDataPath(LoadString(IDS_DATAPATH_SUBDIR) + appName);
      // form the archive file path name
    CString archivePath  = archiveDir + _T("\\") + appName +
         LoadString(IDS_ARCHIVE_FILE_EXT);
    m_frame.SetArchivePath( archivePath);
      // the document default extension
    m_frame.SetDocExt(LoadString(IDS_DOC_DEFAULT_EXT));
      // the document open/save filter
    m_frame.SetDocFilter(LoadString(IDS_FILE_FILTER));
      // the maximum allowed number of MRU entries (limit <= 16 by Win32++)
    m_frame.SetMaxMRU(MIN(_ttoi(LoadString(IDS_MAX_MRU_ENTRIES)), 16));
      // make Win32++ version string
    UINT ver = _WIN32XX_VER;
    CString Win32PPVersion;
    Win32PPVersion.Format(_T("Win32++ Version %d.%d.%d"), ver / 0x100,
        (ver % 0x100) / 0x10, (ver % 0x10));
      // generate compiler information for the About box
    CString compiler;
#ifdef __GNUC__
    compiler.Format(_T("Gnu C++ %d.%d.%d"), __GNUC__, __GNUC_MINOR__,
        __GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
    compiler.Format(_T("MS C++ %d.%d"), _MSC_VER / 100, _MSC_VER % 100);
#else
    compiler = _T("(unknown compiler name)");
#endif
    CString compileDate = __DATE__;
      // put this information into the AboutBox information
    CString aboutBoxInfo;
    aboutBoxInfo.Format(_T("%s\n\n(%s.exe)\n%s\n%s\ncompiled with ")
        _T("%s on %s"), LoadString(IDW_MAIN).c_str(), appName.c_str(),
        LoadString(IDS_APP_VERSION).c_str(), Win32PPVersion.c_str(),
        compiler.c_str(), compileDate.c_str());
    m_frame.GetAboutBox().SetStatus(aboutBoxInfo);
      // now create the Frame Window
    m_frame.Create();   // throws a CWinException on failure  
    return TRUE;
}

/*=============================================================================*/
    CString CApp::
MakeAppDataPath(const CString& subpath) const                /*

    Return a string consisting of the APPDATA environmental path with the
    given subpath appended.  Create this path if it does not exist. If
    an error is encountered, throw a user exception.
*-----------------------------------------------------------------------------*/
{
    ::SetLastError(0);
    CString app_data_path = GetAppDataPath();

    int from, to, next;
    for (from = 0, to = subpath.GetLength(); from < to; from = ++next)
    {
        int nextbk = subpath.Find(_T("\\"), from);
        int nextfwd = subpath.Find(_T("/"), from);
        next = MAX(nextbk, nextfwd);
        if (next < 0)
            next = to;

        CString add = subpath.Mid(from, next - from);
        app_data_path += _T("\\") + add;
        if ((::CreateDirectory(app_data_path, 0) == 0) && 
        GetLastError() != ERROR_ALREADY_EXISTS)
        {
            CString msg = app_data_path + _T("\nDirectory creation error.");
            throw CUserException(msg);
        }
    }
    return app_data_path;
}

/*============================================================================*/
    BOOL CApp::
OnIdle(LONG count)                                                          /*

    Update the status of controls when the message queue is empty.
*-----------------------------------------------------------------------------*/
{
       UNREFERENCED_PARAMETER(count);
       m_frame.UpdateControlUIState();
       return FALSE;
}
/*----------------------------------------------------------------------------*/
