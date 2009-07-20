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

LRESULT CMainWin::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_DESTROY:
	 		// Post the WM_QUIT message to terminate the application.
			::PostQuitMessage(0);
			break;
	}
	
	return WndProcDefault(uMsg, wParam, lParam);
}
