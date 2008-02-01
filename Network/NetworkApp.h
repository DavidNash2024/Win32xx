//////////////////////////////////////////////////
// Network.h

#ifndef NETWORK_H
#define NETWORK_H

#include "Mainfrm.h"


// Declaration of the CFrameApp class
class CNetworkApp : public CWinApp
{
public:
	CNetworkApp(HINSTANCE hInstance);
	virtual ~CNetworkApp();
	virtual BOOL InitInstance();

private:
	CMainFrame m_Frame;
};


#endif // define NETWORK_H

