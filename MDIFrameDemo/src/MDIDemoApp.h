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
	CMainMDIFrame& GetMDIFrame() { return m_MainMDIFrame; }

private:
    CMainMDIFrame m_MainMDIFrame;

};


// returns a reference to the CMDIDemoApp object
inline CMDIDemoApp& GetMDIApp() { return *((CMDIDemoApp*)GetApp()); }


#endif // MDIDEMOAPP_H
