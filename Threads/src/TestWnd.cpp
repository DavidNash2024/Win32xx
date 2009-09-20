//////////////////////////////////////////////
// TestWnd.cpp
//  Definitions for the CTestWindow class



#include "TestWnd.h"
#include "ThreadApp.h"


CTestWindow::CTestWindow() : m_nWindow(-1)
{
}

void CTestWindow::CreateWin(int nWindow)
{
	CreateEx(0L, NULL, _T("Test Window"), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		70 + 20*nWindow, 120 + 20*nWindow, 300, 200, NULL, NULL);

	m_nWindow = nWindow +1;
}

void CTestWindow::OnInitialUpdate()
{
	// Get a reference to the CMainWnd object
	CMainWindow& MainWnd = ((CThreadApp*)GetApp())->GetMainWnd();

	// Post a message to MainWnd when the window is created. The MainWnd window
	//  is in a different thread, so PostMessage is preferred over SendMessage.
	//  SendMessage would wait for the MainWnd thread to respond.
	::PostMessage(MainWnd.GetHwnd(), WM_WINDOWCREATED, 0, 0);
}

LRESULT CTestWindow::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		{
			TCHAR str[80];
			wsprintf(str, _T("Closing test Window %d\n"), m_nWindow);
			TRACE(str);
		}
		break;

	case WM_DESTROY:
		// Post the WM_QUIT message to terminate the thread.
		::PostQuitMessage(0);
		break;

	}

	return WndProcDefault(uMsg, wParam, lParam);
}

