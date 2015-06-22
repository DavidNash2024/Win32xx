/////////////////////////////////////////
// TrayApp.h

#ifndef TRAYAPP_H
#define TRAYAPP_H

#include "View.h"


// Declaration of the CWinApp class
class CTrayApp : public CWinApp
{
public:
    CTrayApp();
    virtual ~CTrayApp() {}
	virtual BOOL InitInstance();
	CView& GetView() { return m_View; }

private:
    CView m_View;
};


// returns a reference to the CTrayApp object
inline CTrayApp& GetTrayApp() { return static_cast<CTrayApp&>(GetApp()); }


#endif // TRAYAPP_H
