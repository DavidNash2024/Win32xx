/////////////////////////////////////////
// App.cpp
// Authors: Robert Tausworthe, David Nash
//

#include "stdafx.h"
#include "App.h"

// Latest CDoc.cpp file compilation date
const CString CApp::m_compiledOn = __DATE__;

// Default constructor.
CApp::CApp()
{
    m_credits = L"CTime, and Scrolling Demo";
    m_version = L"Version 3.0 (2024)";

    // Generate compiler information for the About box
#ifdef __GNUC__
    m_compiledWith << L"GNU C++ " << __GNUC__ << L"."
        << __GNUC_MINOR__ << L"." << __GNUC_PATCHLEVEL__;
#elif defined(_MSC_VER)
    m_compiledWith << L"MS C++ " << _MSC_VER / 100
        << L"." << _MSC_VER % 100;
#elif defined(__BORLANDC__)
    m_compiledWith << L"Borland C++ " << __BORLANDC__ / 0x100
        << L"." << (__BORLANDC__ & 0x0F0) / 0x10;
#else
    m_sCompiled_with = L"(unknown compiler)";
#endif

    m_months = L"Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec";
}

////////////////////////////
// App function definitions.
//

// This method is immediately called when the application starts. It create the
// frame window, and performs initialization of the app. Return TRUE on success.
// Returning FALSE terminates the program.
BOOL CApp::InitInstance()
{
    // Retrieve the application's filename.
    CString appPath = GetCommandLineArgs().front();
    int index = appPath.ReverseFind(L"\\");
    if (index >= 0)
         m_appName = appPath.Mid(index + 1);

    // Generate the About box static information: first the latest
    // date one of the main stream files was compiled.
    ULONG compiled_on = DatInt(CApp::m_compiledOn);
    compiled_on = std::max(compiled_on, DatInt(CDoc::m_compiledOn));
    compiled_on = std::max(compiled_on, DatInt(CMainFrame::m_compiledOn));
    compiled_on = std::max(compiled_on, DatInt(CView::m_compiledOn));
    m_aboutStatement.Format(L"%s\n\n(%s)\n%s\ncompiled with %s\non %s",
        m_credits.c_str(), m_appName.c_str(),
        m_version.c_str(), m_compiledWith.c_str(),
        IntDat(compiled_on).c_str());

    // Set the AboutBoxStatus.
    m_frame.GetAboutBox().SetStatus(m_aboutStatement);

    // Create the frame window.
    m_frame.SetAppName(m_appName);
    m_frame.Create();   // Throws a CWinException on failure.
    return TRUE;
}

// Convert the date, of the form mmm dd yyyy, to a ULONG integer of the form
// yyyymodd, where mmm is character based month, and mo is 0 (Jan) to 11 (Dec).
ULONG CApp::DatInt(const CString &date)
{
    int yyyy = _ttoi(date.Mid(7, 4).c_str());
    int dd   = _ttoi(date.Mid(4, 2).c_str());
    int mo   = CApp::m_months.Find(date.Mid(0, 3)) / 4;
    ULONG   ans  = ((yyyy * 100 + mo) * 100) + dd;
    return  ans;
}

// Convert the ULONG date, of form yyyymodd, to a CString date of the form
// mmm dd yyyy, where mmm is character based month, and mo is 0 (Jan) to
// 11 (Dec).
CString CApp::IntDat(ULONG hexdate)
{
    UINT dd = (hexdate % 100);
    UINT mo = (hexdate / 100) % 100;
    UINT yyyy = (hexdate / 10000);
    CString ans;
    ans.Format(L"%s %02d, %u", CApp::m_months.Mid(4 * mo, 3).c_str(),
        dd, yyyy);
    return ans;
}
