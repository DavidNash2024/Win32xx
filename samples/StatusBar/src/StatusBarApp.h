/////////////////////////////
// FrameApp.h
//

#pragma once

#include "Mainfrm.h"

/////////////////////////////////////////////////////////////////
// CStatusBarApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the main
// frame window when it runs.
class CStatusBarApp : public CWinApp
{
public:
    CStatusBarApp() = default;
    virtual ~CStatusBarApp() override = default;

protected:
    virtual BOOL InitInstance() override;

private:
    CStatusBarApp(const CStatusBarApp&) = delete;
    CStatusBarApp& operator=(const CStatusBarApp&) = delete;

    CMainFrame m_frame;
};

