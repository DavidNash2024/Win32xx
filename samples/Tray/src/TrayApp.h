/////////////////////////////
// TrayApp.h
//

#ifndef TRAYAPP_H
#define TRAYAPP_H

#include "View.h"


////////////////////////////////////////////////////////
// CTrayApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates
// the main window when it runs.
class CTrayApp : public CWinApp
{
public:
    CTrayApp();
    virtual ~CTrayApp() {}

protected:
    // Virtual functions that override base class functions 
    virtual BOOL InitInstance();

private:
    CView m_view;
};


#endif // TRAYAPP_H
