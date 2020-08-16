/////////////////////////////
// PerfApp.cpp
//

#include "stdafx.h"
#include "PerfApp.h"

///////////////////////////////////////
// CPerformanceApp function definitions
//

// Constructor.
CPerformanceApp::CPerformanceApp()
{
}

// Destructor.
CPerformanceApp::~CPerformanceApp()
{
}

// Called when the application starts.
BOOL CPerformanceApp::InitInstance()
{
    m_mainWnd.Create(); // throws a CWinException on failure

    return TRUE;
}

