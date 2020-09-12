/////////////////////////////
// BrowserApp.h
//

#ifndef BROWSERAPP_H
#define BROWSERAPP_H

#include "Mainfrm.h"

///////////////////////////////////
// Declaration of the CWinApp class
//
class CBrowserApp : public CWinApp
{
public:
    CBrowserApp();
    virtual ~CBrowserApp() {}
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};


#endif // BROWSERAPP_H
