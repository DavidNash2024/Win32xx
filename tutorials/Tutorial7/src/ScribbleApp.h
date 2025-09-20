///////////////////////////////////
// ScribbleApp.h

#ifndef SCRIBBLEAPP_H
#define SCRIBBLEAPP_H

#include "wxx_wincore.h"
#include "Mainfrm.h"


///////////////////////////////////////////////////////////////////
// CScribbleApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CScribbleApp : public CWinApp
{
public:
    CScribbleApp() = default;
    virtual ~CScribbleApp() override = default;
    virtual BOOL InitInstance() override;

private:
    CScribbleApp(const CScribbleApp&) = delete;
    CScribbleApp& operator=(const CScribbleApp&) = delete;

    CMainFrame m_frame;
};

#endif // SCRIBBLEAPP_H

