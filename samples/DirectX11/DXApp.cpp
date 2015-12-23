
#include "DXApp.h"

//--------------------------------------------------------------------------------------
// Create's the DirectX view window
//--------------------------------------------------------------------------------------
BOOL CDXApp::InitInstance()
{
	m_DXView.Create();

	return TRUE;
}

//--------------------------------------------------------------------------------------
// The App's messages loop. Calls Render during idle processing
//--------------------------------------------------------------------------------------
int CDXApp::MessageLoop()
{
	// Main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_DXView.Render();
		}
	}

	return LOWORD(msg.wParam);
}

