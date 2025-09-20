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
    CTrayApp() = default;
    virtual ~CTrayApp() override = default;

protected:
    virtual BOOL InitInstance() override;

private:
    CTrayApp(const CTrayApp&) = delete;
    CTrayApp& operator=(const CTrayApp&) = delete;

    CView m_view;
};


#endif // TRAYAPP_H
