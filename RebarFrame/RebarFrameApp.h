//////////////////////////////////////////
// RebarFrameApp.h
//  Declaration of the CRebarFrameApp class


#ifndef REBARFRAMEAPP_H
#define REBARFRAMEAPP_H

#include "Mainfrm.h"


class CRebarFrameApp : public CWinApp
{
public:
	CRebarFrameApp(HINSTANCE hInstance);
	virtual ~CRebarFrameApp() {}

private:
	CMainFrame m_Frame;
};


#endif // define REBARFRAMEAPP_H

