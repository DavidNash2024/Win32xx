/* (02-Aug-2016) [Tab/Indent: 4/4][Line/Box: 80/74]                  (App.cpp) *
********************************************************************************
|                                                                              |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of a basic Single Document Interface 
    (SDI) CApp class for the ScrollWin demo application using the Win32++ 
    Windows interface classes, Copyright c) 2005-2017 David Nash, under 
    permissions granted therein.

 	Caveats: These materials are available under the same provisions as found 
	in the Win32++ copyright.txt notice.

    Programming Notes:
        The programming standards roughly follow those established by the 
    1997-1999 Jet Propulsion Laboratory Network Planning and Preparation 
    Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include <sys\stat.h>
#include <io.h>

#include "StdApp.h"

/*============================================================================*/
    int APIENTRY 
WinMain(HINSTANCE, HINSTANCE, LPSTR, int )                                  /*

    Application entry point.
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
    HANDLE  m_hSem = (HANDLE)sf;
    if (WaitForSingleObject(m_hSem, 0) == WAIT_TIMEOUT)
    {
        ::MessageBox(NULL, _T("The allowed number of instances of this\n")
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
    catch (CException &e)   // catch all CException events
    {
          // Process the exception and quit
        CString msg = e.what() + (CString)_T("\n") +
            e.GetText() + (CString)_T("\nWinMain Goodbye...");
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
    sf.ReleaseSemaphore(1, NULL);
    return rtn;

}

/*******************************************************************************

    CApp class methods

*=============================================================================*/
    CApp::
CApp() : m_nMaxMRUSlots(0)                                                  /*

    Default constructor.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
    CApp::
~CApp()                                                                     /*

    Default destructor.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
    BOOL CApp::
InitInstance()                                                              /*

    This method is immediately called from the base class (CWinApp) Run()
    method to create the frame, perform initialization of the app, and
    return TRUE on success. Returning FALSE terminates the program.
    
    Here, the About box information, app path, app directory, app name,
    app exe name, archive file name, and other constants are generated and
    saved as public data members of this object via the mere declaration of
    the CAppGlobal m_AppGlobal object in App.h.
*-----------------------------------------------------------------------------*/
{
          // extract the app name, directory, and  path names
    CString sAppPath, sAppName, sArcvDir;
    ::GetModuleFileName(NULL, sAppPath.GetBuffer(FILENAME_MAX),
        FILENAME_MAX);
    sAppPath.ReleaseBuffer();
    CFile f; // no file opened here, just using the name parsing parts
    f.SetFilePath(sAppPath);
    sAppName  = f.GetFileNameWOExt();
      // make or locate the archive directory
    sArcvDir = MakeAppDataPath(LoadString(IDS_DATAPATH_SUBDIR) +
        sAppName);
      // form the archive file path name
    m_sArcvPath  = sArcvDir + _T("\\") + sAppName +
         LoadString(IDS_ARCHIVE_FILE_EXT);
      // the maximum allowed number of MRU entries (limited to be under 16
      // by Win32++)
    m_nMaxMRUSlots = MIN(_ttoi(LoadString(IDS_MAX_MRU_ENTRIES)), 16);
      // make Win32++ version string
    CString sWin32Version;
    UINT ver = _WIN32XX_VER;
    sWin32Version.Format(_T("Win32++ Version %d.%d.%d"), ver / 0x100,
        (ver % 0x100) / 0x10, (ver % 0x10));
      // generate compiler information for the About box
    CString sCompiler;
#ifdef __GNUC__
    sCompiler.Format(_T("Gnu C++ %d.%d.%d"), __GNUC__, __GNUC_MINOR__,
        __GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
    sCompiler.Format(_T("MS C++ %d.%d"), _MSC_VER / 100,
        _MSC_VER % 100);
#else
    sCompiler = _T("(unknown compiler name)");
#endif
    CString date = __DATE__; // necesary on VCE2010
    m_sAboutBoxInfo.Format(_T("%s\n\n(%s.exe)\n%s\n%s\ncompiled with ")
        _T("%s on %s"), LoadString(IDW_MAIN).c_str(), sAppName.c_str(),
        LoadString(IDS_APP_VERSION).c_str(), sWin32Version.c_str(),
        sCompiler.c_str(), date.c_str());

      // Create the Frame Window after the archive file name is known
    m_Frame.Create();   // throws a CWinException on failure  
    
    return TRUE;
}

/*=============================================================================*

    Static Members                                                          */

static const CString months = "Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec";

/*============================================================================*/
    ULONG CApp::
DatInt(LPCTSTR pDate)                                             /*

    Convert the pDate, of form mmm dd yyyy, to a long integer of the form
    0xyyyymodd, where mmm is character based month, and mo is 0 (Jan) to
    11 (Dec).
*-----------------------------------------------------------------------------*/
{
    CString date = pDate;
    int     yyyy = _ttoi(date.Mid(7, 4).c_str()),
        dd   = _ttoi(date.Mid(4, 2).c_str()),
        mo   = months.Find(date.Mid(0, 3)) / 4;
    ULONG   ans  = ((yyyy * 100 + mo) * 100) + dd;
    return  ans;
}

/*============================================================================*/
    CString CApp::
IntDat(ULONG hexdate)                                                       /*

    Convert the hex date, of form 0xyyyymodd, to a CString date of the form
    mmm dd yyyy,  where mmm is character based month, and mo is 0 (Jan) to
    11 (Dec).
*-----------------------------------------------------------------------------*/
{
    UINT    dd   = hexdate MOD 100,
        mo   = (hexdate / 100) MOD 100,
        yyyy = (hexdate / 10000);
    CString ans;
    ans.Format(_T("%s %02d, %u"),  months.Mid(4 * mo, 3).c_str(), dd, yyyy);
    return ans;
}

/*============================================================================*/
        void CApp::
Serialize(CArchive &ar)                                                     /*

        Called to serialize the application to or from the archive ar, depending
    on the sense of IsStoring().  Leaves the archive open for for further
    operations.
*-----------------------------------------------------------------------------*/
{
      // perform loading or storing
        if (ar.IsStoring())
        {
                  // each item is written to the archive as a char stream of
          // the proper length, preceded by that length. 

    }
        else    // recovering
        {
                  // each item read from the archive is retrieved by first
          // reading its byte length and then by loading in that number
          // of bytes into the location of that item.

    }
}

/*******************************************************************************

    Static functions

*=============================================================================*/
    CString CApp::
MakeAppDataPath(const CString& subpath)                                     /*

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
        int     nextbk  = subpath.Find(_T("\\"), from),
            nextfwd = subpath.Find(_T("/"), from);
        next    = MAX(nextbk, nextfwd);
        if (next < 0)
            next = to;

        CString add = subpath.Mid(from, next - from);
        app_data_path += _T("\\") + add;
        if ((::CreateDirectory(app_data_path, 0) == 0) && GetLastError() != ERROR_ALREADY_EXISTS)
        {
            CString msg = app_data_path +
                _T("\nDirectory creation error.");
            throw CUserException(msg);
        }
    }
    return app_data_path;
}


