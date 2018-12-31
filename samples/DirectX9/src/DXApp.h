//////////////////////////////////////////////////
// DXApp.h
//  Declaration of the CDXApp class

#ifndef DXAPP_H
#define DXAPP_H

#include "MainFrm.h"


class CDXApp : public CWinApp
{
public:
    CDXApp();
    virtual ~CDXApp() {}
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_frame; }

private:
    CMainFrame m_frame;
};


// returns a pointer to the CDXApp object
inline CDXApp* GetDXApp() { return static_cast<CDXApp*>(GetApp()); }


#endif
