/////////////////////////////////////////
// App.h
// Authors: Robert Tausworthe, David Nash
//

#ifndef SDI_APP_H
#define SDI_APP_H


#include "Mainfrm.h"

///////////////////////////////////////////////////////////////////////////////
// The application class. It is responsible for initializing the application,
// and for maintaining the application state. It is derived from CWinApp, which
// provides the basic application functionality, such as the message loop and
// the accelerator table.
class CApp : public CWinApp
{
public:
    CApp();
    virtual ~CApp() override = default;

    const CString&  GetAppName() const { return m_appName; }
    const CString&  GetAboutStatement() const { return m_aboutStatement; }

protected:
    virtual BOOL    InitInstance() override;

private:
    CApp(const CApp&) = delete;
    CApp& operator=(const CApp&) = delete;

    // Private functions,
    ULONG       DatInt(const CString &);
    CString     IntDat(ULONG);

    CMainFrame  m_frame;            // The mainframe.
    CString     m_appName;          // Name of app: minus directory and .exe.
    CString     m_aboutStatement;   // Latest compile date.
    CString     m_months;           // Month enumeration.
    CString     m_compiledWith;     // Compiler + system info.
    CString     m_version;          // App version info.
    CString     m_credits;          // About box notices.

    static const CString m_compiledOn;   // Latest compile date.
};

#endif // define SDI_APP_H
