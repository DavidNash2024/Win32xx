/* (26-Mar-2025)                                                     (App.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description:  This file contains the CApp Class implementation
    for the CommonDialogs demonstration sample application using the Win32++
    Windows interface classes.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"


// This method creates the frame, performs initialization of the app and
// returns TRUE on success. Returning FALSE terminates the program.
BOOL CApp::InitInstance()
{
    // Initialize data strings containing the AboutBox information, the
    // app path, the app directory, the app name, and the archive path.
    CString appName;
    ::GetModuleFileName(nullptr, appName.GetBuffer(FILENAME_MAX), FILENAME_MAX);
    appName.ReleaseBuffer();

    CFile f; // No file opened here, just used for GetFileNameWOExt.
    f.SetFilePath(appName);
    appName  = f.GetFileNameWOExt();
    m_frame.SetDocExt(LoadString(IDS_DOC_DEFAULT_EXT));
    m_frame.SetDocFilter(LoadString(IDS_FILE_FILTER));

    // Generate the Win32++ version string
    UINT ver = _WIN32XX_VER;
    CString Win32xxVersion;
    Win32xxVersion.Format(L"Win32++ Version %d.%d.%d", ver / 0x100,
        (ver % 0x100) / 0x10, (ver % 0x10));

    // Generate the compiler information.
    CString compiler = "an unknown compiler";

#if defined (__clang__)
    compiler.Format(L"Clang C++ %d.%d.%d", __clang_major__, __clang_minor__,
        __clang_patchlevel__);
#elif defined(__GNUC__)
    compiler.Format(L"Gnu C++ %d.%d.%d", __GNUC__, __GNUC_MINOR__,
        __GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
    compiler.Format(L"MS C++ %d.%d", _MSC_VER / 100, _MSC_VER % 100);
#endif

    CString compileDate = __DATE__;

    // Generate the About Box text.
    CString aboutBoxInfo;
    aboutBoxInfo.Format(L"%s\n\n(%s.exe)\n%s\n%s\ncompiled with "
        L"%s\nbuild date: %s", LoadString(IDW_MAIN).c_str(), appName.c_str(),
        LoadString(IDS_APP_VERSION).c_str(), Win32xxVersion.c_str(),
        compiler.c_str(), compileDate.c_str());
    m_frame.GetAboutBox().SetStatus(aboutBoxInfo);

    m_frame.Create();   // throws a CWinException on failure
    return TRUE;
}

// Update the status of controls when the message queue is empty.
BOOL CApp::OnIdle(LONG)
{
    m_frame.UpdateToolbar();
    return FALSE;
}
