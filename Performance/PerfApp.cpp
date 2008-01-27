//////////////////////////////////////////////////////////
//PerfApp.cpp -  Definitions for the CPerformanceApp class

#include "PerfApp.h"


CPerformanceApp::CPerformanceApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
}

CPerformanceApp::~CPerformanceApp()
{
}

BOOL CPerformanceApp::InitInstance()
{
	m_MainWnd.Create();

	return TRUE;
}

