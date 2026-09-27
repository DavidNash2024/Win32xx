/////////////////////////////
// TabDemoApp.h
//

#pragma once

#include "Mainfrm.h"

//////////////////////////////////////////////////////////////////
// CTabDemoApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CTabDemoApp : public CWinApp
{
public:
    CTabDemoApp() = default;
    virtual ~CTabDemoApp() override = default;

protected:
    virtual BOOL InitInstance() override;

private:
    CTabDemoApp(const CTabDemoApp&) = delete;
    CTabDemoApp& operator=(const CTabDemoApp&) = delete;

    CMainFrame m_frame;
};

