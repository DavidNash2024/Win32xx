///////////////////////////////////////////////////////
//ThreadApp.cpp -  Definitions for the CThreadApp class

#include "ThreadApp.h"



CThreadApp::CThreadApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
}

CThreadApp::~CThreadApp()
{
}

BOOL CThreadApp::InitInstance()
{
	//Start Tracing
	TRACE(_T("Tracing Started"));

	m_MainWnd.Create();

	return TRUE;
}
