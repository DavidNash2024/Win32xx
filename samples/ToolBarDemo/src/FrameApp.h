/////////////////////////////
// FrameApp.h
//

#ifndef FRAMEAPP_H
#define FRAMEAPP_H

#include "Mainfrm.h"


/////////////////////////////////////////////////////////////////
// CToolBarDemoApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the main
// frame window when it runs.
class CToolBarDemoApp : public CWinApp
{
public:
    CToolBarDemoApp();
    virtual ~CToolBarDemoApp();

protected:
    // Virtual functions that override base class functions 
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};


#endif // define FRAMEAPP_H
