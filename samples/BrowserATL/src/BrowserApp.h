/////////////////////////////
// BrowserApp.h
//

#ifndef BROWSERAPP_H
#define BROWSERAPP_H

#include "Mainfrm.h"

/////////////////////////////////////
// Declaration of the CFrameApp class
//
class CBrowserApp : public CWinApp
{
public:
    CBrowserApp();
    virtual ~CBrowserApp();
    BOOL InitInstance();
    const CMainFrame* GetMainFrame() const { return &m_frame; }

private:
    CMainFrame m_frame;
};


// returns a pointer to the CBrowserApp object
inline CBrowserApp* GetBrowserApp() { return static_cast<CBrowserApp*>(GetApp()); }


#endif // define BROWSERAPP_H

