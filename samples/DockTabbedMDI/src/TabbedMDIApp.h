//////////////////////////////////////////////////
// TabbedMDIApp.h

#ifndef TABBEDMDIAPP_H
#define TABBEDMDIAPP_H

#include "Mainfrm.h"


// Declaration of the CDockContainerApp class
class CTabbedMDIApp : public CWinApp
{
public:
	CTabbedMDIApp();
	virtual ~CTabbedMDIApp();
	virtual BOOL InitInstance();
	CMainFrame& GetMainFrame() { return m_Frame; }

private:
	CMainFrame m_Frame;
};


// returns a reference to the CTabbedMDIApp object
inline CTabbedMDIApp& GetTabbedMDIApp() { return static_cast<CTabbedMDIApp&>(*GetApp()); }


#endif // TABBEDMDIAPP_H

