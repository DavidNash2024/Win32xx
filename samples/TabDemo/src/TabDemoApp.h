/////////////////////////////
// TabDemoApp.h
//

#ifndef CONTAINERAPP_H
#define CONTAINERAPP_H


#include "Mainfrm.h"


//////////////////////////////////////////////////////////////////
// CTabDemoApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CTabDemoApp : public CWinApp
{
public:
    CTabDemoApp();
    virtual ~CTabDemoApp();
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;
};


#endif // CONTAINERAPP_H
