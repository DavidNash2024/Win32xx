//////////////////////////////////////////////////
// DXApp.h
//  Declaration of the CDXApp class

#ifndef DXAPP_H
#define DXAPP_H

#include "../Win32++/WinCore.h"
#include "View.h"


class CDXApp : public CWinApp
{
public:
    CDXApp(HINSTANCE hInstance);
    virtual ~CDXApp() {}
	virtual BOOL InitInstance();
	virtual int MessageLoop();

private:
    CView m_View;
};

#endif
