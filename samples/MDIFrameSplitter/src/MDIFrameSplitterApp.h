/////////////////////////////
// MDIFrameSplitterApp.h
//

#ifndef MDIFRAMESPLITTERAPP_H
#define MDIFRAMESPLITTERAPP_H

#include "MainMDIFrm.h"


////////////////////////////////////////////////
// Declaration of the CMDIFrameSplitterApp class
//
class CMDIFrameSplitterApp : public CWinApp
{
public:
    CMDIFrameSplitterApp();
    virtual ~CMDIFrameSplitterApp() {}
    virtual BOOL InitInstance();

private:
    CMainMDIFrame m_mainMDIFrame;
};


#endif // MDIFRAMESPLITTERAPP_H
