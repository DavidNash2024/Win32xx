//////////////////////////////////////////////////
// ThemesApp.h

#ifndef THEMESAPP_H
#define THEMESAPP_H

#include "Mainfrm.h"


// Declaration of the CFrameApp class
class CThemesApp : public CWinApp
{
public:
	CThemesApp();
	virtual ~CThemesApp();
	virtual BOOL InitInstance();
	CMainFrame& GetMainFrame() { return m_Frame; }

private:
	CMainFrame m_Frame;
};


// returns a reference to the CThemesApp object
inline CThemesApp& GetThemeApp() { return *((CThemesApp*)GetApp()); }


#endif // define THEMESAPP_H

