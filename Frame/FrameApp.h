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
	CMainFrame& GetMainFrame() { return m_Frame; }

private:
	CMainFrame m_Frame;
};


// returns a reference to the CFrameApp object
inline CFrameApp& GetFrameApp() { return *((CFrameApp*)GetApp()); }


#endif // define FRAMEAPP_H

