/////////////////////////////
// MDIDemoApp.h
//

#ifndef MDIDEMOAPP_H
#define MDIDEMOAPP_H

#include "MainMDIFrm.h"

///////////////////////////////////////
// Declaration of the CMDIDemoApp class
//
class CMDIDemoApp : public CWinApp
{
public:
    CMDIDemoApp();
    virtual ~CMDIDemoApp() {}
    virtual BOOL InitInstance();

private:
    CMainMDIFrame m_mainMDIFrame;
};


#endif // MDIDEMOAPP_H
