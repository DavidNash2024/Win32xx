//////////////////////////////////
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

private:
    CMainFrame m_frame;
};

#endif //SCRIBBLEAPP_H

