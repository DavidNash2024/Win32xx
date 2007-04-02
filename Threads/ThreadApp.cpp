///////////////////////////////////////////////
//ThreadApp.cpp -  Definitions for the CThreadApp class

#include "ThreadApp.h"


// HANDLE g_ThreadHandles[MAX_THREADS];
// CThreadWnd* g_ThreadCWnds[MAX_THREADS];



CThreadApp::CThreadApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
	//Start Tracing
//	TRACE(TEXT("Tracing Started"));
	


	m_MainWnd.Create();
}

CThreadApp::~CThreadApp()
{

}
