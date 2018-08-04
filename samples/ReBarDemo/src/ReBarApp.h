//////////////////////////////////////////////////
// FrameApp.h

#ifndef REBARAPP_H
#define REBARAPP_H

#include "Mainfrm.h"


// Declaration of the CReBarApp class
class CReBarApp : public CWinApp
{
public:
    CReBarApp();
    virtual ~CReBarApp();
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_frame; }

private:
    CMainFrame m_frame;
};


// a useful function that returns a reference to the CReBarApp object
inline CReBarApp& GetReBarApp() { return static_cast<CReBarApp&>(GetApp()); }


#endif // define REBARAPP_H

