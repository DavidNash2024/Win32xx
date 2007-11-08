//////////////////////////////////
// ScribbleApp.cpp


#include "ScribbleApp.h"

CScribbleApp::CScribbleApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
}

BOOL CScribbleApp::InitInstance()
{
    //Create the Window
    m_Frame.Create();

	return TRUE;
}
