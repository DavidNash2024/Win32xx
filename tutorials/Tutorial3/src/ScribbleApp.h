////////////////////////////////////
// ScribbleApp.h

#include "wxx_wincore.h"
#include "View.h"


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

    CView m_view;
};
