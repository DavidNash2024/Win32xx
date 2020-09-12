/////////////////////////////
// ContainerApp.h
//

#ifndef CONTAINERAPP_H
#define CONTAINERAPP_H

#include "Mainfrm.h"


/////////////////////////////////////////////
// Declaration of the CDockContainerApp class
//
class CDockContainerApp : public CWinApp
{
public:
    CDockContainerApp();
    virtual ~CDockContainerApp();
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};


#endif // CONTAINERAPP_H
