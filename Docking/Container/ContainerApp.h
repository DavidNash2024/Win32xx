//////////////////////////////////////////////////
// ContainerApp.h

#ifndef CONTAINERAPP_H
#define CONTAINERAPP_H

#include "Mainfrm.h"


// Declaration of the CContainerApp class
class CContainerApp : public CWinApp
{
public:
	CContainerApp();
	virtual ~CContainerApp();
	virtual BOOL InitInstance();
	CMainFrame& GetMainFrame() { return m_Frame; }

private:
	CMainFrame m_Frame;
};


// returns a reference to the CContainerApp object
inline CContainerApp& GetContainerApp() { return *((CContainerApp*)GetApp()); }


#endif // CONTAINERAPP_H

