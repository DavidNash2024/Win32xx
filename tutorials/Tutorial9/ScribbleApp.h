////////////////////////////////////////
// ScribbleApp.h

#ifndef SCRIBBLEAPP_H
#define SCRIBBLEAPP_H

#include "targetver.h"
#include "wxx_wincore.h"
#include "Mainfrm.h"


// A class that inherits from CWinApp. 
// It is used to run the application's message loop.
class CScribbleApp : public CWinApp
{
public:
    CScribbleApp();
    virtual ~CScribbleApp() {}
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_frame; }

private:
    CMainFrame m_frame;
};

// returns a reference to the CScribbleApp object
inline CScribbleApp& GetScribbleApp() { return static_cast<CScribbleApp&>(GetApp()); }


#endif //SCRIBBLEAPP_H

