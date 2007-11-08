//////////////////////////////////////////////////
// SplitterApp.h
//  Declaration of the CApp class


#ifndef SPLITTERAPP_H
#define SPLITTERAPP_H

#include "Mainfrm.h"


class CSplitterApp : public CWinApp
{
public:
	CSplitterApp(HINSTANCE hInstance);
	virtual ~CSplitterApp()  {}
	virtual BOOL InitInstance();

private:
	CMainFrame m_Frame;
};


#endif // define EFRAMEAPP_H

