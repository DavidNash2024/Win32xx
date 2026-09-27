/////////////////////////////
// FrameApp.h
//

#pragma once

#include "Mainfrm.h"

////////////////////////////////////////////////////////////////
// CReBarApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CReBarApp : public CWinApp
{
public:
    CReBarApp() = default;
    virtual ~CReBarApp() override = default;

protected:
    virtual BOOL InitInstance() override;

private:
    CReBarApp(const CReBarApp&) = delete;
    CReBarApp& operator=(const CReBarApp&) = delete;

    CMainFrame m_frame;
};

