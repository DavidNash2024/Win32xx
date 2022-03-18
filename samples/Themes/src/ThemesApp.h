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
    CThemesApp();
    virtual ~CThemesApp();

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};


#endif // define THEMESAPP_H
