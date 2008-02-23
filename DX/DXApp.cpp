//////////////////////////////////////////////
// DXApp.cpp

#include "DXApp.h"


CDXApp::CDXApp()
{
}

BOOL CDXApp::InitInstance()
{
	//Create the Window
    m_View.Create();

	return TRUE;
}

int CDXApp::MessageLoop()
// Override CWinApp::MessageLoop to accommodate the needs of DirectX
{
	MSG Msg = {0};
	while( Msg.message!=WM_QUIT )
	{
		if( PeekMessage(&Msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&Msg);
			::DispatchMessage(&Msg);
		}
		else
			m_View.Render();
	}
	return LOWORD(Msg.wParam);
}
