/* (22-Oct-2024) [Tab/Indent: 4/4][Line/Box: 80/74]                  (App.cpp) *
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
      // Retrieve the application's filename.
    CString appName;
    CString appPath = GetCommandLineArgs().front();
    int index = appPath.ReverseFind(L"\\");
    if (index >= 0)
        appName = appPath.Mid(index + 1);

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

    CString date = __DATE__;
    CString aboutBoxInfo;
    aboutBoxInfo.Format(L"%s\n\n%s\n%s\n%s\ncompiled with %s on %s",
        LoadString(IDW_MAIN).c_str(), appName.c_str(),
        LoadString(IDS_APP_VERSION).c_str(), win32Version.c_str(),
        sCompiler.c_str(), date.c_str());
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
