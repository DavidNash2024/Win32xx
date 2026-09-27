/////////////////////////////
// FrameApp.h
//

#pragma once

#include "Mainfrm.h"

////////////////////////////////////////////////////////////////
// CFrameApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CFrameApp : public CWinApp
{
public:
    CFrameApp() = default;
    virtual ~CFrameApp() override = default;

protected:
    virtual BOOL InitInstance() override;
    virtual BOOL OnIdle(LONG) override;

private:
    CFrameApp(const CFrameApp&) = delete;
    CFrameApp& operator=(const CFrameApp&) = delete;

    CMainFrame m_frame;
};

