/////////////////////////////
// MDIFrameSplitterApp.h
//

#pragma once

#include "MainMDIfrm.h"

////////////////////////////////////////////////
// Declaration of the CMDIFrameSplitterApp class
//
class CMDIFrameSplitterApp : public CWinApp
{
public:
    CMDIFrameSplitterApp() = default;
    virtual ~CMDIFrameSplitterApp() override = default;

protected:
    virtual BOOL InitInstance() override;

private:
    CMDIFrameSplitterApp(const CMDIFrameSplitterApp&) = delete;
    CMDIFrameSplitterApp& operator=(const CMDIFrameSplitterApp&) = delete;

    CMainMDIFrame m_mainMDIFrame;
};

