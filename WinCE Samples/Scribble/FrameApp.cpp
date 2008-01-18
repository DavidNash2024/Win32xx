#include "FrameApp.h"

CFrameApp::CFrameApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
}

BOOL CFrameApp::InitInstance()
{
	//Create the Window
    m_Frame.Create();

	return TRUE;
}

