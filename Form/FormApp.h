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

private:
	CMainFrame m_Frame;
};


#endif // define FORMAPP_H

