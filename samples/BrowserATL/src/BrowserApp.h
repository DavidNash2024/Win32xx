/////////////////////////////
// BrowserApp.h
//

#ifndef BROWSERAPP_H
#define BROWSERAPP_H

#include "Mainfrm.h"

//////////////////////////////////////////////////////////////////
// CBrowserApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CBrowserApp : public CWinApp
{
public:
    CBrowserApp();
    virtual ~CBrowserApp();

protected:
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};


#endif // define BROWSERAPP_H
