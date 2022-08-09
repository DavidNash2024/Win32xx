/////////////////////////////
// ContainerApp.h
//

#ifndef CONTAINERAPP_H
#define CONTAINERAPP_H

#include "Mainfrm.h"


/////////////////////////////////////////////////////////////////
// CDockContainerApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the main
// frame window when it runs.
class CDockContainerApp : public CWinApp
{
public:
    CDockContainerApp();
    virtual ~CDockContainerApp();

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();

private:
    CDockContainerApp(const CDockContainerApp&);                // Disable copy construction
    CDockContainerApp& operator = (const CDockContainerApp&);   // Disable assignment operator

    CMainFrame m_frame;
};


#endif // CONTAINERAPP_H
