#include "FrameApp.h"

CFrameApp::CFrameApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
}

BOOL CFrameApp::InitInstance()
{
    TRACE(TEXT("Start Tracing"));

	//Create the Window
    m_Frame.Create();

	return TRUE;
}
