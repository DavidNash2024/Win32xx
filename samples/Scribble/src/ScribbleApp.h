/////////////////////////////
// ScribbleApp.h
//

#ifndef SCRIBBLEAPP_H
#define SCRIBBLEAPP_H

#include "Mainfrm.h"


////////////////////////////////////////////////////////////////
// CScribbleApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
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
