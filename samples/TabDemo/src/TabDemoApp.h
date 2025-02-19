/////////////////////////////
// TabDemoApp.h
//

#ifndef CONTAINERAPP_H
#define CONTAINERAPP_H


#include "Mainfrm.h"


//////////////////////////////////////////////////////////////////
// CTabDemoApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CTabDemoApp : public CWinApp
{
public:
    CTabDemoApp() = default;
    virtual ~CTabDemoApp() override = default;

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance() override;

private:
    CTabDemoApp(const CTabDemoApp&) = delete;
    CTabDemoApp& operator=(const CTabDemoApp&) = delete;

    CMainFrame m_frame;
};


#endif // CONTAINERAPP_H
