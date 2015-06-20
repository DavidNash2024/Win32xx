//////////////////////////////////////////////////
// FrameApp.h

#ifndef STATUSBARAPP_H
#define STATUSBARAPP_H

#include "Mainfrm.h"


// Declaration of the CStatusBarApp class
class CStatusBarApp : public CWinApp
{
public:
	CStatusBarApp();
	virtual ~CStatusBarApp();
	virtual BOOL InitInstance();
	CMainFrame& GetMainFrame() { return m_Frame; }

private:
	CMainFrame m_Frame;
};


// a useful function that returns a reference to the CStatusBarApp object
inline CStatusBarApp& GetStatusBarApp() { return static_cast<CStatusBarApp&>(*GetApp()); }


#endif // define STATUSBARAPP_H

