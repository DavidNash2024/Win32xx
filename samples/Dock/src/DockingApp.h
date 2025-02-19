/////////////////////////////
// DockingApp.h
//

#ifndef DOCKINGAPP_H
#define DOCKINGAPP_H

#include "Mainfrm.h"


////////////////////////////////////////////////////////////
// CDockingApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the
// main frame window when it runs.
class CDockingApp : public CWinApp
{
public:
    CDockingApp() = default;
    virtual ~CDockingApp() override = default;

protected:
    // Virtual functions that override base class functions.
    virtual BOOL InitInstance() override;

private:
    CDockingApp(const CDockingApp&) = delete;
    CDockingApp& operator=(const CDockingApp&) = delete;

    CMainFrame m_MainFrame;
};


#endif // DOCKINGAPP_H
