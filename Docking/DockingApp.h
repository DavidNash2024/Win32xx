//////////////////////////////////////
// DockingApp.h


#ifndef DOCKINGAPP_H
#define DOCKINGAPP_H

#include "MainMDIFrm.h"


class CDockingApp : public CWinApp
{
public:
    CDockingApp();
    virtual ~CDockingApp() {}
	virtual BOOL InitInstance();
	CMainMDIFrame& GetMDIFrame() { return m_MainMDIFrame; }

private:
    CMainMDIFrame m_MainMDIFrame;

};


// returns a reference to the CDockingApp object
inline CDockingApp& GetMDIApp() { return *((CDockingApp*)GetApp()); }


#endif // DOCKINGAPP_H
