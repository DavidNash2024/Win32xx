/////////////////////////////////////////
// BrowserApp.h

#ifndef BROWSERAPP_H
#define BROWSERAPP_H

#include "MyBrowser.h"


// Declaration of the CWinApp class
class CBrowserApp : public CWinApp
{
public:
    CBrowserApp();
    virtual ~CBrowserApp() {}
	virtual BOOL InitInstance();
	CMyBrowser& GetView() { return m_View; }

private:
    CMyBrowser m_View;
};


// returns a reference to the CBrowserApp object
inline CBrowserApp& GetBrowserApp() { return *((CBrowserApp*)GetApp()); }


#endif // BROWSERAPP_H

