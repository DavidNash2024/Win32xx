/////////////////////////////
// MDIFrameSplitterApp.h
//

#ifndef MDIFRAMESPLITTERAPP_H
#define MDIFRAMESPLITTERAPP_H

#include "MainMDIfrm.h"


////////////////////////////////////////////////
// Declaration of the CMDIFrameSplitterApp class
//
class CMDIFrameSplitterApp : public CWinApp
{
public:
    CMDIFrameSplitterApp();
    virtual ~CMDIFrameSplitterApp() {}

protected:
    virtual BOOL InitInstance();

private:
    CMainMDIFrame m_mainMDIFrame;
};


#endif // MDIFRAMESPLITTERAPP_H
