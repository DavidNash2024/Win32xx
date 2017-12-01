//////////////////////////////////////
// DockingApp.h


#ifndef DOCKINGAPP_H
#define DOCKINGAPP_H


#include "Mainfrm.h"


class CDockingApp : public CWinApp
{
public:
    CDockingApp();
    virtual ~CDockingApp() {}
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_MainFrame; }

private:
    CMainFrame m_MainFrame;

};


// returns a reference to the CDockingApp object
inline CDockingApp& GetDockApp() { return static_cast<CDockingApp&>(GetApp()); }


#endif // DOCKINGAPP_H
