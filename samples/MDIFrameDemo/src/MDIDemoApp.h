//////////////////////////////////////
// MDIDemoApp.h

#ifndef MDIDEMOAPP_H
#define MDIDEMOAPP_H

#include "MainMDIFrm.h"


class CMDIDemoApp : public CWinApp
{
public:
    CMDIDemoApp();
    virtual ~CMDIDemoApp() {}
    virtual BOOL InitInstance();
    CMainMDIFrame& GetMDIFrame() { return m_mainMDIFrame; }

private:
    CMainMDIFrame m_mainMDIFrame;

};


// returns a pointer to the CMDIDemoApp object
inline CMDIDemoApp* GetMDIApp() { return static_cast<CMDIDemoApp*>(GetApp()); }


#endif // MDIDEMOAPP_H
