/////////////////////////////
// MDIFrameApp.h
//

#ifndef MDIFRAMEAPP_H
#define MDIFRAMEAPP_H

#include "MainMDIfrm.h"


///////////////////////////////////////////////////////////////////
// CMDIFrameApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CMDIFrameApp : public CWinApp
{
public:
    CMDIFrameApp() = default;
    virtual ~CMDIFrameApp() override = default;

protected:
    virtual BOOL InitInstance() override;

private:
    CMDIFrameApp(const CMDIFrameApp&) = delete;
    CMDIFrameApp& operator = (const CMDIFrameApp&) = delete;

    CMainMDIFrame m_mainMDIFrame;
};


#endif // MDIFRAMEAPP_H
