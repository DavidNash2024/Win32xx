//////////////////////////////////////////////////
// FastGDIApp.h

#ifndef FASTGDIAPP_H
#define FASTGDIAPP_H

#include "Mainfrm.h"


// Declaration of the CFastGDIApp class
class CFastGDIApp : public CWinApp
{
public:
	CFastGDIApp();
	virtual ~CFastGDIApp();
	virtual BOOL InitInstance();
	CMainFrame& GetMainFrame() { return m_Frame; }

private:
	CMainFrame m_Frame;
};


// returns a reference to the CFastGDIApp object
inline CFastGDIApp& GetFrameApp() { return static_cast<CFastGDIApp&>(GetApp()); }


#endif // define FASTGDIAPP_H

