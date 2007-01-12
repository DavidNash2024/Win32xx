//////////////////////////////////////
// MDIDemoApp.h


#ifndef MDIDEMOAPP_H
#define MDIDEMOAPP_H

#include "MainMDIFrm.h"


class CMDIDemoApp : public CMDIApp
{
public:
    CMDIDemoApp(HINSTANCE hInstance);
    virtual ~CMDIDemoApp() {}

private:
    CMainMDIFrame m_MainMDIFrame;

};

#endif // MDIDEMOAPP_H
