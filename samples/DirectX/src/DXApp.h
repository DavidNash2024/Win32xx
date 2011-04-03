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

private:
    CMainFrame m_Frame;
};


// returns a reference to the CDXApp object
inline CDXApp& GetDXApp() { return *((CDXApp*)GetApp()); }


#endif
