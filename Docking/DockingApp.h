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
	CDockFrame& GetDockFrame() { return m_DockFrame; }

private:
    CDockFrame m_DockFrame;

};


// returns a reference to the CDockingApp object
inline CDockingApp& GetDockApp() { return *((CDockingApp*)GetApp()); }


#endif // DOCKINGAPP_H
