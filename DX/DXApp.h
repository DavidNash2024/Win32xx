//////////////////////////////////////////////////
// DXApp.h
//  Declaration of the CDXApp class

#ifndef DXAPP_H
#define DXAPP_H

#include "../WinDev++/WinCore.h"
#include "View.h"


class CDXApp : public CWinApp
{
public:
    CDXApp();
    virtual ~CDXApp() {}
	virtual BOOL InitInstance();
	virtual int MessageLoop();
	CView& GetView() { return m_View; }

private:
    CView m_View;
};


// returns a reference to the CDXApp object
inline CDXApp& GetDXApp() { return *((CDXApp*)GetApp()); }


#endif
