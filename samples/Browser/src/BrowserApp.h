/////////////////////////////////////////
// BrowserApp.h

#ifndef BROWSERAPP_H
#define BROWSERAPP_H

#include "Mainfrm.h"


// Declaration of the CWinApp class
class CBrowserApp : public CWinApp
{
public:
    CBrowserApp();
    virtual ~CBrowserApp() {}
	virtual BOOL InitInstance();
	CMainFrame& GetMainFrame() { return m_Frame; }

private:
    CMainFrame m_Frame;
};


// returns a reference to the CBrowserApp object
inline CBrowserApp& GetBrowserApp() { return static_cast<CBrowserApp&>(*GetApp()); }


#endif // BROWSERAPP_H

