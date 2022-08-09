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
    CMDIFrameApp();
    virtual ~CMDIFrameApp() {}

protected:
    virtual BOOL InitInstance();

private:
    CMDIFrameApp(const CMDIFrameApp&);                // Disable copy construction
    CMDIFrameApp& operator = (const CMDIFrameApp&);   // Disable assignment operator

    CMainMDIFrame m_mainMDIFrame;
};


#endif // MDIFRAMEAPP_H
