///////////////////////////////////////////////
//ThreadApp.cpp -  Definitions for the CThreadApp class

#include "ThreadApp.h"


HANDLE g_ThreadHandles[MAX_THREADS];
CThreadWnd* g_ThreadCWnds[MAX_THREADS];

CThreadApp::CThreadApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
	//Start Tracing
	TRACE(TEXT("Tracing Started"));
}

CThreadApp::~CThreadApp()
{
	for (int i = 0 ; i < MAX_THREADS ; i++)
	{
		delete g_ThreadCWnds[i];
		::CloseHandle(g_ThreadHandles[i]);
	}
}
