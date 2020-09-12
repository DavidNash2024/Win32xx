/////////////////////////////
// TabbedMDIApp.h
//

#ifndef TABBEDMDIAPP_H
#define TABBEDMDIAPP_H

#include "Mainfrm.h"

/////////////////////////////////////////////
// Declaration of the CDockContainerApp class
//
class CTabbedMDIApp : public CWinApp
{
public:
    CTabbedMDIApp();
    virtual ~CTabbedMDIApp();
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};


#endif // TABBEDMDIAPP_H
