/////////////////////////////
// FrameApp.h
//

#ifndef REBARAPP_H
#define REBARAPP_H

#include "Mainfrm.h"


////////////////////////////////////////////////////////////////
// CReBarApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CReBarApp : public CWinApp
{
public:
    CReBarApp() = default;
    virtual ~CReBarApp() override = default;

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance() override;

private:
    CReBarApp(const CReBarApp&) = delete;
    CReBarApp& operator=(const CReBarApp&) = delete;

    CMainFrame m_frame;
};


#endif // define REBARAPP_H
