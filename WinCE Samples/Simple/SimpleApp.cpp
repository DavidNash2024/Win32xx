#include "SimpleApp.h"

CSimpleApp::CSimpleApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
}

BOOL CSimpleApp::InitInstance()
{
    TRACE(TEXT("Start Tracing"));

	//Create the Window
    m_View.Create();

	return TRUE;
}