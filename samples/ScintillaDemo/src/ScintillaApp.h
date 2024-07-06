/////////////////////////////
// ScintillaApp.h
//

#ifndef SCINTILLA_APP_H
#define SCINTILLA_APP_H

#include "Mainfrm.h"


////////////////////////////////////////////////////////////////
// ScintillaApp manages the application. It initializes the Win32++
// framework when it is constructed, and create the main frame
// window when it runs.
class ScintillaApp : public CWinApp
{
public:
    ScintillaApp();
    virtual ~ScintillaApp();

protected:
    // Virtual functions that override base class functions.
    virtual BOOL InitInstance();
    virtual BOOL OnIdle(LONG);

private:
    ScintillaApp(const ScintillaApp&);               // Disable copy construction
    ScintillaApp& operator=(const ScintillaApp&);    // Disable assignment operator

    CMainFrame m_frame;
};


#endif // define SCINTILLA_APP_H
