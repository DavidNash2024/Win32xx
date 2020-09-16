/////////////////////////////
// FrameApp.h
//

#ifndef STATUSBARAPP_H
#define STATUSBARAPP_H

#include "Mainfrm.h"


/////////////////////////////////////////////////////////////////
// CStatusBarApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the main
// frame window when it runs.
class CStatusBarApp : public CWinApp
{
public:
    CStatusBarApp();
    virtual ~CStatusBarApp();
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};


#endif // define STATUSBARAPP_H
