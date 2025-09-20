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
    CPerformanceApp() = default;
    virtual ~CPerformanceApp() override = default;
    CMainWindow& GetMainWnd() {return m_mainWnd;}

protected:
    virtual BOOL InitInstance() override;

private:
    CPerformanceApp(const CPerformanceApp&) = delete;
    CPerformanceApp& operator=(const CPerformanceApp&) = delete;

    CMainWindow m_mainWnd;
};


// returns a pointer to the CPerformanceApp object
inline CPerformanceApp* GetPerfApp() { return static_cast<CPerformanceApp*>(GetApp()); }


#endif  //PERFAPP_H

