/* (26-Aug-2023) [Tab/Indent: 4/4][Line/Box: 80/74]                  (App.cpp) *
********************************************************************************
|                                                                              |
|                Authors: Robert Tausworthe, David Nash, 2020                  |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CApp class for this sample
    program using the Win32++ Windows interface classes.

     Programming Notes: The programming style roughly follows that established
     got the 1995-1999 Jet Propulsion Laboratory Network Planning and
     Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"


/*******************************************************************************

    CApp class methods

*=============================================================================*/
    CApp::
CApp()                                                                      /*

    Here, the About box information, app path, app directory, app name,
    app exe name, archive file name, and other constants are generated and
    saved as public data members of this object via the mere declaration of
    the CAppGlobal m_AppGlobal object in App.h.
*-----------------------------------------------------------------------------*/
{
      // extract the app name, directory, and  path names
    m_months = "Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec";
    CString appPath;
    ::GetModuleFileName(nullptr, appPath.GetBuffer(FILENAME_MAX),
        FILENAME_MAX);
    appPath.ReleaseBuffer();
    CFile f; // no file opened here, just using the name parsing parts
    f.SetFilePath(appPath);
    CString appName = f.GetFileNameWOExt();
      // make or locate the archive directory
    CString arcvDir = MakeAppDataPath(LoadString(IDS_DATAPATH_SUBDIR) +
        appName);
      // form the archive file path name
    CString arcvPath = arcvDir + L"\\" + appName +
        LoadString(IDS_ARCHIVE_FILE_EXT);
      // the maximum allowed number of MRU entries (limited to be under 16
      // by Win32++)
    UINT maxMRUSlots = std::min(_ttoi(LoadString(IDS_MAX_MRU_ENTRIES)), 16);
      // make Win32++ version string
    CString win32Version;
    UINT ver = _WIN32XX_VER;
    win32Version.Format(L"Win32++ Version %d.%d.%d", ver / 0x100,
        (ver % 0x100) / 0x10, (ver % 0x10));
      // generate compiler information for the About box
    CString sCompiler;
#ifdef __GNUC__
        sCompiler.Format(L"Gnu C++ %d.%d.%d", __GNUC__, __GNUC_MINOR__,
            __GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
        sCompiler.Format(L"MS C++ %d.%d", _MSC_VER / 100,
            _MSC_VER % 100);
#else
        sCompiler = L"(unknown compiler)";
#endif
    CString date = __DATE__; // necesary on VCE2010
    CString aboutBoxInfo;
    aboutBoxInfo.Format(L"%s\n\n(%s.exe)\n%s\n%s\ncompiled with %s on %s",
        LoadString(IDW_MAIN).c_str(), appName.c_str(),
        LoadString(IDS_APP_VERSION).c_str(), win32Version.c_str(),
        sCompiler.c_str(), date.c_str());
    m_frame.SetArchivePath(arcvPath);
    m_frame.SetMaxMRUSlots(maxMRUSlots);
    m_frame.GetAboutBox().SetAboutBoxInfo(aboutBoxInfo);
}

/*============================================================================*/
    BOOL CApp::
InitInstance()                                                              /*

    This method is immediately called from the base class (CWinApp) Run()
    method to create the frame, perform initialization of the app, and
    return TRUE on success. Returning FALSE terminates the program.
*-----------------------------------------------------------------------------*/
{
      // Create the Frame Window after the archive file name is known
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}

/*============================================================================*/
    ULONG CApp::
DatInt(LPCWSTR pDate) const                                                 /*

    Convert the pDate, of form mmm dd yyyy, to a long integer of the form
    0xyyyymodd, where mmm is character based month, and mo is 0 (Jan) to
    11 (Dec).
*-----------------------------------------------------------------------------*/
{
    CString date = pDate;
    int     yyyy = _ttoi(date.Mid(7, 4).c_str());
    int     dd   = _ttoi(date.Mid(4, 2).c_str());
    int     mo   = m_months.Find(date.Mid(0, 3)) / 4;
    ULONG   ans  = ((yyyy * 100 + mo) * 100) + dd;
    return  ans;
}

/*============================================================================*/
    CString CApp::
IntDat(ULONG hexdate) const                                                 /*

    Convert the hex date, of form 0xyyyymodd, to a CString date of the form
    mmm dd yyyy,  where mmm is character based month, and mo is 0 (Jan) to
    11 (Dec).
*-----------------------------------------------------------------------------*/
{
    UINT    dd = hexdate % 100;
    UINT    mo = (hexdate / 100) % 100;
    UINT    yyyy = (hexdate / 10000);
    CString ans;
    ans.Format(L"%s %02d, %u",  m_months.Mid(4 * mo, 3).c_str(), dd, yyyy);
    return ans;
}

/*=============================================================================*/
    CString CApp::
MakeAppDataPath(const CString& subpath) const                               /*

    Return a string consisting of the APPDATA environmental path with the
    given subpath appended.  Create this path if it does not exist. If
    an error is encountered, throw a user exception.
*-----------------------------------------------------------------------------*/
{
    CString appDataPath = GetAppDataPath();

    if (!appDataPath.IsEmpty())
    {
        appDataPath += L"\\" + subpath;
        ::SHCreateDirectory(nullptr, appDataPath);

        DWORD attributes = GetFileAttributes(appDataPath);
        if ((attributes == INVALID_FILE_ATTRIBUTES) || !(attributes & FILE_ATTRIBUTE_DIRECTORY))
            throw CFileException(appDataPath, L"Failed to access app directory");
    }
    else
        appDataPath = L".";

    return appDataPath;
}
/*----------------------------------------------------------------------------*/
