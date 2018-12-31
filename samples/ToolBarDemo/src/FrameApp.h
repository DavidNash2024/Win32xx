//////////////////////////////////////////////////
// FrameApp.h

#ifndef FRAMEAPP_H
#define FRAMEAPP_H

#include "Mainfrm.h"


// Declaration of the CToolBarDemoApp class
class CToolBarDemoApp : public CWinApp
{
public:
    CToolBarDemoApp();
    virtual ~CToolBarDemoApp();
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_frame; }

private:
    CMainFrame m_frame;
};


// returns a pointer to the CToolBarDemoApp object
inline CToolBarDemoApp* GetFrameApp() { return static_cast<CToolBarDemoApp*>(GetApp()); }


#endif // define FRAMEAPP_H

