/////////////////////////////
// PerfApp.cpp
//

#include "pch.h"
#include "PerfApp.h"

///////////////////////////////////////
// CPerformanceApp function definitions
//

// Called when the application starts.
BOOL CPerformanceApp::InitInstance()
{
    m_mainWnd.Create(); // throws a CWinException on failure

    return TRUE;
}

