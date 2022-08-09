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
    CReBarApp();
    virtual ~CReBarApp();

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();

private:
    CReBarApp(const CReBarApp&);                // Disable copy construction
    CReBarApp& operator = (const CReBarApp&);   // Disable assignment operator

    CMainFrame m_frame;
};


#endif // define REBARAPP_H
