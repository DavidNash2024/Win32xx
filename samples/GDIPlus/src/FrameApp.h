/////////////////////////////
// FrameApp.h
//

#ifndef FRAMEAPP_H
#define FRAMEAPP_H

#include "Mainfrm.h"


////////////////////////////////////////////////////////////////
// CFrameApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CFrameApp : public CWinApp
{
public:
    CFrameApp();
    virtual ~CFrameApp();
    virtual BOOL InitInstance();

private:
    CFrameApp(const CFrameApp&);                // Disable copy construction
    CFrameApp& operator = (const CFrameApp&);   // Disable assignment operator

    CMainFrame m_frame;
};


#endif // define FRAMEAPP_H
