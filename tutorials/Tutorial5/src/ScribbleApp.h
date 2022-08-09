////////////////////////////////////////
// ScribbleApp.h

#ifndef SCRIBBLEAPP_H
#define SCRIBBLEAPP_H

#include "targetver.h"
#include "wxx_wincore.h"
#include "Mainfrm.h"


///////////////////////////////////////////////////////////////////
// CScribbleApp manages the application. It initializes the Win32++
// framework when it is constructed, and create the main frame
// window when it runs.
class CScribbleApp : public CWinApp
{
public:
    CScribbleApp();
    virtual ~CScribbleApp() {}
    virtual BOOL InitInstance();

private:
    CScribbleApp(const CScribbleApp&);                // Disable copy construction
    CScribbleApp& operator = (const CScribbleApp&);   // Disable assignment operator

    CMainFrame m_frame;
};

#endif // SCRIBBLEAPP_H
