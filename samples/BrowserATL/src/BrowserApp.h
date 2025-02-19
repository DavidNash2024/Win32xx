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
    CBrowserApp() = default;
    virtual ~CBrowserApp() override = default;

protected:
    virtual BOOL InitInstance() override;

private:
    CBrowserApp(const CBrowserApp&) = delete;
    CBrowserApp& operator=(const CBrowserApp&) = delete;

    CMainFrame m_frame;
};


#endif // define BROWSERAPP_H
