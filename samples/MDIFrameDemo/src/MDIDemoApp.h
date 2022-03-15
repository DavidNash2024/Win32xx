/////////////////////////////
// MDIDemoApp.h
//

#ifndef MDIDEMOAPP_H
#define MDIDEMOAPP_H

#include "MainMDIfrm.h"


///////////////////////////////////////////////////////////////////
// CMDIDemoApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main MDI frame
// window when it runs.
class CMDIDemoApp : public CWinApp
{
public:
    CMDIDemoApp();
    virtual ~CMDIDemoApp() {}

protected:
    virtual BOOL InitInstance();

private:
    CMainMDIFrame m_mainMDIFrame;
};


#endif // MDIDEMOAPP_H
