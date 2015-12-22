
#include "DXApp.h"


BOOL CDXApp::InitInstance()
{
	m_View.Create();

	return TRUE;
}


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
			m_View.Render();
		}
	}

	return LOWORD(msg.wParam);
}

