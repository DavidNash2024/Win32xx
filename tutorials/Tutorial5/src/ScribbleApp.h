////////////////////////////////////////
// ScribbleApp.h

#pragma once

#include "wxx_wincore.h"
#include "Mainfrm.h"

///////////////////////////////////////////////////////////////////
// CScribbleApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CScribbleApp : public CWinApp
{
public:
    CScribbleApp() = default;
    virtual ~CScribbleApp() override = default;
    virtual BOOL InitInstance();

private:
    CScribbleApp(const CScribbleApp&) = delete;
    CScribbleApp& operator=(const CScribbleApp&) = delete;

    CMainFrame m_frame;
};

