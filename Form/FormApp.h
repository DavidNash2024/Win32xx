//////////////////////////////////////////////////
// FrameApp.h

#ifndef FORMEAPP_H
#define FORMAPP_H

#include "Mainfrm.h"


// Declaration of the CFrameApp class
class CFormApp : public CWinApp
{
public:
	CFormApp();
	virtual ~CFormApp();
	virtual BOOL InitInstance();
	CMainFrame& GetMainFrame() { return m_Frame; }

private:
	CMainFrame m_Frame;
};


// returns a reference to the CFormApp object
inline CFormApp& GetFormApp() { return *((CFormApp*)GetApp()); }


#endif // define FORMAPP_H

