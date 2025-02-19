/* (20-Oct-2024) [Tab/Indent: 8/8][Line/Box: 80/74]                  (App.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description:  This file contains the CApp Class implementation
    for the CommonDialogs demonstration sample application using the Win32++
    Windows interface classes.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"


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
    ::GetModuleFileName(nullptr, appPath.GetBuffer(FILENAME_MAX), FILENAME_MAX);
    appPath.ReleaseBuffer();
    CFile f; // no file opened here, just using the name parsing parts
    f.SetFilePath(appPath);
    CString appName  = f.GetFileNameWOExt();
      // the document default extension
    m_frame.SetDocExt(LoadString(IDS_DOC_DEFAULT_EXT));
      // the document open/save filter
    m_frame.SetDocFilter(LoadString(IDS_FILE_FILTER));
      // make Win32++ version string
    UINT ver = _WIN32XX_VER;
    CString Win32PPVersion;
    Win32PPVersion.Format(L"Win32++ Version %d.%d.%d", ver / 0x100,
        (ver % 0x100) / 0x10, (ver % 0x10));
      // generate compiler information for the About box
    CString compiler;
#ifdef __GNUC__
    compiler.Format(L"Gnu C++ %d.%d.%d", __GNUC__, __GNUC_MINOR__,
        __GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
    compiler.Format(L"MS C++ %d.%d", _MSC_VER / 100, _MSC_VER % 100);
#else
    compiler = L"(unknown compiler name)";
#endif
    CString compileDate = __DATE__;
      // put this information into the AboutBox information
    CString aboutBoxInfo;
    aboutBoxInfo.Format(L"%s\n\n(%s.exe)\n%s\n%s\ncompiled with "
        L"%s on %s", LoadString(IDW_MAIN).c_str(), appName.c_str(),
        LoadString(IDS_APP_VERSION).c_str(), Win32PPVersion.c_str(),
        compiler.c_str(), compileDate.c_str());
    m_frame.GetAboutBox().SetStatus(aboutBoxInfo);
      // now create the Frame Window
    m_frame.Create();   // throws a CWinException on failure
    return TRUE;
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

/*============================================================================*/
    BOOL CApp::
OnIdle(LONG)                                                                /*

    Update the status of controls when the message queue is empty.
*-----------------------------------------------------------------------------*/
{
       m_frame.UpdateToolbar();
       return FALSE;
}
/*----------------------------------------------------------------------------*/
