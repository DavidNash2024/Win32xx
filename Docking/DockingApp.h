//////////////////////////////////////
// DockingApp.h


#ifndef DOCKINGAPP_H
#define DOCKINGAPP_H

#include "DockFrame.h"


class CDockingApp : public CWinApp
{
public:
    CDockingApp();
    virtual ~CDockingApp() {}
	virtual BOOL InitInstance();
	CDockFrame& GetMDIFrame() { return m_MainMDIFrame; }

private:
    CDockFrame m_MainMDIFrame;

};


// returns a reference to the CDockingApp object
inline CDockingApp& GetMDIApp() { return *((CDockingApp*)GetApp()); }


#endif // DOCKINGAPP_H
