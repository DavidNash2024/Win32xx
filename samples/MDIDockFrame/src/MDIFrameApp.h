/////////////////////////////
// MDIFrameApp.h
//

#ifndef MDIFRAMEAPP_H
#define MDIFRAMEAPP_H

#include "MainMDIFrm.h"


///////////////////////////////////////////////////////////////////
// CMDIFrameApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CMDIFrameApp : public CWinApp
{
public:
    CMDIFrameApp();
    virtual ~CMDIFrameApp() {}
    virtual BOOL InitInstance();

private:
    CMainMDIFrame m_mainMDIFrame;
};


#endif // MDIFRAMEAPP_H
