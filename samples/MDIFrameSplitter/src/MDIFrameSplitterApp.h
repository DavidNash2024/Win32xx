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
    CMDIFrameSplitterApp() = default;
    virtual ~CMDIFrameSplitterApp() override = default;

protected:
    virtual BOOL InitInstance() override;

private:
    CMDIFrameSplitterApp(const CMDIFrameSplitterApp&) = delete;
    CMDIFrameSplitterApp& operator=(const CMDIFrameSplitterApp&) = delete;

    CMainMDIFrame m_mainMDIFrame;
};


#endif // MDIFRAMESPLITTERAPP_H
