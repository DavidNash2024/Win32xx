//////////////////////////////////////////////////
// FrameApp.h

#ifndef FRAMEAPP_H
#define FRAMEAPP_H

#include "Mainfrm.h"


// Declaration of the CReBarDemoApp class
class CReBarDemoApp : public CWinApp
{
public:
	CReBarDemoApp();
	virtual ~CReBarDemoApp();
	virtual BOOL InitInstance();
	CMainFrame* GetMainFrame() { return &m_Frame; }

private:
	CMainFrame m_Frame;
};


// a useful function that returns a pointer to the CReBarDemoApp object
inline CReBarDemoApp* GetFrameApp() { return static_cast<CReBarDemoApp*>(GetApp()); }


#endif // define FRAMEAPP_H

