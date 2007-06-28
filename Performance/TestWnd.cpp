//////////////////////////////////////////////
// TestWnd.cpp
//  Definitions for the CTestWindow class

#include "PerfApp.h"
#include "TestWnd.h"


CTestWindow::CTestWindow()
{
}

void CTestWindow::CreateWin(int i)
{
	m_nWindow = i + 1;
	tStringStream str;
	str << _T("Test Window ") << m_nWindow;
	CreateEx(0L, NULL, str.str().c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		420, 50 + i, 300, 200, NULL, NULL);
}

void CTestWindow::OnInitialUpdate()
{
	// Get a reference to the CMainWnd object
	CMainWindow& MainWnd = ((CPerformanceApp*)GetApp())->GetMainWnd();

	// Post a message to MainWnd when the window is created. 
	::PostMessage(MainWnd.GetHwnd(), WM_WINDOWCREATED, 0, 0);
}

LRESULT CTestWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static LRESULT nMessages = 0;
	switch (uMsg)
	{
	case WM_CLOSE:
		{
			tStringStream str;
			str << _T("Closing test Window #") << m_nWindow;
			TRACE(str.str().c_str());
		}
		break;

	case WM_TESTMESSAGE:
		// return the number of WM_TESTMESSAGE messages processsed so far
		return ++nMessages;
	}

	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

