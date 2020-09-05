/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]                  (App.cpp) *
********************************************************************************
|                                                                              |
|               Authors: Robert C. Tausworthe, David Nash, 2020                |
|                                                                              |
===============================================================================*

    Contents Description:  The WinMain() function and  CApp Class implementation
    for this application using the Win32++ framework.

    Programming Notes: The programming style roughly follows that established
    for the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "App.h"

  // static constant initiallization: latest CDoc.cpp file compilation date
const CString CApp::m_compiledOn = __DATE__;

/*******************************************************************************

    Windows API entry point

*=============================================================================*/
    int APIENTRY
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

/*******************************************************************************

    CApp class methods

*=============================================================================*/
    CApp::
CApp()                                                                      /*

    Default constructor.
*-----------------------------------------------------------------------------*/
{
    m_credits = _T("CTime, MRU, CArchive, and Scrolling Demo");
    m_version = _T("Version 2.0 (2020)");

      // generate compiler information for the About box
#ifdef __GNUC__
    m_compiledWith = _T("GNU C++ ") + CString(__GNUC__)
        + _T(".") + CString(__GNUC_MINOR__) + _T(".") + CString(__GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
    m_compiledWith = _T("MS C++ ") + CString(_MSC_VER / 100)
        + _T(".") + CString(_MSC_VER % 100);
#elif defined(__BORLANDC__)
    m_compiledWith = _T("Borland C++ ") + CString(__BORLANDC__ / 0x100)
        + _T(".") + CString((__BORLANDC__ & 0x0F0) / 0x10);
#else
    m_sCompiled_with = _T("(unknown compiler)");
#endif

    m_months =_T("Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec");
      // serialized data file type (here: "time-demo info")
    m_archiveFileType = _T(".arc");
}

/*============================================================================*/
    BOOL CApp::
InitInstance()                                                              /*

    This method is immediately called from the Run() method above to
    create the frame, perform initialization of the app, and  return
    TRUE on success. Returning FALSE terminates the program.

    Here, the About box information, command line arguments, app path,
    app directory, app name, app exe name, archive file name, and  help
    file name are generated and saved as public data members of this object.
*-----------------------------------------------------------------------------*/
{
         // extract the app name, directory, and  path names
    ::GetModuleFileName(NULL, m_appPath.GetBuffer(FILENAME_MAX),
        FILENAME_MAX);
    m_appPath.ReleaseBuffer();
    CFile f;
    f.SetFilePath(m_appPath.c_str());
    m_appDir = f.GetFileDirectory();
    m_appName = f.GetFileNameWOExt();
      // locate the archive file
    m_arcvDir = MakeAppDataPath(_T("win32++\\") + m_appName);
      // form the archive file path name
    m_arcvFile  = m_arcvDir + _T("\\") + m_appName + m_archiveFileType;

      // generate the About box static information: first the latest
      // date one of the main stream files was compiled
    ULONG compiled_on = DatInt(CApp::m_compiledOn);
    compiled_on = MAX(compiled_on, DatInt(CDoc::m_compiledOn));
    compiled_on = MAX(compiled_on, DatInt(CMainFrame::m_compiledOn));
    compiled_on = MAX(compiled_on, DatInt(CView::m_compiledOn));
    m_aboutStatement.Format(_T("%s\n\n(%s.exe)\n%s\ncompiled with ")
        _T("%s\non %s"), m_credits.c_str(), m_appName.c_str(),
        m_version.c_str(), m_compiledWith.c_str(),
        IntDat(compiled_on).c_str());

      // Set the AboutBoxStatus
    m_frame.GetAboutBox().SetStatus(m_aboutStatement);
      //Create the Frame Window
    m_frame.SetAppName(m_appName);
    m_frame.SetArcFileName(m_arcvFile);
    m_frame.Create();   // throws a CWinException on failure
    return TRUE;
}

/*============================================================================*/
    ULONG CApp::
DatInt(const CString &date)                                                 /*

    Convert the date, of form mmm dd yyyy, to a ULONG integer of the form
    yyyymodd, where mmm is character based month, and  mo is 0 (Jan) to
    11 (Dec).
*-----------------------------------------------------------------------------*/
{
    int yyyy = _ttoi(date.Mid(7, 4).c_str());
    int dd   = _ttoi(date.Mid(4, 2).c_str());
    int mo   = CApp::m_months.Find(date.Mid(0, 3)) / 4;
    ULONG   ans  = ((yyyy * 100 + mo) * 100) + dd;
    return  ans;
}

/*============================================================================*/
    CString CApp::
IntDat(ULONG hexdate)                                                       /*

    Convert the ULONG date, of form yyyymodd, to a CString date of the form
    mmm dd yyyy,  where mmm is character based month, and  mo is 0 (Jan) to
    11 (Dec).
*-----------------------------------------------------------------------------*/
{
    UINT dd = (hexdate % 100);
    UINT mo = (hexdate / 100) % 100;
    UINT yyyy = (hexdate / 10000);
    CString ans;
    ans.Format(_T("%s %02d, %u"), CApp::m_months.Mid(4 * mo, 3).c_str(),
        dd, yyyy);
    return ans;
}
/*----------------------------------------------------------------------------*/
