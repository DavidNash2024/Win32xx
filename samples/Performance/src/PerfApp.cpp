//////////////////////////////////////////////////////////
//PerfApp.cpp -  Definitions for the CPerformanceApp class

#include "stdafx.h"
#include "PerfApp.h"


CPerformanceApp::CPerformanceApp()
{
}

CPerformanceApp::~CPerformanceApp()
{
}

BOOL CPerformanceApp::InitInstance()
{
    m_mainWnd.Create(); // throws a CWinException on failure  

    return TRUE;
}

