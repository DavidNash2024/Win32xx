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
    CMainMDIFrame& GetMDIFrame() { return m_mainMDIFrame; }

private:
    CMainMDIFrame m_mainMDIFrame;
};


// returns a pointer to the CMDIFrameApp object
inline CMDIFrameApp* GetMDIApp() { return static_cast<CMDIFrameApp*>(GetApp()); }


#endif // MDIFRAMEAPP_H
