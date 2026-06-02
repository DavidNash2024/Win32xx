/////////////////////////////
// ScintillaApp.h
//

#ifndef SCINTILLA_APP_H
#define SCINTILLA_APP_H

#include "Mainfrm.h"


////////////////////////////////////////////////////////////////
// ScintillaApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class ScintillaApp : public CWinApp
{
public:
    ScintillaApp() = default;
    virtual ~ScintillaApp() = default;

protected:
    virtual BOOL InitInstance() override;
    virtual BOOL OnIdle(LONG) override;

private:
    ScintillaApp(const ScintillaApp&) = delete;
    ScintillaApp& operator=(const ScintillaApp&) = delete;

    CMainFrame m_frame;
};


#endif // define SCINTILLA_APP_H
