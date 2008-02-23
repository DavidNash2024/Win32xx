//////////////////////////////////////////////////
// BrowserApp.h

#ifndef BROWSERAPP_H
#define BROWSERAPP_H

#include "Mainfrm.h"


// Declaration of the CFrameApp class
class CBrowserApp : public CWinApp
{
public:
	CBrowserApp();
	virtual ~CBrowserApp();
	BOOL InitInstance();

private:
	CMainFrame m_Frame;
};


#endif // define BROWSERAPP_H

