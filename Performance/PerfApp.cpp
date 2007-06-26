//////////////////////////////////////////////////////////
//PerfApp.cpp -  Definitions for the CPerformanceApp class

#include "PerfApp.h"



CPerformanceApp::CPerformanceApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
	//Start Tracing
	TRACE(_T("Tracing Started"));

	m_MainWnd.Create();
}

CPerformanceApp::~CPerformanceApp()
{
}
