/////////////////////////////
// FrameApp.h
//

#ifndef REBARAPP_H
#define REBARAPP_H

#include "Mainfrm.h"

/////////////////////////////////////
// Declaration of the CReBarApp class
//
class CReBarApp : public CWinApp
{
public:
    CReBarApp();
    virtual ~CReBarApp();
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};


#endif // define REBARAPP_H
