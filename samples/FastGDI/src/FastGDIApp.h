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
    CMainFrame& GetMainFrame() { return m_frame; }

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};


// returns a pointer to the CFastGDIApp object
inline CFastGDIApp* GetFrameApp() { return static_cast<CFastGDIApp*>(GetApp()); }


#endif // define FASTGDIAPP_H

