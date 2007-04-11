///////////////////////////////////////////////////////
//ThreadApp.cpp -  Definitions for the CThreadApp class

#include "ThreadApp.h"



CThreadApp::CThreadApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
	//Start Tracing
	TRACE(TEXT("Tracing Started"));

	m_MainWnd.Create();
}

CThreadApp::~CThreadApp()
{
}
