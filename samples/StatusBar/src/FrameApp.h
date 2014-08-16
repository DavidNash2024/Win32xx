//////////////////////////////////////////////////
// FrameApp.h

#ifndef FRAMEAPP_H
#define FRAMEAPP_H

#include "Mainfrm.h"


// Declaration of the CFrameApp class
class CFrameApp : public CWinApp
{
public:
	CFrameApp();
	virtual ~CFrameApp();
	virtual BOOL InitInstance();
	CMainFrame* GetMainFrame() { return &m_Frame; }

private:
	CMainFrame m_Frame;
};


// a useful function that returns a pointer to the CFrameApp object
inline CFrameApp* GetFrameApp() { return static_cast<CFrameApp*>(GetApp()); }


#endif // define FRAMEAPP_H

