/////////////////////////////
// ThemesApp.h
//

#ifndef THEMESAPP_H
#define THEMESAPP_H

#include "Mainfrm.h"


/////////////////////////////////////////////////////////////////
// CThemesApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CThemesApp : public CWinApp
{
public:
    CThemesApp() = default;
    virtual ~CThemesApp() override = default;

protected:
    virtual BOOL InitInstance() override;

private:
    CThemesApp(const CThemesApp&) = delete;
    CThemesApp& operator=(const CThemesApp&) = delete;

    CMainFrame m_frame;
};


#endif // define THEMESAPP_H
