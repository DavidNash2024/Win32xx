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

private:
    CMainMDIFrame m_MainMDIFrame;

};

#endif // MDIDEMOAPP_H
