////////////////////////////////////
// ScribbleApp.h

#include "wxx_wincore.h"
#include "View.h"


///////////////////////////////////////////////////////////////////
// CScribbleApp manages the application. It initializes the Win32++
// framework when it is constructed, and create the view window
// when it runs.
class CScribbleApp : public CWinApp
{
public:
    CScribbleApp();
    virtual ~CScribbleApp() override {}

protected:
    virtual BOOL InitInstance() override;

private:
    CScribbleApp(const CScribbleApp&) = delete;
    CScribbleApp& operator=(const CScribbleApp&) = delete;

    CView m_view;
};
