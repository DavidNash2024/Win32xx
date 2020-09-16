/////////////////////////////
// FrameApp.h
//

#ifndef FRAMEAPP_H
#define FRAMEAPP_H

#include "Mainfrm.h"


////////////////////////////////////////////////////////////////
// CFrameApp manages the application. It initializes the Win32++
// framework when it is constructed, and create the main frame
// window when it runs.
class CFrameApp : public CWinApp
{
public:
    CFrameApp();
    virtual ~CFrameApp();
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};


#endif // define FRAMEAPP_H
