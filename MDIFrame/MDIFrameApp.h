//////////////////////////////////////
// MDIFrameApp.h


#ifndef MDIFRAMEAPP_H
#define MDIFRAMEAPP_H

#include "MainMDIFrm.h"


class CMDIFrameApp : public CMDIApp
{
public:
    CMDIFrameApp(HINSTANCE hInstance);
    virtual ~CMDIFrameApp() {}
	virtual BOOL InitInstance();

private:
    CMainMDIFrame m_MainMDIFrame;

};

#endif // MDIFRAMEAPP_H
