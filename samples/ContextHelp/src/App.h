//////////////////////////////////////////////////
// App.h

#ifndef SDI_APP_H
#define SDI_APP_H

#include "Mainfrm.h"


// Declaration of the CFrameApp class
class CApp : public CWinApp
{
public:
    CApp();
    virtual ~CApp();
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_frame; }

private:
    CMainFrame m_frame;

};


// Handy global functions
inline CApp* TheApp() { return static_cast<CApp*>(GetApp()); }


#endif // define SDI_APP_H

