/////////////////////////////
// PerfApp.h
//

#ifndef PERFAPP_H
#define PERFAPP_H

#include "MainWnd.h"


/////////////////////////////////////////////////////////////////
// CPerformanceApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the main
// window when it runs.
class CPerformanceApp : public CWinApp
{
public:
    CPerformanceApp();
    virtual ~CPerformanceApp();
    virtual BOOL InitInstance();
    CMainWindow& GetMainWnd() {return m_mainWnd;}

private:
    CMainWindow m_mainWnd;
};


// returns a pointer to the CPerformanceApp object
inline CPerformanceApp* GetPerfApp() { return static_cast<CPerformanceApp*>(GetApp()); }


#endif  //PERFAPP_H

