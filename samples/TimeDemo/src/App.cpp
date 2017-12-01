/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]                  (App.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

    Contents Description:  The WinMain() function and  CApp Class
    implementation for a skeleton SDI application using the Win32++
    Windows interface classes, Copyright (c) 2005-2015 David Nash,
    under permissions granted therein.

        Caveats: The copyright displayed above extends only to the author's
    original contributions to the subject class, and  to the alterations,
    additions, deletions, and  other treatments of materials that may have
    been extracted from the cited sources.  Unaltered portions of those
    materials retain their original copyright status. The author hereby
    grants permission to any person obtaining a copy of this treatment
    of the subject class and  any associated documentation composed by
    the author, to utilize this material, free of charge and  without
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

    Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
        Planning and Preparation Subsystem project for C++ programming.
        
    Acknowledgement:
    The author would like to thank and acknowledge the advice, critical
    review, insight, and assistance provided by David Nash in the development
    of this work.       

********************************************************************************

    Implementation of the CApp class.  Also contains WinMain()

*******************************************************************************/

#include "stdafx.h"
#include "App.h"

/*******************************************************************************

    Static constant and variable initialization         */

  // About box dialog and  other information
CString CApp::m_sAboutStatement = _T("Hello World!");
const CString CApp::m_sCredits  = _T("CTime, MRU, CArchive, and Scrolling Demo");
const CString CApp::m_sVersion  = _T("Version 1.0 (2015)");
const CString CApp::m_sCompiled_with = _T("(TODO: name your compiler here)");
const CString CApp::m_sCompiled_on = __DATE__;
const CString CApp::m_months    =
        _T("Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec");
  // serialized data file type (here: "time-demo info")
static  const   LPCTSTR archive_file_type     = _T(".arc");

/*******************************************************************************

    Local non-class function

*=============================================================================*/
    static CString
MakeAppDataPath(const CString& subpath)                 /*

    Return a string giving the path APPDATA environmental path, with the
    given subpath appended.  Create this path if it does not exist. If
    an error is encountered, throw a user exception.
*-----------------------------------------------------------------------------*/
{
    ::SetLastError(0);
    CString appdata = GetAppDataPath();

    int from, to, next;
    for (from = 0, to = subpath.GetLength(); from < to; from = ++next)
    {
        int     nextbk  = subpath.Find(_T("\\"), from),
            nextfwd = subpath.Find(_T("/"), from);
        next    = MAX(nextbk, nextfwd);
        if (next < 0)
            next = to;

        CString add = subpath.Mid(from, next - from);
        appdata += _T("\\") + add;
        if (!SUCCEEDED(::CreateDirectory(appdata, 0)))
        {
            CString msg = appdata + _T("\nDirectory creation error.");
            throw CUserException(msg);
        }
    }
    return appdata;
}

/*******************************************************************************

    Windows API entry point

*=============================================================================*/
    int APIENTRY 
WinMain(HINSTANCE, HINSTANCE, LPSTR, int)               /*

    Application entry point. None of the entry parameters are used and
    are therefore omitted above.
*-----------------------------------------------------------------------------*/
{
      // semaphore name, instances, and  handle: make sure the name is 
      // unique to this application
    static const CString szSemaphoreName = _T("Unique name: TimeDemo");
       // number of allowed instances: here, 1 avoids archive file clashes
    static const int nInstances = 1;
      // set default return value
    int rtn = -1; 
      // Create and  check the semaphore that limits the number of
      // simultaneously executing instances of this application
      // to m_nInstances.
    static  HANDLE m_hSemaphore;
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
            CString msg,
                 what(e.what());
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
CApp()                                  /*

    Default constructor.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
    CApp::
~CApp()                                 /*

    Default destructor.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
    BOOL CApp::
InitInstance()                              /*

    This method is immediately called from the Run() method above to
    create the frame, perform initialization of the app, and  return
    TRUE on success. Returning FALSE terminates the program.
    
    Here, the About box information, command line arguments, app path,
    app directory, app name, app exe name, archive file name, and  help
    file name are generated and saved as public data members of this object.
*-----------------------------------------------------------------------------*/
{
         // extract the app name, directory, and  path names
    ::GetModuleFileName(NULL, m_sAppPath.GetBuffer(FILENAME_MAX),
        FILENAME_MAX);
    m_sAppPath.ReleaseBuffer();
    CFile f;
    f.SetFilePath(m_sAppPath.c_str());
    m_sAppDir = f.GetFileDirectory();
    m_sAppName = f.GetFileNameWOExt();
      // locate the archive file
    m_sArcvDir = MakeAppDataPath(_T("win32++\\") + m_sAppName);
      // form the archive file path name
    m_sArcvFile  = m_sArcvDir + _T("\\") + m_sAppName + archive_file_type;

      // generate the About box static information: first the latest
      // date one of the main stream files was compiled
    ULONG compiled_on = DatInt(CApp::m_sCompiled_on);
    compiled_on = MAX(compiled_on, DatInt(CDoc::m_sCompiled_on));
    compiled_on = MAX(compiled_on, DatInt(CMainFrame::m_sCompiled_on));
    compiled_on = MAX(compiled_on, DatInt(CView::m_sCompiled_on));
    m_sAboutStatement.Format(_T("%s\n\n(%s.exe)\n%s\ncompiled with ")
        _T("%s\non %s"), m_sCredits.c_str(), m_sAppName.c_str(),
        m_sVersion.c_str(), m_sCompiled_with.c_str(),
        IntDat(compiled_on).c_str());

      //Create the Frame Window
    m_Frame.Create();   // throws a CWinException on failure  

    return TRUE;
}

/*=============================================================================*

    Static Members                                                  */

/*============================================================================*/
    ULONG CApp::
DatInt(const CString &date)                                             /*

    Convert the date, of form mmm dd yyyy, to a long integer of the form
    0xyyyymodd, where mmm is character based month, and  mo is 0 (Jan) to
    11 (Dec).
*-----------------------------------------------------------------------------*/
{
    int     yyyy = _ttoi(date.Mid(7, 4).c_str()),
        dd   = _ttoi(date.Mid(4, 2).c_str()),
        mo   = CApp::m_months.Find(date.Mid(0, 3)) / 4;
    ULONG   ans  = ((yyyy * 100 + mo) * 100) + dd;
    return  ans;
}

/*============================================================================*/
    CString CApp::
IntDat(ULONG hexdate)                                               /*

    Convert the hex date, of form 0xyyyymodd, to a CString date of the form
    mmm dd yyyy,  where mmm is character based month, and  mo is 0 (Jan) to
    11 (Dec).
*-----------------------------------------------------------------------------*/
{
    UINT    dd   = (hexdate % 100),
        mo   = (hexdate / 100) % 100,
        yyyy = (hexdate / 10000);
    CString ans;
    ans.Format(_T("%s %02d, %u"), CApp::m_months.Mid(4 * mo, 3).c_str(), 
        dd, yyyy);
    return ans;
}

