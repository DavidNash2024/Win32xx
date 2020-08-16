/////////////////////////////
// PerfApp.h
//

#ifndef PERFAPP_H
#define PERFAPP_H

#include "MainWnd.h"

///////////////////////////////////////////
// Declaration of the CPerformanceApp class
//
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

