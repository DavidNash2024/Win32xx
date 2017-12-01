//////////////////////////////////////
// MDIFrameSplitterApp.h

#ifndef MDIFRAMESPLITTERAPP_H
#define MDIFRAMESPLITTERAPP_H

#include "MainMDIFrm.h"


class CMDIFrameSplitterApp : public CWinApp
{
public:
    CMDIFrameSplitterApp();
    virtual ~CMDIFrameSplitterApp() {}
    virtual BOOL InitInstance();
    CMainMDIFrame& GetMDIFrame() { return m_MainMDIFrame; }

private:
    CMainMDIFrame m_MainMDIFrame;

};


// returns a reference to the CMDIFrameSplitterApp object
inline CMDIFrameSplitterApp& GetMDIApp() { return static_cast<CMDIFrameSplitterApp&>(GetApp()); }


#endif // MDIFRAMESPLITTERAPP_H
