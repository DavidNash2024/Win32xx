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

LRESULT CMainWin::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_DESTROY:
	 		// Post the WM_QUIT message to terminate the application.
			::PostQuitMessage(0);
			break;
	}
	return 0L;
}
