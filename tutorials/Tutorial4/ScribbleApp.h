////////////////////////////////////
// ScribbleApp.h

#include "wxx_wincore.h"
#include "View.h"


// A class that inherits from CWinApp. 
// It is used to run the application's message loop.
class CScribbleApp : public CWinApp
{
public:
    CScribbleApp();
    virtual ~CScribbleApp() {}
    virtual BOOL InitInstance();

private:
    CView m_view;
};
