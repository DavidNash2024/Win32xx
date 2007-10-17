//////////////////////////////////////////////////
// FrameApp.h

#ifndef FRAMEAPP_H
#define FRAMEAPP_H

#include "Mainfrm.h"


// Declaration of the CFrameApp class
class CPSApp : public CWinApp
{
public:
	CPSApp(HINSTANCE hInstance);
	virtual ~CPSApp();

private:
	CMainFrame m_Frame;
};


#endif // define FRAMEAPP_H

