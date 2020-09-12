/////////////////////////////
// FrameApp.h
//

#ifndef STATUSBARAPP_H
#define STATUSBARAPP_H

#include "Mainfrm.h"

//////////////////////////////////////////
// Declaration of the CStatusBarApp class.
//
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
