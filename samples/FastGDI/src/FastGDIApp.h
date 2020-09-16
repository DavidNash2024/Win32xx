//////////////////////////////////////////////////
// FastGDIApp.h

#ifndef FASTGDIAPP_H
#define FASTGDIAPP_H

#include "Mainfrm.h"

//////////////////////////////////////////////////////////////////
// CFastGDIApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CFastGDIApp : public CWinApp
{
public:
    CFastGDIApp();
    virtual ~CFastGDIApp();
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_frame; }

private:
    CMainFrame m_frame;
};


// returns a pointer to the CFastGDIApp object
inline CFastGDIApp* GetFrameApp() { return static_cast<CFastGDIApp*>(GetApp()); }


#endif // define FASTGDIAPP_H

