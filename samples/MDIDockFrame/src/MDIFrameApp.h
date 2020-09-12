/////////////////////////////
// MDIFrameApp.h
//

#ifndef MDIFRAMEAPP_H
#define MDIFRAMEAPP_H

#include "MainMDIFrm.h"

////////////////////////////////////////
// Declaration of the CMDIFrameApp class
//
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
