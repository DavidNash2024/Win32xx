/////////////////////////////
// TabDemoApp.h
//

#ifndef CONTAINERAPP_H
#define CONTAINERAPP_H


#include "Mainfrm.h"

////////////////////////////////////////
// Declaration of the CTabDemoApp class.
//
class CTabDemoApp : public CWinApp
{
public:
    CTabDemoApp();
    virtual ~CTabDemoApp();
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_frame; }

private:
    CMainFrame m_frame;
};


// returns a pointer to the CTabDemoApp object
inline CTabDemoApp* GetTabDemoApp() { return static_cast<CTabDemoApp*>(GetApp()); }


#endif // CONTAINERAPP_H

