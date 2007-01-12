//////////////////////////////////////////////
// DXApp.cpp

#include "..\Win32++\WinCore.h"
#include "DXApp.h"


CDXApp::CDXApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
	TRACE("Tracing Started");

	//Create the Window
    m_View.Create();
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
