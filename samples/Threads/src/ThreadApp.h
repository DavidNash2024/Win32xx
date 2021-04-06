/////////////////////////////
// ThreadApp.h
//

#ifndef THREADAPP_H
#define THREADAPP_H

#include "MainWnd.h"


/////////////////////////////////////////////////////////
// CThreadApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates
// the main window when it runs.
class CThreadApp : public CWinApp
{
public:
    CThreadApp();
    virtual ~CThreadApp();
    virtual BOOL InitInstance();
    CMainWindow& GetMainWnd() {return m_mainWnd;}

private:
    CMainWindow m_mainWnd;
};


#endif  //THREADAPP_H

