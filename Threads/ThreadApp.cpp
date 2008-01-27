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
	m_MainWnd.Create();

	return TRUE;
}
