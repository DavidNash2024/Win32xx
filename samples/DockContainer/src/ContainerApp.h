//////////////////////////////////////////////////
// ContainerApp.h

#ifndef CONTAINERAPP_H
#define CONTAINERAPP_H

#include "Mainfrm.h"


// Declaration of the CDockContainerApp class
class CDockContainerApp : public CWinApp
{
public:
    CDockContainerApp();
    virtual ~CDockContainerApp();
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_frame; }

private:
    CMainFrame m_frame;
};


// returns a reference to the CDockContainerApp object
inline CDockContainerApp& GetContainerApp() { return static_cast<CDockContainerApp&>(GetApp()); }


#endif // CONTAINERAPP_H

