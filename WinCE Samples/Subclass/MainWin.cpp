//////////////////////////////
// MainWin.Cpp

#include "MainWin.h"


CMainWin::CMainWin()
{
	::InitCommonControls();
}

void CMainWin::OnCreate()
{
	m_Button.Create(m_hWnd);
}

LRESULT CMainWin::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_DESTROY:
	 		// Post the WM_QUIT message to terminate the application.
			::PostQuitMessage(0);
			return 0L;
	}

	// Pass unhandled messages on for default processing
	return WndProcDefault(hwnd, uMsg, wParam, lParam);
}
