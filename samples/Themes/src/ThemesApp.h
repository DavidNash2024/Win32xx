/////////////////////////////
// ThemesApp.h
//

#ifndef THEMESAPP_H
#define THEMESAPP_H

#include "Mainfrm.h"

//////////////////////////////////////
// Declaration of the CFrameApp class.
//
class CThemesApp : public CWinApp
{
public:
    CThemesApp();
    virtual ~CThemesApp();
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};


#endif // define THEMESAPP_H
