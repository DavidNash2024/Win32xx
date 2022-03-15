/////////////////////////////
// TabbedMDIApp.h
//

#ifndef TABBEDMDIAPP_H
#define TABBEDMDIAPP_H

#include "Mainfrm.h"


////////////////////////////////////////////////////////////
// CTabbedMDIApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the
// main frame window when it runs.
class CTabbedMDIApp : public CWinApp
{
public:
    CTabbedMDIApp();
    virtual ~CTabbedMDIApp();

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};


#endif // TABBEDMDIAPP_H
