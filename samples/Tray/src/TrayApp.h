/////////////////////////////
// TrayApp.h
//

#ifndef TRAYAPP_H
#define TRAYAPP_H

#include "View.h"

////////////////////////////////////
// Declaration of the CWinApp class.
//
class CTrayApp : public CWinApp
{
public:
    CTrayApp();
    virtual ~CTrayApp() {}
    virtual BOOL InitInstance();

private:
    CView m_view;
};


#endif // TRAYAPP_H
